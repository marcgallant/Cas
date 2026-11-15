#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include <colors.h>

Parser *parser_init(const char *prog, const char *description) {
    Parser *parser = calloc(1, sizeof(Parser));
    if (prog) parser->prog = prog;
    if (description) parser->description = description;

    parser->arguments = dlist_init();
    parser->options = dlist_init();

    return parser;    
}

Subparser parser_add_subparser(Parser *parser, const char *help) {
    Argument *subparser = calloc(1, sizeof(Argument));
    *subparser = (Argument) {
        .type = ARG_SUBPARSER,
        .help = help,
        .data.subparser.parsers = dlist_init()
    };
    dlist_push_back(parser->arguments, subparser);
    return subparser->data.subparser; 
};

void subparser_destroy(Subparser parser) {
    struct dlist *list = parser.parsers;
    while (list->head) {
        struct dlist_item *tmp = list->head;
        Parser *parser = tmp->data;
        parser_destroy(parser);

        list->head = list->head->next;
        free(tmp);
    }
    free(list);
}

void print_optional(Argument *optional) {
    int n = 0;
    n += printf("  ");

    const char **names = optional->data.optional.names;

    for (size_t i = 0; names[i]; i++) {
        if (names[i][1] == '-')
            n += printf(CYAN BOLD "%s" RESET, names[i]) - 5 - 4 - 4;
        else
            n += printf(GREEN BOLD "%s" RESET, names[i]) - 5 - 4 - 4;

        if (names[i + 1])
            n += printf(", ");
    }

    if (optional->choices) {
        n += printf(YELLOW BOLD "{") - 5 - 4;
        for (size_t i = 0; optional->choices[i]; i++) {
            n += printf("%s", optional->choices[i]);               

            if (optional->choices[i + 1])
                n += printf(",");
        }

        n += printf("}" RESET) - 4;
    }

    if (n > 24) {
        puts("\n");
        n = 0;
    }

    printf("%*s", 24 - n, "");

    printf("%s\n", optional->help);
}

void print_positional(Argument *positional) {
    (void)positional;
    //TODO
}

void print_subparser(Argument *subparser) {
    (void)subparser;
    //TODO
}

void print_arguments(struct dlist *arguments) {
    if (!arguments || arguments->size == 0) return;

    struct dlist_item *tmp = arguments->head;
    while (tmp) {
        Argument *arg= tmp->data;
        if (arg->type == ARG_POSITIONAL)
            print_positional(arg);
        else
            print_subparser(arg);

        tmp = tmp->next;
    }
}

void print_options(struct dlist *options) {
    if (!options || options->size == 0) return;

    puts(BLUE BOLD "options:\n" RESET);

    struct dlist_item *tmp = options->head;
    while (tmp) {
        print_optional(tmp->data);
        tmp = tmp->next;
    }
}

void parser_print_usage(Parser *parser) {
    if (!parser) return;

    printf(BLUE BOLD "usage: " MAGENTA "%s" RESET, parser->prog);
    //TODO
    printf("\n");
}

void parser_print_help(Parser *parser) {
    parser_print_usage(parser);
 
    printf("%s\n\n", parser->description);

    print_arguments(parser->arguments);
    print_options(parser->options);
}

void parser_destroy(Parser *parser) {
    dlist_destroy(parser->options);

    struct dlist *list = parser->arguments;
    while (list->head) {
        struct dlist_item *tmp = list->head;
        Argument *arg = tmp->data;
        if (arg->type == ARG_SUBPARSER)
            subparser_destroy(arg->data.subparser);

        list->head = list->head->next;
        free(tmp->data);
        free(tmp);
    }
    free(list);

    free(parser);
}
