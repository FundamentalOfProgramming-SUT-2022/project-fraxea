#include "errors.h"

void readCommandLine();
void switchCommand(char **, char **, char *);
void createFile(char **);
int stillRemaining(char **, char, char *);
void cat(char **, char **);
char* Dastkary(char **, int);
void insertstr(char **, char **, int);
int findPosition(int, int, char *);
int contentFile(char *, char **);
void writeInFile(char *, char *);
void writeMiddleString(char **, char *, int);
int findPath(char **, char *, char);
void removestr(char **);
int removeMiddleString(char **, char, int, int);
void copystr(char *);
int readMiddleString(char **, char, int, int, char **);
void cutstr(char *);
void pastestr(char *);
int readClipboard(char **);
int isNew(int **, int);
int searchString(char *, int, char *, int, char *);
int countString(char *, int, char *, int, char *, int **, int *, int);
char* Dastkary2(char *, char, int, int *, char *);
void find(char *);

int main() {
    while (1) readCommandLine();
    return 0;
}

void readCommandLine() {
    static unsigned long size = SIZE;
    char *line = (char *) malloc(size);
    char *output = (char *) malloc(size);
    char *c_n = (char *) malloc(size);
    getline(&line, &size, stdin);
    sscanf(line, "%s", c_n);
    switchCommand(&line, &output, c_n);
    free(output); free(c_n);
}

void switchCommand(char **line, char **output, char *c_n) {
    int armin = 0;
    /*while (line[0] != '\0')*/
        if (!strcmp(c_n, "createfile")) createFile(line);
        else if (!strcmp(c_n, "cat")) cat(line, output);
        else if (!strcmp(c_n, "insertstr")) {insertstr(line, output, armin); return;}
        else if (!strcmp(c_n, "removestr")) removestr(line);
        // else if (!strcmp(c_n, "copystr")) copystr(line);
        // else if (!strcmp(c_n, "cutstr")) cutstr(line);
        // else if (!strcmp(c_n, "pastestr")) pastestr(line);
        // else if (!strcmp(c_n, "find")) find(line);
        /*else if ...*/
        else error4();
    printOutPut(*output);
    /* input $  output # neither -
    √ creatfile -
    √ insertstr $
    √ cat #
    √ removestr -
    √ copystr -
    √ cutstr -
    √ pastetr -
    find $ #
    replace $
    grep $ #
    undo -
    auto -
    compare #
    tree #
    !!!armqn
    */
}

void createFile(char **line) {
    *line += strlen("createfile --file ");
    char *path = (char *) malloc(SIZE);
    char terminal = '\n';
    if (**line == '"') {
        ++*line;
        terminal = '"';
    } ++*line;// else **line = '\'
    while (stillRemaining(line, terminal, path)) mkdir(path, 0777);
    FILE *fp = fopen(path, "r");
    if (fp != NULL) error1();
    else {fclose(fp); fp = fopen(path, "a");}
    fclose(fp); free(path);
}

int stillRemaining(char **line, char terminal, char *path) {
    while (1) {
        path[strlen(path)] = **line;
        ++*line;
        if (**line == '/') return 1;
        if (**line == terminal || **line == '\n') return 0;
    }
}

void cat(char **line, char **str) {
    *line += strlen("cat --file ");
    char *path = (char *) malloc(SIZE); // address of file
    if (findPath(line, path, '\n')) return;
    if (contentFile(path, str)) return;
    free(path);
}

char* Dastkary(char **line, int space) {
    char terminal = ' ';
    char *dstr = (char *) malloc(SIZE);
    *line += space; if (space) terminal = '"';
    int a = 0;
    for (int j = 0; ; j++, ++*line) {
        if (a) {
            a = 0;
            if (**line == 'n') dstr[j] = '\n';
            else dstr[j] = **line;
        }
        else if (**line == '\\') {j--; a = 1;}
        else if (**line == terminal) break;
        else dstr[j] = **line;
    }
    *line += space; return dstr;
}

