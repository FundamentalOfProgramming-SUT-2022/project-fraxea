#include <ncurses.h>

struct curser {
    int line;
    int pos;
    int a_line; // line of curser at the beggining of visual mode
    int a_pos; // pos of curser at the beggining of visual mode
    int direction; // 1 or -1
};

struct content {
    char *content;
    char **ith_line;
    int num_line;
    int start_line;
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

void update_ith_line(struct content *); // using content to update
void update_content(struct content *); // using ith_line
void move_up_curser(struct content *, struct curser *); // move up with 'w'
void move_down_curser(struct content *, struct curser *); // move down with 's'
void move_right_curser(struct content *, struct curser *); // move right with 'd'
void move_left_curser(struct content *, struct curser *); // move left with 'a'
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


int main() {
    
    return 0;
}


