#include "phase1_2.h"
#define N 14 // max nl

struct curser {
    int line;
    int pos;
    int direct; // for selection
};

struct content {
    char *str; // content of file
    char **il; // content of i+s line
    int nl; // number of lines 
    int sl; // content of start line
};

struct bottom {
    int mode; // 0 command 1 normal, 2 insert, 3 visual
    int save; // 1 saved, 2 unsaved, 3 not saved yet
    char *name;
    char *cb; // command bar; first char ':' or '/'
};

struct highlight {
    int **s_s;
    int cnt;
};

void update_ith_line(struct content *); // using content to update 12 lines
int index_content(struct content *, struct curser *); // find the index in str with curser
char* merge_line(char **, int); // merge lines with \n between them
int size_lines_n(char **, int); // need for malloc
void move_up_curser(struct content *, struct curser *); // move up with 'w'
void move_down_curser(struct content *, struct curser *); // move down with 's'
void move_right_curser(struct content *, struct curser *); // move right with 'd'
void move_left_curser(struct curser *); // move left with 'a'
void show_bottom(struct bottom *); // show the live state of bottoom
void command_mode(struct bottom *, struct content *); // read command first with ':'
void show_content(struct content *); // used in command mode
void show_content_normal(struct content *, struct curser *); // live state normal mode
void normal_mode(char *); // ready to get shortcut keys and moving
void init_content(struct content *, char *); // malloc and line = 0
void init_curser(struct curser *);
void init_bottom(struct bottom *, char *);
void insert_mode(struct bottom *, struct content *, struct curser *); // escape = 27
void find_curser(struct content *, struct curser *, int); // sl if needed
void switch_command(struct bottom *, struct content *); // save and saveas with name
void save_file(struct bottom *, struct content *);
void save_file_as(struct bottom *, struct content *);
void open_file(struct bottom *, struct content *);
void undo_normal(struct bottom *, struct content *, struct curser *); // undo to last saved with 'z'
void auto_indent_normal(struct bottom *, struct content *, struct curser *); // auto-indent normal with '='
void selection_mode(struct bottom *, struct content *, struct curser *);
int selected(struct content *, struct curser *, int, int);
void copy_selection(struct content *, struct curser *); // copy selection mode with 'c'
void show_content_visual(struct content *, struct curser *); // live state visual mode
void delete_selection(struct bottom *, struct content *, struct curser *); // delete selection mode
void cut_selection(struct bottom *, struct content *, struct curser *); // cut selection mode with 'x'
void paste_normal(struct content *, struct curser *); // paste normal mode with 'v'
void find_h(struct bottom *, struct content *, struct curser *); // live state find '/'
void go_first_highlight(struct content *, struct curser *, struct highlight); // first highlight with 'n'
void show_highlight(struct content *, struct curser *, struct highlight);
int in_highlight(struct content *, struct curser *, struct highlight, int, int);

void replace_curser(struct bottom *, struct content *, struct curser *); // replace curser

void update_ith_line(struct content *p) {
    int i = 0;
    for (int a = 0; a < p->sl; i++) {
        if (p->str[i] == '\n') a++;
    }
    p->nl = 0;
    for (int a = 0; p->nl < N && i < strlen(p->str); i++) {
        if (p->str[i] == '\n') {
            p->il[p->nl][a] = '\0';
            strcat(p->il[p->nl], " ");
            a = 0;
            p->nl++;
        }
        else p->il[p->nl][a++] = p->str[i];
    }
    if (p->nl > N) p->nl = N - 1;
}

int index_content(struct content *p, struct curser *m) {
    int i = 0;
    for (int a = 0; a < m->line; i++) {
        if (p->str[i] == '\n') a++;
    }
    return i + m->pos;
}

char* merge_line(char **il, int n) {
    char *str = (char *) malloc(size_lines_n(il, n));
    int a = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < strlen(il[i]); j++) {
            str[a++] = il[i][j];
        }
        str[a++] = '\n';
    }
    return str;
}

