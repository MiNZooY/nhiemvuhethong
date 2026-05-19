#pragma once
#include <iostream>
using namespace std;

template<typename T>
struct PriorityQueue {


    // Data members
    T* heap;
    int capacity;
    int count;


    // Constructor
    PriorityQueue(int cap = 100) {

        capacity = cap;
        count = 0;
        heap = new T[capacity];
    }


    // Destructor
    ~PriorityQueue() {
        delete[] heap;
    }


    // Heapify up
    void heapifyUp(int index) {

        while(index > 0) {

            int parent = (index - 1) / 2;

            if(heap[parent] <= heap[index]) {
                break;
            }

            swap(heap[parent], heap[index]);

            index = parent;
        }
    }


    // Heapify down
    void heapifyDown(int index) {

        while(true) {

            int left = index * 2 + 1;
            int right = index * 2 + 2;

            int smallest = index;

            if(left < count && heap[left] < heap[smallest]) {
                smallest = left;
            }

            if(right < count && heap[right] < heap[smallest]) {
                smallest = right;
            }

            if(smallest == index) {
                break;
            }

            swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }


    // Insert
    void insert(T value) {

        if(count == capacity) {
            return;
        }

        heap[count] = value;
        heapifyUp(count);
        count++;
    }


    // Extract
    T extract() {

        if(empty()) {
            throw underflow_error("Priority Queue empty");
        }

        T root = heap[0];
        heap[0] = heap[count - 1];
        count--;
        heapifyDown(0);
        return root;
    }


    // Peek
    T peek() {

        if(empty()) {
            throw underflow_error("Priority Queue empty");
        }
        return heap[0];
    }


    // Empty
    bool empty() {
        return count == 0;
    }


    // Size
    int size() {
        return count;
    }


    // Print
    void print() {

        for(int i = 0; i < count; i++) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }
};