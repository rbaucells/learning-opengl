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
                // TODO: 'erase' the element from data
                T* temp = new T[arraySize - 1];

                memcpy(temp, data, sizeof(T) * i);
                memcpy(temp + i, data + (i + 1), ((arraySize - 1) - i) * sizeof(T));

                delete[] data;
                data = temp;
                elementCount--;
                break;
            }
        }
    }

    void reserve(const int size) {
        assert(size >= arraySize);
        T* temp = new T[size];
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

    ~List() {
        delete[] data;
    }
};