int size_lines_n(char **il, int n) {
    int a = 0;
    for (int i = 0; i < n; i++, a++) {
        for (int j = 0; j < strlen(il[i]); j++, a++); 
    }
    return a + SIZE;
}

void move_up_curser(struct content *p, struct curser *m) {
    if (!m->line) return;
    if (m->line - 5 < p->sl && p->sl) {
        p->sl--;
        if (p->nl < 12) p->nl++;
        update_ith_line(p);
    }
    m->line--;
    if (m->pos >= strlen(p->il[m->line - p->sl])) m->pos = strlen(p->il[m->line - p->sl]) - 1;
}

void move_down_curser(struct content *p, struct curser *m) {
    int n = numLine(p->str);
    if (m->line == n - 1) return;
    if (m->line + 5 > p->sl + N && n > p->sl + N) {
        p->sl++;
        update_ith_line(p);
    }
    m->line++;
    if (m->pos >= strlen(p->il[m->line - p->sl])) m->pos = strlen(p->il[m->line - p->sl]) - 1;
}

void move_right_curser(struct content *p, struct curser *m) {
    if (m->pos == strlen(p->il[m->line - p->sl]) - 1) return;
    m->pos++;
}

void move_left_curser(struct curser *m) {
    if (!m->pos) return;
    m->pos--;
}

void show_bottom(struct bottom *b) {
    char *mode = (char *) malloc(SIZE);
    if (b->mode <= 1) sprintf(mode, "NORMAL");
    if (b->mode == 2) sprintf(mode, "INSERT");
    if (b->mode == 3) sprintf(mode, "VISUAL");
    attron(COLOR_PAIR(1)); printw(" %s ", mode); attroff(COLOR_PAIR(1));
    printw(" ");
    attron(COLOR_PAIR(10));
    printw("  %s  ", b->name);
    if (b->save > 1) sprintf(mode, " + ");
    else sprintf(mode, " ");
    printw("%s", mode);
    attroff(COLOR_PAIR(10));
    printw("\n%s", b->cb);
    free(mode);
}

void command_mode(struct bottom *b, struct content *p) {
    b->mode = 0;
    sprintf(b->cb, ":");
    char c;
    while (1) {
        clear();
        show_content(p);
        show_bottom(b);
        refresh();
        c = getch();
        if (c == 10) break;
        if (c == 127) b->cb[strlen(b->cb) - 1] = '\0';
        else b->cb[strlen(b->cb)] = c;
    }
    switch_command(b, p);
    printw("(press any key to continue...)\n");
    refresh();
    getch();
    for (int i = strlen(b->cb); i; i--) b->cb[i - 1] = '\0';
    b->mode = 1;
}

void show_content(struct content *p) {
    for (int i = p->sl; i < p->nl + p->sl; i++) {
        attron(COLOR_PAIR(5)); printw("%4i ", i + 1); attroff(COLOR_PAIR(5));
        for (int j = 0; j < strlen(p->il[i - p->sl]); j++) {
            printw("%c", p->il[i - p->sl][j]);
        }
        printw("\n");
    }
    for (int i = p->nl; i < N; i++) {
        attron(COLOR_PAIR(3)); printw("~"); attroff(COLOR_PAIR(3));
        printw("\n");
    }
}

void show_content_normal(struct content *p, struct curser *m) {
    for (int i = p->sl; i < p->nl + p->sl; i++) {
        attron(COLOR_PAIR(5)); printw("%4i ", i + 1); attroff(COLOR_PAIR(5));
        for (int j = 0; j < strlen(p->il[i - p->sl]); j++) {
            if (i == m->line && j == m->pos) attron(COLOR_PAIR(4));
            printw("%c", p->il[i - p->sl][j]);
            if (i == m->line && j == m->pos) attroff(COLOR_PAIR(4));
        }
        printw("\n");
    }
    for (int i = p->nl; i < N; i++) {
        attron(COLOR_PAIR(3)); printw("~"); attroff(COLOR_PAIR(3));
        printw("\n");
    }
}

