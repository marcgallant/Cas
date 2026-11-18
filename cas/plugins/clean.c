#include "plugins.h"

Parser *clean_parser_init(void) {
    Parser *clean_parser = parser_init("clean", "Clean build artifacts, keep sstate cache and downloads.");
    dlist_push_back(clean_parser->options, help_init());
    dlist_push_back(clean_parser->options, dry_run_init());
    dlist_push_back(clean_parser->options, isar_init());
    dlist_push_back(clean_parser->arguments, config_init());
    return clean_parser;
}

Parser *cleansstate_parser_init(void) {
    Parser *cleansstate_parser = parser_init("cleansstate", "Clean build artifacts and sstate cache.");
    dlist_push_back(cleansstate_parser->options, help_init());
    dlist_push_back(cleansstate_parser->options, dry_run_init());
    dlist_push_back(cleansstate_parser->options, isar_init());
    dlist_push_back(cleansstate_parser->arguments, config_init());

    return cleansstate_parser;
}

Parser *cleanall_parser_init(void) {
    Parser *cleanall_parser = parser_init("cleanall", "Clean build artifacts, sstate-cache and downloads.");
    dlist_push_back(cleanall_parser->options, help_init());
    dlist_push_back(cleanall_parser->options, dry_run_init());
    dlist_push_back(cleanall_parser->options, isar_init());
    dlist_push_back(cleanall_parser->arguments, config_init());

    return cleanall_parser;
}

Parser *purge_parser_init(void) {
    Parser *purge_parser = parser_init("purge", "Purge all data managed by kas, including managed repos.");
    dlist_push_back(purge_parser->options, help_init());
    dlist_push_back(purge_parser->options, dry_run_init());
    dlist_push_back(purge_parser->options, isar_init());
    dlist_push_back(purge_parser->arguments, config_init());

    static const char *preserve_repo_refs_names[] = {
        "--preserve-repo-refs",
        NULL
    };

    Argument *preserve_repo_refs = calloc(1, sizeof(Argument));
    *preserve_repo_refs = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Do not remove the reference repositories",
        .data.optional.names = preserve_repo_refs_names

    };
    dlist_push_back(purge_parser->options, preserve_repo_refs);

    return purge_parser;
}
