#ifndef AIZOPROJEKT_HELPER_H
#define AIZOPROJEKT_HELPER_H

#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <valarray>
#include "DynamicArray.h"
#include "ArgumentParser.h"
#include "Sorter.h"
#include "Timer.h"

class Helper {
public:
    static void runWithArguments(Arguments& args) {
        // Map of mode handlers
        static const std::unordered_map<Mode, std::function<void(Arguments&)>> modeHandlers = {
                {Mode::HELP, [](Arguments&){ ArgumentParser::printHelp(); }},
                {Mode::FILE_TEST, fileTest},
                {Mode::BENCHMARK, benchmark},
                {Mode::DRUNK, drunk},
                {Mode::RUN_TESTS, runTests}
        };

        auto handler = modeHandlers.find(args.mode);
        if (handler != modeHandlers.end()) {
            handler->second(args);
        } else {
            throw std::invalid_argument("Invalid mode");
        }
    }

private:
    // Type definitions to simplify code
    using IntSorter = std::function<void(DynamicArray<int>*)>;
    using FloatSorter = std::function<void(DynamicArray<float>*)>;
    using DataGenerator = std::function<void*(Arguments&)>;

    // Template function to read data from file
    template <typename T>
    static DynamicArray<T>* readDataFromFile(const std::string& filename) {
        std::filesystem::path originalPath = std::filesystem::current_path();

        try {
            std::filesystem::current_path(originalPath.parent_path());

            std::ifstream inputFile(filename);
            if (!inputFile) {
                throw std::runtime_error("Unable to open input file: " + filename);
            }

            int size;
            if (!(inputFile >> size)) {
                throw std::runtime_error("Failed to read size from input file: " + filename);
            }

            auto* data = new DynamicArray<T>(size);
            for (int i = 0; i < size; ++i) {
                T value;
                if (!(inputFile >> value)) {
                    throw std::runtime_error("Failed to read value from input file: " + filename);
                }
                data->insert(i, value);
            }

            std::filesystem::current_path(originalPath);
            std::cout << "Data read from file: " << filename << std::endl;
            return data;
        } catch (...) {
            std::filesystem::current_path(originalPath);
            throw;
        }
    }

    // Template function to write data to file
    template <typename T>
    static void writeDataToFile(const std::string& filename, const DynamicArray<T>* data) {
        std::ofstream outputFile(filename);
        if (!outputFile) {
            throw std::runtime_error("Unable to open output file: " + filename);
        }

        outputFile << data->getSize() << std::endl;

        for (size_t i = 0; i < data->getSize(); ++i) {
            outputFile << data->get(i) << std::endl;
        }
        std::cout << "Data written to file: " << filename << std::endl;
    }

    // Unified data generator function
    template <typename T>
    static DynamicArray<T>* generateData(const Arguments& args) {
        // Map of data distribution generators
        static const std::unordered_map<int, std::function<DynamicArray<T>*(const Arguments&)>> generators = {
                {0, generateTestData<T>}, // Random
                {1, generateTestDataDescending<T>}, // Descending
                {2, generateTestDataAscending<T>}, // Ascending
                {3, generateTestData_33<T>}, // 33% Sorted
                {4, generateTestData_66<T>} // 66% Sorted
        };

        auto generator = generators.find(args.distribution);
        if (generator != generators.end()) {
            return generator->second(args);
        } else {
            throw std::invalid_argument("Invalid distribution choice");
        }
    }

    // Basic random data generator
    template <typename T>
    static DynamicArray<T>* generateTestData(const Arguments& args) {
        auto* data = new DynamicArray<T>(args.size);
        for (int i = 0; i < args.size; ++i) {
            if (args.dataType == 0) { // int
                data->insert(i, rand() % (std::numeric_limits<int>::max()));
            } else if (args.dataType == 1) { // float
                data->insert(i, (float)(rand()) / (float)(rand()));
            } else if (args.dataType == 2) { // char
                data->insert(i, (char) rand() % 256);
            } else {
                throw std::invalid_argument("Invalid data type");
            }
        }
        std::cout << "Test data generated with size: " << args.size << std::endl;
        //writeDataToFile("before.txt", data);
        return data;
    }

