#include "plugins.h"

Parser *shell_parser_init() {
    Parser *shell_parser = parser_init("shell", "Run a shell in the build environment.");
    dlist_push_back(shell_parser->options, help_init());
    dlist_push_back(shell_parser->options, skip_init());
    dlist_push_back(shell_parser->options, force_checkout_init());
    dlist_push_back(shell_parser->options, update_init());
    dlist_push_back(shell_parser->options, preserve_env_init());
    dlist_push_back(shell_parser->options, keep_config_unchanged_init());

    static const char *command_names[] = {
        "-c"
        "--command",
        NULL
    };

    static const char *command_metavar[] = {
        "COMMAND",
        NULL
    };

    Argument *command = calloc(1, sizeof(Argument));
    *command = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Run command",
        .metavar = command_metavar,
        .nargs = 1,
        .required = false,
        .data.optional.names = command_names
    };
    dlist_push_back(shell_parser->options, command);

    dlist_push_back(shell_parser->arguments, config_init());

    return shell_parser;
}
