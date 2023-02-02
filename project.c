#include "errors.h"

void readCommandLine(char *);
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
void copystr(char **);
int readMiddleString(char *, char, int, int, char **);
void cutstr(char **);
void pastestr(char **);
int readClipboard(char **);
int isNew(int **, int);
int searchString(char *, int, char *, int, char *);
int countString(char *, int, char *, int, char *, int **, int *, int);
char* Dastkary2(char **, int, char *);
void find(char **, char **, int);
struct features featureOnOff(char **);
char* makeDecision(char **, char *, char *, char *);
char* findAt(char *, char *, char *, int);
char* findAt_ByWord(char *, char *, char *, int);
char* findCount(char *, char *, char *);
char* findAll(char *, char *, char *);
char* findAll_Byword(char *, char *, char *);
int byWord(char *, int);
struct features_rep featureOnOffReplace(char **);
void makeDecisionReplace(char **, char *, char *, char *, char *, char *);
void replace(char **);
void replaceAt(char *, char *, char *, int, char *, char *);
void replaceAll(char *, char *, char *, char *, char *);
void grep(char **, char **, int);
int getFeatures(char **);
char* findPattern(char **, char *, int);
char* finalGrep(struct grep_pattern *, int, int);
void backup_i(char *);
void undo(char **);
void goback(char *);

int main() {
    unsigned long size = SIZE;
    char *line = (char *) malloc(size);
    while (1) {
        getline(&line, &size, stdin);
        readCommandLine(line);
    }
    return 0;
}

void readCommandLine(char *line) {
    char *output = (char *) malloc(SIZE);
    char *c_n = (char *) malloc(SIZE);
    sscanf(line, "%s", c_n);
    switchCommand(&line, &output, c_n);
    if (*output) free(output); free(c_n);
}

