#pragma once
#include <iostream>
#include <vector>
#include <list>
using namespace std;

template<typename K, typename V>
struct HashTable {


    // Data members
    int SIZE;
    vector<list<pair<K, V>>> table;


    // Constructor
    HashTable(int size = 101) {

        SIZE = size;
        table.resize(SIZE);
    }


    // Hash function
    int hashFunction(K key) {
        return key % SIZE;
    }


    // Insert
    void insert(K key, V value) {

        int index = hashFunction(key);

        for(auto& item : table[index]) {

            if(item.first == key) {

                item.second = value;

                return;
            }
        }
        table[index].push_back({key, value});
    }


    // Contains
    bool contains(K key) {

        int index = hashFunction(key);

        for(auto item : table[index]) {

            if(item.first == key) {
                return true;
            }
        }
        return false;
    }


    // Find
    V find(K key) {

        int index = hashFunction(key);

        for(auto item : table[index]) {

            if(item.first == key) {
                return item.second;
            }
        }
        throw runtime_error("Key not found");
    }


    // Remove
    void remove(K key) {

        int index = hashFunction(key);

        table[index].remove_if(

            [key](pair<K, V> item) {
                return item.first == key;
            }
        );
    }


    // Clear
    void clear() {

        for(int i = 0; i < SIZE; i++) {
            table[i].clear();
        }
    }


    // Size
    int size() {

        int count = 0;

        for(int i = 0; i < SIZE; i++) {
            count += table[i].size();
        }
        return count;
    }
};