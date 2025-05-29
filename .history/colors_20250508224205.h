#ifndef COLORS_H
#define COLORS_H

// Reset all attributes
#define RESET "\033[0m"

// Text colors
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Background colors
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

// Text effects
#define BOLD "\033[1m"
#define DIM "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define REVERSE "\033[7m"
#define HIDDEN "\033[8m"

// Game-specific color combinations
#define WELCOME_COLOR CYAN BOLD
#define ERROR_COLOR RED BOLD
#define SUCCESS_COLOR GREEN BOLD
#define HINT_COLOR YELLOW
#define COMMAND_COLOR BLUE BOLD
#define COORDINATE_COLOR CYAN
#define HASHED_CELL RED BOLD
#define CAPITAL_LETTER BOLD BG_WHITE BLACK
#define NORMAL_LETTER WHITE

// Clear screen
#define CLEAR_SCREEN "\033[2J\033[H"

#endif // COLORS_H