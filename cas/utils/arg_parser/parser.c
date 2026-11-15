#include "parser.h"

#include <stdlib.h>

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

void parser_print_usage(Parser *parser) {
     
}

void parser_print_help(Parser *parser) {
    
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
