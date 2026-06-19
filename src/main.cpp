#include "todolist.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue() {
    cout << "\nPress Enter to return to menu...";
    string dummy;
    getline(cin, dummy);
}

void showMenu() {
    cout << "==============================================" << endl;
    cout << "           TODO LIST APPLICATION              " << endl;
    cout << "==============================================" << endl;
    cout << " 1. Add New Task" << endl;
    cout << " 2. View All Tasks" << endl;
    cout << " 3. Delete Task" << endl;
    cout << " 4. Undo Last Deletion" << endl;
    cout << " 5. View Next Urgent Task" << endl;
    cout << " 6. Edit Task" << endl;
    cout << " 7. Mark Task as Completed" << endl;
    cout << " 0. Exit" << endl;
    cout << "==============================================" << endl;
    cout << "Enter your choice (0-7): ";
}

int main() {
    TodoListApp app;
    initApp(app);

    string choiceStr;
    int choice = -1;

    do {
        clearScreen();
        showMenu();
        getline(cin, choiceStr);
        if (choiceStr.empty()) continue;

        try {
            choice = stoi(choiceStr);
        } catch(...) {
            cout << "\n[Error] Invalid input! Please enter a number between 0 and 7." << endl;
            pressEnterToContinue();
            continue;
        }

        if (choice == 0) {
            clearScreen();
            cout << "\nSaving data... Exiting. Thank you for using TodoList App!" << endl;
            break;
        }

        clearScreen(); // Clear screen before showing task output

        switch (choice) {
            case 1: {
                string title, desc, dueDate, priorityStr;
                int priority = 3;

                cout << "--- Add New Task ---" << endl;
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

                while (true) {
                    cout << "Enter due date (YYYY-MM-DD or leave blank): ";
                    getline(cin, dueDate);
                    if (isValidDate(formatDateString(dueDate))) {
                        break;
                    }
                    cout << "  -> [Error] Invalid date! Please enter a real date (e.g., 2026-02-07)." << endl;
                }

                string project;
                cout << "Enter project/goal (or leave blank): ";
                getline(cin, project);

                addTask(app, title, desc, priority, dueDate, project);
                break;
            }
            case 2: {
                viewAllTasks(app);
                cout << "\nOptions: [s] Sort tasks manually | [Enter] Return to menu: ";
                string opt;
                getline(cin, opt);
                if (opt == "s" || opt == "S") {
                    cout << "\n--- Sort Tasks ---" << endl;
                    cout << "1. Alphabetical (A-Z, Case-Insensitive)" << endl;
                    cout << "2. Alphabetical (Z-A)" << endl;
                    cout << "3. By Priority (Highest first)" << endl;
                    cout << "Choose sorting method (1-3): ";
                    string sortChoiceStr;
                    getline(cin, sortChoiceStr);
                    
                    bool sorted = false;
                    if (sortChoiceStr == "1") {
                        sortTasks(app, CompareTaskTitleAscCaseInsensitive());
                        cout << "\nTasks sorted Alphabetically (A-Z)." << endl;
                        sorted = true;
                    } else if (sortChoiceStr == "2") {
                        sortTasks(app, CompareTaskTitleDesc());
                        cout << "\nTasks sorted Alphabetically (Z-A)." << endl;
                        sorted = true;
                    } else if (sortChoiceStr == "3") {
                        sortTasks(app, CompareTaskPriority());
                        cout << "\nTasks sorted by Priority." << endl;
                        sorted = true;
                    } else {
                        cout << "\n[Error] Invalid sorting choice." << endl;
                    }
                    
                    if (sorted) {
                        reindexTasks(app);
                        saveApp(app);
                        cout << "\n";
                        viewAllTasks(app); // Show the freshly sorted list
                    }
                }
                break;
            }
            case 3: {
                string idStr;
                int id;
                cout << "--- Delete Task ---" << endl;
                viewAllTasks(app); // Show list of tasks so user knows what to delete
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
            case 4:
                cout << "--- Undo Deletion ---" << endl;
                undoDelete(app);
                break;
            case 5:
                cout << "--- Next Urgent Task ---" << endl;
                processNextUrgentTask(app);
                break;
            case 6: {
                string idStr;
                int id;
                cout << "--- Edit Task ---" << endl;
                viewAllTasks(app);
                cout << "Enter task ID to edit: ";
                getline(cin, idStr);
                try {
                    id = stoi(idStr);
                    if (!app.taskLookup.contains(id)) {
                        cout << "[Error] Task ID not found." << endl;
                        break;
                    }
                    Task t = app.taskLookup.find(id);
                    
                    cout << "\nEditing Task [" << t.id << "] " << t.title << endl;
                    cout << "Leave field empty and press Enter to keep current value." << endl;
                    
                    string title, desc, priorityStr, dueDate;
                    
                    cout << "New title (current: " << t.title << "): ";
                    getline(cin, title);
                    
                    cout << "New description (current: " << t.description << "): ";
                    getline(cin, desc);
                    
                    cout << "New priority (1=High, 2=Medium, 3=Low, current: " << t.priority << "): ";
                    getline(cin, priorityStr);
                    int priority = -1;
                    if (!priorityStr.empty()) {
                        try { priority = stoi(priorityStr); } catch(...) {}
                    }
                    
                    while (true) {
                        cout << "New due date (current: " << (t.dueDate.empty() ? "None" : t.dueDate) << ") [Type 'clear' to remove]: ";
                        getline(cin, dueDate);
                        if (dueDate.empty() || isValidDate(formatDateString(dueDate))) {
                            break;
                        }
                        cout << "  -> [Error] Invalid date! Please enter a real date." << endl;
                    }
                    
                    string project;
                    cout << "New project (current: " << (t.project.empty() ? "None" : t.project) << ") [Type 'clear' to remove]: ";
                    getline(cin, project);

                    editTask(app, id, title, desc, priority, dueDate, project);
                } catch(...) {
                    cout << "[Error] Invalid ID format." << endl;
                }
                break;
            }
            case 7: {
                string idStr;
                int id;
                cout << "--- Mark Task Completed ---" << endl;
                viewAllTasks(app); // Show list of tasks so user knows what to complete
                cout << "Enter task ID to mark as completed: ";
                getline(cin, idStr);
                try {
                    id = stoi(idStr);
                    markTaskCompleted(app, id);
                } catch(...) {
                    cout << "[Error] Invalid ID format." << endl;
                }
                break;
            }
            default:
                cout << "\n[Error] Invalid choice! Please select between 0 and 7." << endl;
        }

        pressEnterToContinue();

    } while (choice != 0);

    destroyApp(app);
    return 0;
}
