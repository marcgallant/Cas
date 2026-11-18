#ifndef PARSER_H
#define PARSER_H

#include <dlist/dlist.h>

#include "argument.h"
#include "formatter.h"

typedef struct {
    const char      *prog;
    const char      *description;

    struct dlist    *arguments;
    struct dlist    *options;

    Formatter *formatter;
} Parser;

Parser *parser_init(const char *prog, const char *description);

Subparser parser_add_subparser(Parser *parser, const char *help);

// int parser_parse_args(Parser *parser, int argc, char **argv);

void parser_print_usage(Parser *parser);

void parser_print_help(Parser *parser);

void parser_destroy(Parser *parser);

#endif /* ! PARSER_H */
