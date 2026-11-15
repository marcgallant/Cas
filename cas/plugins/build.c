#include "plugins.h"

Parser *build_parser_init(void) {
    Parser *build_parser = parser_init("build" , "Checks out all necessary repositories and builds using bitbake as specified in the configuration file."); 
    dlist_push_back(build_parser->options, help_init());
    dlist_push_back(build_parser->options, skip_init());
    dlist_push_back(build_parser->options, force_checkout_init());
    dlist_push_back(build_parser->options, update_init());

    dlist_push_back(build_parser->options, keep_config_unchanged_init());

    static const char *target_names[] = {
        "--target",
        NULL
    };

    Argument *target = calloc(1, sizeof(Argument));
    *target = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Select target to build",
        .metavar = "TARGET",
        .nargs = 1,
        .required = false,
        .data.optional.names = target_names
        
    };
    dlist_push_back(build_parser->options, target);

    static const char *task_names[] = {
        "-c",
        "-cmd",
        "--task",
        NULL
    };

    Argument *task = calloc(1, sizeof(Argument));
    *task = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Select which task should be executed",
        .metavar = "TASK",
        .nargs = 1,
        .required = false,
        .data.optional.names = task_names
        
    };
    dlist_push_back(build_parser->options, task);

    static const char *provenance_names[] = {
        "--provenance",
        NULL
    };

    static const char *provenance_choices[] = {
        "true",
        "mode=min",
        "mode=max",
        NULL
    };
    
    Argument *provenance = calloc(1, sizeof(Argument));
    *provenance = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Enable provenance attestation generation",
        //.nargs = 1,
        .required = false,
        .choices = provenance_choices,
        .data.optional.names = provenance_names
        
    };
    dlist_push_back(build_parser->options, provenance);

    dlist_push_back(build_parser->arguments, config_init());

    Argument *extra_bitbake_args = calloc(1, sizeof(Argument));
    *provenance = (Argument) {
        .type = ARG_POSITIONAL,
        .value_type = ARG_STRING,
        .help = "Extra arguments to pass to bitbake (typically requires separation via '--')",
        .nargs = -1, //TODO: define
        .required = false,
        .data.positional.name = "extra_bitbake_args" 
    };
    dlist_push_back(build_parser->arguments, extra_bitbake_args);
    
    return build_parser;
}
