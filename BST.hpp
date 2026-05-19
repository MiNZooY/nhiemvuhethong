#pragma once
#include <iostream>
using namespace std;

template<typename T>
struct BST {


    // Node
    struct Node {

        T data;

        Node* left;
        Node* right;

        Node(T value) {

            data = value;

            left = nullptr;
            right = nullptr;
        }
    };


    // Data members
    Node* root;


    // Constructor
    BST() {
        root = nullptr;
    }


    // Insert helper
    Node* insert(Node* node, T value) {

        if(node == nullptr) {
            return new Node(value);
        }

        if(value < node->data) {
            node->left = insert(node->left, value);
        }
        else {
            node->right = insert(node->right, value);
        }

        return node;
    }


    // Insert
    void insert(T value) {
        root = insert(root, value);
    }


    // Search helper
    bool search(Node* node, T value) {

        if(node == nullptr) {
            return false;
        }

        if(node->data == value) {
            return true;
        }

        if(value < node->data) {
            return search(node->left, value);
        }

        return search(node->right, value);
    }


    // Search
    bool search(T value) {
        return search(root, value);
    }


    // Inorder helper
    void inorder(Node* node) {

        if(node == nullptr) {
            return;
        }

        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }


    // Inorder traversal
    void inorder() {
        inorder(root);
        cout << endl;
    }
};