#pragma once
#include <iostream>
using namespace std;

template<typename T>
struct Queue {


    // Data members
    T* data;
    int frontIndex;
    int rearIndex;
    int capacity;


    // Constructor
    Queue(int cap = 100) {
        capacity = cap;
        data = new T[capacity];
        frontIndex = 0;
        rearIndex = -1;
    }


    // Destructor
    ~Queue() {
        delete[] data;
    }


    // Enqueue
    void enqueue(T value) {

        if(rearIndex == capacity - 1) {
            return;
        }
        data[++rearIndex] = value;
    }


    // Dequeue
    T dequeue() {

        if(empty()) {
            throw underflow_error("Queue empty");
        }
        return data[frontIndex++];
    }


    // Front
    T front() {

        if(empty()) {
            throw underflow_error("Queue empty");
        }
        return data[frontIndex];
    }


    // Empty
    bool empty() {
        return frontIndex > rearIndex;
    }


    // Size
    int size() {
        return rearIndex - frontIndex + 1;
    }


    // Print
    void print() {

        for(int i = frontIndex; i <= rearIndex; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }
};