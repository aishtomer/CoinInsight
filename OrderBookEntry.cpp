#include "OrderBookEntry.h"

// This function converts a string to the corresponding OrderBookType enum value
OrderBookType OrderBookEntry::stringToOrderBookType(const std::string &s) {
    // If the input string is "bid", return the bid enum value
    if (s == "bid"){
        return OrderBookType::bid;
    }

    // If the input string is "ask", return the ask enum value
    if (s == "ask"){
        return OrderBookType::ask;
    }
    
    // If the input string is neither "ask" nor "bid", return the unknown enum value
    return OrderBookType::unknown;
}

// This function compares two OrderBookEntry objects based on their timestamps
// It returns true if the timestamp of the first object is less than the timestamp of the second object, false otherwise
bool OrderBookEntry::compareByTimestampAsc(OrderBookEntry &e1, OrderBookEntry &e2) {
    return e1.timestamp < e2.timestamp;
}

// This function converts an OrderBookType enum value to a string
std::string OrderBookEntry::orderBookTypeToString(OrderBookType t) {
    // If the input OrderBookType is ask, return the string "ask"
    if (t == OrderBookType::ask){
        return "ask";
    }
    // If the input OrderBookType is bid, return the string "bid"
    if (t == OrderBookType::bid){
        return "bid";
    }
    // If the input OrderBookType is neither ask nor bid, return the string "unknown"
    return "unknown";
}

// This function returns a string representation of the OrderBookEntry object
std::string OrderBookEntry::toString() const {
    std::string s;
    s += timestamp + " | " + product + " | " + orderBookTypeToString(orderType) + " | " + std::to_string(price);
    return s;
}
