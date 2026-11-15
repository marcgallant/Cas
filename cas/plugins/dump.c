#include "plugins.h"

Parser *dump_parser_init(void) {
    Parser *dump_parser = parser_init("dump", "Expand and dump the final config to stdout. When resolving branches, this is done before patches are applied.");
    dlist_push_back(dump_parser->options, help_init());
    dlist_push_back(dump_parser->options, skip_init());
    dlist_push_back(dump_parser->options, force_checkout_init());
    dlist_push_back(dump_parser->options, update_init());
    dlist_push_back(dump_parser->options, indent_init());
    dlist_push_back(dump_parser->options, sort_init());
        
    static const char *format2_names[] = {
        "--format",
        NULL
    };

    static const char *format2_choices[] = {
        "yaml",
        "json",
        NULL
    };
    
    Argument *format2 = calloc(1, sizeof(Argument));
    *format2 = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Output format (default: yaml)",
        //.nargs = 1,
        .required = false,
        .default_value.string = "yaml",
        .choices = format2_choices,
        .data.optional.names = format2_names
    };
    dlist_push_back(dump_parser->options, format2);

    static const char *resolve_refs_names[] = {
        "--resolve-refs",
        NULL
    };

    Argument *resolve_refs = calloc(1, sizeof(Argument));
    *resolve_refs = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Replace floating refs with exact SHAs. Overrides are removed",
        .data.optional.names = resolve_refs_names
        
    };
    dlist_push_back(dump_parser->options, resolve_refs);

    static const char *resolve_local_names[] = {
        "--resolve-local",
        NULL
    };

    Argument *resolve_local = calloc(1, sizeof(Argument));
    *resolve_local = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Add tracking information of root repo",
        .data.optional.names = resolve_local_names
    };
    dlist_push_back(dump_parser->options, resolve_local);

    static const char *resolve_env_names[] = {
        "--resolve-env",
        NULL
    };

    Argument *resolve_env = calloc(1, sizeof(Argument));
    *resolve_env = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Set env defaults to captured env value",
        .data.optional.names = resolve_env_names 
    };
    dlist_push_back(dump_parser->options, resolve_env);

    static const char *lock_names[] = {
        "--lock",
        NULL
    };

    Argument *lock = calloc(1, sizeof(Argument));
    *lock = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Create lockfile with exact SHAs",
        .data.optional.names = lock_names
    };
    dlist_push_back(dump_parser->options, lock);
    dlist_push_back(dump_parser->arguments, config_init());
    
    return dump_parser;
}
