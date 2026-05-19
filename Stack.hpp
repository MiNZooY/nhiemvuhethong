#pragma once
#include <iostream>
using namespace std;

template<typename T>
struct Stack {


    // Data members
    T* data;
    int topIndex;
    int capacity;


    // Constructor
    Stack(int cap = 100) {
        capacity = cap;
        data = new T[capacity];
        topIndex = -1;
    }


    // Destructor
    ~Stack() {
        delete[] data;
    }


    // Push
    void push(T value) {

        if(topIndex == capacity - 1) {
            return;
        }
        data[++topIndex] = value;
    }


    // Pop
    T pop() {

        if(empty()) {
            throw underflow_error("Stack empty");
        }
        return data[topIndex--];
    }


    // Top
    T top() {

        if(empty()) {
            throw underflow_error("Stack empty");
        }
        return data[topIndex];
    }


    // Empty
    bool empty() {
        return topIndex == -1;
    }


    // Size
    int size() {
        return topIndex + 1;
    }


    // Print
    void print() {

        for(int i = topIndex; i >= 0; i--) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};