    // Descending order data generator
    template<typename T>
    static DynamicArray<T>* generateTestDataDescending(const Arguments& args) {
        auto* data = generateTestData<T>(args);
        Sorter<T>::quickSortDesc(data, 0, data->getSize() - 1);
        std::cout << "Test data generated in descending order with size: " << args.size << std::endl;
        //writeDataToFile("before.txt", data);
        return data;
    }

    // Ascending order data generator
    template<typename T>
    static DynamicArray<T>* generateTestDataAscending(const Arguments& args) {
        auto* data = generateTestData<T>(args);
        Sorter<T>::quickSort(data, 0, data->getSize() - 1);
        std::cout << "Test data generated in ascending order with size: " << args.size << std::endl;
        //writeDataToFile("before.txt", data);
        return data;
    }

    // 33% sorted data generator
    template <typename T>
    static DynamicArray<T>* generateTestData_33(const Arguments& args) {
        auto* data = generateTestData<T>(args);
        size_t oneThirdSize = data->getSize() / 3;
        Sorter<T>::quickSort(data, 0, oneThirdSize);
        Sorter<T>::quickSortDesc(data, oneThirdSize + 1, data->getSize() - 1);
        std::cout << "Test data generated with 33% sorted with size: " << args.size << std::endl;
        //writeDataToFile("before.txt", data);
        return data;
    }

    // 66% sorted data generator
    template <typename T>
    static DynamicArray<T>* generateTestData_66(const Arguments& args) {
        auto* data = generateTestData<T>(args);
        size_t twoThirdSize = data->getSize() * 2 / 3;
        Sorter<T>::quickSort(data, 0, twoThirdSize);
        Sorter<T>::quickSortDesc(data, twoThirdSize + 1, data->getSize() - 1);
        std::cout << "Test data generated with 66% sorted with size: " << args.size << std::endl;
        //writeDataToFile("before.txt", data);
        return data;
    }

    // Execute sort algorithm based on type and algorithm choice
    template <typename T>
    static void executeSort(DynamicArray<T>* data, int algorithmChoice, Timer& timer) {
        // Map sorting algorithms to their implementations
        static const std::unordered_map<int, std::function<void(DynamicArray<T>*)>> sorters = {
                {0, [](DynamicArray<T>* d) { Sorter<T>::bubbleSort(d); }},
                {1, [](DynamicArray<T>* d) { Sorter<T>::mergeSort(d); }},
                {2, [](DynamicArray<T>* d) { Sorter<T>::insertSort(d); }},
                {3, [](DynamicArray<T>* d) { Sorter<T>::binaryInsertSort(d); }},
                {5, [](DynamicArray<T>* d) { Sorter<T>::heapSort(d); }},
                {6, [](DynamicArray<T>* d) { Sorter<T>::shellSort(d); }}
        };

        // Special case for Quick Sort which needs start/end indices
        if (algorithmChoice == 4) {
            timer.start();
            Sorter<T>::quickSort(data, 0, data->getSize() - 1);
            timer.stop();
            return;
        }

        auto sorter = sorters.find(algorithmChoice);
        if (sorter != sorters.end()) {
            timer.start();
            sorter->second(data);
            timer.stop();
        } else {
            throw std::invalid_argument("Invalid algorithm choice");
        }
    }

    // Execute drunk sort algorithm
    template <typename T>
    static void executeDrunkSort(DynamicArray<T>* data, int algorithmChoice, int drunkLevel, Timer& timer) {
        if (algorithmChoice == 0) { // Bubble Sort Drunk
            timer.start();
            Sorter<T>::drunkBubbleSort(data, drunkLevel);
            timer.stop();
        } else if (algorithmChoice == 1) { // Quick Sort Drunk
            timer.start();
            Sorter<T>::drunkQuickSort(data, 0, data->getSize() - 1, drunkLevel);
            timer.stop();
        } else {
            throw std::invalid_argument("Invalid algorithm choice for DRUNK mode");
        }
    }

    // File test executor
    template <typename T>
    static void executeFileTest(const Arguments& args) {
        Timer dummyTimer; // Timer not used but needed for API consistency
        auto* data = readDataFromFile<T>(args.inputFile);
        executeSort<T>(data, args.algorithm, dummyTimer);
        writeDataToFile(args.outputFile, data);
        Sorter<T>::isCorrect(data);
        delete data;
    }

