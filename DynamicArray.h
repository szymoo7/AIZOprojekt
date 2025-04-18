//
// Created by szyme on 18.04.2025.
//

#ifndef AIZOPROJEKT_DYNAMICARRAY_H
#define AIZOPROJEKT_DYNAMICARRAY_H


#include <cstddef>
#include <stdexcept>

template <typename T>
class DynamicArray {
private:
    T* data;            // Pointer to dynamically allocated array
    size_t size;        // Current number of elements
    size_t capacity;    // Total capacity of the array

    // Helper function to reallocate memory with new capacity
    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];

        // Copy existing elements to new array
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        // Delete old array
        delete[] data;

        // Update pointers and capacity
        data = newData;
        capacity = newCapacity;
    }

public:
    // Constructor with optional initial capacity
    DynamicArray(size_t initialCapacity = 10) : size(0), capacity(initialCapacity) {
        data = new T[capacity];
    }

    // Destructor
    ~DynamicArray() {
        delete[] data;
    }

    // Add element to the end of array
    void add(const T& element) {
        // Check if we need to increase capacity
        if (size == capacity) {
            resize(capacity * 2);
        }

        // Add element and increase size
        data[size] = element;
        size++;
    }

    // Insert element at specified index
    void insert(size_t index, const T& element) {
        // Check if index is valid
        if (index > size) {
            throw std::out_of_range("Index out of range for insertion");
        }

        // Check if we need to increase capacity
        if (size == capacity) {
            resize(capacity * 2);
        }

        // Shift elements to make space for the new element
        for (size_t i = size; i > index; i--) {
            data[i] = data[i - 1];
        }

        // Insert element and increase size
        data[index] = element;
        size++;
    }

    // Delete element at specified index
    void deleteAt(size_t index) {
        // Check if index is valid
        if (index >= size) {
            throw std::out_of_range("Index out of range for deletion");
        }

        // Shift elements to remove the element
        for (size_t i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }

        // Decrease size
        size--;

        // Optional: Shrink the array if it's too empty
        if (size < capacity / 4 && capacity > 10) {
            resize(capacity / 2);
        }
    }

    // Access element at index
    T & get(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        T& value = data[index];
        return value;
    }

    // Set element at index
    void set(size_t index, const T& element) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        data[index] = element;
    }

    // Get current number of elements
    size_t getSize() const {
        return size;
    }

    // Get current capacity
    size_t getCapacity() const {
        return capacity;
    }
};


#endif //AIZOPROJEKT_DYNAMICARRAY_H
