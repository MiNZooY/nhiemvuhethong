#include "Console.h"
#include <iostream>

namespace Console {

    void Initialize() {
        // Kích hoạt tiếng Việt có dấu UTF-8 trên Windows Console
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        // Enable virtual terminal processing for ANSI escape sequences
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }

        // Disable Quick Edit Mode so clicks in the console don't pause execution
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        if (hIn != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hIn, &dwMode)) {
                dwMode &= ~ENABLE_QUICK_EDIT_MODE;
                SetConsoleMode(hIn, dwMode | ENABLE_EXTENDED_FLAGS);
            }
        }

        // Resize window to comfortable dimensions for 3-column text dashboard
        // Width: 100, Height: 35
        HWND hwnd = GetConsoleWindow();
        if (hwnd) {
            RECT rect;
            GetWindowRect(hwnd, &rect);
            MoveWindow(hwnd, rect.left, rect.top, 920, 720, TRUE);
        }

        HideCursor();
        SetTitle("OVERCOOKED: DRINK MASTERS [TEXT CO-OP 2P]");
    }

    void ResetCursor() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = {0, 0};
        SetConsoleCursorPosition(hOut, coord);
    }

    void HideCursor() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hOut, &cursorInfo);
        cursorInfo.bVisible = FALSE; // set the cursor visibility to false
        SetConsoleCursorInfo(hOut, &cursorInfo);
    }

    void Clear() {
        std::cout << "\033[2J\033[H" << std::flush;
    }

    void SetTitle(const std::string& title) {
        SetConsoleTitleA(title.c_str());
    }

    bool IsWindowFocused() {
        return (GetForegroundWindow() == GetConsoleWindow());
    }
}