    // File test handler
    static void fileTest(Arguments& args) {
        if (args.dataType == 0) { // int
            executeFileTest<int>(args);
        } else if (args.dataType == 1) { // float
            executeFileTest<float>(args);
        }
        else if (args.dataType == 2) { // char
            executeFileTest<char>(args);
        } else {
            throw std::invalid_argument("Invalid data type");
        }
    }

    // Benchmark handler
    static void benchmark(Arguments& args) {
        Timer timer;
        DynamicArray<int>* times = new DynamicArray<int>(100);

        for(int i = 0; i < 100; i++) {
            if (args.dataType == 0) { // int
                auto* data = generateData<int>(args);
                try {
                    executeSort<int>(data, args.algorithm, timer);
                    std::cout << "Sorting completed in " << timer.result() << " ms." << std::endl;
                    Sorter<int>::isCorrect(data);
                    times->add(timer.result());
                } catch (const std::exception& e) {
                    delete data;
                    throw;
                }
                delete data;
            } else if (args.dataType == 1) { // float
                auto* data = generateData<float>(args);
                try {
                    executeSort<float>(data, args.algorithm, timer);
                    std::cout << "Sorting completed in " << timer.result() << " ms." << std::endl;
                    Sorter<float>::isCorrect(data);
                    times->add(timer.result());
                } catch (const std::exception& e) {
                    delete data;
                    throw;
                }
                delete data;
            }
            else if (args.dataType == 2) { // char
                auto* data = generateData<char>(args);
                try {
                    executeSort<char>(data, args.algorithm, timer);
                    std::cout << "Sorting completed in " << timer.result() << " ms." << std::endl;
                    Sorter<char>::isCorrect(data);
                    times->add(timer.result());
                } catch (const std::exception& e) {
                    delete data;
                    throw;
                }
                delete data;
            } else {
                throw std::invalid_argument("Invalid data type");
            }
        }

        if (!args.outputFile.empty()) {
            writeDataToExcel(args.outputFile, times, args);
        }
        delete times;
    }

    // Drunk sort handler
    static void drunk(Arguments& args) {
        Timer timer;
        DynamicArray<int>* times = new DynamicArray<int>(100);

        for (int i = 0; i < 100; i++) {
            if (args.dataType == 0) { // int
                auto* data = generateData<int>(args);
                try {
                    executeDrunkSort<int>(data, args.algorithm, args.drunkLevel, timer);
                    std::cout << "Sorting completed in " << timer.result() << " ms." << std::endl;
                    Sorter<int>::isCorrect(data);
                    times->add(timer.result());
                } catch (const std::exception& e) {
                    delete data;
                    throw;
                }
                delete data;
            } else if (args.dataType == 1) { // float
                auto* data = generateData<float>(args);
                try {
                    executeDrunkSort<float>(data, args.algorithm, args.drunkLevel, timer);
                    std::cout << "Sorting completed in " << timer.result() << " ms." << std::endl;
                    Sorter<float>::isCorrect(data);
                    times->add(timer.result());
                } catch (const std::exception& e) {
                    delete data;
                    throw;
                }
                delete data;
            } else if (args.dataType == 2) { // char
                auto* data = generateData<char>(args);
                try {
                    executeDrunkSort<char>(data, args.algorithm, args.drunkLevel, timer);
                    std::cout << "Sorting completed in " << timer.result() << " ms." << std::endl;
                    Sorter<char>::isCorrect(data);
                    times->add(timer.result());
                } catch (const std::exception& e) {
                    delete data;
                    throw;
                }
                delete data;
            } else {
                throw std::invalid_argument("Invalid data type");
            }
        }

        writeDataToExcel(args.outputFile, times, args);
        delete times;
    }

