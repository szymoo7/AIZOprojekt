#ifndef AIZOPROJEKT_HELPER_H
#define AIZOPROJEKT_HELPER_H

#include <string>
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "DynamicArray.h"
#include "ArgumentParser.h"
#include "Sorter.h"
#include "Timer.h"

class Helper {
public:



    static void runWithArguments(Arguments& args) {
        switch (args.mode) {
            case Mode::HELP:
                ArgumentParser::printHelp();
                break;

            case Mode::FILE_TEST: {
                fileTest(args);
                break;
            }

            case Mode::BENCHMARK: {
                benchmark(args);
                break;
            }

            default:
                throw std::invalid_argument("Invalid mode");
        }

    }

private:
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

    template <typename T>
    static DynamicArray<T>* generateTestData(const Arguments& args) {
        auto* data = new DynamicArray<T>(args.size);
        for (int i = 0; i < args.size; ++i) {
            if (args.dataType == 0) { // int
                data->insert(i, rand() % (std::numeric_limits<int>::max()));
            } else if (args.dataType == 1) { // float
                data->insert(i, (float)(rand()) / (float)(rand()));
            }
        }
        std::cout << "Test data generated with size: " << args.size << std::endl;
        writeDataToFile("before.txt", data);
        return data;
    }

    template<typename T>
    static DynamicArray<T>* generateTestDataDescending(const Arguments& args) {
        auto* data = generateTestData<T>(args);
        // Sort the data in descending order using Bubble Sort
        Sorter<T>::bubbleSortDesc(data);
        std::cout << "Test data generated in descending order with size: " << args.size << std::endl;
        writeDataToFile("before.txt", data);
        return data;
    }

    template<typename T>
    static DynamicArray<T>* generateTestDataAscending(const Arguments& args) {
        auto* data = generateTestData<T>(args);
        // Sort the data in ascending order using Bubble Sort
        Sorter<T>::bubbleSort(data);
        std::cout << "Test data generated in ascending order with size: " << args.size << std::endl;
        writeDataToFile("before.txt", data);
        return data;
    }

    template <typename T>
    static DynamicArray<T>* generateTestData_33(const Arguments& args) {
        auto* data = generateTestData<T>(args);

        // Sort the first 33% of the data using Bubble Sort
        size_t oneThirdSize = data->getSize() / 3;
        Sorter<T>::bubbleSortPartial(data, 0, oneThirdSize);
        std::cout << "Test data generated with 33% sorted with size: " << args.size << std::endl;
        writeDataToFile("before.txt", data);
        return data;
    }

    template <typename T>
    static DynamicArray<T>* generateTestData_66(const Arguments& args) {
        auto* data = generateTestData<T>(args);

        // Sort the first 66% of the data using Bubble Sort
        size_t twoThirdSize = data->getSize() * 2 / 3;
        Sorter<T>::bubbleSortPartial(data, 0, twoThirdSize);
        std::cout << "Test data generated with 66% sorted with size: " << args.size << std::endl;
        writeDataToFile("before.txt", data);
        return data;
    }

