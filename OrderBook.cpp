// including all the necessary C++ libraries and header files
#include <map>
#include <utility>
#include <algorithm>
#include "OrderBook.h"
#include "CSVReader.h"
#include "Calculator.h"

OrderBook::OrderBook(const std::string &filename) {

    // Read and parse a CSV file to extract OrderBookEntry objects and store them in the 'orders' field
    orders = CSVReader::readCSV(filename);

    // Populate the 'products' field with the distinct products present in the 'orders' field
    products = populateProducts();

    // Populate the 'timestamps' field with the distinct timestamps present in the 'orders' field
    timestamps = populateTimestamps();
}

// This function returns a vector of distinct products present in the 'orders' field
std::vector<std::string> OrderBook::populateProducts() {

    // Create a map to store the products as keys and a boolean value indicating their presence in the map
    std::map<std::string, bool> prodMap;

    // Iterate over the 'orders' field
    for (const OrderBookEntry &e: orders) {
        // Add the product to the map
        prodMap[e.product] = true;
    }

    // Reserve space for the 'products' vector to prevent frequent reallocations
    products.reserve(prodMap.size());

    // Iterate over the map
    for (auto const &e: prodMap) {
        // Add the product to the 'products' vector
        products.push_back(e.first);
    }
    // Return the 'products' vector
    return products;
}

// This function returns a vector of distinct timestamps present in the 'orders' field
std::vector<std::string> OrderBook::populateTimestamps() {

    // Create a map to store the timestamps as keys and a boolean value indicating their presence in the map
    std::map<std::string, bool> timeMap;

    // Iterate over the 'orders' field
    for (const OrderBookEntry &e: orders) {
        // Add the timestamp to the map
        timeMap[e.timestamp] = true;
    }

    // Reserve space for the 'timestamps' vector to prevent frequent reallocations
    timestamps.reserve(timeMap.size());

    // Iterate over the map
    for (auto const &e: timeMap) {
        // Add the timestamp to the 'timestamps' vector
        timestamps.push_back(e.first);
    }

    // Sort the 'timestamps' vector in ascending order using the compareTimestamps function from the Calculator class
    std::sort(timestamps.begin(), timestamps.end(), Calculator::compareTimestamps);

    // Return the 'timestamps' vector
    return timestamps;
}

// This function returns a vector of OrderBookEntry objects that match the specified criteria
std::vector<OrderBookEntry>
OrderBook::getOrders(OrderBookType type, const std::string &product, const std::string &timestamp) {
    // Create a vector to store the matching OrderBookEntry objects
    std::vector<OrderBookEntry> orders_sub;

    // Iterate over the 'orders' field
    for (const OrderBookEntry &e: orders) {
        if (
                e.orderType == type && // Check if the OrderBookType of the current OrderBookEntry object matches the input OrderBookType
                (product.empty() || e.product == product) &&  // Check if the product of the current OrderBookEntry object matches the input product string, if the input product string is no
                (timestamp.empty() || e.timestamp == timestamp) // Check if the timestamp of the current OrderBookEntry object matches the input timestamp string, if the input timestamp string is not empty
                )
            // If the current OrderBookEntry object matches the specified criteria, add it to the 'orders_sub' vector
            orders_sub.push_back(e);
    }

    // Return the 'orders_sub' vector
    return orders_sub;
}

// This function returns the earliest timestamp present in the 'timestamps' field
std::string OrderBook::getEarliestTime() {
    // Return the first timestamp in the 'timestamps' field
    return timestamps[0];
}

// This function returns the next timestamp after the input timestamp, if it exists, or the earliest timestamp if it does not
std::pair<std::string, int> OrderBook::getNextTime(const std::string &timestamp) {
    // A string to store the next timestamp
    std::string nextTimestamp;
    // An integer to store the index of the next timestamp in the 'timestamps' field
    unsigned int timestampIndex;

    // Iterate over the 'timestamps' field
    for (int i = 0; i < timestamps.size(); ++i) {
        // Check if the current timestamp is after the input timestamp
        if (timestamps[i] > timestamp) {
            // If it is, store it in 'nextTimestamp'
            nextTimestamp = timestamps[i];
            // Store its index in 'timestampIndex'
            timestampIndex = i;
            // Break out of the loop
            break;
        }
    }
    // If no next timestamp was found
    if (nextTimestamp.empty()) {
        // Set 'nextTimestamp' to the earliest timestamp
        nextTimestamp = timestamps[0];
        // Set 'timestampIndex' to 0
        timestampIndex = 0;
    }
    // Return a pair containing the 'nextTimestamp' and 'timestampIndex'
    return {nextTimestamp, timestampIndex};
}

// This function returns a reference to the 'products' field
const std::vector<std::string> &OrderBook::getProducts() const {
    return products;
}

// This function returns a reference to the 'timestamps' field
const std::vector<std::string> &OrderBook::getTimestamps() const {
    return timestamps;
}

// This function returns true if the input product string is present in the 'products' field, false otherwise
bool OrderBook::checkProductExists(std::string product) {
    return std::any_of(products.begin(), products.end(), [&product](const std::string &p) { return p == product; });
}

// This function returns true if the input orderType string is a valid OrderBookType, false otherwise
bool OrderBook::isValidOrderType(const std::string &orderType) const {
    return orderBookTypes.count(orderType) > 0;
}


