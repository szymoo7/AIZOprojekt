#ifndef AIZOPROJEKT_SORTER_H
#define AIZOPROJEKT_SORTER_H

#include <iostream>
#include "DynamicArray.h"

template <typename T>
class Sorter {
public:
    // Bubble Sort
    static void bubbleSort(DynamicArray<T>* data) {
        for (size_t i = 0; i < data->getSize() - 1; ++i) {
            for (size_t j = 0; j < data->getSize() - i - 1; ++j) {
                if (data->get(j) > data->get(j + 1)) {
                    T temp = data->get(j);
                    data->set(j, data->get(j + 1));
                    data->set(j + 1, temp);
                }
            }
        }
    }

    static void bubbleSortPartial(DynamicArray<T>* data, size_t start, size_t end) {
        for (size_t i = start; i < end - 1; ++i) {
            for (size_t j = start; j < end - (i - start) - 1; ++j) {
                if (data->get(j) > data->get(j + 1)) {
                    T temp = data->get(j);
                    data->set(j, data->get(j + 1));
                    data->set(j + 1, temp);
                }
            }
        }
    }

    static void bubbleSortDesc(DynamicArray<T>* data) {
        for (size_t i = 0; i < data->getSize() - 1; ++i) {
            for (size_t j = 0; j < data->getSize() - i - 1; ++j) {
                if (data->get(j) < data->get(j + 1)) {
                    T temp = data->get(j);
                    data->set(j, data->get(j + 1));
                    data->set(j + 1, temp);
                }
            }
        }
    }

    // Merge Sort
    static void mergeSort(DynamicArray<T>* data) {
        mergeSortHelper(data, 0, data->getSize() - 1);
    }

    // Insert Sort
    static void insertSort(DynamicArray<T>* data) {
        if (data->getSize() <= 1) return;

        for (size_t i = 1; i < data->getSize(); i++) {
            T key = data->get(i);
            data->deleteAt(i);  // Remove current element

            // Find the correct position in the sorted portion
            size_t j = 0;
            while (j < i && data->get(j) <= key) {
                j++;
            }

            // Insert the element at the correct position
            data->insert(j, key);
        }
    }

    // Binary Insert Sort
    static void binaryInsertSort(DynamicArray<T>* data) {
        if (data->getSize() <= 1) return;

        for (size_t i = 1; i < data->getSize(); i++) {
            T key = data->get(i);

            // Find the correct position using binary search
            size_t left = 0;
            size_t right = i;

            while (left < right) {
                size_t mid = left + (right - left) / 2;
                if (data->get(mid) <= key) {
                    left = mid + 1;
                } else {
                    right = mid;
                }
            }

            // If the element should be inserted before its current position
            if (left < i) {
                data->deleteAt(i);
                data->insert(left, key);
            }
        }
    }

    // Quick Sort
    static void quickSort(DynamicArray<T>* data, int low, int high) {
        if (low < high) {
            int pi = partition(data, low, high);
            quickSort(data, low, pi - 1);
            quickSort(data, pi + 1, high);
        }
    }

    // Heap Sort
    static void heapSort(DynamicArray<T>* data) {
        int n = data->getSize();
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(data, n, i);
        }
        for (int i = n - 1; i > 0; i--) {
            T temp = data->get(0);
            data->set(0, data->get(i));
            data->set(i, temp);
            heapify(data, i, 0);
        }
    }

    // Check if the array is sorted
    static void isCorrect(const DynamicArray<T>* data) {
        for (size_t i = 0; i < data->getSize() - 1; ++i) {
            if (data->get(i) > data->get(i + 1)) {
                std::cout << "Array is not sorted correctly." << std::endl;
            }
        }
        std::cout << "Array is sorted correctly." << std::endl;
    }

private:
    // Helper for Merge Sort
    static void mergeSortHelper(DynamicArray<T>* data, size_t left, size_t right) {
        if (left < right) {
            size_t mid = left + (right - left) / 2;

            // Recursively sort the two halves
            mergeSortHelper(data, left, mid);
            mergeSortHelper(data, mid + 1, right);

            // Merge the sorted halves
            merge(data, left, mid, right);
        }
    }

    // Merge function for Merge Sort
    static void merge(DynamicArray<T>* data, size_t left, size_t mid, size_t right) {
        size_t n1 = mid - left + 1;
        size_t n2 = right - mid;

        T* leftArray = new T[n1];
        T* rightArray = new T[n2];

        for (size_t i = 0; i < n1; ++i) {
            leftArray[i] = data->get(left + i);
        }
        for (size_t j = 0; j < n2; ++j) {
            rightArray[j] = data->get(mid + 1 + j);
        }

        size_t i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (leftArray[i] <= rightArray[j]) {
                data->set(k, leftArray[i]);
                ++i;
            } else {
                data->set(k, rightArray[j]);
                ++j;
            }
            ++k;
        }

        while (i < n1) {
            data->set(k, leftArray[i]);
            ++i;
            ++k;
        }

        while (j < n2) {
            data->set(k, rightArray[j]);
            ++j;
            ++k;
        }

        delete[] leftArray;
        delete[] rightArray;
    }

    // Partition function for Quick Sort
    static int partition(DynamicArray<T>* data, int low, int high) {
        T pivot = data->get(high);
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            if (data->get(j) < pivot) {
                i++;
                T temp = data->get(i);
                data->set(i, data->get(j));
                data->set(j, temp);
            }
        }
        T temp = data->get(i + 1);
        data->set(i + 1, data->get(high));
        data->set(high, temp);
        return (i + 1);
    }

    // Heapify function for Heap Sort
    static void heapify(DynamicArray<T>* data, int n, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && data->get(left) > data->get(largest)) {
            largest = left;
        }
        if (right < n && data->get(right) > data->get(largest)) {
            largest = right;
        }
        if (largest != i) {
            T temp = data->get(i);
            data->set(i, data->get(largest));
            data->set(largest, temp);
            heapify(data, n, largest);
        }
    }

};

#endif // AIZOPROJEKT_SORTER_H