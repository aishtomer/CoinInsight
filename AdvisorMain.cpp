// Include various standard C++ libraries and custom header files for use in the program.
#include <ios>
#include <limits>
#include <vector>
#include <iostream>
#include <algorithm>
#include "CSVReader.h"
#include "Calculator.h"
#include "AdvisorMain.h"

// AdvisorMain constructor
// Initializes member variables to their default values
AdvisorMain::AdvisorMain() = default;

std::string AdvisorMain::readUserCommand() {
    // variable to store the line of input from the user
    std::string line;

    // print a prompt to the console
    std::cout << USERPROMPT;

    // read a line of input from the user and store it in the "line" variable
    std::getline(std::cin, line);

    // return the value of the "line" variable
    return line;
}

void AdvisorMain::handleUserCommand(std::string &userCommand){
    // tokenise the user command using the space character as a delimiter
    std::vector<std::string> cmd = CSVReader::tokenise(userCommand, ' ');

    if (cmd.empty()) {
        // if no command is specified
        std::cout << BOTPROMPT << "Empty input! Please enter a command: " << std::endl;
        // print the list of available commands
        printHelp();
        // throw an exception
        throw std::invalid_argument("Empty input, no command specified");
    }

    // if the command is "help"
    if (cmd[0] == "help"){
        // if no arguments are provided
        if (cmd.size() == 1){
            printHelp();
        }
        // if an argument is provided and it is a valid command
        else if (helpMap.count(cmd[1])){
            // print the help information for that specific command
            printHelpForCmd(cmd[1]);
        }
        // if an argument is provided but it is not a valid command
        else{
            // print an error message
            std::cout << BOTPROMPT << "Invalid argument to 'help': " << cmd[1] << " (unknown command)" << std::endl;
        }
    } 
    else if (cmd[0] == "prod"){
        // if the command is "prod"
        // print the list of available products
        printAvailableProducts();
    } 
    else if (cmd[0] == "min" || cmd[0] == "max"){
        // if the command is "min" or "max"
        // print the minimum or maximum value of the specified product
        printProductMinMaxOfType(cmd);
    } 
    else if (cmd[0] == "avg"){
        // if the command is "avg"
        // print the average value of the specified product over a given number of timesteps
        printProductAvgOfTypeOverTimesteps(cmd);
    } 
    else if (cmd[0] == "predict"){
        // if the command is "predict"
        // predict the next maximum or minimum value of the specified product
        predictProductNextMaxMinOfType(cmd);
    } 
    else if (cmd[0] == "time"){
        // if the command is "time"
        // print the current time
        printTime();
    } 
    else if (cmd[0] == "step"){
        // if the command is "step"
        // move to the next timestep
        moveToNextTimestep();
    } 
    else if (cmd[0] == "list"){
        // if the command is "list"
        // if no arguments are provided
        if (cmd.size() < 2)
            // throw an exception
            throw std::invalid_argument("Invalid argument for list <bid/ask>");
        // print the list of current orders of the specified type (bid or ask)
        printAllCurrentOrdersOfType(cmd[1]);
    } 
    else if (cmd[0] == "exit"){
        // if the command is "exit"
        // terminate the program gracefully
        terminateGracefully();
    } 
    else{
        // if the command is not recognized
        // print an error message
        std::cout << BOTPROMPT << "Invalid command." << std::endl;
        // print the list of available commands
        printHelp();
        // throw an exception
        throw std::invalid_argument("Invalid command");
    }
}

void AdvisorMain::userPrompt(){
    // variable to store the user's command
    std::string userCommand;

    // keep asking for user input until user chooses to exit
    do{
        // clear the userCommand variable
        userCommand.clear();
        // print a newline
        std::cout << std::endl;
        // print a prompt
        std::cout << BOTPROMPT
                  << "Please enter a command, or help for a list of commands (type 'exit' to exit)"
                  << std::endl;

        // read the user's command
        userCommand = readUserCommand();
        // handle the user's command
        handleUserCommand(userCommand);

    } while (userCommand != "exit"); // continue until the user enters the "exit" command
}


void AdvisorMain::init() {
    if (currentTime.first.empty()) {
        // initialize the current time to the earliest time step in the order book
        currentTime = {orderBook.getEarliestTime(), 0}; 
    }
    try {
        // prompt the user for a command
        userPrompt();
    } catch (const std::invalid_argument &e) { 
        // if an exception is thrown due to an invalid command or arguments
        // flush the input stream before retrying
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // show the user what went wrong
        std::cout << BOTPROMPT << e.what() << std::endl << std::endl; 
        // retry the initialization
        init();
    }
}

