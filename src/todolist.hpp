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

    // Default constructor (required for heap arrays/data buffers in library structs)
    Task() : id(0), title(""), description(""), priority(3), dueDate(""), isCompleted(false) {}

    Task(int id, string title, string desc, int priority, string dueDate)
        : id(id), title(title), description(desc), priority(priority), dueDate(dueDate), isCompleted(false) {}

    // Operator overloads for sorting/heaps (Priority Queue)
    // PriorityQueue in the library is a Min-Heap: heap[parent] <= heap[index].
    // If we want high-priority tasks (priority value = 1) at the root, we want them to compare "smaller".
    bool operator<(const Task& other) const {
        return priority < other.priority;
    }

    bool operator<=(const Task& other) const {
        return priority <= other.priority;
    }

    bool operator>(const Task& other) const {
        return priority > other.priority;
    }

    bool operator>=(const Task& other) const {
        return priority >= other.priority;
    }

    bool operator==(const Task& other) const {
        return id == other.id;
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
    // Note: K must support % operator (int is perfect). HashTable size 101 by default.
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
    if (!fs::exists("data")) {
        fs::create_directory("data");
    }
    ofstream outFile("data/todo_data.txt");
    if (!outFile.is_open()) {
        outFile.open("../data/todo_data.txt");
    }
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
 * @brief Loads tasks from file.
 */
void loadApp(TodoListApp& app) {
    // Reset app state
    clear(app.taskListHead);
    app.taskListHead = nullptr;
    app.taskLookup.clear();
    // Safely reset state indices/counters instead of assigning temporaries (to avoid double-free/dangling pointers)
    app.urgentTasks.count = 0;
    app.todayQueue.frontIndex = 0;
    app.todayQueue.rearIndex = -1;
    app.undoStack.topIndex = -1;
    app.nextTaskId = 1;

    ifstream inFile("data/todo_data.txt");
    if (!inFile.is_open()) {
        inFile.open("../data/todo_data.txt");
    }
    if (!inFile.is_open()) {
        return; // File doesn't exist yet, normal on first boot
    }

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

            // Insert into local structures
            insertBack(app.taskListHead, t);
            app.taskLookup.insert(t.id, t);
            if (t.priority == 1) {
                app.urgentTasks.insert(t);
            }

            if (id >= app.nextTaskId) {
                app.nextTaskId = id + 1;
            }
        }
    }
    inFile.close();
}

/**
 * @brief Adds a new task to the manager.
 * Integrates: LinkedList (insertBack) and HashTable (insert).
 */
void addTask(TodoListApp& app, const string& title, const string& desc, int priority, const string& dueDate) {
    Task newTask(app.nextTaskId++, title, desc, priority, dueDate);
    
    // 1. Insert into LinkedList
    insertBack(app.taskListHead, newTask);
    
    // 2. Insert into HashTable for lookup
    app.taskLookup.insert(newTask.id, newTask);

    // 3. Insert into Priority Queue if it is high priority (priority == 1)
    if (priority == 1) {
        app.urgentTasks.insert(newTask);
    }

    saveApp(app);
    cout << "Added task successfully! ID: " << newTask.id << endl;
}

/**
 * @brief Displays all active tasks.
 * Integrates: LinkedList traversal.
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
             << " | Due: " << t.dueDate 
             << " | Completed: " << (t.isCompleted ? "Yes" : "No") << endl;
        temp = temp->next;
    }
    cout << "========================\n" << endl;
}

/**
 * @brief Finds a task using its ID in O(1) average time.
 * Integrates: HashTable (find).
 */
void searchTaskById(TodoListApp& app, int id) {
    try {
        if (app.taskLookup.contains(id)) {
            Task t = app.taskLookup.find(id);
            cout << "\n--- Task Found (ID: " << t.id << ") ---" << endl;
            cout << "Title: " << t.title << endl;
            cout << "Description: " << t.description << endl;
            cout << "Priority: " << t.priority << endl;
            cout << "Due Date: " << t.dueDate << endl;
            cout << "Status: " << (t.isCompleted ? "Completed" : "In Progress") << endl;
        } else {
            cout << "Task ID " << id << " not found in lookup table." << endl;
        }
    } catch (const exception& e) {
        cout << "Error during lookup: " << e.what() << endl;
    }
}

