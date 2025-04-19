#ifndef AIZOPROJEKT_ARGUMENTPARSER_H
#define AIZOPROJEKT_ARGUMENTPARSER_H

#include <iostream>
#include <string>

enum class Mode {
    HELP,
    FILE_TEST,
    BENCHMARK,
    DRUNK,
    RUN_TESTS
};

struct Arguments {
    Mode mode = Mode::HELP;
    int algorithm = 0;
    int dataType = 0;
    std::string inputFile;
    std::string outputFile;
    int size = 0;
    int distribution = 0; // New field for distribution
    int drunkLevel = 0; // New field for drunk level
};

class ArgumentParser {
public:
    static Arguments parseArguments(int argc, char* argv[]) {
        Arguments args;

        if (argc < 2) {
            args.mode = Mode::HELP;
            return args;
        }

        std::string modeStr(argv[1]);
        if (modeStr == "--help") {
            args.mode = Mode::HELP;
        }
        else if (modeStr == "--file" && argc >= 5) {
            args.mode = Mode::FILE_TEST;
            args.algorithm = std::stoi(argv[2]);
            args.dataType = std::stoi(argv[3]);
            args.inputFile = argv[4];

            if (argc == 6) {
                args.outputFile = argv[5];
            }
        }
        else if (modeStr == "--test" && argc >= 6) {
            args.mode = Mode::BENCHMARK;
            args.algorithm = std::stoi(argv[2]);
            args.dataType = std::stoi(argv[3]);
            args.size = std::stoi(argv[4]);
            args.outputFile = argv[5];

            if (argc == 7) {
                args.distribution = std::stoi(argv[6]); // Parse distribution parameter
            }
            else {
                args.distribution = 0; // Default to random distribution if not provided
            }
        }
        else if (modeStr == "--drunk" && argc >= 7) {
            args.mode = Mode::DRUNK;
            args.algorithm = std::stoi(argv[2]);
            args.dataType = std::stoi(argv[3]);
            args.size = std::stoi(argv[4]);
            args.drunkLevel = std::stoi(argv[5]);
            args.outputFile = argv[6];

            if (args.drunkLevel < 0 || args.drunkLevel > 10) {
                args.drunkLevel = 0; // Default to sober if invalid level
            }

            if (argc == 8) {
                args.distribution = std::stoi(argv[7]); // Parse distribution parameter
            } else {
                args.distribution = 0; // Default to random distribution if not provided
            }
        }
        else if (modeStr == "--run_tests") {
            args.mode = Mode::RUN_TESTS;
        }

        else {
            args.mode = Mode::HELP;
        }

        return args;
    }

    static void printHelp() {
        std::cout << "Usage:\n"
                  << "FILE TEST MODE:\n"
                  << "./YourProject --file <algorithm> <type> <inputFile> [outputFile]\n"
                  << "  <algorithm> Sorting algorithm to use (0 - Bubble, 1 - Merge, 2 - Insert, 3 - Binary Insert, 4 - Quick, 5 - Heap, 6 - Shell).\n"
                  << "  <type> Data type to load (e.g., 0 - int, 1 - float, 2 - char).\n"
                  << "  <inputFile> Input file containing the data to be sorted.\n"
                  << "  [outputFile] If provided, the sorted values will be saved to this file.\n\n"
                  << "BENCHMARK MODE:\n"
                  << "./YourProject --test <algorithm> <type> <size> <outputFile> [distribution]\n"
                  << "  <algorithm> Sorting algorithm to use (0 - Bubble, 1 - Merge, 2 - Insert, 3 - Binary Insert, 4 - Quick, 5 - Heap, 6 - Shell).\n"
                  << "  <type> Data type to generate (e.g., 0 - int, 1 - float, 2 - char).\n"
                  << "  <size> Number of elements to generate (instance size).\n"
                  << "  <outputFile> File where the benchmark results should be saved.\n"
                  << "  [distribution] Data distribution (0 - Random (default), 1 - Descending, 2 - Ascending, 3 - 33% sorted, 4 - 66% sorted).\n\n"
                  << "DRUNK MODE:\n"
                  << "./YourProject --drunk <algorithm> <type> <size> <drunk level> <outputFile> [distribution] \n"
                  << "  <algorithm> Sorting algorithm to use (0 - Bubble, 1 - Quick).\n"
                  << "  <type> Data type to generate (e.g., 0 - int, 1 - float).\n"
                  << "  <size> Number of elements to generate (instance size).\n"
                  << "  <drunk level> Level of drunkenness:\n"
                  << "  0 - sober (default), 1 - slightly drunk, 2 - happy, 3 - even happier, 4 - al dente, 5 - best day ever, 6 - has probably birthday.\n"
                  << "  7 - tells you how much he/she loves you, 8 - invites you to his/her weeding, 9 - wants to marry you, 10 - how is he/she still alive\n"
                  << "  <outputFile> File where the benchmark results should be saved.\n"
                  << "  [distribution] Data distribution (0 - Random (default), 1 - Descending, 2 - Ascending, 3 - 33% sorted, 4 - 66% sorted).\n\n"
                  << "RUN TESTS:\n"
                  << "./YourProject --run_tests <option> <outputFile>\n"
                  << "  <option> Run benchmark with given options:\n"
                  << "  0 - Every algorithm (Bubble sort, merge, insert, binary insert, heap, shell) for every data type (int, float, char)\n"
                  << "  and every size (10000, 20000, 40000, 80000, 160000) and every distribution (random, descending, ascending, 33%, 66%)\n"
                  << "  <outputFile> File where the benchmark results should be saved.\n\n"
                  << "HELP MODE:\n"
                  << "./YourProject --help\n"
                  << "  Displays this help message.\n";
    }
};

#endif // AIZOPROJEKT_ARGUMENTPARSER_H