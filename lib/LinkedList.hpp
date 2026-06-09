#pragma once 

#include <iostream>

template<typename T> 
struct Node {
    T data;
    Node* next;
};

// Create a new node with data
template<typename T>
Node<T>* createNode(T value) {
    Node<T>* newNode = new Node<T>();
    newNode->data = value;
    newNode->next = nullptr;
    return newNode;
}

// Insert at front
template<typename T>
void insertFront(Node<T>*& head, T value) {
    Node<T>* newNode = createNode(value);
    newNode->next = head;
    head = newNode;
}

// Insert at back
template<typename T>
void insertBack(Node<T>*& head, T value) {
    Node<T>* newNode = createNode(value);
    if (head == nullptr) {
        head = newNode;
        return;
    }
    Node<T>* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Insert at index
template<typename T>
void insertAt(Node<T>*& head, int index, T value) {
    if (index < 0) return;
    if (index == 0) {
        insertFront(head, value);
        return;
    }
    Node<T>* temp = head;
    for (int i = 0; i < index - 1; i++) {
        if (temp == nullptr) return; // index out of bounds
        temp = temp->next;
    }
    if (temp == nullptr) return;
    Node<T>* newNode = createNode(value);
    newNode->next = temp->next;
    temp->next = newNode;
}

// Remove value (first occurrence)
template<typename T>
void remove(Node<T>*& head, T value) {
    if (head == nullptr) return;
    if (head->data == value) {
        Node<T>* del = head;
        head = head->next;
        delete del;
        return;
    }
    Node<T>* temp = head;
    while (temp->next != nullptr && temp->next->data != value) {
        temp = temp->next;
    }
    if (temp->next != nullptr) {
        Node<T>* del = temp->next;
        temp->next = del->next;
        delete del;
    }
}

// Remove at index
template<typename T>
void removeAt(Node<T>*& head, int index) {
    if (head == nullptr || index < 0) return;
    if (index == 0) {
        Node<T>* del = head;
        head = head->next;
        delete del;
        return;
    }
    Node<T>* temp = head;
    for (int i = 0; i < index - 1; i++) {
        if (temp->next == nullptr) return;
        temp = temp->next;
    }
    if (temp->next == nullptr) return;
    Node<T>* del = temp->next;
    temp->next = del->next;
    delete del;
}

// Find value
template<typename T>
bool find(Node<T>* head, T value) {
    Node<T>* temp = head;
    while (temp != nullptr) {
        if (temp->data == value) return true;
        temp = temp->next;
    }
    return false;
}

// Print list
template<typename T>
void print(Node<T>* head) {
    Node<T>* temp = head;
    while (temp != nullptr) {
        std::cout << temp->data << ' ';
        temp = temp->next;
    }
    std::cout << std::endl;
}

// Size of list
template<typename T>
int size(Node<T>* head) {
    int count = 0;
    Node<T>* temp = head;
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Clear list
template<typename T>
void clear(Node<T>*& head) {
    while (head != nullptr) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }
}

// Check empty
template<typename T>
bool empty(Node<T>* head) {
    return head == nullptr;
}