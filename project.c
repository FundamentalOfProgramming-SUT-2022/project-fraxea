#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define _len_ 1000

void switchCommand();
void createFile(char *);
int fnd(char *, int *);
void ssc(char *, char *, int );

int main() {
    while (1) switchCommand();
    return 0;
}

void switchCommand() {
    char *cm_l = malloc(_len_);
    char *command_name = malloc(_len_);
    fgets(cm_l, _len_ , stdin);
    sscanf(cm_l, "%s", command_name);
    if (!strcmp(command_name, "createfile")) {
        createFile(cm_l);
    }

    else printf("invalid command\n");
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

void createFile(char *cm_l) {
    cm_l += strlen("createfile") + 1;
    char *cm_p = malloc(_len_);
    sscanf(cm_l, "%s", cm_p);
    if (strcmp(cm_p, "–file")) {
        printf("invalid command\n");
        return;
    }
    cm_l += strlen("–file /");
    int i = strlen("root"), space = 0;
    if (*cm_l == '"') {
        cm_l++;
        space = 1;
    }
    while (fnd(cm_l, &i)) {
        ssc(cm_l, cm_p, i);
        mkdir(cm_p, 0777);
    }
    if (space) {
        if (*(cm_l + i) == '\n') i--;
        else { 
            printf("invalid command\n");
            return;
        }
    }
    struct stat c;
    FILE *a;
    ssc(cm_l, cm_p, i);
    if (stat(cm_p, &c) == 0) printf("The file is already exist!\n");
    a = fopen(cm_p, "w");
    fclose(a);
}

int fnd(char *cm_l, int *i) {
    (*i)++;
    while (1) {
        if (*(cm_l + *i) == '/') return 1;
        if (*(cm_l + *i) == '\n') return 0;
        (*i)++;
    }
    return 0;
}

void ssc(char *reference, char *target, int index) {
    for (int i = 0; i < index; i++) {
        *(target + i) = *(reference + i);
    }
}