void normal_mode(char *path) {
    struct content p;
    init_content(&p, path);
    struct curser m;
    init_curser(&m);
    struct bottom b;
    init_bottom(&b, path);
    char c;
    while (1) {
        clear();
        show_content_normal(&p, &m);
        show_bottom(&b);
        refresh();
        c = getch();
        if (c == 'w') move_up_curser(&p, &m);
        if (c == 's') move_down_curser(&p, &m);
        if (c == 'd') move_right_curser(&p, &m);
        if (c == 'a') move_left_curser(&m);
        if (c == ':') command_mode(&b, &p);
        if (c == 'i') insert_mode(&b, &p, &m);
        if (c == 'z') undo_normal(&b, &p, &m);
        if (c == '=') auto_indent_normal(&b, &p, &m);
        if (c == 'q') selection_mode(&b, &p, &m);
        if (c == 'v') paste_normal(&p, &m);
        if (c == '/') find_h(&b, &p, &m);
    }
}

void init_content(struct content *p, char *path) {
    p->nl = 0;
    p->sl = 0;
    p->il = (char **) malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++) {
        p->il[i] = (char *) malloc(SIZE);
        p->il[i][0] = '\0';
    }
    p->str = (char *) malloc(SIZE);
    FILE *fp = fopen(path, "r");
    int size;
    for (size = 0; getc(fp) != EOF; size++);
    rewind(fp);
    p->str = realloc(p->str, size + SIZE);
    for (int i = 0; i < size; i++) p->str[i] = getc(fp);
    p->str[size] = '\0';
    if (!size) sprintf(p->str, "\n");
    fclose(fp);
    update_ith_line(p);
}

void init_curser(struct curser *m) {
    m->line = 0;
    m->pos = 0;
}

void init_bottom(struct bottom *b, char *path) {
    b->cb = (char *) malloc(SIZE);
    b->name = (char *) malloc(SIZE);
    b->cb[0] = '\0';
    b->mode = 1;
    sprintf(b->name, "%s", path);
    if (!strcmp(b->name, " ")) b->save = 3;
    else b->save = 1;
}

void insert_mode(struct bottom *b, struct content *p, struct curser *m) {
    char c;
    char *s = (char *) malloc(2);
    int i;
    b->mode = 2;
    while (1) {
        clear();
        show_content_normal(p, m);
        show_bottom(b);
        refresh();
        c = getch();
        if (c == 27) break;
        i = index_content(p, m);
        if (c == 127) {
            if (!i) continue;
            removeMiddleString(&p->str, 'b', i--, 1);
        }
        else {
            sprintf(s, "%c", c);
            writeMiddleString(&p->str, s, i++);
        }
        find_curser(p, m, i);
        update_ith_line(p);
        if (b->save == 1) b->save = 2;
    }
    b->mode = 1;
    free(s);
}

void find_curser(struct content *p, struct curser *m, int i) {
    int j, a = 0;
    for (j = 0; j < i; j++) {
        if (p->str[j] == '\n') a++;
    }
    m->line = a; j = 0;
    for (int k = 0; k < a; j++) {
        if (p->str[j] == '\n') k++;
    }
    
    m->pos = i - j;
    if (m->line < p->sl) p->sl = m->line;
    if (p->sl + N <= m->line) p->sl = m->line - N + 1;
}

