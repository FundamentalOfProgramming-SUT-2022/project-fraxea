#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SIZE 1000

void switchCommand();
void createFile(char *);
int stillRemaining(char *, char, char *, int *, int *);
void cat(char *);
char* Dastkary(char *, char, int, int *);
void insertstr(char *);
int findPosition(int, int, char *);
int contentFile(char *, char **);
void writeInFile(char *, char *);
void writeMiddleString(char **, char *, int);
int findPath(char *, char *, int *, int *, char);
void removestr(char *);
int removeMiddleString(char **, char, int, int);
void copystr(char *);
int readMiddleString(char **, char, int, int, char **);
void cutstr(char *);

int main() {
    while (1) switchCommand();
    return 0;
}

void switchCommand() {
    static unsigned long size = SIZE;
    char *line = (char *) malloc(size);
    char *l1 = (char *) malloc(size);
    getline(&line, &size, stdin);
    sscanf(line, "%s", l1);
    if (strcmp(l1, "createfile") == 0) createFile(line);
    else if (strcmp(l1, "cat") == 0) cat(line);
    else if (strcmp(l1, "insertstr") == 0) insertstr(line);
    else if (strcmp(l1, "removestr") == 0) removestr(line);
    else if (strcmp(l1, "copystr") == 0) copystr(line);
    else if (strcmp(l1, "cutstr") == 0) cutstr(line);
    /*else if ...*/
    else printf("invalid command\n");
    free(line);
    free(l1);
    /*
    √ creatfile
    √ insertstr
    √ cat
    √ removestr
    √ copystr
    √ cutstr
    pastetr
    find
    replace
    grep
    undo
    auto
    compare
    tree
    armin
    */
}