    static void fileTest(const Arguments& args) {
        if (args.algorithm == 0) { // Bubble Sort
            if (args.dataType == 0) { // int
                auto* data = readDataFromFile<int>(args.inputFile);
                Sorter<int>::bubbleSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<int>::isCorrect(data);
                delete data;
            } else if (args.dataType == 1) { // float
                auto* data = readDataFromFile<float>(args.inputFile);
                Sorter<float>::bubbleSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<float>::isCorrect(data);
                delete data;
            }
        } else if (args.algorithm == 1) { // Merge Sort
            if (args.dataType == 0) { // int
                auto* data = readDataFromFile<int>(args.inputFile);
                Sorter<int>::mergeSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<int>::isCorrect(data);
                delete data;
            } else if (args.dataType == 1) { // float
                auto* data = readDataFromFile<float>(args.inputFile);
                Sorter<float>::mergeSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<float>::isCorrect(data);
                delete data;
            }
        } else if (args.algorithm == 2) { // Insert Sort
            if (args.dataType == 0) { // int
                auto* data = readDataFromFile<int>(args.inputFile);
                Sorter<int>::insertSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<int>::isCorrect(data);
                delete data;
            } else if (args.dataType == 1) { // float
                auto* data = readDataFromFile<float>(args.inputFile);
                Sorter<float>::insertSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<float>::isCorrect(data);
                delete data;
            }
        } else if (args.algorithm == 3) { // Binary Insert Sort
            if (args.dataType == 0) { // int
                auto* data = readDataFromFile<int>(args.inputFile);
                Sorter<int>::binaryInsertSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<int>::isCorrect(data);
                delete data;
            } else if (args.dataType == 1) { // float
                auto* data = readDataFromFile<float>(args.inputFile);
                Sorter<float>::binaryInsertSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<float>::isCorrect(data);
                delete data;
            }
        }
        else if (args.algorithm == 4) { // Quick Sort
            if (args.dataType == 0) { // int
                auto* data = readDataFromFile<int>(args.inputFile);
                Sorter<int>::quickSort(data, 0, data->getSize() - 1);
                writeDataToFile(args.outputFile, data);
                Sorter<int>::isCorrect(data);
                delete data;
            } else if (args.dataType == 1) { // float
                auto* data = readDataFromFile<float>(args.inputFile);
                Sorter<float>::quickSort(data, 0, data->getSize() - 1);
                writeDataToFile(args.outputFile, data);
                Sorter<float>::isCorrect(data);
                delete data;
            }
        } else if (args.algorithm == 5) { // Heap Sort
            if (args.dataType == 0) { // int
                auto* data = readDataFromFile<int>(args.inputFile);
                Sorter<int>::heapSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<int>::isCorrect(data);
                delete data;
            } else if (args.dataType == 1) { // float
                auto* data = readDataFromFile<float>(args.inputFile);
                Sorter<float>::heapSort(data);
                writeDataToFile(args.outputFile, data);
                Sorter<float>::isCorrect(data);
                delete data;
            }
        }
        else {
            if (args.mode == Mode::FILE_TEST) {
                throw std::invalid_argument("Invalid algorithm choice for FILE_TEST mode");
            }
        }
    }

    static void benchmark(const Arguments& args) {
        Timer timer;
        DynamicArray<int>* times = new DynamicArray<int>(100);
        for(int i = 0; i < 100; i++) {

            switch (args.dataType) {
                case 0: {  // Added braces to create a new scope
                    DynamicArray<int> *data = nullptr;
                    switch (args.distribution) {
                        case 0:
                            data = generateTestData<int>(args);
                            break;
                        case 1:
                            data = generateTestDataDescending<int>(args);
                            break;
                        case 2:
                            data = generateTestDataAscending<int>(args);
                            break;
                        case 3:
                            data = generateTestData_33<int>(args);
                            break;
                        case 4:
                            data = generateTestData_66<int>(args);
                            break;
                        default:
                            throw std::invalid_argument("Invalid distribution choice");
                    }

                    switch (args.algorithm) {
                        case 0: // Bubble Sort
                            timer.start();
                            Sorter<int>::bubbleSort(data);
                            timer.stop();
                            break;
                        case 1: // Merge Sort
                            timer.start();
                            Sorter<int>::mergeSort(data);
                            timer.stop();
                            break;
                        case 2: // Insert Sort
                            timer.start();
                            Sorter<int>::insertSort(data);
                            timer.stop();
                            break;
                        case 3: // Binary Insert Sort
                            timer.start();
                            Sorter<int>::binaryInsertSort(data);
                            timer.stop();
                            break;
                        case 4: // Quick Sort
                            timer.start();
                            Sorter<int>::quickSort(data, 0, data->getSize() - 1);
                            timer.stop();
                            break;
                        case 5: // Heap Sort
                            timer.start();
                            Sorter<int>::heapSort(data);
                            timer.stop();
                            break;
                        default:
                            delete data;  // Added cleanup before throwing
                            throw std::invalid_argument("Invalid algorithm choice for BENCHMARK mode");
                    }

                    // Write sorted data to file if output file is specified
//                if (!args.outputFile.empty()) {
//                    writeDataToFile(args.outputFile, data);
//                }


                    std::cout << "Sorting completed in " << timer.result() << " ms." << std::endl;
                    Sorter<int>::isCorrect(data);

                    delete data;  // Added memory cleanup
                    break;
                }
                case 1: {  // Added braces to create a new scope
                    DynamicArray<float> *dataFloat = nullptr;
                    switch (args.distribution) {
                        case 0:
                            dataFloat = generateTestData<float>(args);
                            break;
                        case 1:
                            dataFloat = generateTestDataDescending<float>(args);
                            break;
                        case 2:
                            dataFloat = generateTestDataAscending<float>(args);
                            break;
                        case 3:
                            dataFloat = generateTestData_33<float>(args);
                            break;
                        case 4:
                            dataFloat = generateTestData_66<float>(args);
                            break;
                        default:
                            throw std::invalid_argument("Invalid distribution choice");
                    }
                    switch (args.algorithm) {
                        case 0: // Bubble Sort
                            timer.start();
                            Sorter<float>::bubbleSort(dataFloat);
                            timer.stop();
                            break;
                        case 1: // Merge Sort
                            timer.start();
                            Sorter<float>::mergeSort(dataFloat);
                            timer.stop();
                            break;
                        case 2: // Insert Sort
                            timer.start();
                            Sorter<float>::bubbleSort(dataFloat);
                            timer.stop();
                            break;
                        default:
                            delete dataFloat;  // Added cleanup before throwing
                            throw std::invalid_argument("Invalid algorithm choice for BENCHMARK mode");
                    }

                    // Write sorted data to file if output file is specified
                    if (!args.outputFile.empty()) {
                        writeDataToExcel(args.outputFile, times, args);
                    }

                    // Output benchmark results
                    std::cout << "Sorting completed in " << timer.result() << " ms." << std::endl;
                    Sorter<float>::isCorrect(dataFloat);
                    delete dataFloat;  // Added memory cleanup
                    break;
                }
                default:
                    throw std::invalid_argument("Invalid data type");
            }

            times->add(timer.result());
        }
        if (!args.outputFile.empty()) {
            writeDataToExcel(args.outputFile, times, args);
        }
        delete times;
    }

