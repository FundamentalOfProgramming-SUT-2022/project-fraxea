#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ncurses.h>

#define SIZE 1000

struct features {
    int f[4]; // 0: at | 1: byword | 2: count | 3: all
    int sum; // how many
    int at;
};

struct features_rep {
    int f[2];
    int sum;
    int at;
};

struct grep_pattern {
    char *path;
    int i;
};

void printOutPut(char *out) {
    printw("%s", out); refresh();
}
// File exist!
void error1() {
    printw("File exist!\n"); refresh();
}
// Directory doesn't exist!
void error2() {
    printw("Directory doesn't exist!\n"); refresh();
}
// File doesn't exist!
void error3() {
    printw("File doesn't exist!\n"); refresh();
}
// invalid command!
void error4() {
    printw("invalid command!\n"); refresh();
}
// The line is not available!
void error5() {
    printw("The line is not available!\n"); refresh();
}
// The line is shorter!
void error6() {
    printw("The line is shorter!\n"); refresh();
}
// Too many characters to remove!
void error7() {
    printw("Too many characters to remove!\n"); refresh();
}
// Too many characters to copy!
void error8() {
    printw("Too many characters to copy!\n"); refresh();
}
// Too many characters to cut!
void error9() {
    printw("Too many characters to cut!\n"); refresh();
}
// The clipboard is empty!
void error10() {
    printw("The clipboard is empty!\n"); refresh();
}
// These features cannot come together!
void error11() {
    printw("These features cannot come together!\n"); refresh();
}
// Cannot find this!
void error12() {
    printw("Cannot find this!\n"); refresh();
}
// Replaced succesfully!
void messege13() {
    printw("Replaced succesfully!\n"); refresh();
}
// There is no change yet!
void error14() {
    printw("There is no change yet!\n"); refresh();
}
// Curly braces are incorrect!
void error15() {
    printw("Curly braces are incorrect!\n"); refresh();
}

