#include "plugins.h"

Parser *checkout_parser_init(void) {
    Parser *checkout_parser = parser_init("checkout", "Checks out all necessary repositories and sets up the build directory as specified in the configuration file.");
    dlist_push_back(checkout_parser->options, help_init());
    dlist_push_back(checkout_parser->options, skip_init());
    dlist_push_back(checkout_parser->options, force_checkout_init());
    dlist_push_back(checkout_parser->options, update_init());

    dlist_push_back(checkout_parser->arguments, config_init());
    
    return checkout_parser;
}
