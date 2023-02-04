#include "commands_2.h"

int main() {
    initscr();
    start_color();
    init_pair(10, COLOR_WHITE, COLOR_BLUE);
    init_pair(1, COLOR_WHITE, COLOR_CYAN);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    noecho();
    normal_mode("root");
    return 0;
}