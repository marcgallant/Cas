#include "plugins.h"

Parser *lock_parser_init(void) {
    Parser *lock_parser = parser_init("lock", "Create and update kas project lockfiles.");
    dlist_push_back(lock_parser->options, help_init());
    dlist_push_back(lock_parser->options, skip_init());
    dlist_push_back(lock_parser->options, force_checkout_init());
    dlist_push_back(lock_parser->options, update_init());
    dlist_push_back(lock_parser->options, indent_init());
    dlist_push_back(lock_parser->options, sort_init());
    dlist_push_back(lock_parser->arguments, config_init());

    return lock_parser;
}

