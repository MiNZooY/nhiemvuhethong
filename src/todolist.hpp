#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Include the DSA library headers
#include "../lib/LinkedList.hpp"
#include "../lib/Stack.hpp"
#include "../lib/Queue.hpp"
#include "../lib/PriorityQueue.hpp"
#include "../lib/HashTable.hpp"
#include "../lib/Algorithms.hpp"

/**
 * @struct Task
 * @brief Represents a task in the TodoList application.
 * Overloads comparison operators to integrate smoothly with PriorityQueue (min-heap) and Algorithms (sorting).
 */
struct Task {
    int id;                 // Unique task ID
    string title;           // Task title
    string description;     // Short description
    int priority;           // Priority level (e.g., 1 = High, 2 = Medium, 3 = Low)
    string dueDate;         // YYYY-MM-DD
    bool isCompleted;       // Completion status

    // Default constructor
    Task() : id(0), title(""), description(""), priority(3), dueDate(""), isCompleted(false) {}

    Task(int id, string title, string desc, int priority, string dueDate)
        : id(id), title(title), description(desc), priority(priority), dueDate(dueDate), isCompleted(false) {}

    // Operator overloads for sorting/heaps
    // Order rules: 
    // 1. Incompleted tasks < Completed tasks
    // 2. Tasks with due dates < Tasks without due dates
    // 3. By Date (ascending)
    // 4. By Priority (ascending: 1 -> 3)
    // 5. By ID (ascending)
    bool operator<(const Task& other) const {
        if (isCompleted != other.isCompleted) return !isCompleted;
        
        int len1 = dueDate.length();
        int len2 = other.dueDate.length();
        
        if (len1 == 0 || len2 == 0) {
            if (len1 != len2) return len1 > len2;
        } else {
            int minLen = (len1 < len2) ? len1 : len2;
            string sub1 = dueDate.substr(0, minLen);
            string sub2 = other.dueDate.substr(0, minLen);
            
            if (sub1 != sub2) {
                return sub1 < sub2; // Chronological comparison
            }
            
            if (len1 != len2) {
                return len1 > len2; // Specificity comparison
            }
        }
        
        if (priority != other.priority) return priority < other.priority;
        return id < other.id;
    }

    bool operator==(const Task& other) const {
        return id == other.id;
    }

    bool operator<=(const Task& other) const {
        return (*this < other) || (*this == other);
    }

    bool operator>(const Task& other) const {
        return !(*this <= other);
    }

    bool operator>=(const Task& other) const {
        return !(*this < other);
    }

    bool operator!=(const Task& other) const {
        return id != other.id;
    }
};

/**
 * @struct TodoListApp
 * @brief Holds the application state container using 5+ DSA library components.
 */
struct TodoListApp {
    // 1. LinkedList (Procedural Node Pointer): Stores active tasks dynamically
    Node<Task>* taskListHead;

    // 2. HashTable: Maps task ID (int) -> Task for O(1) retrieval
    HashTable<int, Task> taskLookup;

    // 3. PriorityQueue: Min-Heap for fetching the highest-priority tasks
    PriorityQueue<Task> urgentTasks;

    // 4. Stack: Undo history of deleted tasks
    Stack<Task> undoStack;

    // 5. Queue: Today's scheduled task execution pipeline
    Queue<Task> todayQueue;

    int nextTaskId;
};

// Forward declaration of load and save helpers
void loadApp(TodoListApp& app);
void saveApp(const TodoListApp& app);
void viewAllTasks(const TodoListApp& app);

/**
 * @brief Initializes the TodoListApp state and loads saved data.
 */
void initApp(TodoListApp& app) {
    app.taskListHead = nullptr;
    app.nextTaskId = 1;
    loadApp(app);
}

/**
 * @brief Cleans up, saves data, and deallocates the TodoListApp state.
 */
void destroyApp(TodoListApp& app) {
    saveApp(app);
    clear(app.taskListHead);
}

/**
 * @brief Saves current active tasks to file.
 */
void saveApp(const TodoListApp& app) {
    string dataDir = "data";
    string filePath = "data/todo_data.txt";
    if (fs::exists("../src")) {
        dataDir = "../data";
        filePath = "../data/todo_data.txt";
    }

    if (!fs::exists(dataDir)) {
        fs::create_directory(dataDir);
    }
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        cout << "Warning: Could not open todo_data.txt for writing!" << endl;
        return;
    }

    // Write header
    outFile << "# ID|Title|Description|Priority|DueDate|IsCompleted\n";

    Node<Task>* temp = app.taskListHead;
    while (temp != nullptr) {
        Task t = temp->data;
        outFile << t.id << "|"
                << t.title << "|"
                << t.description << "|"
                << t.priority << "|"
                << t.dueDate << "|"
                << (t.isCompleted ? 1 : 0) << "\n";
        temp = temp->next;
    }
    outFile.close();
}

/**
 * @brief Sorts tasks inside the linked list based on Task operator< rules.
 */
