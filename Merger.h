#pragma once
#include "DataEntry.h"
#include "FileHandler.h"
#include "Constants.h"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

class Merger {
private:
    std::vector<FileHandler> files;                             // List of all files
    std::queue<size_t> open_files;                              // Queue referencing currently open files to manage file stream limits.
    std::unordered_map<std::string, size_t> filename_to_index;  // Maps file names to their indices in 'files'.
    std::string outputFile;                                     // Output file
    std::priority_queue<DataEntry, std::vector<DataEntry>, 
        std::greater<DataEntry>> minHeap;                       // Min heap for sorting DataEntry objects
    std::mutex heap_mutex;                                      // Mutex for thread-safe heap access 
public:
    // Parameterized Constructor
    Merger(const std::string& inputFiles, const std::string&& outputFile);

    // Main method responsible for orchestrating the entire flow.
    // Uses K-Way Heap Sort for merging.
    // Initializes the Min Heap after reading a chunk from all files.
    // Once a file chunk is written completely to the output, loads another chunk.
    // Ensures the size of the Min Heap (memory used) never exceeds the initial limit.
    void mergeAndSort();

    // Reads all files up to a specified point using multiple threads.
    void readFilesConcurrently();

    // Write to the output file and process remaining data in chunks as needed.
    void writeAndProcessChunks();

    // Processes a batch of files in chunks. 
    // Reads data from each file, pushing it to a min-heap. 
    // Opens and closes file streams dynamically to ensure MAX_OPEN_FILES is not exceeded for optimal resource usage
    void processFileBatch(size_t startIdx, size_t endIdx);

    // Close all files stored in queue
    void closeAllFiles();
};

