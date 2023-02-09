// include a file only once during the compilation process.
#pragma once

// create a header guard to ensure that a header file is included only once in a program.
#ifndef ADVISORBOT_CALCULATOR_H
#define ADVISORBOT_CALCULATOR_H

// include necessary C++ libraries and header files
#include <string>
#include <vector>
#include <map>
#include "OrderBookEntry.h"
#include "CSVReader.h"

class Calculator {

public:

    // Calculate average price of orders in a given vector of OrderBookEntry objects
    static double calculateAveragePriceOfOrders(const std::vector<OrderBookEntry> &orders);

    // Calculate average price of orders in a given vector of double values
    static double calculateAveragePriceOfOrders(const std::vector<double> &orders);

    // Retrieve maximum price from a vector of OrderBookEntry objects
    static double getHighPrice(std::vector<OrderBookEntry> &orders);

    // Find minimum price in a vector of OrderBookEntry objects
    static double getLowPrice(std::vector<OrderBookEntry> &orders);

    // Calculate average minimum or maximum price over all time steps, based on a given vector of orders
    // and a string indicating whether to calculate the minimum or maximum.
    static double calculateAverageMinMaxOverTimesteps(const std::vector<std::vector<OrderBookEntry>> &ordersPerTime, 
                                                    const std::string &minOrMax);

    // Compare two timestamp strings. Intended to be used for sorting and similar purposes.
    static bool compareTimestamps(const std::string &t1, const std::string &t2);
};


#endif //ADVISORBOT_CALCULATOR_H