void sortTasks(TodoListApp& app) {
    int count = size(app.taskListHead);
    if (count <= 1) return;

    Task* arr = new Task[count];
    Node<Task>* curr = app.taskListHead;
    int i = 0;
    while (curr != nullptr) {
        arr[i++] = curr->data;
        curr = curr->next;
    }

    quickSort(arr, 0, count - 1);
    clear(app.taskListHead);

    for (int j = 0; j < count; ++j) {
        insertBack(app.taskListHead, arr[j]);
    }

    delete[] arr;
}

/**
 * @brief Re-indexes all task IDs sequentially from 1 to N and rebuilds structures.
 */
void reindexTasks(TodoListApp& app) {
    HashTable<int, int> oldToNew;
    int newId = 1;
    Node<Task>* temp = app.taskListHead;
    while (temp) {
        oldToNew.insert(temp->data.id, newId);
        temp->data.id = newId;
        newId++;
        temp = temp->next;
    }
    app.nextTaskId = newId;

    // Rebuild lookup
    app.taskLookup.clear();
    temp = app.taskListHead;
    while(temp) {
        app.taskLookup.insert(temp->data.id, temp->data);
        temp = temp->next;
    }

    // Rebuild Priority Queue
    while(!app.urgentTasks.empty()) app.urgentTasks.extract();
    temp = app.taskListHead;
    while(temp) {
        if (!temp->data.isCompleted) {
           app.urgentTasks.insert(temp->data);
        }
        temp = temp->next;
    }

    // Rebuild todayQueue
    int qSize = app.todayQueue.size();
    for(int i=0; i<qSize; i++) {
        Task t = app.todayQueue.dequeue();
        if (oldToNew.contains(t.id)) {
            t.id = oldToNew.find(t.id);
            app.todayQueue.enqueue(t);
        }
    }
}

/**
 * @brief Unified helper to synchronize sort, reindex, and save operations.
 */
void syncData(TodoListApp& app, bool silent = true) {
    sortTasks(app);
    reindexTasks(app);
    saveApp(app);
    if (!silent) {
        viewAllTasks(app);
    }
}

/**
 * @brief Loads tasks from file.
 */
