// prevent header file from being included more than once
#pragma once
#ifndef ADVISORBOT_ORDERBOOK_H
#define ADVISORBOT_ORDERBOOK_H

// include necessary standard C++ libraries and header files
#include <map>
#include <vector>
#include <string>
#include "CSVReader.h"
#include "OrderBookEntry.h"

// Order Book Class
class OrderBook {
    public:
        // Construct an object by reading a CSV data file.
        explicit OrderBook(const std::string &filename);

        // Return a vector of Orders that match the specified filters, or all Orders if no filters are supplied.
        std::vector<OrderBookEntry>
        getOrders(OrderBookType type, const std::string &product = "", const std::string &timestamp = "");

        // Return the earliest time in the orderbook.
        std::string getEarliestTime();

        // Return the next time after the specified time in the orderbook. If there is no next time, 
        // return the earliest time in the orderbook.
        std::pair<std::string, int> getNextTime(const std::string &timestamp);

        // Determine whether a product with the given name exists in the dataset.
        bool checkProductExists(std::string product);

        // Determine whether a given string represents a valid order type based on the corresponding Enum.
        bool isValidOrderType(const std::string &orderType) const;

        // Retrieve the products vector.
        const std::vector<std::string> &getProducts() const;

        // Retrieve the timestamps vector.
        const std::vector<std::string> &getTimestamps() const;

        // A map of valid order book types and their corresponding Enum values, with the string values 
        // as the keys and the Enum values as the corresponding values.
        std::map<std::string, OrderBookType> orderBookTypes = {
                {"ask", OrderBookType::ask},
                {"bid", OrderBookType::bid}
        };

    private:
        // Populate a vector with all known products in the dataset.
        std::vector<std::string> populateProducts();

        // Populate a vector with all timestamps in the dataset.
        std::vector<std::string> populateTimestamps();

        // Vectors for storing specific parts of CSV data in memory for efficient access.
        std::vector<OrderBookEntry> orders;
        std::vector<std::string> products;
        std::vector<std::string> timestamps;
};

#endif //ADVISORBOT_ORDERBOOK_H
