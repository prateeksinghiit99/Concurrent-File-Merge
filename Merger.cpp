#include "Merger.h"

namespace fs = std::filesystem;

// Parameterized Constructor
Merger::Merger(const std::string& folderPath, const std::string&& outputFile)
    : outputFile(outputFile) {
    try {
        if (fs::exists(folderPath) && fs::is_directory(folderPath)) {
            for (const auto& entry : fs::directory_iterator(folderPath)) {
                if (fs::is_regular_file(entry.status())) {
                    std::string FilePath = entry.path().string();
                    std::string FileName = entry.path().stem().string();
                    filename_to_index[FileName] = files.size();
                    files.emplace_back(std::move(FilePath), std::move(FileName));
                }
            }
        }
        else {
            std::cerr << "The specified path is not a valid directory.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Main method responsible for orchestrating the entire flow.
// Uses K-Way Heap Sort for merging.
// Initializes the Min Heap after reading a chunk from all files.
// Once a file chunk is written completely to the output, loads another chunk.
// Ensures the size of the Min Heap (memory used) never exceeds the initial limit.
void Merger::mergeAndSort() {

    // Invoke readFilesConcurrently() to initially read the files and create a min heap. 
    readFilesConcurrently();

    // Then, call WriteAndProcessChunks() to write to the output file and process remaining data in chunks as needed.
    writeAndProcessChunks();

    // close all input files that are still open
    closeAllFiles();
}

// Reads all files up to a specified point using multiple threads.
void Merger::readFilesConcurrently() {
    std::vector<std::thread> threads;
    const size_t filesPerThread = (files.size() + Constants::NUM_THREADS - 1) / Constants::NUM_THREADS; // Ceiling division

    // Launch threads
    for (size_t i = 0; i < Constants::NUM_THREADS; i++) {
        size_t startIdx = i * filesPerThread;
        size_t endIdx = std::min(startIdx + filesPerThread, files.size());
        threads.emplace_back(&Merger::processFileBatch, this, startIdx, endIdx);
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}

// Write to the output file and process remaining data in chunks as needed.
void Merger::writeAndProcessChunks() {
    std::ofstream output_file(outputFile);
    output_file << "Symbol, Timestamp, Price, Size, Exchange, Type\n";  // Write header

    // Pop elements from min_heap and start writing
    while (!minHeap.empty()) {
        const DataEntry& minimum_dataEntry = minHeap.top();

        // Write Entry to Output file
        output_file << minimum_dataEntry.toCSV() << '\n';

        // Fetch file corresponding to the data entry
        size_t file_index = filename_to_index[minimum_dataEntry.getSymbol()];
        FileHandler& file = files[file_index];
        int lines_written_to_output = file.getLinesWrittenToOutput();
        lines_written_to_output++;

        // Load the next chunk if all lines from the current file have been written.
        if (!file.isEndOfFile() && lines_written_to_output == Constants::CHUNK_LINES) {
            lines_written_to_output = 0;
            processFileBatch(file_index, file_index +1);
        }
        
        // Increment the count of lines written from the current file chunk to the output.
        file.setLinesWrittenToOutput(std::move(lines_written_to_output));

        // Popping at the end, as pop uses move semantics, will clear the DataEntry object being referenced.
        minHeap.pop();
    }

    output_file.close();
}

// Processes a batch of files in chunks. 
// Reads data from each file, pushing it to a min-heap. 
// Opens and closes file streams dynamically to ensure MAX_OPEN_FILES is not exceeded for optimal resource usage
void Merger::processFileBatch(size_t startIdx, size_t endIdx) {

    for (size_t i = startIdx; i < endIdx; i++) {
        
        FileHandler& file = files[i];
        std::ifstream& input_file = file.fileStream;

        // Open a file and add it to the open_files queue.
        if (!input_file.is_open()) {
            input_file.open(file.getFilePath(), std::ios::in);
            open_files.push(i);

            // Close the first file in the queue if it exceeds the maximum limit.
            if (open_files.size() > Constants::MAX_OPEN_FILES) {
                size_t& fileindex = open_files.front();
                open_files.pop();
                files[fileindex].fileStream.close();
            }
        }

        // Move to the last read byte position
        input_file.seekg(file.getLastByte());

        std::string line;
        size_t lines_read = 0;

        // Skip the header if it's the first read
        if (file.getLastByte() == 0 && std::getline(input_file, line)) {
            // Skip header
        }

        while (lines_read < Constants::CHUNK_LINES && std::getline(input_file, line)) {
            DataEntry entry(line, file.getFileName());
            {
                // Lock the mutex before accessing the minHeap
                std::lock_guard<std::mutex> lock(heap_mutex);
                minHeap.push(std::move(entry));  // Move the DataEntry object to the heap
            }
            lines_read++;
        }

        file.setLastByte(input_file.tellg()); // Update the byte position
        if (input_file.eof()) {
            file.setEndOfFile(true);
        }
    }
}

// Close all files stored in queue
void Merger::closeAllFiles() {
    while (!open_files.empty()) {
        size_t& fileindex = open_files.front();
        open_files.pop();
        files[fileindex].fileStream.close();
    }
}