void AdvisorMain::printHelp() {
    // print a message to the console
    std::cout << BOTPROMPT << "The available commands are:" << std::endl;
    // print a separator line
    std::cout << "---------------------------" << std::endl;
    // iterate through the helpMap container
    for (const auto &e: helpMap) {  
        // print each command in the container
        std::cout << e.first << std::endl;
    }
    // print another separator line
    std::cout << "---------------------------" << std::endl;
}

void AdvisorMain::printHelpForCmd(const std::string &cmd) {
    // retrieve the command and its help text from the helpMap container
    std::pair<std::string, std::string> cmdHelp = helpMap[cmd];

    // print the command and its help text to the console
    std::cout << cmdHelp.first << " -> " << cmdHelp.second << std::endl;
}

void AdvisorMain::printAvailableProducts() {
    // flag to aid in comma printing logic (no comma before first product etc.)
    bool first = true; 

    // iterate through the list of products
    for (const std::string &p: orderBook.getProducts()) {
        if (!first) {
            // if this is not the first product
            // print a comma
            std::cout << ',';
        } 
        else {
            // if this is the first product
            // print a prompt
            std::cout << BOTPROMPT;
            // set the flag to false
            first = false;
        }
        // print the product name
        std::cout << p;
    }
    // print a newline
    std::cout << std::endl;
}

void AdvisorMain::printProductMinMaxOfType(const std::vector<std::string> &cmd) {
    if (cmd.size() < 3) // must be something like '<min/max> <product> <bid/ask>'
        throw std::invalid_argument("Invalid arguments to 'min'/'max'");

    // the first argument specifies whether to find the minimum or maximum value
    std::string min_or_max = cmd[0];

    // the second argument specifies the product
    std::string product = cmd[1];

    // the third argument specifies the order type (bid or ask)
    std::string orderType = cmd[2];

    // verify that the product specified by the user actually exists
    auto products = orderBook.getProducts();
    if (!orderBook.checkProductExists(product)) {
        // print an error message
        std::cout << "Unknown product: " << product << std::endl;
        // throw an exception
        throw std::invalid_argument("Unknown product");
    }

    // verify that the order type specified by the user is a valid one
    OrderBookType orderBookType;
    // if the order type is valid
    if (orderBook.isValidOrderType(orderType)) {
        // retrieve the corresponding OrderBookType object
        orderBookType = orderBook.orderBookTypes[orderType];
    } 
    // if the order type is not valid
    else {
        // print an error message
        std::cout << "Invalid argument for <bid/ask>: " << orderType << std::endl;
        // throw an exception
        throw std::invalid_argument("Invalid argument for <bid/ask>");
    }

    // retrieve the orders of the specified type and product at the current time
    std::vector<OrderBookEntry> orders = orderBook.getOrders(orderBookType, product, currentTime.first);

    // variable to store the minimum or maximum price
    double price;

    // if the user wants to find the minimum price
    if (min_or_max == "min"){
        // find the minimum price
        price = Calculator::getLowPrice(orders);
    }
    // if the user wants to find the maximum price
    else if (min_or_max == "max"){
        // find the maximum price
        price = Calculator::getHighPrice(orders);
    }
    // if the user specified an invalid argument for min/max
    else{
        throw std::invalid_argument("Invalid argument for <min/max>");
    }

    // print the result to the console
    std::cout << BOTPROMPT << "The " << min_or_max << " " << orderType << " for " << product << " is " << price << std::endl;
}

