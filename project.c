#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SIZE 1000

void switchCommand();
int createFile(char *);
int stillRemaining(char *, char , char *, int *, int *);
int cat(char *);

int main() {
    while (1) switchCommand();
    return 0;
}

void switchCommand() {
    static unsigned long size = SIZE;
    char *line = (char *) malloc(size);
    getline(&line, &size, stdin);
    // if filename == "createfile --file "
    char *l1 = (char *) malloc(size);
    sscanf(line, "%s", l1);
    if (strcmp(l1, "createfile") == 0) {
        if(!createFile(line)) printf("invalid command\n");
    }
    else if (strcmp(l1, "cat") == 0) {
        if(!cat(line)) printf("invalid command\n");
    }
    /*else if ...*/
    else printf("invalid command\n");
    free(line);
    free(l1);
    /*
    creatfile
    insertstr
    cat
    removestr
    copystr
    pastetr
    find
    replace
    grep
    undo
    auto
    compare
    tree


    How can I compare two strings without for loop in C/C++?


    I think I can do that with pointer. I am looking for an algorithm with O(1), hence I am not going to use functions in libraries(like strcmp in string.h). Is that possible at all?

If it is not possible can you prove that? Thanks.
    */
}

int createFile(char *line) {
    int l = strlen("createfile --file "), i = 0;
    char *path = (char *) malloc(SIZE);
    char terminal = '\n', c;
    // check the space
    c = line[l++];
    if (c == '"') {
        l++;
        terminal = '"';
    } // else c = '/'
    while (stillRemaining(line, terminal, path, &i, &l)) // make directories
        mkdir(path, 0777);
    // create file if new
    FILE *fp = fopen(path, "r");
    if (fp != NULL) printf("File exist!\n");
    else fp = fopen(path, "a");
    fclose(fp);
    return 1;
}

int stillRemaining(char *line, char terminal, char *path, int *i, int *l) {
    char c;
    while (1) {
        c = line[*l];
        (*l)++;
        if (c == terminal) return 0;
        path[*i] = c;
        (*i)++;
        if (c == '/') return 1;
    }
}

int cat(char *line) {
    int l = strlen("cat --file ");
    int i = 0;
    char *path = (char *) malloc(SIZE);
    char *str = (char *) malloc(SIZE);
    char terminal = '\n', c;
    // check the space
    c = line[l++];
    if (c == '"') {
        l++;
        terminal = '"';
    } // else c = '/'
    while (stillRemaining(line, terminal, path, &i, &l));

    FILE *fp = fopen(path, "r");
    for (int i = 0; i < SIZE; i++) {
        c = getc(fp);
        if (c == EOF) break;
        str[i] = c;
    }
    printf("%s", str);
    fclose(fp);
    free(str);
    free(path);
    return 1;
}
