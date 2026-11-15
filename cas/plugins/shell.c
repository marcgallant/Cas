#include "plugins.h"

Parser *shell_parser_init() {
    Parser *shell_parser = parser_init("shell", "Run a shell in the build environment.");
    dlist_push_back(shell_parser->options, help_init());
    dlist_push_back(shell_parser->options, skip_init());
    dlist_push_back(shell_parser->options, force_checkout_init());
    dlist_push_back(shell_parser->options, update_init());
    dlist_push_back(shell_parser->options, preserve_env_init());
    dlist_push_back(shell_parser->options, keep_config_unchanged_init());
 
    static const char *command2_names[] = {
        "-c"
        "--command",
        NULL
    };

    Argument *command2 = calloc(1, sizeof(Argument));
    *command2 = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Run command",
        .metavar = "COMMAND",
        .nargs = 1,
        .required = false,
        .data.optional.names = command2_names
    };
    dlist_push_back(shell_parser->options, command2);

    dlist_push_back(shell_parser->arguments, config_init());
    

    return shell_parser;
}