void loadApp(TodoListApp& app) {
    // Reset app state
    clear(app.taskListHead);
    app.taskListHead = nullptr;
    app.taskLookup.clear();
    app.urgentTasks.count = 0;
    app.todayQueue.frontIndex = 0;
    app.todayQueue.rearIndex = -1;
    app.undoStack.topIndex = -1;
    app.nextTaskId = 1;

    string filePath = "data/todo_data.txt";
    if (fs::exists("../src")) {
        filePath = "../data/todo_data.txt";
    }

    ifstream inFile(filePath);
    if (!inFile.is_open()) return;

    string line;
    while (getline(inFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        string idStr, title, desc, priorityStr, dueDate, completedStr;

        if (getline(ss, idStr, '|') &&
            getline(ss, title, '|') &&
            getline(ss, desc, '|') &&
            getline(ss, priorityStr, '|') &&
            getline(ss, dueDate, '|') &&
            getline(ss, completedStr)) {

            int id = stoi(idStr);
            int priority = stoi(priorityStr);
            bool completed = (completedStr == "1");

            Task t(id, title, desc, priority, dueDate);
            t.isCompleted = completed;

            // Raw insertion, bypass syncData because we are loading
            insertBack(app.taskListHead, t);
        }
    }
    inFile.close();
    
    // Sort and re-index naturally ensures loaded data matches correct format
    syncData(app, true);
}

/**
 * @brief Normalizes date string (e.g. 2026-2-7 to 2026-02-07, also converts / to -)
 */
string formatDateString(string dateStr) {
    if (dateStr.empty() || dateStr == "clear" || dateStr == "none") {
        return dateStr;
    }
    
    for(char& c : dateStr) {
        if (c == '/') c = '-';
    }
    
    stringstream ss(dateStr);
    string part;
    vector<string> parts;
    
    while (getline(ss, part, '-')) {
        parts.push_back(part);
    }
    
    string result = "";
    for (size_t i = 0; i < parts.size(); i++) {
        string p = parts[i];
        if (i > 0 && p.length() == 1) { // Pad month and day
            p = "0" + p;
        }
        result += p;
        if (i < parts.size() - 1) {
            result += "-";
        }
    }
    
    return result;
}

/**
 * @brief Validates if a formatted date string represents a real date.
 */
bool isValidDate(const string& dateStr) {
    if (dateStr.empty() || dateStr == "clear" || dateStr == "none") return true;

    stringstream ss(dateStr);
    string part;
    vector<string> parts;
    
    while (getline(ss, part, '-')) {
        parts.push_back(part);
    }
    
    if (parts.size() > 3 || parts.size() == 0) return false;
    
    try {
        int year = stoi(parts[0]);
        if (year < 1900 || year > 2100) return false;
        
        if (parts.size() >= 2) {
            int month = stoi(parts[1]);
            if (month < 1 || month > 12) return false;
            
            if (parts.size() == 3) {
                int day = stoi(parts[2]);
                if (day < 1) return false;
                
                int maxDays = 31;
                if (month == 4 || month == 6 || month == 9 || month == 11) maxDays = 30;
                else if (month == 2) {
                    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
                    maxDays = isLeap ? 29 : 28;
                }
                if (day > maxDays) return false;
            }
        }
    } catch (...) {
        return false;
    }
    
    return true;
}

/**
 * @brief Adds a new task to the manager.
 */
void addTask(TodoListApp& app, const string& title, const string& desc, int priority, const string& dueDate) {
    string formattedDate = formatDateString(dueDate);
    Task newTask(app.nextTaskId++, title, desc, priority, formattedDate);
    
    insertBack(app.taskListHead, newTask);
    syncData(app, true);
    
    cout << "Added task successfully! (Tasks have been auto-sorted and re-indexed)" << endl;
}

/**
 * @brief Displays all active tasks.
 */
void viewAllTasks(const TodoListApp& app) {
    if (empty(app.taskListHead)) {
        cout << "No tasks available." << endl;
        return;
    }

    cout << "\n=== All Active Tasks ===" << endl;
    Node<Task>* temp = app.taskListHead;
    while (temp != nullptr) {
        Task t = temp->data;
        cout << "[" << t.id << "] " << t.title 
             << " | Priority: " << t.priority 
             << " | Due: " << (t.dueDate.empty() ? "None" : t.dueDate)
             << " | Completed: " << (t.isCompleted ? "Yes" : "No") << endl;
        temp = temp->next;
    }
    cout << "========================\n" << endl;
}

/**
 * @brief Removes a task by ID.
 */
void deleteTask(TodoListApp& app, int id) {
    if (!app.taskLookup.contains(id)) {
        cout << "Task ID " << id << " not found." << endl;
        return;
    }

    Task taskToDelete = app.taskLookup.find(id);
    remove(app.taskListHead, taskToDelete);

    try {
        app.undoStack.push(taskToDelete);
    } catch (...) {}
    
    syncData(app, true);
    cout << "Deleted task with old ID: " << id << " and re-indexed remaining tasks." << endl;
}

/**
 * @brief Restores the last deleted task.
 */
void undoDelete(TodoListApp& app) {
    if (app.undoStack.empty()) {
        cout << "Nothing to undo." << endl;
        return;
    }

    Task restoredTask = app.undoStack.pop();
    insertBack(app.taskListHead, restoredTask);
    syncData(app, true);
    
    cout << "Restored task: " << restoredTask.title << " (Tasks have been auto-sorted and re-indexed)" << endl;
    viewAllTasks(app);
}

/**
 * @brief Retrieves the next highest-priority task.
 */
void processNextUrgentTask(TodoListApp& app) {
    if (app.urgentTasks.empty()) {
        cout << "No urgent tasks remaining." << endl;
        return;
    }

    Task nextTask = app.urgentTasks.extract();
    cout << "\n>>> NEXT URGENT TASK TO DO: " << endl;
    cout << "[" << nextTask.id << "] " << nextTask.title 
         << " (Priority: " << nextTask.priority << ", Due: " << nextTask.dueDate << ")" << endl;
}

/**
 * @brief Edits an existing task.
 */
void editTask(TodoListApp& app, int id, const string& newTitle, const string& newDesc, int newPriority, const string& newDueDate) {
    if (!app.taskLookup.contains(id)) {
        cout << "Task ID " << id << " not found." << endl;
        return;
    }

    string formattedDate = formatDateString(newDueDate);

    Node<Task>* temp = app.taskListHead;
    while (temp) {
        if (temp->data.id == id) {
            if (!newTitle.empty()) temp->data.title = newTitle;
            if (!newDesc.empty()) temp->data.description = newDesc;
            if (newPriority >= 1 && newPriority <= 3) temp->data.priority = newPriority;
            if (!formattedDate.empty()) {
                if (formattedDate == "clear" || formattedDate == "none") {
                    temp->data.dueDate = "";
                } else {
                    temp->data.dueDate = formattedDate;
                }
            }
            break;
        }
        temp = temp->next;
    }

    syncData(app, true);
    cout << "Task [" << id << "] edited successfully! Data automatically re-sorted and re-indexed." << endl;
}

/**
 * @brief Mark a task as completed
 */
void markTaskCompleted(TodoListApp& app, int id) {
    if (!app.taskLookup.contains(id)) {
        cout << "Task ID " << id << " not found." << endl;
        return;
    }
    
    Task t = app.taskLookup.find(id);
    if (t.isCompleted) {
        cout << "Task is already completed!" << endl;
        return;
    }
    
    Node<Task>* temp = app.taskListHead;
    while (temp) {
        if (temp->data.id == id) {
            temp->data.isCompleted = true;
            break;
        }
        temp = temp->next;
    }
    
    syncData(app, true);
    cout << "Task [" << id << "] marked as completed! Data automatically re-sorted and re-indexed." << endl;
}