void switch_command(struct bottom *b, struct content *p) {
    char *cn = (char *) malloc(SIZE);
    char *output = (char *) malloc(SIZE);
    sscanf(b->cb, ":%s", cn);
    if (!strcmp(cn, "save")) {save_file(b, p); return;}
    if (!strcmp(cn, "saveas")) {save_file_as(b, p); return;}
    if (!strcmp(cn, "open")) {open_file(b, p); return;}
    b->cb++;
    switchCommand(&b->cb, &output, cn);
    if (strlen(output)) {
        FILE *fp = fopen(" ", "w");
        fwrite(output, 1, strlen(output), fp);
        fclose(fp);
        normal_mode(" ");
    }
}

void save_file(struct bottom *b, struct content *p) {
    if (b->save == 3) {
        printw("\nTake a name for file! ");
        return;
    }
    backup_i(b->name);
    writeInFile(b->name, p->str);
    b->save = 1;
    printw("\nSaved! ");
}

void save_file_as(struct bottom *b, struct content *p) {
    b->cb += strlen(":saveas ");
    sprintf(b->name, "%s", b->cb);
    fopen(b->name, "w");
    backup_i(b->name);
    writeInFile(b->name, p->str);
    b->save = 1;
    printw("\nSaved! ");
}

void open_file(struct bottom *b, struct content *p) {
    b->cb += strlen(":open ");
    fopen(b->name, "w");
    backup_i(b->name);
    writeInFile(b->name, p->str);
    if (fopen(b->cb, "r") == NULL) printw("\nFile doesn't exist! ");
    else normal_mode(b->cb);
}

void undo_normal(struct bottom *b, struct content *p, struct curser *m) {
    goback(b->name);
    init_bottom(b, b->name);
    init_content(p, b->name);
    init_curser(m);
}

void auto_indent_normal(struct bottom *b, struct content *p, struct curser *m) {
    if (curlyBrace(p->str)) return;
    closingPairs(&p->str);
    if (b->save == 1) b->save = 2;
    update_ith_line(p);
    init_curser(m);
}

void selection_mode(struct bottom *b, struct content *p, struct curser *m) {
    char c;
    b->mode = 3;
    m->direct = index_content(p, m);
    while (1) {
        clear();
        show_content_visual(p, m);
        show_bottom(b);
        refresh();
        c = getch();
        if (c == 'w') move_up_curser(p, m);
        if (c == 's') move_down_curser(p, m);
        if (c == 'd') move_right_curser(p, m);
        if (c == 'a') move_left_curser(m);
        if (c == 'q') break;
        if (c == 'c') {copy_selection(p, m); break;}
        if (c == 'x') {cut_selection(b, p, m); break;}
    }
    b->mode = 1;
}

int selected(struct content *p, struct curser *m, int i, int j) {
    int i2 = index_content(p, m);
    struct curser a;
    a.line = i;
    a.pos = j;
    a.direct = index_content(p, &a);
    if (i2 <= a.direct && a.direct < m->direct) return 1;
    if (m->direct <= a.direct && a.direct < i2) return 1;
    return 0;
}

void show_content_visual(struct content *p, struct curser *m) {
    for (int i = p->sl; i < p->nl + p->sl; i++) {
        attron(COLOR_PAIR(5)); printw("%4i ", i + 1); attroff(COLOR_PAIR(5));
        for (int j = 0; j < strlen(p->il[i - p->sl]); j++) {
            if (selected(p, m, i, j)) attron(COLOR_PAIR(6));
            printw("%c", p->il[i - p->sl][j]);
            if (selected(p, m, i, j)) attroff(COLOR_PAIR(6));
        }
        printw("\n");
    }
    for (int i = p->nl; i < N; i++) {
        attron(COLOR_PAIR(3)); printw("~"); attroff(COLOR_PAIR(3));
        printw("\n");
    }
}

void copy_selection(struct content *p, struct curser *m) {
    int i2 = index_content(p, m);
    char *boz = (char *) malloc(SIZE);
    if (m->direct >= i2) {
        for (int i = i2; i < m->direct; i++) boz[i - i2] = p->str[i];
    }
    else {
        for (int i = m->direct; i < i2; i++) boz[i - m->direct] = p->str[i];
    }
    writeInFile("clipboard", boz);
    update_ith_line(p);
}

