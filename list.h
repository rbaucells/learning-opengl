#pragma once
#include <cassert>
#include <cstring>
#include <iostream>

template<typename T>
class List {
private:
    T *data;

    // how many elements
    int elementCount = 0;

    // how big is data array
    int arraySize = 1;

public:
    List() {
        data = new T[1];
    }

    explicit List(const int startSize) {
        data = new T[startSize];
        arraySize = startSize;
    }

    void add(const T &object) {
        // we need to copy and things
        if (elementCount >= arraySize) {
            reserve(arraySize * 2);
        }

        data[elementCount] = object;
        elementCount++;
    }

    void remove(const T &object) {
        assert(arraySize > 0);
        // [1, 2, 3, 4, 5] -> [1, 2, 4, 5]
        // object = 3
        // objIndex = 2

        for (int i = 0; i < elementCount; i++) {
            T element = data[i];
            if (element == object) {
                arraySize--;
                T *temp = new T[arraySize];

                memcpy(temp, data, sizeof(T) * i);
                memcpy(temp + i, data + (i + 1), (arraySize - i) * sizeof(T));

                delete[] data;
                data = temp;
                elementCount--;
                break;
            }
        }
    }

    void reserve(const int size) {
        assert(size >= arraySize);
        T *temp = new T[size];
        memcpy(temp, data, sizeof(T) * arraySize);
        delete[] data;
        data = temp;
        arraySize = size;
    }

    void shrinkToFit() {
        reserve(arraySize);
    }

    void clear() {
        delete[] data;
        data = new T[1];
        elementCount = 0;
        arraySize = 1;
    }

    void printState() {
        std::cout << "List Information: \n";

        std::cout << "  Element Count: " << elementCount << "\n";
        std::cout << "  Array Size: " << arraySize << "\n";

        std::string dataString = "[ ";

        for (int i = 0; i < elementCount; i++) {
            dataString.append(std::to_string(data[i]));
            if (i + 1 != elementCount)
                dataString.append(", ");
            else
                dataString.append(" ");
        }

        dataString.append("] \n");

        std::cout << "  " << dataString;
    }

    T at(int index) {
        return data[index];
    }
    T operator[](const int index) {
        return at(index);
    }

    T at(int index) const {
        return data[index];
    }
    T operator[](const int index) const {
        return at(index);
    }

    /**
     * @return How many elements are in the list
     */
    int count() const {
        return elementCount;
    }

    /**
     * @return how many 'slots' are in the array; size of the array
     */
    int size() const {
        return arraySize;
    }

    void operator+=(const T& object) {
        add(object);
    }

    void operator<<(const T& object) {
        add(object);
    }

    void operator-=(const T& object) {
        remove(object);
    }

    /**
     * @param object What are you trying to find
     * @return The index of that object in the array
     * @note Returns -1 if not found
     */
    int find(const T& object) const {
        for (int i = 0; i < elementCount; i++) {
            if (data[i] == object)
                return i;
        }

        return -1;
    }

    ~List() {
        delete[] data;
    }
};
