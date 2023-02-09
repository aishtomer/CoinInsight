#include "CSVReader.h"
#include "Calculator.h"

// Calculate the average price of orders in a given vector of OrderBookEntry objects.
double Calculator::calculateAveragePriceOfOrders(const std::vector<OrderBookEntry> &orders) {
    // Return 0 if the vector is empty.
    if (orders.empty())
        return 0;

    double acc = 0;
    // Iterate through each OrderBookEntry in the vector and add the price to the accumulator.
    for (const OrderBookEntry &e: orders) {
        acc += e.price;
    }

    // Return the average by dividing the total price by the number of orders.
    return acc / (double) orders.size();
}

// Calculate the average price of orders in a given vector of double values.
double Calculator::calculateAveragePriceOfOrders(const std::vector<double> &orders) {
    // Return 0 if the vector is empty.
    if (orders.empty())
        return 0;

    double acc = 0;
    // Iterate through each double value in the vector and add it to the accumulator.
    for (const double e: orders) {
        acc += e;
    }

    // Return the average by dividing the total price by the number of orders.
    return acc / (double) orders.size();
}

// Find the minimum price in a vector of OrderBookEntry objects.
double Calculator::getLowPrice(std::vector<OrderBookEntry> &orders) {
    // Set the minimum value to the first price in the vector.
    double min = orders[0].price;
    // Iterate through each OrderBookEntry in the vector.
    for (const OrderBookEntry &e: orders) {
        // If the current OrderBookEntry's price is less than the current minimum value, update the minimum value.
        if (e.price < min) min = e.price;
    }
    return min;
}

// Retrieve the maximum price from a vector of OrderBookEntry objects.
double Calculator::getHighPrice(std::vector<OrderBookEntry> &orders) {
    // Set the maximum value to the first price in the vector.
    double max = orders[0].price;
    // Iterate through each OrderBookEntry in the vector.
    for (const OrderBookEntry &e: orders) {
        // If the current OrderBookEntry's price is greater than the current maximum value, update the maximum value.
        if (e.price > max) max = e.price;
    }
    return max;
}

// Calculate the average minimum or maximum price over all time steps, based on a given vector of orders
// and a string indicating whether to calculate the minimum or maximum.
double Calculator::calculateAverageMinMaxOverTimesteps(const std::vector<std::vector<OrderBookEntry>> &ordersPerTime,
                                                       const std::string &minOrMax) {
    std::vector<double> minOrMaxPrices;

    // Iterate through each vector of OrderBookEntry objects in the given vector.
    for (std::vector<OrderBookEntry> orders: ordersPerTime) {
        // If the minOrMax parameter is "min", calculate the minimum price for the current vector of orders.
        // If the minOrMax parameter is "max", calculate the maximum price for the current vector of orders.
        // Add the calculated price to the minOrMaxPrices vector.
        if (minOrMax == "min")
            minOrMaxPrices.push_back(getLowPrice(orders));
        else if (minOrMax == "max")
            minOrMaxPrices.push_back(getHighPrice(orders));
    }

    // Return the average of the prices in the minOrMaxPrices vector.
    return calculateAveragePriceOfOrders(minOrMaxPrices);
}

// Compare two timestamp strings. Returns true if t1 is less than t2.
// Intended to be used for sorting and similar purposes.
bool Calculator::compareTimestamps(const std::string &t1, const std::string &t2) {
    return t1 < t2;
}


