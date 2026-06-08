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

    T* L = new T[n1];
    T* R = new T[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
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


// Heap Sort
template<typename T, typename compare = less<T>>
void heapify(T arr[], int n, int i, compare comp = compare()) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && comp(arr[largest], arr[l]))
        largest = l;

    if (r < n && comp(arr[largest], arr[r]))
        largest = r;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest, comp);
    }
}

template<typename T, typename compare = less<T>>
void heapSort(T arr[], int n, compare comp = compare()) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, comp);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, comp);
    }
}


// Selection Sort
template<typename T, typename compare = less<T>>
void selectionSort(T arr[], int n, compare comp = compare()) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (comp(arr[j], arr[min_idx]))
                min_idx = j;
        }
        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
        }
    }
}


// Insertion Sort
template<typename T, typename compare = less<T>>
void insertionSort(T arr[], int n, compare comp = compare()) {
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && comp(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}


// Counting Sort
template<typename T, typename compare = less<T>>
void countingSort(T arr[], int n, compare comp = compare()) {
    if (n <= 1) return;
    T maxVal = arr[0];
    T minVal = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxVal) maxVal = arr[i];
        if (arr[i] < minVal) minVal = arr[i];
    }

    int range = static_cast<int>(maxVal - minVal + 1);
    int* count = new int[range]();
    T* output = new T[n];

    for (int i = 0; i < n; i++) {
        count[static_cast<int>(arr[i] - minVal)]++;
    }

    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        output[count[static_cast<int>(arr[i] - minVal)] - 1] = arr[i];
        count[static_cast<int>(arr[i] - minVal)]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    delete[] count;
    delete[] output;
}


// Helper for Radix Sort
template<typename T>
void countingSortForRadix(T arr[], int n, int exp) {
    T* output = new T[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++) {
        count[(static_cast<int>(arr[i]) / exp) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        output[count[(static_cast<int>(arr[i]) / exp) % 10] - 1] = arr[i];
        count[(static_cast<int>(arr[i]) / exp) % 10]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    delete[] output;
}

// Radix Sort
template<typename T, typename compare = less<T>>
void radixSort(T arr[], int n, compare comp = compare()) {
    if (n <= 1) return;
    T maxVal = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxVal) {
            maxVal = arr[i];
        }
    }

    for (int exp = 1; static_cast<int>(maxVal) / exp > 0; exp *= 10) {
        countingSortForRadix(arr, n, exp);
    }
}


// Linear Search
template<typename T, typename compare = less<T>>
int linearSearch(T arr[], int n, const T& key, compare comp = compare()) {
    for (int i = 0; i < n; i++) {
        if (!comp(arr[i], key) && !comp(key, arr[i]))
            return i;
    }
    return -1;
} 


// Binary Search 
template<typename T, typename compare = less<T>>
int binarySearch(T arr[], int n, const T& key, compare comp = compare()) {
    int left = 0;
    int right = n - 1;
    while(left <= right) {
        int mid = left + (right - left) / 2;
        if(!comp(arr[mid], key) && !comp(key, arr[mid])) return mid;
        if(comp(arr[mid], key)) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}