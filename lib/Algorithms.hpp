#pragma once 

#include<iostream>
using namespace std;

// Bubble Sort 
template<typename T, typename compare = less<T>>
void bubbleSort(T arr[], int n, compare comp = compare()) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if(comp(arr[j + 1], arr[j]))
                swap(arr[j], arr[j + 1]);
        }
    }
}


// Quick Sort
template<typename T, typename compare = less<T>>
int partition (T arr[], int low, int high, compare comp = compare()) {
    T pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if(comp(arr[j], pivot)) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

template<typename T, typename compare = less<T>>
void quickSort(T arr[], int low, int high, compare comp = compare()) {
    if (low < high) {
        int pi = partition(arr, low, high, comp);
        quickSort(arr, low, pi - 1, comp);
        quickSort(arr, pi + 1, high, comp);
    }
}


// Merge Sort
template<typename T, typename compare = less<T>>
void merge(T arr[], int left, int mid, int right, compare comp = compare()) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    T* L = new T(n1);
    T* R = new T(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + 1];
    }

    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if(comp(L[i], R[j])) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }

    delete [] L;
    delete [] R;
}

template<typename T, typename compare = less<T>>
void mergeSort(T arr[], int left, int right, compare comp = compare()) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid, comp);
    mergeSort(arr, mid + 1, right, comp);
    merge(arr, left, mid, right, comp);
}


// Linear Search
template<typename T, typename compare = less<T>>
int linearSearch(T arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key);
            return i;
    }
    return -1;
} 


// Binary Search 
template<typename T, typename compare = less<T>>
int binarySearch(T arr[], int n, int key) {
    int left = 0;
    int right = n - 1;
    while(left <= right) {
        int mid = left + (right - left) / 2;
        if(arr[mid] == key) return mid;
        if(arr[mid] < key) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}