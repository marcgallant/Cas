#define _GNU_SOURCE

#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>

#include <arg_parser/parser.h>
#include <distro/distro.h>
#include <logger/logger.h>

#include "context.h"
#include "plugins/plugins.h"
#include "version.h"

Parser *kas_parser_init(void) {
    Parser *parser = parser_init("kas", "kas - setup tool for bitbake based project");
    dlist_push_back(parser->options, help_init());

    static const char *version_names[] = {
        "--version",
        NULL
    };

    Argument *version = calloc(1, sizeof(Argument));
    *version = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "show program's version number and exit",
        .data.optional.names = version_names
    };
    dlist_push_back(parser->options, version);

    static const char *log_level_names[] = {
        "-l",
        "--log-level",
        NULL
    };

    static const char *log_level_choices[] = {
        "debug",
        "info",
        "warning",
        "error",
        "critical",
        NULL
    };

    Argument *log_level = calloc(1, sizeof(Argument));
    *log_level = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Set log level (default: info)",
        //.nargs = 1,
        .required = false,
        .default_value.string = "info",
        .choices = log_level_choices,
        .data.optional.names = log_level_names
    };
    dlist_push_back(parser->options, log_level);

    Subparser subparser = parser_add_subparser(parser, "sub command help");

    dlist_push_back(subparser.parsers, build_parser_init());
    dlist_push_back(subparser.parsers, checkout_parser_init());
    dlist_push_back(subparser.parsers, clean_parser_init());
    dlist_push_back(subparser.parsers, cleansstate_parser_init());
    dlist_push_back(subparser.parsers, cleanall_parser_init());
    dlist_push_back(subparser.parsers, purge_parser_init());
    dlist_push_back(subparser.parsers, diff_parser_init());
    dlist_push_back(subparser.parsers, dump_parser_init());
    dlist_push_back(subparser.parsers, for_all_repos_parser_init());
    dlist_push_back(subparser.parsers, lock_parser_init());
    dlist_push_back(subparser.parsers, menu_parser_init());
    dlist_push_back(subparser.parsers, shell_parser_init());

    return parser;
}

int main(int argc, char *argv[]) {
    (void)argc;

    Parser *parser = kas_parser_init();

    Distro *distro = distro_get();

    log_info("%s %s started on %s %s", basename(argv[0]), VERSION_STRING, distro->name, distro->version_id);

    parser_destroy(parser);

    return 0;
}
