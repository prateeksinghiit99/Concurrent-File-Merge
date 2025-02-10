#include "DataEntry.h"

// Default Constructor
DataEntry::DataEntry() = default;

// Parameterized Constructor for Creating a DataEntry object from file entry
DataEntry::DataEntry(const std::string& line, const std::string& fileName) 
    : symbol(fileName){
    using namespace std::chrono;
    std::istringstream ss(line);
    char delim;

    // Convert String to TimeStamp
    std::tm tm = {};
    ss >> std::get_time(&tm,"%Y-%m-%d %H:%M:%S") >> delim;
    timestamp = system_clock::from_time_t(std::mktime(&tm));
    int milliseconds_;
    ss >> milliseconds_;
    timestamp += milliseconds(milliseconds_);

    // Extract data directly from the line using move semantics where possible
    ss >> delim >> price >> delim >> size >> delim;

    std::getline(ss, exchange, ',');  // Read until the first comma
    std::getline(ss, type);
}

// Move Constructor
DataEntry::DataEntry(DataEntry&& other) noexcept
    : symbol(std::move(other.symbol)),
    timestamp(std::move(other.timestamp)),
    price(std::move(other.price)),
    size(std::move(other.size)),
    exchange(std::move(other.exchange)),
    type(std::move(other.type)) {
}

// Getter function to get symbol
const std::string& DataEntry::getSymbol() const { return symbol; }

// Convert DataEntry Object to Comma Seperated String
std::string DataEntry::toCSV() const {
    using namespace std::chrono;
    std::ostringstream ss;

    ss << symbol << ", ";

    // Convert TimeStamp to String
    auto time_since_epoch = timestamp.time_since_epoch();               // Get the time since epoch as a duration
    auto seconds_ = duration_cast<seconds>(time_since_epoch);           // Calculate seconds and milliseconds
    auto milliseconds_ = duration_cast<milliseconds>(time_since_epoch) % 1000;

    std::time_t t = seconds_.count();   // Convert seconds to tm struct for formatting
    std::tm tm = {};
    localtime_s(&tm, &t);

    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "." << std::setw(3) 
        << std::setfill('0') << milliseconds_.count();      // Format time to string with milliseconds

    // Add the rest of the fields
    ss << ", " << price << ", " << size
        << "," << exchange << "," << type;

    return ss.str();
}

// Move Assignment Operator
DataEntry& DataEntry::operator=(DataEntry&& other) noexcept {
    if (this != &other) {
        symbol = std::move(other.symbol);
        timestamp = std::move(other.timestamp);
        price = std::move(other.price);
        size = std::move(other.size);
        exchange = std::move(other.exchange);
        type = std::move(other.type);
    }
    return *this;
}


bool DataEntry::operator>(const DataEntry& other) const {
    if (timestamp != other.timestamp) return timestamp > other.timestamp;
    return symbol > other.symbol;
}