void createFile(char *line) {
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
    free(path);
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

void cat(char *line) {
    int l = strlen("cat --file ");
    int i = 0;
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    printf("%s", path);
    if (findPath(line, path, &i, &l, '\n')) return;
    if (contentFile(path, &str)) return;
    printf("%s", str);
    free(str); free(path);
}

char* Dastkary(char *str, char terminal, int space, int *i) {
    char *dstr = (char *) malloc(SIZE);
    int j = 0;
    for (int a = 0; ; (*i)++) {
        if (a) {
            if (str[*i] == 'n') dstr[j++] = '\n';
            else dstr[j++] = str[*i];
            a = 0;
        }
        else if (str[*i] == terminal) {
            if (space && a) dstr[j++] = str[*i];
            else break;
        }
        else if (str[*i] == '\\') a = 1;
        else dstr[j++] = str[*i];
    }
    return dstr;
}

void insertstr(char *line) {
    int l = strlen("insertstr --file ");
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    char *s = (char *) malloc(SIZE); // string to insert
    int _line, _char, i = 0;
    if (findPath(line, path, &i, &l, ' ')) return;
    if (contentFile(path, &str)) return;
    line += l + strlen("--str ");
    l = 0; // lenght of the string for insert
    if (*line == '"') s = Dastkary(++line, '"', 1, &l);
    else s = Dastkary(line, ' ', 0, &l);
    line += l + strlen(" --pos ");
    sscanf(line, "%i:%i", &_line, &_char); // find position
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    writeMiddleString(&str, s, i);
    writeInFile(path, str);
    free(path); free(str); free(s);
}

int findPosition(int _line, int _char, char *str) {
    int i;
    for (i = 0; _line > 1; i++) {
        if (str[i] == '\0') {
            printf("The line is not available!\n");
            return -1;
        }
        if (str[i] == '\n') _line--;
    }
    if (i) i++;
    while (_char--) {
        if (str[i] == '\n' || str[i] == '\0') {
            printf("The line is shorter!\n");
            return -1;
        }
        i++;
    }
    return i;
}

int contentFile(char *path, char **str) {
    FILE *fp = fopen(path, "r");
    char c;
    if (fp == NULL) {
        printf("File doesn't exist!\n");
        fclose(fp);
        return 1;
    }
    for (int i = 0; ; i++) {
        if (i == strlen(*str)) *str = realloc(*str, strlen(*str) + SIZE);
        c = getc(fp);
        if (c == EOF) break;
        (*str)[i] = c;
    }
    fclose(fp);
    return 0;
}

void writeInFile(char *path, char *str) {
    FILE *fp = fopen(path, "w");
    fwrite(str, 1, strlen(str), fp);
    fclose(fp);
}

void writeMiddleString(char **str, char *s, int i) {
    char *tail = (char *) malloc(SIZE);
    for (int j = 0; ; j++) {
        if (j == strlen(tail)) tail = realloc(tail, strlen(tail) + SIZE);
        tail[j] = (*str)[i + j];
        if (tail[j] == '\0') break;
    }
    (*str)[i] = '\0';
    strcat(*str, s);
    strcat(*str, tail);
    free(tail);
}

int findPath(char *line, char *path, int *i, int *l, char t) {
    char terminal = t;
    char c = line[(*l)++]; // check the space
    if (c == '"') {
        (*l)++;
        terminal = '"';
    } // else c = '/'
    FILE *fp;
    while (stillRemaining(line, terminal, path, i, l)) {
        fp = fopen(path, "r");
        if (fp == NULL && line[*l + 1] != '/') {
            printf("Directory doesn't exist!\n");
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void removestr(char *line) {
    int l = strlen("removestr --file ");
    char *str = (char *) malloc(SIZE);
    char *path = (char *) malloc(SIZE);
    char t = ' ';
    int _line, _char, size, i = 0;
    if (findPath(line, path, &i, &l, t)) return;
    if (contentFile(path, &str)) return;
    line += l + strlen(" --pos");
    sscanf(line, "%i:%i -size %i -%c", &_line, &_char, &size, &t); // find position
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    if (removeMiddleString(&str, t, i, size)) printf("Too many characters to remove!\n");
    writeInFile(path, str);
    free(path); free(str);
}

int removeMiddleString(char **str, char t, int i, int size) {
    char *tail = (char *) malloc(SIZE);
    if (t == 'b' && i < size) return 1;
    if (t == 'f' && strlen(*str) < i + size) return 1;
    if (t == 'f') i += size;
    for (int j = 0; ; j++) {
        if (j == strlen(tail)) tail = realloc(tail, strlen(tail) + SIZE);
        tail[j] = (*str)[i + j];
        if (tail[j] == '\0') break;
    }
    (*str)[i - size] = '\0';
    strcat(*str, tail);
    free(tail);
    return 0;
}

void copystr(char *line) {
    int l = strlen("copystr --file ");
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    char *s = (char *) malloc(SIZE); // string to insert
    char t;
    int size, _line, _char, i = 0;
    if (findPath(line, path, &i, &l, ' ')) return;
    if (contentFile(path, &str)) return;
    line += l + strlen("--pos ");
    sscanf(line, "%i:%i -size %i -%c", &_line, &_char, &size, &t); // find position
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    if (readMiddleString(&str, t, i, size, &s))
        printf("Too many characters to copy!\n");
    writeInFile("clipboard", s);
    free(path); free(str); free(s);
}

int readMiddleString(char **str, char t, int i, int size, char **s) {
    if (t == 'b' && i < size) return 1;
    if (t == 'f' && strlen(*str) < i + size) return 1;
    if (t == 'b') i -= size;
    for (int j = 0; j < size; j++) {
        if (j == strlen(*s)) *s = realloc(*s, strlen(*s) + SIZE);
        (*s)[j] = (*str)[i + j];
    }
    return 0;
}

void cutstr(char *line) {
    int l = strlen("cutstr --file ");
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    char *s = (char *) malloc(SIZE); // string to insert
    char t;
    int size, _line, _char, i = 0;
    if (findPath(line, path, &i, &l, ' ')) return;
    if (contentFile(path, &str)) return;
    line += l + strlen("--pos ");
    sscanf(line, "%i:%i -size %i -%c", &_line, &_char, &size, &t); // find position
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    if (readMiddleString(&str, t, i, size, &s))
        printf("Too many characters to cut!\n");
    writeInFile("clipboard", s);
    removeMiddleString(&str, t, i, size);
    free(path); free(str); free(s);
}

