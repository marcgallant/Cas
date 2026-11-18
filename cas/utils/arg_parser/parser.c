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

void optional_print_usage(Argument *optional) {
    printf("[");
    const char **names = optional->data.optional.names;
    if (names[0][1] == '-')
        printf(CYAN "%s", names[0]);
    else
        printf(GREEN "%s", names[0]);

    printf(RESET);

    if (optional->choices) {
        printf(YELLOW);
        printf(" {");
        for (size_t i = 0; optional->choices[i]; i++) {
            printf("%s", optional->choices[i]);

            if (optional->choices[i + 1])
                printf(",");
        }

        printf("}");
        printf(RESET);
    }

    printf(YELLOW);

    if (optional->nargs > 0)
    {
        if (optional->metavar[1] != NULL) {
            for (int i = 0; i < optional->nargs; i++) {
                printf(" %s", optional->metavar[i]);
            }
        }
        else {
            for (int i = 0; i < optional->nargs; i++) {
                printf(" %s", optional->metavar[0]);
            }
        }
    } else if (optional->nargs == NARGS_OPTIONAL) {
        printf(YELLOW " [%s]", optional->metavar[0]);
    } else if (optional->nargs == NARGS_ONE_OR_MORE) {
        printf(" %s [%s ...]", optional->metavar[0], optional->metavar[0]);

    } else if (optional->nargs == NARGS_ZERO_OR_MORE) {
        printf(" %s ...", optional->metavar[0]);
    }

    printf(RESET);
    printf("]");
}

void optional_print_help(Argument *optional) {
    int n = 0;
    n += printf("  ");

    const char **names = optional->data.optional.names;

    for (size_t i = 0; names[i]; i++) {
        printf(BOLD);

        if (names[i][1] == '-')
            n += printf(CYAN "%s", names[i]) - 5;
        else
            n += printf(GREEN "%s", names[i]) - 5;

        printf(RESET);

        if (names[i + 1])
            n += printf(", ");
    }

    if (optional->choices) {
        printf(YELLOW BOLD);
        n += printf(" {");
        for (size_t i = 0; optional->choices[i]; i++) {
            n += printf("%s", optional->choices[i]);

            if (optional->choices[i + 1])
                n += printf(",");
        }

        n += printf("}");
        printf(RESET);
    }

    printf(YELLOW BOLD);

    if (optional->nargs > 0)
    {
        if (optional->metavar[1] != NULL) {
            for (int i = 0; i < optional->nargs; i++) {
                n += printf(" %s", optional->metavar[i]);
            }
        }
        else {
            for (int i = 0; i < optional->nargs; i++) {
                n += printf(" %s", optional->metavar[0]);
            }
        }
    } else if (optional->nargs == NARGS_OPTIONAL) {
        n += printf(" [%s]", optional->metavar[0]);
    } else if (optional->nargs == NARGS_ONE_OR_MORE) {
        n += printf(" %s [%s ...]", optional->metavar[0], optional->metavar[0]);

    } else if (optional->nargs == NARGS_ZERO_OR_MORE) {
        n += printf(" [%s ...]", optional->metavar[0]);
    }

    printf(RESET);

    if (n >= 24) {
        puts("");
        n = 0;
    }

    printf("%*s%s\n", 24 - n, "", optional->help);
}

void positional_print_usage(Argument *positional) {
    printf(GREEN);
    if (!positional->required)
        printf("[");

    const char *name = positional->data.positional.name;

    if (positional->nargs > 0) {
        for (int i = 0; i < positional->nargs; i++) {
            printf("%s", name);
        }
    } else if (positional->nargs == NARGS_OPTIONAL) {
        printf( "[%s]", name);
    } else if (positional->nargs == NARGS_ONE_OR_MORE) {
        printf( "%s [%s ...]", name, name);

    } else if (positional->nargs == NARGS_ZERO_OR_MORE) {
        printf( "%s ...", name);
    }

    if (!positional->required)
        printf("]");

    printf(RESET);
}

