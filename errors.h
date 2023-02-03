#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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
    printf("%s", out);
}
// File exist!
void error1() {
    printf("File exist!\n");
}
// Directory doesn't exist!
void error2() {
    printf("Directory doesn't exist!\n");
}
// File doesn't exist!
void error3() {
    printf("File doesn't exist!\n");
}
// invalid command!
void error4() {
    printf("invalid command!\n");
}
// The line is not available!
void error5() {
    printf("The line is not available!\n");
}
// The line is shorter!
void error6() {
    printf("The line is shorter!\n");
}
// Too many characters to remove!
void error7() {
    printf("Too many characters to remove!\n");
}
// Too many characters to copy!
void error8() {
    printf("Too many characters to copy!\n");
}
// Too many characters to cut!
void error9() {
    printf("Too many characters to cut!\n");
}
// The clipboard is empty!
void error10() {
    printf("The clipboard is empty!\n");
}
// These features cannot come together!
void error11() {
    printf("These features cannot come together!\n");
}
// Cannot find this!
void error12() {
    printf("Cannot find this!\n");
}
// Replaced succesfully!
void messege13() {
    printf("Replaced succesfully!\n");
}
// There is no change yet!
void error14() {
    printf("There is no change yet!\n");
}
// Curly braces are incorrect!
void error15() {
    printf("Curly braces are incorrect!\n");
}