void insertstr(char **line, char **s, int armin) {
    *line += strlen("insertstr --file ");
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    int _line, _char, i = 0;
    if (findPath(line, path, ' ')) return;
    if (contentFile(path, &str)) return;
    if (!armin) {
        *line += strlen(" --str ");
        if (**line == '"') i = 1;
        *s = Dastkary(line, i);
    }
    sscanf(*line, " --pos %i:%i", &_line, &_char); // find position
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    writeMiddleString(&str, *s, i);
    writeInFile(path, str);
    free(path); free(str);
}

int findPosition(int _line, int _char, char *str) {
    int i;
    for (i = 0; _line > 1; i++) {
        if (str[i] == '\0') {error5(); return -1;}
        if (str[i] == '\n') _line--;
    }
    while (_char--) {
        if (str[i] == '\n' || str[i] == '\0') {error6(); return -1;}
        i++;
    }
    return i;
}

int contentFile(char *path, char **str) {
    FILE *fp = fopen(path, "r");
    char c;
    if (fp == NULL) {
        error3();
        fclose(fp);
        return 1;
    }
    for (int i = 0; ; i++) {
        if (i == strlen(*str)) *str = realloc(*str, strlen(*str) + SIZE);
        c = getc(fp);
        if (c == EOF) break;
        (*str)[i] = c;
    }
    fclose(fp); return 0;
}

void writeInFile(char *path, char *str) {
    FILE *fp = fopen(path, "w");
    fwrite(str, 1, strlen(str), fp);
    fclose(fp);
}

void writeMiddleString(char **str, char *s, int i) {
    int size = strlen(*str) - i;
    char *tail = (char *) malloc(size);
    for (int j = 0; j < size; j++) tail[j] = (*str)[i + j];
    (*str)[i] = '\0';
    strcat(*str, s);
    strcat(*str, tail);
    free(tail);
}

int findPath(char **line, char *path, char t) {
    char terminal = t;
    if (**line == '"') {
        ++*line;
        terminal = '"';
    } // else **line = '/'
    FILE *fp; ++*line;
    while (stillRemaining(line, terminal, path)) {
        fp = fopen(path, "r");
        if (fp == NULL && **line == '/') {
            error2();
            fclose(fp); return 1;
        } fclose(fp);
    }
    if (**line == '"') ++*line;
    fclose(fp); return 0;
}

void removestr(char **line) {
    *line += strlen("removestr --file ");
    char *str = (char *) malloc(SIZE);
    char *path = (char *) malloc(SIZE);
    char t = ' ';
    int _line, _char, size, i = 0;
    if (findPath(line, path, t)) return;
    if (contentFile(path, &str)) return;
    sscanf(*line, " --pos %i:%i -size %i -%c", &_line, &_char, &size, &t);
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    if (removeMiddleString(&str, t, i, size)) error7();
    writeInFile(path, str);
    free(path); free(str);
}