void positional_print_help(Argument *positional) {
    int n = 0;
    n += printf("  ");

    printf(GREEN BOLD);

    if (positional->nargs > 0) {
        for (int i = 0; i < positional->nargs; i++) {
            n += printf("%s", positional->data.positional.name);
        }
    } else {
        n += printf("%s", positional->data.positional.name);
    }

    printf(RESET);

    if (n >= 24) {
        puts("");
        n = 0;
    }

    printf("%*s%s\n", 24 - n, "", positional->help);
}

void subparser_print_usage(Argument *subparser) {
    printf(GREEN);
    printf("{");
    struct dlist_item *tmp = subparser->data.subparser.parsers->head;
    while (tmp) {
        Parser *parser = tmp->data;
        printf("%s", parser->prog);

        if (tmp->next)
            printf(",");

        tmp = tmp->next;
    }

    printf("}");
    printf(RESET);
}

void subparser_print_help(Argument *subparser) {
    int n = 0;
    n += printf("  ");

    printf(GREEN BOLD);
    n += printf("{");

    struct dlist_item *tmp = subparser->data.subparser.parsers->head;
    while (tmp) {
        Parser *parser = tmp->data;
        n += printf("%s", parser->prog);

        if (tmp->next)
            n += printf(",");

        tmp = tmp->next;
    }

    n += printf("}");
    printf(RESET);

    if (n >= 24) {
        puts("");
        n = 0;
    }

    printf("%*s%s\n", 24 - n, "", subparser->help);

    tmp = subparser->data.subparser.parsers->head;
    while (tmp) {
        Parser *parser = tmp->data;

        n = 0;
        n += printf("    ");

        printf(GREEN BOLD);

        n += printf("%s", parser->prog);

        printf(RESET);

        if (n >= 24) {
            puts("");
            n = 0;
        }

        printf("%*s%s\n", 24 - n, "", parser->description);

        tmp = tmp->next;
    }
}

void arguments_print_usage(struct dlist *arguments) {
    if (!arguments || arguments->size == 0) return;

    struct dlist_item *tmp = arguments->head;
    while (tmp) {
        printf(" ");
        Argument *arg= tmp->data;
        if (arg->type == ARG_POSITIONAL)
            positional_print_usage(arg);
        else
            subparser_print_usage(arg);

        tmp = tmp->next;
    }
}

void arguments_print_help(struct dlist *arguments) {
    if (!arguments || arguments->size == 0) return;

    puts(BLUE BOLD "positional arguments:" RESET);

    struct dlist_item *tmp = arguments->head;
    while (tmp) {
        Argument *arg= tmp->data;
        if (arg->type == ARG_POSITIONAL)
            positional_print_help(arg);
        else
            subparser_print_help(arg);

        tmp = tmp->next;
    }
}

void options_print_usage(struct dlist *options) {
    if (!options || options->size == 0) return;

    struct dlist_item *tmp = options->head;
    while (tmp) {
        printf(" ");
        optional_print_usage(tmp->data);
        tmp = tmp->next;
    }
}

void options_print_help(struct dlist *options) {
    if (!options || options->size == 0) return;

    puts(BLUE BOLD "options:" RESET);

    struct dlist_item *tmp = options->head;
    while (tmp) {
        optional_print_help(tmp->data);
        tmp = tmp->next;
    }
}

void parser_print_usage(Parser *parser) {
    if (!parser) return;

    printf(BLUE BOLD "usage: " MAGENTA "%s" RESET, parser->prog);

    options_print_usage(parser->options);
    arguments_print_usage(parser->arguments);

    printf("\n");
}

void parser_print_help(Parser *parser) {
    parser_print_usage(parser);

    printf("\n%s\n\n", parser->description);

    arguments_print_help(parser->arguments);
    puts("");
    options_print_help(parser->options);
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
