// prevent header file from being included more than once
#pragma once
#ifndef ADVISORBOT_ORDERBOOKENTRY_H
#define ADVISORBOT_ORDERBOOKENTRY_H

// include necessary standard C++ libraries
#include <string>
#include <utility>

/*
Note:
I utilized the code from the OrderBookEntry module, specifically the merklerex portion, in the creation of advisorbot. 
Some of the code remains unchanged, while other parts were modified or rewritten by me.
*/

enum class OrderBookType {
    bid,
    ask,
    unknown
};

class OrderBookEntry {
    public:

        OrderBookEntry(
                double _price,
                std::string _timestamp,
                std::string _product,
                OrderBookType _orderType
        ) : price(_price),
            timestamp(std::move(_timestamp)),
            product(std::move(_product)),
            orderType(_orderType) {
        }

        // Convert string values to their corresponding Enum type using a map.
        static OrderBookType stringToOrderBookType(const std::string &s);

        // Convert an Enum type to its string representation.
        static std::string orderBookTypeToString(OrderBookType t);

        // A helper method for comparing two entries based on their Timestamp values.
        static bool compareByTimestampAsc(OrderBookEntry &e1, OrderBookEntry &e2);

        // Generate a string representation of an OrderBookEntry. 
        std::string toString() const;

        double price;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
};


#endif //ADVISORBOT_ORDERBOOKENTRY_H