    // Run tests handler
    static void runTests(Arguments& args) {
        std::cout << "Running tests..." << std::endl;

        int algorithms[] = {0, 1, 2, 3, 4, 5}; // Bubble, Merge, Insert, Binary Insert, Quick, Heap
        int dataTypes[] = {0, 1, 2}; // int, float, char
        int sizes[] = {10000, 20000, 30000, 40000, 50000};
        int distributions[] = {0, 1, 2, 3, 4}; // Random, Descending, Ascending, 33%, 66%

        for (int algorithm : algorithms) {
            for (int dataType : dataTypes) {
                for (int size : sizes) {
                    for (int distribution : distributions) {
                        Arguments testCase = {
                                .mode = Mode::BENCHMARK,
                                .algorithm = algorithm,
                                .dataType = dataType,
                                .inputFile = "",
                                .outputFile = "results.csv",
                                .size = size,
                                .distribution = distribution,
                                .drunkLevel = 0
                        };
                        try {
                            benchmark(testCase);
                        } catch (const std::exception& e) {
                            std::cerr << "Error during test: " << e.what() << std::endl;
                        }
                    }
                }
            }
        }
    }

    // Excel data writer
    static void writeDataToExcel(const std::string &filename, DynamicArray<int>* times, const Arguments &args) {
        bool fileExists = std::filesystem::exists(filename);
        bool isEmpty = fileExists && std::filesystem::file_size(filename) == 0;

        std::ofstream outputFile(filename, std::ios::app); // Open in append mode
        if (!outputFile) {
            throw std::runtime_error("Unable to open output file: " + filename);
        }

        if (!fileExists || isEmpty) {
            // Write column names if the file doesn't exist or is empty
            outputFile << "RecordNumber,MinTime,MaxTime,AvgTime,MedianTime,StdDev,Algorithm,Distribution,Size,DataType,DrunkLevel" << std::endl;
        }

        static int recordNumber = 1; // Static variable to keep track of record numbers
        Sorter<int>::quickSort(times, 0, times->getSize() - 1);
        int minTime = times->get(0);
        int maxTime = times->get(times->getSize() - 1);
        int medianTime = times->get(49);
        auto algorithm = algToString(args.algorithm);
        auto distribution = distToString(args.distribution);
        auto dataType = typeToString(args.dataType);
        float avgTime = 0;

        for(int i = 0; i < 100; i++) {
            avgTime += times->get(i);
        }

        avgTime /= 100;

        // calculate standard deviation
        float stdDev = 0;
        for(int i = 0; i < 100; i++) {
            stdDev += (times->get(i) - avgTime) * (times->get(i) - avgTime);
        }

        stdDev  = std::sqrt(stdDev / 100);
        outputFile << recordNumber++ << ","
                    << minTime << ","
                    << maxTime << ","
                    << avgTime << ","
                    << medianTime << ","
                    << stdDev << ","
                    << (args.mode == Mode::DRUNK ? algToString(args.algorithm) + " Drunk" : algToString(args.algorithm)) << ","
                    << distribution << ","
                    << args.size << ","
                    << dataType << ","
                    << args.drunkLevel
                    << std::endl;


        std::cout << "Data written to file: " << filename << std::endl;
    }

    // Helper translation methods
    static std::string algToString(int algorithm) {
        static const std::unordered_map<int, std::string> algorithmNames = {
                {0, "Bubble Sort"},
                {1, "Merge Sort"},
                {2, "Insert Sort"},
                {3, "Binary Insert Sort"},
                {4, "Quick Sort"},
                {5, "Heap Sort"},
                {6, "Shell Sort"},
        };

        auto it = algorithmNames.find(algorithm);
        return it != algorithmNames.end() ? it->second : "Unknown Algorithm";
    }

    static std::string distToString(int distribution) {
        static const std::unordered_map<int, std::string> distributionNames = {
                {0, "Random"},
                {1, "Descending"},
                {2, "Ascending"},
                {3, "33% Sorted"},
                {4, "66% Sorted"},
        };

        auto it = distributionNames.find(distribution);
        return it != distributionNames.end() ? it->second : "Unknown Type";
    }

    static std::string typeToString(int dataType) {
        static const std::unordered_map<int, std::string> typeNames = {
                {0, "Integer"},
                {1, "Float"},
                {2, "Character"},
        };

        auto it = typeNames.find(dataType);
        return it != typeNames.end() ? it->second : "Unknown Type";
    }
};

#endif // AIZOPROJEKT_HELPER_H