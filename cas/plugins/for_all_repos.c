#include "plugins.h"

Parser *for_all_repos_parser_init(void) {
    Parser *for_all_repos_parser = parser_init("for-all-repos", "Runs a specified command in all checked out repositories.");
    dlist_push_back(for_all_repos_parser->options, help_init());
    dlist_push_back(for_all_repos_parser->options, skip_init());
    dlist_push_back(for_all_repos_parser->options, force_checkout_init());
    dlist_push_back(for_all_repos_parser->options, update_init());
    dlist_push_back(for_all_repos_parser->options, preserve_env_init());
    dlist_push_back(for_all_repos_parser->options, keep_config_unchanged_init());
    dlist_push_back(for_all_repos_parser->arguments, config_init());

    Argument *command = calloc(1, sizeof(Argument));
    *command = (Argument) {
        .type = ARG_POSITIONAL,
        .value_type = ARG_STRING,
        .help = "Command to be executed as a string.",
        .nargs = 0,
        .required = true,
        .data.positional.name = "command"
    };
    dlist_push_back(for_all_repos_parser->arguments, command);

    return for_all_repos_parser;
}
