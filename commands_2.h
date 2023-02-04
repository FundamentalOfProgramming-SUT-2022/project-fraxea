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
    int mode; // normal, insert, visual
    int save; // saved, unsaved, not saved yet
    char *name;
    char *command_bar;
};

struct highlight {
    // for find highlight
};

void update_ith_line(struct content *); // using content to update 12 lines
void update_content(struct content *); // using ith_line to update
char* merge_line(char **, int); // merge lines with \n between them
int size_lines_n(char **, int); // need for malloc
void move_up_curser(struct content *, struct curser *); // move up with 'w'
void move_down_curser(struct content *, struct curser *); // move down with 's'
void move_right_curser(struct content *, struct curser *); // move right with 'd'
void move_left_curser(struct curser *); // move left with 'a'
void show_bottom(struct bottom *); // show the live state of bottoom
void start_command(struct bottom *); // reading the command in command bar
void show_content_normal(struct content *, struct curser *); // live state normal mode
void show_content_visual(struct content *, struct curser *); // live state visual mode
void delete_selection(struct content *, struct curser *); // delete selection mode
void copy_selection(struct content *, struct curser *); // copy selection mode with 'c'
void cut_selection(struct content *, struct curser *); // cut selection mode with 'x'
void paste_normal(struct content *, struct curser *); // paste normal mode with 'v'
void save_file(struct bottom *, struct content *); // save and saveas with name
void open_file(struct bottom *, struct content *); // open new and save previous
void undo_normal(struct bottom *, struct content *); // undo to last saved with 'z'
void find_h(struct bottom *, struct content *, struct curser *); // live state find highlight
void go_first_highlight(struct content *, struct curser *); // first highlight with 'n'
void replace_curser(struct bottom *, struct content *, struct curser *); // replace curser
void auto_indent_normal(struct content *, struct curser *); // auto-indent normal with '='


void update_ith_line(struct content *p) {
    int i = 0;
    for (int a = 1; a < p->sl; i++) {
        if (p->str[i] == '\n') a++;
    }
    p->nl = 0;
    for (int a = 0; p->nl < N && i < strlen(p->str); i++) {
        if (p->str[i] == '\n') {
            p->il[p->nl][a] = '\0';
            a = 0;
            p->nl++;
        }
        else p->il[p->nl][a++] = p->str[i];
    }
}

void update_content(struct content *p) {
    int i = 0;
    char *middle;
    for (int a = 0; a < p->sl; i++) {
        if (p->str[i] == '\n') a++;
    }
    int f = i;
    for (int a = 0; a < p->nl; i++) {
        if (p->str[i] == '\n') a++;
    }
    removeMiddleString(&(p->str), 'f', f, i - f);
    middle = merge_line(p->il, p->nl);
    writeMiddleString(&(p->str), middle, i);
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
    if (m->line == n) return;
    if (m->line + 5 > p->sl + N && n > p->sl + N) {
        p->sl++;
        update_ith_line(p);
    }
    m->line++;
    if (m->pos >= strlen(p->il[m->line - p->sl])) m->pos = strlen(p->il[m->line - p->sl]) - 1;
}

void move_right_curser(struct content *p, struct curser *m) {
    if (m->pos == strlen(p->il[m->line - p->sl]) + 1) return;
    m->pos++;
}

void move_left_curser(struct curser *m) {
    if (!m->pos) return;
    m->pos--;
}
