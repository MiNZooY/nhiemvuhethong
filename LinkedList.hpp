#pragma once 

#include <iostream>
using namespace std;

template<typename T> 
class LinkedList {
    private: 

    // Node
    struct Node {
        T* data;
        Node* next;

        Node(T value) {
            data = value; 
            next = nullptr;
        }
    };

    Node* head;
    int count;

    public:
    
    // Constructor
    LinkedList() {
        head == nullptr;
        count = 0;
    }

    // Destructor
    ~LinkedList() {
        clear();
    }

    // Insert at front 
    void insertFront (T value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        count++;
    }

    // Insert at back
    void insertBack (T value) {
        Node* newNode = new Node(value);
        if(head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            
            temp->next = newNode;
        }
        count++;
    }

    // Insert at index 
    void insertAt(int index, T value) {
        if (index < 0 || index > count) return;
        if (index == 0) {
            insertFront(value);
            return;
        }
        
        Node* newNode = new Node(value);
        Node* temp = head;
        for (int i = 0; i < index - 1; i++) {
            temp = temp->next;
        }

        newNode->next = temp->next;
        temp->next = newNode;
        count++;
    }


    // Remove value
    void remove(T value) {
        if (head == nullptr) return;
        if (head->data == value) {
            Node* del = head;
            head = head->next;
            delete del;
            count--;
        }
        
        Node* temp = head;
        while(temp->next != nullptr && temp->next->data != value) {
            temp = temp->next;
        }

        if(temp->next != nullptr) {
            Node* del = temp->next;
            temp->next = del->next;
            delete del;
            count--;
        }
    }

    // Remove at index 
    void removeAt(int index) {
        if (index < 0 || index >= count) return;
        if (index == 0) {
            Node* del = head;
            head = head->next;
            delete del;
            count--;
            return;
        }

        Node* temp = head;
        for(int i = 0; i < index - 1; i++) {
            temp = temp->next;
        }

        Node* del = temp->next;
        temp->next = del->next;
        delete del;
        count--;
    }

    // Find Value
    bool find(T value) {
        Node* temp = head;
        while(temp->next != nullptr) {
            if (temp->data == value) return true;
            temp = temp->next;
        }
        return false;
    }

    // Print
    void print() {
        Node* temp = head;
        while(temp != nullptr) {
            cout << temp->data << ' ';
            temp = temp->next;
        }

        cout << endl;
    }


    // Size
    int size() {
        return count;
    }


    // Clear list
    void clear() {
        while(head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }

    // Check empty
    bool empty() {
        return head == nullptr;
    }
};