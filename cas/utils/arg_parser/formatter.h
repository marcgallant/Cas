#ifndef FORMATTER_H
#define FORMATTER_H

typedef struct {
    int indent;
    int max_help_position;
    int width;
} Formatter;

Formatter *formatter_init(void);

void format(Formatter *formatter, const char *text);

#endif /* ! FORMATTER_H */
