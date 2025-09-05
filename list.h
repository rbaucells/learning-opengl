#pragma once
#include <cassert>
#include <cstring>
#include <iostream>

template<typename T>
class List {
private:
    T *dataArray;

    // how many elements
    int elementCount = 0;

    // how big is data array
    int arraySize = 1;

public:
    List() {
        dataArray = new T[1];
    }

    explicit List(const int startSize) {
        dataArray = new T[startSize];
        arraySize = startSize;
    }

    List(std::initializer_list<T> elements) {
        dataArray = new T[elements.size()];

        std::copy(elements.begin(), elements.end(), dataArray);
        elementCount = elements.size();
        arraySize = elements.size();
    };

    void add(const T &object) {
        // we need to copy and things
        if (elementCount >= arraySize) {
            reserve(arraySize * 2);
        }

        dataArray[elementCount] = object;
        elementCount++;
    }

    void addAt(const T &object, const int index) {
        if (elementCount + 1 >= arraySize)
            reserve(arraySize * 2);

        for (int i = elementCount + 1; i > index; i--) {
            dataArray[i] = dataArray[i - 1];
        }

        dataArray[index] = object;
        elementCount++;
    }

    void remove(const T &object) {
        assert(arraySize > 0);
        // [1, 2, 3, 4, 5] -> [1, 2, 4, 5]
        // object = 3
        // objIndex = 2

        for (int i = 0; i < elementCount; i++) {
            T element = dataArray[i];
            if (element == object) {
                arraySize--;
                T *temp = new T[arraySize];

                memcpy(temp, dataArray, sizeof(T) * i);
                memcpy(temp + i, dataArray + (i + 1), (arraySize - i) * sizeof(T));

                delete[] dataArray;
                dataArray = temp;
                elementCount--;
                break;
            }
        }
    }

    void removeAt(const int index) {
        assert(arraySize > 0);
        // [1, 2, 3, 4, 5] -> [1, 2, 4, 5]
        // object = 3
        // objIndex = 2

        T element = dataArray[index];
        arraySize--;
        T *temp = new T[arraySize];

        memcpy(temp, dataArray, sizeof(T) * index);
        memcpy(temp + index, dataArray + (index + 1), (arraySize - index) * sizeof(T));

        delete[] dataArray;
        dataArray = temp;
        elementCount--;
    }

    void removeNoAlloc(const T &object) {
        // [1, 2, 3, 4, 5] -> [1, 2, 4, 5]
        // object = 3
        // objIndex = 2
        bool shifting = false;

        for (int i = 0; i < elementCount; i++) {
            if (shifting) {
                dataArray[i - 1] = dataArray[i];
            }
            else {
                if (dataArray[i] == object) {
                    shifting = true;
                }
            }
        }

        // if we were shifting, we found something
        if (shifting)
            elementCount--;
    }

    void removeNoAllocAt(const int index) {
        // [1, 2, 3, 4, 5] -> [1, 2, 4, 5]
        // object = 3
        // objIndex = 2

        for (int i = index + 1; i < elementCount; i++) {
            dataArray[i - 1] = dataArray[i];
        }

        elementCount--;
    }

    void reserve(const int size) {
        assert(size >= arraySize);
        T *temp = new T[size];
        memcpy(temp, dataArray, sizeof(T) * arraySize);
        delete[] dataArray;
        dataArray = temp;
        arraySize = size;
    }

    void shrinkToFit() {
        reserve(elementCount);
    }

    void clear() {
        delete[] dataArray;
        dataArray = new T[1];
        elementCount = 0;
        arraySize = 1;
    }

    void printState() {
        std::cout << "List Information: \n";

        std::cout << "  Element Count: " << elementCount << "\n";
        std::cout << "  Array Size: " << arraySize << "\n";

        std::string dataString = "[ ";

        for (int i = 0; i < elementCount; i++) {
            dataString.append(std::to_string(dataArray[i]));
            if (i + 1 != elementCount)
                dataString.append(", ");
            else
                dataString.append(" ");
        }

        dataString.append("] \n");

        std::cout << "  " << dataString;
    }

    T at(int index) {
        return dataArray[index];
    }

    T operator[](const int index) {
        return at(index);
    }

    T at(int index) const {
        return dataArray[index];
    }

    T operator[](const int index) const {
        return at(index);
    }

    /**
     * @return How many elements are in the list
     */
    [[nodiscard]] int count() const {
        return elementCount;
    }

    /**
     * @return how many 'slots' are in the array; size of the array
     */
    [[nodiscard]] int size() const {
        return arraySize;
    }

    [[nodiscard]] bool empty() const {
        return count() > 0;
    }

    void operator+=(const T &object) {
        add(object);
    }

    void operator<<(const T &object) {
        add(object);
    }

    void operator-=(const T &object) {
        remove(object);
    }

    /**
     * @param object What are you trying to find
     * @return The index of that object in the array
     * @note Returns -1 if not found
     */
    int find(const T &object) const {
        for (int i = 0; i < elementCount; i++) {
            if (dataArray[i] == object)
                return i;
        }

        return -1;
    }

    T *data() {
        return dataArray;
    }

    ~List() {
        delete[] dataArray;
    }
};
