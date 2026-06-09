#include "todolist.hpp"
#include <iostream>
#include <string>

using namespace std;

void showMenu() {
    cout << "\n==============================================" << endl;
    cout << "           TODO LIST APPLICATION              " << endl;
    cout << "==============================================" << endl;
    cout << " 1. Add New Task" << endl;
    cout << " 2. View All Tasks" << endl;
    cout << " 3. Search Task by ID" << endl;
    cout << " 4. Delete Task" << endl;
    cout << " 5. Undo Last Deletion" << endl;
    cout << " 6. View Next Urgent Task" << endl;
    cout << " 7. Add Task to Today's Queue" << endl;
    cout << " 8. Complete First Task in Today's Queue" << endl;
    cout << " 9. Sort All Tasks by Priority" << endl;
    cout << " 0. Exit" << endl;
    cout << "==============================================" << endl;
    cout << "Enter your choice (0-9): ";
}

int main() {
    TodoListApp app;
    initApp(app);

    cout << "Welcome to the Course Project TodoList App!" << endl;
    cout << "Procedural C++ Templates Library Integration Demo." << endl;

    string choiceStr;
    int choice = -1;

    do {
        showMenu();
        getline(cin, choiceStr);
        if (choiceStr.empty()) continue;

        try {
            choice = stoi(choiceStr);
        } catch(...) {
            cout << "\n[Error] Invalid input! Please enter a number between 0 and 9." << endl;
            continue;
        }

        switch (choice) {
            case 1: {
                string title, desc, dueDate, priorityStr;
                int priority = 3;

                cout << "\n--- Add New Task ---" << endl;
                cout << "Enter title: ";
                getline(cin, title);
                cout << "Enter description: ";
                getline(cin, desc);
                
                cout << "Enter priority (1=High, 2=Medium, 3=Low): ";
                getline(cin, priorityStr);
                try {
                    priority = stoi(priorityStr);
                    if (priority < 1 || priority > 3) priority = 3;
                } catch(...) {
                    cout << "  -> Invalid priority. Defaulting to 3 (Low)." << endl;
                    priority = 3;
                }

                cout << "Enter due date (e.g. YYYY-MM-DD): ";
                getline(cin, dueDate);

                addTask(app, title, desc, priority, dueDate);
                break;
            }
            case 2:
                viewAllTasks(app);
                break;
            case 3: {
                string idStr;
                int id;
                cout << "Enter task ID to search: ";
                getline(cin, idStr);
                try {
                    id = stoi(idStr);
                    searchTaskById(app, id);
                } catch(...) {
                    cout << "[Error] Invalid ID format." << endl;
                }
                break;
            }
            case 4: {
                string idStr;
                int id;
                cout << "Enter task ID to delete: ";
                getline(cin, idStr);
                try {
                    id = stoi(idStr);
                    deleteTask(app, id);
                } catch(...) {
                    cout << "[Error] Invalid ID format." << endl;
                }
                break;
            }
            case 5:
                undoDelete(app);
                break;
            case 6:
                processNextUrgentTask(app);
                break;
            case 7: {
                string idStr;
                int id;
                cout << "Enter task ID to add to today's queue: ";
                getline(cin, idStr);
                try {
                    id = stoi(idStr);
                    queueForToday(app, id);
                } catch(...) {
                    cout << "[Error] Invalid ID format." << endl;
                }
                break;
            }
            case 8:
                completeTodayTask(app);
                break;
            case 9:
                sortTasksByPriority(app);
                break;
            case 0:
                cout << "\nSaving data... Exiting. Thank you for using TodoList App!" << endl;
                break;
            default:
                cout << "\n[Error] Invalid choice! Please select between 0 and 9." << endl;
        }
    } while (choice != 0);

    destroyApp(app);
    return 0;
}
