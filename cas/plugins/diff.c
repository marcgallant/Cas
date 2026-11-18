#include "plugins.h"

Parser *diff_parser_init(void) {
    Parser *diff_parser = parser_init("diff", "Compare two KAS configurations.");
    dlist_push_back(diff_parser->options, help_init());
    dlist_push_back(diff_parser->options, skip_init());
    dlist_push_back(diff_parser->options, force_checkout_init());
    dlist_push_back(diff_parser->options, update_init());

    static const char *format_names[] = {
        "--format",
        NULL
    };

    static const char *format_choices[] = {
        "json",
        "text",
        NULL
    };

    Argument *format = calloc(1, sizeof(Argument));
    *format = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Diff output format (default: text)",
        //.nargs = 1,
        .required = false,
        .default_value.string = "text",
        .choices = format_choices,
        .data.optional.names = format_names
    };

    dlist_push_back(diff_parser->options, format);

    static const char *oneline_names[] = {
        "--oneline",
        NULL
    };

    Argument *oneline = calloc(1, sizeof(Argument));
    *oneline = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Use git oneline output for differing commits.",
        .data.optional.names = oneline_names

    };
    dlist_push_back(diff_parser->options, oneline);

    static const char *no_color_names[] = {
        "--no-color",
        NULL
    };

    Argument *no_color = calloc(1, sizeof(Argument));
    *no_color = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Disable colored highlighting of diffs.",
        .data.optional.names = no_color_names

    };
    dlist_push_back(diff_parser->options, no_color);

    static const char *commit_only_names[] = {
        "--commit-only",
        NULL
    };

    Argument *commit_only = calloc(1, sizeof(Argument));
    *commit_only = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "This will not display the differences in the KAS configurations; it will only list commits resulting from different repository revisions.",
        .data.optional.names = commit_only_names
    };
    dlist_push_back(diff_parser->options, commit_only);

    static const char *content_only_names[] = {
        "--content-only",
        NULL
    };

    Argument *content_only = calloc(1, sizeof(Argument));
    *content_only = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "This will only display the differences in the KAS configurations and will not include repository differences.",
        .data.optional.names = content_only_names
    };
    dlist_push_back(diff_parser->options, content_only);

    Argument *config1 = calloc(1, sizeof(Argument));
    *config1 = (Argument) {
        .type = ARG_POSITIONAL,
        .value_type = ARG_STRING,
        .help = "The first config file to be compared.",
        .nargs = 1,
        .required = true,
        .data.positional.name = "config1"
    };
    dlist_push_back(diff_parser->arguments, config1);

    Argument *config2 = calloc(1, sizeof(Argument));
    *config2 = (Argument) {
        .type = ARG_POSITIONAL,
        .value_type = ARG_STRING,
        .help = "The second config file to be compared.",
        .nargs = 1,
        .required = true,
        .data.positional.name = "config2"
    };
    dlist_push_back(diff_parser->arguments, config2);

    return diff_parser;
}
