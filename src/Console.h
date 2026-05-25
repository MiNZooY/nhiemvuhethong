#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <windows.h>

namespace Console {
    // Colors mapped to ANSI Escape Codes
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[38;2;255;85;85m";     // Player 1
    const std::string BLUE    = "\033[38;2;85;85;255m";     // Player 2
    const std::string GREEN   = "\033[38;2;85;255;85m";     // Ready status
    const std::string YELLOW  = "\033[38;2;255;255;85m";    // Warnings / Instructions
    const std::string CYAN    = "\033[38;2;85;255;255m";     // Processing
    const std::string MAGENTA = "\033[38;2;255;85;255m";    // Recipes
    const std::string WHITE   = "\033[37m";
    const std::string GRAY    = "\033[90m";
    const std::string GOLD    = "\033[38;2;255;190;0m";

    // Initialize console properties (size, UTF-8 CP, cursor, QuickEdit)
    void Initialize();

    // Reset console cursor to top-left to avoid flickering on redraw
    void ResetCursor();

    // Hide console cursor
    void HideCursor();

    // Clean console screen
    void Clear();

    // Set console title
    void SetTitle(const std::string& title);

    // Check if the game window is currently focused
    bool IsWindowFocused();
}

#endif // CONSOLE_H
