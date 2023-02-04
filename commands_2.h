#include <ncurses.h>
#include "commands.h"
#define N 14 // max nl

struct curser {
    int line;
    int pos;
    int a_line; // line of curser at the beggining of visual mode
    int a_pos; // pos of curser at the beggining of visual mode
    int direction; // 1 or -1
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
    // for find highlight
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
void show_content_visual(struct content *, struct curser *); // live state visual mode
void delete_selection(struct content *, struct curser *); // delete selection mode
void copy_selection(struct content *, struct curser *); // copy selection mode with 'c'
void cut_selection(struct content *, struct curser *); // cut selection mode with 'x'
void paste_normal(struct content *, struct curser *); // paste normal mode with 'v'
void save_file(struct bottom *, struct content *); // save and saveas with name
void open_file(struct bottom *, struct content *); // open new and save previous
void undo_normal(struct bottom *, struct content *); // undo to last saved with 'z'
void find_h(struct bottom *, struct content *, struct curser *); // live state find '/'
void go_first_highlight(struct content *, struct curser *); // first highlight with 'n'
void replace_curser(struct bottom *, struct content *, struct curser *); // replace curser
void auto_indent_normal(struct content *, struct curser *); // auto-indent normal with '='


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
    else sprintf(mode, "   ");
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
    struct bottom b;
    init_bottom(&b, path);
    struct content p;
    init_content(&p, path);
    struct curser m;
    init_curser(&m);
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
        // if (c == '=') auto_indent_normal(&p, &m);
        // if (c == 'z') undo_normal(&b, &p);
        // if (c == 'v') paste_normal(&p, &m);
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
    if (fp == NULL) sprintf(p->str, " ");
    else {
        int size;
        for (size = 0; getc(fp) != EOF; size++);
        rewind(fp);
        p->str = realloc(p->str, size + 1);
        for (int i = 0; i < size; i++) p->str[i] = getc(fp);
        p->str[size] = '\0';
    }
    fclose(fp);
    update_ith_line(p);
}

void init_curser(struct curser *m) {
    m->line = 0;
    m->pos = 0;
}

void init_bottom(struct bottom *b, char *path) {
    b->cb = (char *) malloc(SIZE);
    b->cb[0] = '\0';
    b->mode = 1;
    b->name = path;
    if (fopen(path, "r") == NULL) b->save = 3;
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
        printw("\nline = %i pos = %i", m->line, m->pos);
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
        b->save = 2;
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
