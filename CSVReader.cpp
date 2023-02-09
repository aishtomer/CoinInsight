// include necessary C++ libraries and header files
#include <iostream>
#include <fstream>
#include "CSVReader.h"
#include "OrderBookEntry.h"

// Default constructor
CSVReader::CSVReader() = default;

// Read a CSV data file and convert the records into a vector of OrderBookEntry objects.
// Takes a string representing the file path as input.
// Returns a vector of OrderBookEntry objects.
std::vector<OrderBookEntry> CSVReader::readCSV(const std::string &csvFilename) {
    // vector to store OrderBookEntry objects
    std::vector<OrderBookEntry> entries;

    FILE *fp;
    char line_buffer[1024];
    // open file in read mode
    fp = fopen(csvFilename.c_str(), "r");

    // if file could not be opened, throw an error
    if (fp == nullptr) {
        std::cout << "Couldn't open CSV File: " << csvFilename << std::endl;
        throw std::runtime_error("Couldn't open CSV File!");
    }

    // read file line by line
    while (fgets(line_buffer, 1024, fp) != nullptr) {
        try {
            // convert line to OrderBookEntry object
            OrderBookEntry obe = stringsToOBE(tokenise(line_buffer, ','));
            // add OrderBookEntry object to vector
            entries.push_back(obe);
        } 
        // if there is an error in the data, catch it and print a message
        catch (const std::exception &e) {
            std::cout << "CSVReader::readCSV bad data - " << e.what() << std::endl;
        }
    }

    // close file
    fclose(fp);
    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenise(const std::string &csvLine, char separator) {
    // vector to store tokens
    std::vector<std::string> tokens;
    // variables to store start and end indices of token
    signed int start, end;
    std::string token;
    // find index of first non-separator character
    start = csvLine.find_first_not_of(separator, 0);
    do {
        // find index of next separator character
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break; // exit loop if there are no more tokens

        // if end index was found, get token from start to end index
        // else, get token from start to end of line
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);

        // add token to vector
        tokens.push_back(token);

        // set start index to index after end index
        start = end + 1;
    } while (end > 0);
    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens) {
    double price;
    // if there are not 5 tokens, there is an error in the data
    if (tokens.size() != 5) {
        std::cout << "Bad line, expected 5 tokens, got: " << tokens.size() << std::endl;
        throw std::exception{};
    }
    // there are 5 tokens
    try {
        // convert token to double
        price = std::stod(tokens[3]);
    } 
    // if there is an error converting the token to double, throw an exception
    catch (std::exception &e) {
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << std::endl;
        throw;
    }
    // create OrderBookEntry object with the converted price and the remaining tokens
    OrderBookEntry obe{price, tokens[0], tokens[1], OrderBookEntry::stringToOrderBookType(tokens[2])};
    return obe;
}