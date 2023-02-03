#include "commands.h"

int main() {
    unsigned long size = SIZE;
    char *line = (char *) malloc(size);
    while (1) {
        getline(&line, &size, stdin);
        readCommandLine(line);
    }
    return 0;
}