/**
 * @brief Removes a task by ID. Stores the removed task in the undo stack.
 * Integrates: LinkedList (remove), HashTable (remove), and Stack (push).
 */
void deleteTask(TodoListApp& app, int id) {
    if (!app.taskLookup.contains(id)) {
        cout << "Task ID " << id << " not found." << endl;
        return;
    }

    // 1. Get task from HashTable lookup
    Task taskToDelete = app.taskLookup.find(id);

    // 2. Remove from LinkedList
    remove(app.taskListHead, taskToDelete);

    // 3. Remove from HashTable
    app.taskLookup.remove(id);

    // 4. Push to Undo Stack
    try {
        app.undoStack.push(taskToDelete);
    } catch (...) {
        // Stack capacity reached, ignore
    }

    saveApp(app);
    cout << "Deleted task with ID: " << id << " (Can be undone using undo)." << endl;
}

/**
 * @brief Restores the last deleted task.
 * Integrates: Stack (pop, empty), LinkedList (insertBack), and HashTable (insert).
 */
void undoDelete(TodoListApp& app) {
    if (app.undoStack.empty()) {
        cout << "Nothing to undo." << endl;
        return;
    }

    // 1. Pop from stack
    Task restoredTask = app.undoStack.pop();

    // 2. Insert back to list
    insertBack(app.taskListHead, restoredTask);

    // 3. Insert back to lookup
    app.taskLookup.insert(restoredTask.id, restoredTask);

    saveApp(app);
    cout << "Restored task: [" << restoredTask.id << "] " << restoredTask.title << endl;
}

/**
 * @brief Retrieves the next highest-priority task.
 * Integrates: PriorityQueue (extract, empty).
 */
void processNextUrgentTask(TodoListApp& app) {
    if (app.urgentTasks.empty()) {
        cout << "No urgent tasks remaining." << endl;
        return;
    }

    Task nextTask = app.urgentTasks.extract();
    cout << "\n>>> NEXT URGENT TASK TO DO: " << endl;
    cout << "[" << nextTask.id << "] " << nextTask.title 
         << " (Priority: " << nextTask.priority << ")" << endl;
}

/**
 * @brief Enqueues a task to today's work pipeline.
 * Integrates: Queue (enqueue).
 */
void queueForToday(TodoListApp& app, int id) {
    if (!app.taskLookup.contains(id)) {
        cout << "Task ID " << id << " not found." << endl;
        return;
    }

    Task taskToQueue = app.taskLookup.find(id);
    try {
        app.todayQueue.enqueue(taskToQueue);
        cout << "Task \"" << taskToQueue.title << "\" added to today's workflow." << endl;
    } catch (...) {
        cout << "Today's queue is full!" << endl;
    }
}

/**
 * @brief Serves/completes the first task in today's pipeline.
 * Integrates: Queue (dequeue, empty).
 */
void completeTodayTask(TodoListApp& app) {
    if (app.todayQueue.empty()) {
        cout << "No tasks scheduled for today." << endl;
        return;
    }

    Task t = app.todayQueue.dequeue();
    cout << "Completed task: \"" << t.title << "\" from today's workflow!" << endl;
}

/**
 * @brief Sorts active tasks by priority using quickSort from the library.
 * Integrates: Algorithms (quickSort).
 */
void sortTasksByPriority(TodoListApp& app) {
    int count = size(app.taskListHead);
    if (count <= 1) return;

    // 1. Copy linked list nodes to a dynamic array
    Task* arr = new Task[count];
    Node<Task>* curr = app.taskListHead;
    int i = 0;
    while (curr != nullptr) {
        arr[i++] = curr->data;
        curr = curr->next;
    }

    // 2. Run quickSort from Algorithms.hpp
    // Compares tasks by priority (1 is highest, 3 is lowest).
    quickSort(arr, 0, count - 1);

    // 3. Clear existing list structure
    clear(app.taskListHead);

    // 4. Rebuild the list with sorted tasks
    for (int j = 0; j < count; ++j) {
        insertBack(app.taskListHead, arr[j]);
    }

    delete[] arr;
    saveApp(app);
    cout << "Sorted tasks by priority successfully!" << endl;
    viewAllTasks(app);
}