void switchCommand(char **line, char **output, char *c_n) {
    int armin = 0;
    /*while (line[0] != '\0')*/
        if (!strcmp(c_n, "createfile")) createFile(line);
        else if (!strcmp(c_n, "cat")) cat(line, output);
        else if (!strcmp(c_n, "insertstr")) {insertstr(line, output, armin); return;}
        else if (!strcmp(c_n, "removestr")) removestr(line);
        else if (!strcmp(c_n, "copystr")) copystr(line);
        else if (!strcmp(c_n, "cutstr")) cutstr(line);
        else if (!strcmp(c_n, "pastestr")) pastestr(line);
        else if (!strcmp(c_n, "find")) find(line, output, armin);
        else if (!strcmp(c_n, "replace")) replace(line);
        else if (!strcmp(c_n, "grep")) grep(line, output, armin);
        else if (!strcmp(c_n, "undo")) undo(line);
        /*else if ...*/
        else error4();
    printOutPut(*output);
    /* input $  output # neither -
    √ creatfile -
    √ @insertstr $
    √ cat #
    √ @removestr -
    √ copystr -
    √ @cutstr -
    √ @pastetr -
    √ find $ #
    √ @replace -
    √ grep $ #
    √ undo @
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
    backup_i(path);
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
    if (fp == NULL) {error3(); fclose(fp); return 1;}
    int size;
    for (size = 0; getc(fp) != EOF; size++);
    rewind(fp);
    *str = realloc(*str, size);
    for (int i = 0; i < size; i++) (*str)[i] = getc(fp);
    fclose(fp); return 0;
}

void writeInFile(char *path, char *str) {
    FILE *fp = fopen(path, "w");
    fwrite(str, 1, strlen(str), fp);
    fclose(fp);
}

void writeMiddleString(char **str, char *s, int i) {
    int size = strlen(*str) - i;
    *str = realloc(*str, strlen(*str) + strlen(s));
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
    return 0;
}

void removestr(char **line) {
    *line += strlen("removestr --file ");
    char *str = (char *) malloc(SIZE); // content file
    char *path = (char *) malloc(SIZE);
    char t = ' ';
    int _line, _char, size, i = 0;
    if (findPath(line, path, t)) return;
    if (contentFile(path, &str)) return;
    sscanf(*line, " --pos %i:%i -size %i -%c", &_line, &_char, &size, &t);
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    if (removeMiddleString(&str, t, i, size)) error7();
    backup_i(path);
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

void copystr(char **line) {
    *line += strlen("copystr --file ");
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    char t, *s; // string to insert
    int size, _line, _char, i;
    if (findPath(line, path, ' ')) return;
    if (contentFile(path, &str)) return;
    sscanf(*line, " --pos %i:%i -size %i -%c", &_line, &_char, &size, &t);
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    if (readMiddleString(str, t, i, size, &s)) {error8(); return;}
    writeInFile("clipboard", s);
    free(path); free(str); free(s);
}

int readMiddleString(char *str, char t, int i, int size, char **s) {
    if (t == 'b' && i < size) return 1;
    if (t == 'f' && strlen(str) < i + size) return 1;
    if (t == 'b') i -= size;
    *s = (char *) malloc(size);
    for (int j = 0; j < size; j++) (*s)[j] = str[i + j];
    return 0;
}

void cutstr(char **line) {
    *line += strlen("cutstr --file ");
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    char t, *s; // string to insert
    int size, _line, _char, i;
    if (findPath(line, path, ' ')) return;
    if (contentFile(path, &str)) return;
    sscanf(*line, " --pos %i:%i -size %i -%c", &_line, &_char, &size, &t);
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    if (readMiddleString(str, t, i, size, &s)) {error9(); return;}
    writeInFile("clipboard", s);
    removeMiddleString(&str, t, i, size);
    backup_i(path);
    writeInFile(path, str); // The only difference between copy and cut!
    free(path); free(str); free(s);
}

void pastestr(char **line) {
    *line += strlen("pastestr --file ");
    char *path = (char *) malloc(SIZE); // address of file
    char *str = (char *) malloc(SIZE); // content of file
    char *s; // string to insert
    int _line, _char, i = 0;
    if (readClipboard(&s)) return;
    if (findPath(line, path, ' ')) return;
    if (contentFile(path, &str)) return;
    sscanf(*line, " --pos %i:%i", &_line, &_char);
    i = findPosition(_line, _char, str);
    if (i == -1) return;
    writeMiddleString(&str, s, i);
    backup_i(path);
    writeInFile(path, str);
    free(path); free(str); free(s);
}

int readClipboard(char **str) {
    FILE *fp = fopen("clipboard", "r");
    if (fp == NULL) {error10(); fclose(fp); return 1;}
    int size;
    for (size = 0; getc(fp) != EOF; size++);
    rewind(fp);
    *str = (char *) malloc(size);
    for (int i = 0; i < size; i++) (*str)[i] = getc(fp);
    fclose(fp); return 0;
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
    if (star_s[j] != '*') {
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
    if (k) return countString(str, i + 1, s, 0, star_s, s_s, cnt, 0);
    return countString(str, i + 1, s, 0, star_s, s_s, cnt, 0);
}

char* Dastkary2(char **line, int space, char *star_s) {
    char terminal = ' ';
    char *dstr = (char *) malloc(SIZE);
    *line += space; if (space) terminal = '"';
    int a = 0;
    for (int j = 0; ; j++, ++*line) {
        if (a) {
            a = 0;
            if (**line == 'n') dstr[j] = '\n';
            else dstr[j] = **line; star_s[j] = ' ';
        }
        else if (**line == '\\') {j--; a = 1;}
        else if (**line == terminal) break;
        else {
            dstr[j] = **line; star_s[j] = ' ';
            if (**line == '*') star_s[j] = '*';
        }
    }
    *line += space; return dstr;
}

void find(char **line, char **output, int armin) {
    *line += strlen("find");
    char *path = (char *) malloc(SIZE);
    char *str = (char *) malloc(SIZE); // content of file
    char *s = (char *) malloc(SIZE); // string to find
    char *star_s = (char *) malloc(SIZE); // wildcard
    int i = 0;
    if (armin) for (int i = 0; ; i++) {
        s[i] = (*output)[i];
        if ((*output)[i] == '\0') break;
    }
    else {
        *line += strlen(" --str ");
        if (**line == '"') i = 1;
        s = Dastkary2(line, i, star_s);
    }
    *line += strlen(" --file ");
    if (findPath(line, path, ' ')) return;
    if (contentFile(path, &str)) return;
    *output = makeDecision(line, str, s, star_s);
    free(path); free(str); free(s); free(star_s);
}

struct features featureOnOff(char **line) {
    struct features feature; // init
    for (int i = 0; i < 4; i++) feature.f[i] = 0;
    char *d = (char *) malloc(SIZE); // get features
    for (feature.sum = 0; sscanf(*line, " %s", d) > -1; feature.sum++) {
        if (!strcmp(d, "-byword")) feature.f[1]++;
        else if (!strcmp(d, "-count")) feature.f[2]++;
        else if (!strcmp(d, "-all")) feature.f[3]++;
        else {
            feature.f[0]++;
            sscanf(*line, " %s %i", d, &feature.at);
            *line += strlen(d) + 1;
            sscanf(*line, "%s", d);
        }
        *line += strlen(d) + 1;
    }
    return feature;
}

char* makeDecision(char **line, char *str, char *s, char *star_s) {
    struct features feature = featureOnOff(line);
    if (feature.sum == 0) return findAt(str, s, star_s, 1);
    if (feature.sum == 1) {
        if (feature.f[0]) return findAt(str, s, star_s, feature.at);
        if (feature.f[1]) return findAt_ByWord(str, s, star_s, 1);
        if (feature.f[2]) return findCount(str, s, star_s);
        return findAll(str, s, star_s);
    }
    if (feature.sum == 2) {
        if (!feature.f[1] || feature.f[2]) {error11(); return "\0";}
        if (feature.f[3]) return findAll_Byword(str, s, star_s);
        if (feature.f[0]) return findAt_ByWord(str, s, star_s, feature.at);
    }
    error11(); return "\0";
}

char* findAt(char *content, char *find, char *star_f, int at) {
    int count = 0;
    int **find_all = (int **) malloc(SIZE * sizeof(int *));
    char *output = (char *) malloc(SIZE);
    countString(content, 0, find, 0, star_f, find_all, &count, 0);
    if (count < at || at < 1) sprintf(output, "-1\n");
    else sprintf(output, "%i\n", find_all[at - 1][0]);
    free(find_all); return output;
}

char* findAt_ByWord(char *content, char *find, char *star_f, int at) {
    int count = 0;
    int **find_all = (int **) malloc(SIZE * sizeof(int *));
    char *output = (char *) malloc(SIZE);
    countString(content, 0, find, 0, star_f, find_all, &count, 0);
    if (count < at || at < 1) sprintf(output, "-1\n");
    else sprintf(output, "%i\n", byWord(content, find_all[at - 1][0]));
    free(find_all); return output;
}

char* findCount(char *content, char *find, char *star_f) {
    int count = 0;
    int **find_all = (int **) malloc(SIZE * sizeof(int *));
    char *output = (char *) malloc(SIZE);
    countString(content, 0, find, 0, star_f, find_all, &count, 0);
    sprintf(output, "%i\n", count);
    free(find_all); return output;
}

char* findAll(char *content, char *find, char *star_f) {
    int count = 0;
    int **find_all = (int **) malloc(SIZE * sizeof(int *));
    char *output = (char *) malloc(SIZE);
    char *output_ = (char *) malloc(SIZE);
    countString(content, 0, find, 0, star_f, find_all, &count, 0);
    if (count) {
        sprintf(output, "%i", find_all[0][0]);
        for (int i = 1; i < count; i++) {
            sprintf(output_, ", %i", find_all[i][0]);
            strcat(output, output_);
        }
        strcat(output, "\n");
    }
    else sprintf(output, "-1\n");
    free(find_all); return output;
}

char* findAll_Byword(char *content, char *find, char *star_f) {
    int count = 0;
    int **find_all = (int **) malloc(SIZE * sizeof(int *));
    char *output = (char *) malloc(SIZE);
    char *output_ = (char *) malloc(SIZE);
    countString(content, 0, find, 0, star_f, find_all, &count, 0);
    if (count) {
        sprintf(output, "%i", byWord(content, find_all[0][0]));
        for (int i = 1; i < count; i++) {
            sprintf(output_, ", %i", byWord(content, find_all[i][0]));
            strcat(output, output_);
        }
        strcat(output, "\n");
    }
    else sprintf(output, "-1\n");
    free(find_all); return output;
}

int byWord(char *s, int j) {
    int w = 0;
    for (int i = 0; i < j; i++) if (s[i] == ' ' || s[i] == '\n') w++;
    return w;
}

struct features_rep featureOnOffReplace(char **line) {
    struct features_rep feature; // init
    for (int i = 0; i < 2; i++) feature.f[i] = 0;
    char *d = (char *) malloc(SIZE); // get features
    for (feature.sum = 0; sscanf(*line, " %s", d) > -1; feature.sum++) {
        if (!strcmp(d, "-all")) feature.f[1]++;
        else {
            feature.f[0]++;
            sscanf(*line, " %s %i", d, &feature.at);
            *line += strlen(d) + 1;
            sscanf(*line, "%s", d);
        }
        *line += strlen(d) + 1;
    }
    return feature;
}

void makeDecisionReplace(char **line, char *str, char *s, char *star_s, char *rep, char *path) {
    struct features_rep feature = featureOnOffReplace(line);
    if (feature.sum == 0) replaceAt(str, s, star_s, 1, rep, path);
    else if (feature.sum == 1) {
        if (feature.f[0]) replaceAt(str, s, star_s, feature.at, rep, path);
        else replaceAll(str, s, star_s, rep, path);
    }
    else error11();
}

void replace(char **line) {
    *line += strlen("replace");
    char *path = (char *) malloc(SIZE);
    char *str = (char *) malloc(SIZE); // content of file
    char *s = (char *) malloc(SIZE); // string to find
    char *star_s = (char *) malloc(SIZE); // wildcard
    char *rep = (char *) malloc(SIZE); // string to replace
    int i = 0;
    *line += strlen(" --str1 ");
    if (**line == '"') i = 1;
    s = Dastkary2(line, i, star_s);
    *line += strlen(" --str2 ");
    i = 0;
    if (**line == '"') i = 1;
    rep = Dastkary(line, i);
    *line += strlen(" --file ");
    if (findPath(line, path, ' ')) return;
    if (contentFile(path, &str)) return;
    makeDecisionReplace(line, str, s, star_s, rep, path);
    free(path); free(str); free(s); free(star_s); free(rep);
}

void replaceAt(char *content, char *find, char *star_f, int at, char *rep, char *path) {
    int count = 0;
    int **find_all = (int **) malloc(SIZE * sizeof(int *));
    countString(content, 0, find, 0, star_f, find_all, &count, 0);
    if (count < at || at < 1) error12();
    else {
        removeMiddleString(&content, 'f', find_all[at - 1][0], find_all[at - 1][1] - find_all[at - 1][0]);
        writeMiddleString(&content, rep, find_all[at - 1][0]);
        backup_i(path);
        writeInFile(path, content);
        messege13();
    }
    free(find_all);
}

void replaceAll(char *content, char *find, char *star_f, char *rep, char *path) {
    int count = 0;
    int **find_all = (int **) malloc(SIZE * sizeof(int *));
    countString(content, 0, find, 0, star_f, find_all, &count, 0);
    if (!count) error12();
    else {
        // elimination
        int el = 0;
        int *ins = (int *) malloc(SIZE * sizeof(int));
        int findrep = 0;
        countString(rep, 0, find, 0, star_f, find_all, &findrep, 0);
        while (1) {
            count = 0;
            countString(content, 0, find, 0, star_f, find_all, &count, 0);
            if (!count) break;
            ins[el++] = find_all[0][0];
            removeMiddleString(&content, 'f', find_all[0][0], find_all[0][1] - find_all[0][0]);
        }
        // insertion
        for (int i = 0; i < el; i++) writeMiddleString(&content, rep, ins[i] + i * strlen(rep));
        backup_i(path);
        writeInFile(path, content); messege13();
    }
}

void grep(char **line, char **output, int armin) {
    *line += strlen("grep");
    char *str = (char *) malloc(SIZE); // content of file
    char *s = (char *) malloc(SIZE); // pattern
    int i = 0;
    int ft = getFeatures(line);
    if (ft) *line += strlen(" -l");
    if (armin) for (int i = 0; ; i++) {
        s[i] = (*output)[i];
        if ((*output)[i] == '\0') break;
    }
    else {
        *line += strlen(" --str ");
        if (**line == '"') i = 1;
        s = Dastkary(line, i);
    }
    *line += strlen(" --files ");
    *output = findPattern(line, s, ft);
    free(str); free(s);
}

int getFeatures(char **line) {
    char *feature = (char *) malloc(SIZE);
    sscanf(*line, "%s", feature);
    if (!strcmp(feature, "-l")) return 1;
    if (!strcmp(feature, "-c")) return 2;
    return 0;
}

char* findPattern(char **line, char *pattern, int ft) {
    char *path = (char *) malloc(SIZE);
    char *content = (char *) malloc(SIZE);
    struct grep_pattern *P = (struct grep_pattern *) malloc(SIZE * sizeof(struct grep_pattern));
    int ln = 0, count;
    int **find_all = (int **) malloc(SIZE * sizeof(int *));
    (*line)[strlen(*line) - 1] = ' ';
    while (**line != '\0') {
        free(path); path = (char *) malloc(SIZE);
        if (findPath(line, path, ' ')) return "\0";
        if (contentFile(path, &content)) return "\0";
        count = 0;
        countString(content, 0, pattern, 0, pattern, find_all, &count, 0);
        if (count) for (int i = 0; i < count; i++) {
            (P + ln + i)->path = (char *) malloc(SIZE);
            for (int k = 0; path[k] != '\0'; k++) (P + ln + i)->path[k] = path[k];
            (P + ln + i)->i = find_all[i][0];
        }
        ln += count;
        ++*line;
    }
    return finalGrep(P, ft, ln);
}

char* finalGrep(struct grep_pattern *P, int ft, int ln) {
    char *output = (char *) malloc(SIZE);
    char *output_ = (char *) malloc(SIZE);
    int k;
    *output = '\0'; *output_ = '\0';
    if (ft == 2) sprintf(output, "%i\n", ln);
    else if (ft == 1) for (int i = 0; i < ln; i++) {
        if (strcmp(output_, (P + i)->path)) {
            output_ = (P + i)->path;
            strcat(output, output_);
            strcat(output, "\n");
        }
    }
    else for (int i = 0; i < ln; i++) {
        contentFile((P + i)->path, &output_);
        for (k = (P + i)->i; k && output_[k - 1] != '\n'; k--);
        removeMiddleString(&output_, 'b', k, k);
        for (k = 0; output_[k] != '\n' && output_[k] != '\0'; k++);
        output_[k] = '\0';
        strcat(output, (P + i)->path); strcat(output, ": ");
        strcat(output, output_); strcat(output, "\n");
    }
    free(output_); return output;
}

void backup_i(char *path) {
    char *path_i = (char *) malloc(SIZE);
    char *content = (char *) malloc(SIZE);
    char *path_dot = (char *) malloc(SIZE);
    char *_i_ = (char *) malloc(SIZE);
    int a;
    contentFile(path, &content);
    for (int i = 0; i < strlen(path); i++) {
        path_dot[i] = path[i];
        if (path[i] == '/') a = i;
    }
    writeMiddleString(&path_dot, ".", a + 1);
    for (a = 0; ; a++) {
        sprintf(path_i, "%s", path_dot);
        sprintf(_i_, "(%i)", a);
        strcat(path_i, _i_);
        if (fopen(path_i, "r") == NULL) break;
    }
    writeInFile(path_i, content);
    free(path_i); free(content); free(path_dot); free(_i_);
}

void undo(char **line) {
    *line += strlen("undo --file ");
    char *path = (char *) malloc(SIZE); // address of file
    if (findPath(line, path, '\n')) return;
    goback(path);
    free(path);
}

void goback(char *path) {
    char *path_i = (char *) malloc(SIZE);
    char *content = (char *) malloc(SIZE);
    char *path_dot = (char *) malloc(SIZE);
    char *_i_ = (char *) malloc(SIZE);
    int a;
    for (int i = 0; i < strlen(path); i++) {
        path_dot[i] = path[i];
        if (path[i] == '/') a = i;
    }
    writeMiddleString(&path_dot, ".", a + 1);
    for (a = 0; ; a++) {
        sprintf(path_i, "%s", path_dot);
        sprintf(_i_, "(%i)", a);
        strcat(path_i, _i_);
        if (fopen(path_i, "r") == NULL) break;
    }
    if (!a) {error14(); return;}
    sprintf(path_i, "%s", path_dot);
    sprintf(_i_, "(%i)", a - 1);
    strcat(path_i, _i_);
    contentFile(path_i, &content);
    writeInFile(path, content);
    remove(path_i);
}
