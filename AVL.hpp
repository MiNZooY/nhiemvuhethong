#pragma once
#include <iostream>
using namespace std;

template<typename T>
struct AVL {


    // Node
    struct Node {

        T data;
        Node* left;
        Node* right;

        int height;

        Node(T value) {

            data = value;
            left = nullptr;
            right = nullptr;
            height = 1;
        }
    };


    // Data members
    Node* root;


    // Constructor
    AVL() {
        root = nullptr;
    }


    // Height
    int height(Node* node) {

        if(node == nullptr) {
            return 0;
        }
        return node->height;
    }


    // Balance factor
    int getBalance(Node* node) {

        if(node == nullptr) {
            return 0;
        }
        return height(node->left) - height(node->right);
    }


    // Right rotate
    Node* rightRotate(Node* y) {

        Node* x = y->left;

        Node* T2 = x->right;

        x->right = y;

        y->left = T2;

        y->height =
            max(height(y->left), height(y->right)) + 1;

        x->height =
            max(height(x->left), height(x->right)) + 1;

        return x;
    }


    // Left rotate
    Node* leftRotate(Node* x) {

        Node* y = x->right;

        Node* T2 = y->left;

        y->left = x;

        x->right = T2;

        x->height =
            max(height(x->left), height(x->right)) + 1;

        y->height =
            max(height(y->left), height(y->right)) + 1;

        return y;
    }


    // Insert helper
    Node* insert(Node* node, T value) {

        if(node == nullptr) {
            return new Node(value);
        }

        if(value < node->data) {
            node->left = insert(node->left, value);
        }
        else if(value > node->data) {
            node->right = insert(node->right, value);
        }
        else {
            return node;
        }

        node->height =
            1 + max(height(node->left),
                    height(node->right));

        int balance = getBalance(node);


        // Left Left
        if(balance > 1 &&
           value < node->left->data) {
            return rightRotate(node);
        }


        // Right Right
        if(balance < -1 &&
           value > node->right->data) {
            return leftRotate(node);
        }


        // Left Right
        if(balance > 1 &&
           value > node->left->data) {

            node->left = leftRotate(node->left);

            return rightRotate(node);
        }


        // Right Left
        if(balance < -1 &&
           value < node->right->data) {

            node->right = rightRotate(node->right);

            return leftRotate(node);
        }

        return node;
    }


    // Insert
    void insert(T value) {
        root = insert(root, value);
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