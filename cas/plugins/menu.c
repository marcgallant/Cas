#include "plugins.h"

Parser *menu_parser_init(void) {
    Parser *menu_parser = parser_init("menu", "Provides a configuration menu and triggers the build of the choices.");
    dlist_push_back(menu_parser->options, help_init());

    Argument *kconfig = calloc(1, sizeof(Argument));
    *kconfig = (Argument) {
        .type = ARG_POSITIONAL,
        .value_type = ARG_STRING,
        .help = "Kconfig file",
        .nargs = 1,
        .required = false,
        .data.positional.name = "kconfig" 
    };
    dlist_push_back(menu_parser->arguments, kconfig);
    
    return menu_parser;
}
