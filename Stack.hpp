#pragma once

#include <iostream>
using namespace std;

template<typename T>
class Stack {
private:
    T* data;
    int topIndex;
    int capacity;

public:
    Stack(int cap = 100) {
        capacity = cap;
        data = new T[capacity];
        topIndex = -1;
    }

    ~Stack() {
        delete[] data;
    }

    void push(const T& value) {
        if(topIndex == capacity - 1) return;
        data[++topIndex] = value;
    }

    T pop() {
        return data[topIndex--];
    }

    T top() {
        return data[topIndex];
    }

    bool empty() {
        return topIndex == -1;
    }

    int size() {
        return topIndex + 1;
    }
};