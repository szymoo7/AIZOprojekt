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

    // Drunk Bubble Sort
    static void drunkBubbleSort(DynamicArray<T>* data, int drunkLevel) {

        size_t n = data->getSize();

        // Pijana część — działa z błędami, losowo
        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = 0; j < n - i - 1; ++j) {
                int randomValue = rand() % 100;

                // Czasami nie porównujemy — jesteśmy "rozkojarzeni"
                if (randomValue < drunkLevel * 5) {
                    continue;
                }

                // Normalna zamiana
                if (data->get(j) > data->get(j + 1)) {
                    T temp = data->get(j);
                    data->set(j, data->get(j + 1));
                    data->set(j + 1, temp);
                } else {
                    // Może przypadkowo zamienimy dobry porządek?
                    if (randomValue < drunkLevel * 2) {
                        T temp = data->get(j);
                        data->set(j, data->get(j + 1));
                        data->set(j + 1, temp);
                    }
                }
            }
        }

        // Wytrzeźwienie — jedna 100% trzeźwa iteracja żeby wszystko naprawić
        bool sorted = false;
        while (!sorted) {
            sorted = true;
            for (size_t j = 0; j < n - 1; ++j) {
                if (data->get(j) > data->get(j + 1)) {
                    T temp = data->get(j);
                    data->set(j, data->get(j + 1));
                    data->set(j + 1, temp);
                    sorted = false;
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

    // Quick Sort descending
    static void quickSortDesc(DynamicArray<T>* data, int low, int high) {
        if (low < high) {
            int pi = partition(data, low, high);
            quickSortDesc(data, low, pi - 1);
            quickSortDesc(data, pi + 1, high);
        }
    }

    static void drunkQuickSort(DynamicArray<T>* data, int low, int high, int drunkLevel) {
        if (low < high) {
            int pi = drunkPartition(data, low, high, drunkLevel);

            int newDrunkLevel = std::max(0, drunkLevel - 1);

            drunkQuickSort(data, low, pi - 1, newDrunkLevel);
            drunkQuickSort(data, pi + 1, high, newDrunkLevel);
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

    // Shell Sort
    static void shellSort(DynamicArray<T>* data) {
        int n = data->getSize();
        for (int gap = n / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < n; i++) {
                T temp = data->get(i);
                int j;
                for (j = i; j >= gap && data->get(j - gap) > temp; j -= gap) {
                    data->set(j, data->get(j - gap));
                }
                data->set(j, temp);
            }
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

    // Descending partition function for Quick Sort
    static int partitionDesc(DynamicArray<T>* data, int low, int high) {
        T pivot = data->get(high);
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            if (data->get(j) > pivot) {
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

    static int drunkPartition(DynamicArray<T>* data, int low, int high, int drunkLevel) {
        int pivotIndex = high;

        if (drunkLevel > 0) {
            int range = high - low + 1;
            int offset = rand() % std::min(range, drunkLevel + 1);
            pivotIndex = low + offset;
        }

        T pivotValue = data->get(pivotIndex);
        data->set(pivotIndex, data->get(high));
        data->set(high, pivotValue);

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