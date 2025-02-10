#include "FileHandler.h"

// Constructors
FileHandler::FileHandler(std::string&& filePath, std:: string&& fileName) 
    : filePath(std::move(filePath)), fileName(std::move(fileName)), lastByte(0), 
    is_end_of_file(false), lines_written_to_output(0) {}

// Getter functions
const std::string& FileHandler::getFilePath() const { return filePath; }
const std::string& FileHandler::getFileName() const { return fileName; }
std::streampos FileHandler::getLastByte() const { return lastByte; }
bool FileHandler::isEndOfFile() const { return is_end_of_file; }
int FileHandler::getLinesWrittenToOutput() const { return lines_written_to_output; }

// Setter functions
void FileHandler::setLastByte(std::streampos&& new_last_byte) {
    lastByte = std::move(new_last_byte);
}

void FileHandler::setLinesWrittenToOutput(int&& last_read_line) {
    lines_written_to_output = std::move(last_read_line);
}
void FileHandler::setEndOfFile(bool&& end_of_file) {
    is_end_of_file = std::move(end_of_file);
}