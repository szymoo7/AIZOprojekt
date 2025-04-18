#include <iostream>
#include <vector>
#include "ArgumentParser.h"
#include "Helper.h"
#include <thread>

int main(int argc, char* argv[]) {
    try {
        srand(time(nullptr));// Seed the random number generator
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second
        auto args = ArgumentParser::parseArguments(argc, argv);
        Helper::runWithArguments(args);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        ArgumentParser::printHelp();
        return 1;
    }

    return 0;
}