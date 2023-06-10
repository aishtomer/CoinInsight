# CoinInsight

CoinInsight is a command-line program that helps a cryptocurrency investor analyze data
available on an exchange. The user can enter commands to see a list of available products,
print the minimum or maximum value of a product, print the average value of a product over
a given number of timesteps, predict the next maximum or minimum value of a product, print
the current time, move to the next timestep, or print a list of current orders. The user can
also enter the "help" command to see a list of available commands or get help information
for a specific command. The user can enter the "exit" command to terminate the program
gracefully. CoinInsight provides the user with a convenient way to access and analyze
product data in a command-line interface.

https://user-images.githubusercontent.com/91372700/218247206-88b608d5-ca49-44eb-ae0d-2511c32a6ec1.mp4

## Run on Desktop

1. Open terminal in the folder.
2. Run `g++ --std=c++11 main.cpp AdvisorMain.cpp Calculator.cpp CSVReader.cpp OrderBook.cpp OrderBookEntry.cpp`
3. Run `./a.out`