    static void writeDataToExcel(const std::string &filename, DynamicArray<int>* times, const Arguments &args) {
        bool fileExists = std::filesystem::exists(filename);
        bool isEmpty = fileExists && std::filesystem::file_size(filename) == 0;

        std::ofstream outputFile(filename, std::ios::app); // Open in append mode
        if (!outputFile) {
            throw std::runtime_error("Unable to open output file: " + filename);
        }

        if (!fileExists || isEmpty) {
            // Write column names if the file doesn't exist or is empty
            outputFile << "RecordNumber,MinTime,MaxTime,AvgTime,MedianTime,Algorithm,Distribution,Size,DataType" << std::endl;
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


        outputFile << recordNumber++ << ","
                   << minTime << ","
                   << maxTime << ","
                   << avgTime << ","
                   << medianTime << ","
                   << algorithm << ","
                   << distribution << ","
                   << args.size << ","
                   << dataType << std::endl;



        std::cout << "Data written to file: " << filename << std::endl;
    }

    static std::string algToString(int algorithm) {
        static const std::unordered_map<int, std::string> algorithmNames = {
                {0, "Bubble Sort"},
                {1, "Merge Sort"},
                {2, "Insert Sort"},
                {3, "Binary Insert Sort"},
                {4, "Quick Sort"},
                {5, "Heap Sort"}
        };

        auto it = algorithmNames.find(algorithm);
        return it != algorithmNames.end() ? it->second : "Unknown Algorithm";
    }

    static std::string distToString(int algorithm) {
        static const std::unordered_map<int, std::string> algorithmNames = {
                {0, "Random"},
                {1, "Descending"},
                {2, "Ascending"},
                {3, "33% Sorted"},
                {4, "66% Sorted"},
        };

        auto it = algorithmNames.find(algorithm);
        return it != algorithmNames.end() ? it->second : "Unknown Type";
    }

    static std::string typeToString(int algorithm) {
        static const std::unordered_map<int, std::string> algorithmNames = {
                {0, "Integer"},
                {1, "Float"},
        };

        auto it = algorithmNames.find(algorithm);
        return it != algorithmNames.end() ? it->second : "Unknown Type";
    }



//    static void analyze(const Arguments& args) {
//        benchmark(args);
//    }

};

#endif // AIZOPROJEKT_HELPER_H