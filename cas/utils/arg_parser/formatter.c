#include "formatter.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

Formatter *formatter_init(void) {
    Formatter *formatter = calloc(1, sizeof(Formatter));

    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        return NULL;

    *formatter = (Formatter) {
        .indent = 2,
        .max_help_position = 24,
        .width = ws.ws_col - 2
    };

    return formatter;
}

void format(Formatter *formatter, const char *text) {
    int len = strlen(text);
    int start = 0;

    while (start < len) {
        int end = start + formatter->width - formatter->max_help_position;

        if (end >= len) {
            printf("%s\n", text + start);
            break;
        }

        while (end > start && !isspace(text[end])) {
            end--;
        }

        if (end == start) {
            end = start + formatter->width - formatter->max_help_position;
        }

        printf("%.*s\n%*s", end - start, text + start, formatter->max_help_position, "");

        start = end;

        while (isspace(text[start])) {
            start++;
        }
    }
}
