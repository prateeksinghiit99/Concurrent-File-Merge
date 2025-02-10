#pragma once
#include <string>
#include <sstream>
#include <chrono>

class DataEntry {
private:
    std::string symbol;                                 // Stock Symbol
    std::chrono::system_clock::time_point timestamp;    // TimeStamp
    double price;                                       // Stock Price
    int size;                                           // Order Quantity
    std::string exchange;                               // Exchange
    std::string type;                                   // Order type
public:
    // Constructors
    DataEntry();                                                        // Default Constructor
    DataEntry(const std::string& line, const std::string& fileName);    // Parameterized Constructor
    DataEntry(DataEntry&& other) noexcept;                              // Move Constructor

    // Getter function
    const std::string& getSymbol() const;
    
    std::string toCSV() const;                          // Convert DataEntry Object to Comma Seperated String
    
    // Operator Overloading
    DataEntry& operator=(DataEntry&& other) noexcept;   // Move Assignment Operator
    bool operator>(const DataEntry& other) const;       // Greater than > operator
};