void delete_selection(struct bottom *b, struct content *p, struct curser *m) {
    int i2 = index_content(p, m);
    if (m->direct >= i2) removeMiddleString(&p->str, 'f', i2, m->direct - i2);
    else removeMiddleString(&p->str, 'f', m->direct, i2 - m->direct);
    update_ith_line(p);
    if (b->save == 1) b->save = 2;
}

void cut_selection(struct bottom *b, struct content *p, struct curser *m) {
    copy_selection(p, m);
    delete_selection(b, p, m);
}

void paste_normal(struct content *p, struct curser *m) {
    FILE *fp = fopen("clipboard", "r");
    int size;
    for (size = 0; getc(fp) != EOF; size++);
    rewind(fp);
    char *boz = (char *) malloc(size + 1);
    for (int i = 0; i < size; i++) boz[i] = getc(fp);
    fclose(fp);
    writeMiddleString(&p->str, boz, index_content(p, m));
    update_ith_line(p);
}

void find_h(struct bottom *b, struct content *p, struct curser *m) {
    sprintf(b->cb, "/");
    char c;
    while (1) {
        clear();
        show_content(p);
        show_bottom(b);
        refresh();
        c = getch();
        if (c == 10) break;
        if (c == 127) b->cb[strlen(b->cb) - 1] = '\0';
        else b->cb[strlen(b->cb)] = c;
    }
    struct highlight h;
    h.cnt = 0;
    h.s_s = (int **) malloc(SIZE * sizeof(int *));
    countString(p->str, 0, b->cb + 1, 0, b->cb + 1, h.s_s, &h.cnt, 0);
    if (h.cnt) while (1) {
        clear();
        show_highlight(p, m, h);
        show_bottom(b);
        refresh();
        if (getch() != 'n') break;
        go_first_highlight(p, m, h);
    }
    else printw("\nNothing found! ");
    for (int i = strlen(b->cb); i; i--) b->cb[i - 1] = '\0';
}

void go_first_highlight(struct content *p, struct curser *m, struct highlight h) {
    m->line = 0;
    for (int i = 0; i < h.s_s[0][0]; i++) if (p->str[i] == '\n') m->line++;
    int i = 0;
    for (int a = 0; a < m->line; i++) if (p->str[i] == '\n') a++;
    m->pos = h.s_s[0][0] - i;
    i = 0;
    p->sl = m->line;
    for (int a = 0; a < strlen(p->str); a++) if (p->str[a] == '\n') i++;
    if (i - p->sl >= N) p->nl = N;
    else p->nl = i - p->sl;
}

void show_highlight(struct content *p, struct curser *m, struct highlight h) {
    for (int i = p->sl; i < p->nl + p->sl; i++) {
        attron(COLOR_PAIR(5)); printw("%4i ", i + 1); attroff(COLOR_PAIR(5));
        for (int j = 0; j < strlen(p->il[i - p->sl]); j++) {
            if (in_highlight(p, m, h, i, j)) attron(COLOR_PAIR(7));
            printw("%c", p->il[i - p->sl][j]);
            if (in_highlight(p, m, h, i, j)) attroff(COLOR_PAIR(7));
        }
        printw("\n");
    }
    for (int i = p->nl; i < N; i++) {
        attron(COLOR_PAIR(3)); printw("~"); attroff(COLOR_PAIR(3));
        printw("\n");
    }
}

int in_highlight(struct content *p, struct curser *m, struct highlight h, int i, int j) {
    struct curser y;
    y.line = i;
    y.pos = j;
    y.direct = index_content(p, &y);
    for (int x = 0; x < h.cnt; i++) {
        if (h.s_s[x][0] <= y.direct && y.direct <= h.s_s[x][1]) return 1;
    }
    return 0;
}
