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


    // Preorder Helper
    void preorder(Node* node) {
        if(node == nullptr) {
            return;
        }

        cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }


    // Preorder traversal
    void preorder() {
        preorder(root);
        cout << endl;
    }


    // Postorder helper
    void postorder(Node* node) {
        if(node == nullptr) {
            return;
        }

        postorder(node->left);
        postorder(node->right);
        cout << node->data << " ";
    }


    // Postorder traversal
    void postorder() {
        postorder(root);
        cout << endl;
    }


    // Remove helper
    Node* remove(Node* node, T value) {

    if(node == nullptr) {
        return nullptr;
    }

    if(value < node->data) {
        node->left = remove(node->left, value);
    }
    else if(value > node->data) {
        node->right = remove(node->right, value);
    }
    else {

        // Khong con
        if(node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }

        // Con ben phai
        if(node->left == nullptr) {
            Node* temp = node->right;
            delete node;
            return temp;
        }

        // Con ben trai
        if(node->right == nullptr) {
            Node* temp = node->left;
            delete node;
            return temp;
        }

        // Co 2 con
        Node* temp = findMin(node->right);

        node->data = temp->data;

        node->right = remove(node->right, temp->data);
    }

    return node;
}


    // Remove 
    void remove(T value) {
        root = remove(root, value);
    }
};