int removeMiddleString(char **str, char t, int i, int size) {
    if (t == 'b' && i < size) return 1;
    if (t == 'f' && strlen(*str) < i + size) return 1;
    if (t == 'f') i += size;
    int size_tail = strlen(*str) - i;
    char *tail = (char *) malloc(size_tail);
    for (int j = 0; j < size_tail; j++) tail[j] = (*str)[i + j];
    (*str)[i - size] = '\0';
    strcat(*str, tail);
    free(tail); return 0;
}
/*
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
    writeInFile(path, str);
    free(path); free(str); free(s);
}

void pastestr(char *line) {
    int l = strlen("pastestr --file ");
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    char *s = (char *) malloc(SIZE); // string to insert
    int _line, _char, i = 0;
    if (readClipboard(&s)) return;
    if (findPath(line, path, &i, &l, ' ')) return;
    if (contentFile(path, &str)) return;
    line += l + strlen("--pos ");
    sscanf(line, "%i:%i", &_line, &_char); // find position
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    writeMiddleString(&str, s, i);
    writeInFile(path, str);
    free(path); free(str); free(s);
}

int readClipboard(char **str) {
    FILE *fp = fopen("clipboard", "r");
    char c;
    if (fp == NULL) {
        printf("The clipboard is empty!\n");
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

int isNew(int **s_s, int cnt) {
    for (int i = 0; i < cnt; i++) {
        if ((s_s[i][0] == s_s[cnt][0]) && (s_s[i][1] == s_s[cnt][1])) return 0;
    }
    return 1;
}

int searchString(char *str, int i, char *s, int j, char *star_s) {
    if (s[j] == '\0') return 1;
    if (i > strlen(str)) return -1;
    if (star_s[j] == ' ') {
        if (str[i] == s[j]) {
            if (searchString(str, i + 1, s, j + 1, star_s) > -1) return i;
        }
        if (j) return -1;
        return searchString(str, i + 1, s, 0, star_s);
    }
    int k; // else star_s[j] = '*'
    for (k = 0; (str[i + k] != '\0') && (str[i + k] != '\n') && (str[i + k] != ' '); k++);
    for (int m = k; m > -1; m--) {
        if (searchString(str, i + m, s, j + 1, star_s) > -1) return i;
    }
    if (j) return -1;
    if (k) return searchString(str, i + k, s, 0, star_s);
    return searchString(str, i + 1, s, 0, star_s);
}

int countString(char *str, int i, char *s, int j, char *star_s, int **s_s, int *cnt, int len) {
    if (s[j] == '\0') {
        s_s[*cnt] =  (int *) malloc(2 * sizeof(int));
        s_s[*cnt][1] = i;
        return 1;
    }
    if (i > strlen(str)) return -1;
    if (star_s[j] == ' ') {
        if (str[i] == s[j]) {
            if (countString(str, i + 1, s, j + 1, star_s, s_s, cnt, len + 1) > -1) {
                if (j) return 1;
                s_s[*cnt][0] = i;
                if (isNew(s_s, *cnt)) (*cnt)++;
            }
        }
        if (j) return -1;
        return countString(str, i + 1, s, 0, star_s, s_s, cnt, 0);
    }
    int k; // else star_s[j] = '*'
    for (k = 0; (str[i + k] != '\0') && (str[i + k] != '\n') && (str[i + k] != ' '); k++);
    for (int m = k; m > -1; m--) {
        if (countString(str, i + m, s, j + 1, star_s, s_s, cnt, len + m) > -1) {
            s_s[*cnt][0] = i - len;
            if (isNew(s_s, *cnt)) (*cnt)++;
        }
    }
    if (j) return -1;
    if (k) return countString(str, i + k, s, 0, star_s, s_s, cnt, 0);
    return countString(str, i + 1, s, 0, star_s, s_s, cnt, 0);
}

char* Dastkary2(char *str, char terminal, int space, int *i, char *star_s) {
    char *dstr = (char *) malloc(SIZE);
    int j = 0;
    for (int a = 0; ; (*i)++) {
        if (a) {
            if (str[*i] == 'n') dstr[j++] = '\n';
            else dstr[j++] = str[*i];
            star_s[j - 1] = ' ';
            a = 0;
        }
        else if (str[*i] == terminal) {
            if (space && a) dstr[j++] = str[*i];
            else {
                if (terminal == '"') (*i)++;
                break;
            }
        }
        else if (str[*i] == '\\') a = 1;
        else {
            dstr[j++] = str[*i];
            if (str[*i] == '*') star_s[j - 1] = '*';
            else star_s[j - 1] = ' ';
        }
    }
    return dstr;
}

void find(char *line) {
    int l;
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    char *s = (char *) malloc(SIZE); // string to find
    char *star_s = (char *) malloc(SIZE); // wildcard
    int i = 0;
    line += strlen("find --str ");
    l = 0; // lenght of string for finding
    if (*line == '"') s = Dastkary2(++line, '"', 1, &l, star_s);
    else s = Dastkary2(line, ' ', 0, &l, star_s);
    line += l + strlen(" --file ");
    l = 0;
    if (findPath(line, path, &i, &l, ' ')) return;
    if (contentFile(path, &str)) return;
    i = searchString(str, 0, s, 0, star_s); // normall one
    printf("%i\n", i);
    free(path); free(str); free(s); free(star_s);
}
*/