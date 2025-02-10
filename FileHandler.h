#pragma once
#include "DataEntry.h"
#include <fstream>
#include <queue>
#include <string>

class FileHandler {
private:
    const std::string filePath;            // Full path of the file 
    const std::string fileName;            // File name without extension 
    std::streampos lastByte;               // Position of the last byte to resume processing/reading
    bool is_end_of_file;                   // Whether the file has reached its end
    int lines_written_to_output;           // Number of lines written to output in the latest chunk
public:
    std::ifstream fileStream;              // File stream to handle file operations

    FileHandler(std::string&& filePath, std::string&& fileName);

    // Getter functions
    const std::string& getFilePath() const;
    const std::string& getFileName() const;
    std::streampos getLastByte() const;
    bool isEndOfFile() const;
    int getLinesWrittenToOutput() const;

    // Setter functions
    void setLastByte(std::streampos&& new_last_byte);
    void setLinesWrittenToOutput(int&& last_read_line);
    void setEndOfFile(bool&& end_of_file);
};