void AdvisorMain::printProductAvgOfTypeOverTimesteps(const std::vector<std::string> &cmd) {
    // if there are not enough arguments, throw an exception
    if (cmd.size() < 4) // must be something like 'avg <product> <ask/bid> <timesteps>'
        throw std::invalid_argument("Invalid arguments to 'avg'");

    // the second argument specifies the product
    std::string product = cmd[1];
    // the third argument specifies the order type (bid or ask)
    std::string orderType = cmd[2];


    int timeSteps;
    try {
        // the fourth argument specifies the number of time steps
        timeSteps = std::stoi(cmd[3]);
    } 
    catch (const std::exception &e) {
        std::cout << "Bad value for 'timesteps' when calling 'avg': " << cmd[3] << std::endl;
        throw;
    }

    // verify that the product specified by the user actually exists
    if (!orderBook.checkProductExists(product)) {
        // print an error message
        std::cout << "Unknown product: " << product << std::endl;
        // throw an exception
        throw std::invalid_argument("Unknown product");
    }

    // verify that the order type specified by the user is a valid one
    OrderBookType orderBookType;
    // if the order type is valid
    if (orderBook.isValidOrderType(orderType)) {
        // retrieve the corresponding OrderBookType object
        orderBookType = orderBook.orderBookTypes[orderType];
    } 
    // if the order type is not valid
    else {
        // print an error message
        std::cout << "Invalid argument for <bid/ask>: " << orderType << std::endl;
        // throw an exception
        throw std::invalid_argument("Invalid argument for <bid/ask>");
    }

    // retrieve all orders of the specified type and product
    std::vector<OrderBookEntry> orders = orderBook.getOrders(orderBookType, product);
    // sort the orders by ascending timestamp
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestampAsc);

    // variables to store the number of time steps to consider and the number to skip
    int timeStepsBack, timeStepsSkip;

    // if the requested number of time steps is further back than the current time step
    if (timeSteps > currentTime.second) {
        // consider only the current time step
        timeStepsBack = std::max(currentTime.second, 1);
        timeStepsSkip = 0;
        std::cout << BOTPROMPT << "number of timesteps (" << timeSteps << ") is too far back." << std::endl;
        std::cout << BOTPROMPT << "current step is " << timeStepsBack << ", therefore the maximum amount of "
                  << timeStepsBack << " timesteps will be used." << std::endl;
    } else {
        timeStepsBack = timeSteps;
        timeStepsSkip = std::max(currentTime.second - timeSteps, 0);
    }

    std::vector<OrderBookEntry> ordersBack;
    ordersBack = std::vector<OrderBookEntry>(orders.begin() + timeStepsSkip, orders.begin() + timeStepsBack);

    double calculatedAvg = Calculator::calculateAveragePriceOfOrders(ordersBack);
    std::cout << BOTPROMPT << "The average " << product << " " << orderType << " price over the last " << timeStepsBack
              << " timesteps was " << calculatedAvg << std::endl;
}

void AdvisorMain::predictProductNextMaxMinOfType(const std::vector<std::string> &cmd) {
    std::string minOrMax = cmd[1];
    std::string product = cmd[2];
    std::string orderType = cmd[3];

    if (minOrMax != "min" && minOrMax != "max")
        throw std::invalid_argument("Invalid argument for <min/max>");

    // verify product passed by user actually exists
    if (!orderBook.checkProductExists(product)) {
        std::cout << "Unknown product: " << product << std::endl;
        throw std::invalid_argument("Unknown product");
    }

    // verify order type passed by user is a valid one
    OrderBookType orderBookType;
    if (orderBook.isValidOrderType(orderType)) {
        orderBookType = orderBook.orderBookTypes[orderType];
    } else {
        std::cout << "Invalid argument for <bid/ask>: " << orderType << std::endl;
        throw std::invalid_argument("Invalid argument for <bid/ask>");
    }

    std::vector<std::vector<OrderBookEntry>> ordersPerTimestep;
    for (int i = 0; i <= currentTime.second; i++) {
        ordersPerTimestep.push_back(orderBook.getOrders(orderBookType, product, orderBook.getTimestamps()[i]));
    }

    double predicted = Calculator::calculateAverageMinMaxOverTimesteps(ordersPerTimestep, minOrMax);

    std::cout << BOTPROMPT << "The predicted " << minOrMax << " " << orderType << " price of " << product
              << " for the next time step is " << predicted << std::endl;
}

void AdvisorMain::printTime() const {
    std::cout << BOTPROMPT << currentTime.first << std::endl;
}

void AdvisorMain::moveToNextTimestep() {
    currentTime = orderBook.getNextTime(currentTime.first);
    std::cout << BOTPROMPT << "now at " << currentTime.first << std::endl;
}

void AdvisorMain::terminateGracefully() {
    std::cout << "Exiting." << std::endl;
    exit(0);
}

void AdvisorMain::printAllCurrentOrdersOfType(const std::string &orderType) {
    if (orderType.empty() || !orderBook.isValidOrderType(orderType)) {
        std::cout << "Invalid argument for list <bid/ask>: " << orderType << std::endl;
        throw std::invalid_argument("Invalid argument for list <bid/ask>");
    }

    std::vector<OrderBookEntry> orders;
    orders = orderBook.getOrders(OrderBookEntry::stringToOrderBookType(orderType), "", currentTime.first);

    if (orders.empty()) {
        std::cout << BOTPROMPT << "No " << orderType << "s found for current time step: ("
                  << currentTime.first << ")." << std::endl;
    } else {
        std::cout << BOTPROMPT << orderType << "s for current time step (" << currentTime.first << "):" << std::endl;
        for (const OrderBookEntry &e: orders) {
            std::cout << e.toString() << std::endl;
        }
    }
}
//
