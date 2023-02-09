// prevent header file from being included more than once
#pragma once
#ifndef ADVISORBOT_CSVREADER_H
#define ADVISORBOT_CSVREADER_H

// include necessary standard C++ libraries and header files
#include <vector>
#include <string>
#include "OrderBookEntry.h"

/*
Note: 
Some of the code in this module is based on the CSVReader module introduced in the 'merklerex' course, with some 
modifications made by the author of this module. The original code is used with permission.
*/

// Class for reading CSV data and converting records into OrderBookEntry objects
class CSVReader {
    public:
        // Default constructor
        CSVReader();

        // Read a CSV data file and convert the records into a vector of OrderBookEntry objects.
        // Takes a string representing the file path as input.
        // Returns a vector of OrderBookEntry objects.
        static std::vector<OrderBookEntry> readCSV(const std::string &csvFile);

        // Split a CSV record (line) into a vector of individual values (tokens).
        // Takes a string representing a CSV record and a character separator as input.
        // Returns a vector of strings, where each string is a token in the record.
        static std::vector<std::string> tokenise(const std::string &csvLine, char separator);

    private:
        // A private utility function that helps convert raw CSV rows to OrderBookEntry objects.
        // Takes a vector of strings as input, where each string represents a token in the CSV record.
        // Returns an OrderBookEntry object.
        static OrderBookEntry stringsToOBE(std::vector<std::string> tokens);
};


#endif //ADVISORBOT_CSVREADER_H
