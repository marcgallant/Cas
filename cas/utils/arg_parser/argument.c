#include "argument.h"

#include <stdlib.h>

Argument *help_init(void) {
    static const char *help_names[] = {
        "-h",
        "--help",
        NULL
    };

    Argument *help = calloc(1, sizeof(*help));
    if (!help)
        return NULL;

    *help = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "show this help message and exit",
        .data.optional.names = help_names
    };

    return help;
}

Argument *skip_init(void) {
    static const char *skip_names[] = {
        "--skip",
        NULL
    };

    static const char *step_metavar[] = {
        "STEP",
        NULL
    };

    Argument *skip = calloc(1, sizeof(Argument));
    *skip = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_STRING,
        .help = "Skip build steps. To skip more than one step, use this argument multiple times. Possible choices: setup_dir, setup_home, init_setup_repos, repo_setup_loop, finish_setup_repos, repos_checkout, repos_check_signatures, repos_apply_patches, setup_environ, write_bbconfig.",
        .metavar = step_metavar,
        .nargs = 1,
        .required = false,
        .data.optional.names = skip_names
    };
    return skip;
}

Argument *force_checkout_init(void) {
    static const char *force_checkout_names[] = {
        "--force-checkout",
        NULL
    };

    Argument *force_checkout = calloc(1, sizeof(Argument));
    *force_checkout = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Always checkout the desired commit/branch/tag of each repository, discarding any local changes",
        .data.optional.names = force_checkout_names
    };
    return force_checkout;
}

Argument *update_init(void) {
    static const char *update_names[] = {
        "--update",
        NULL
    };

    Argument *update = calloc(1, sizeof(Argument));
    *update = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Pull new upstream changes to the desired branch even if it is already checked out locally",
        .data.optional.names = update_names
    };
    return update;
}

Argument *dry_run_init(void) {
    static const char *dry_run_names[] = {
        "--dry-run",
        NULL
    };

    Argument *dry_run = calloc(1, sizeof(Argument));
    *dry_run = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Do not remove anything, just print what would be removed",
        .data.optional.names = dry_run_names
    };
    return dry_run;
}

Argument *isar_init(void) {
    static const char *isar_names[] = {
        "--isar",
        NULL
    };

    Argument *isar = calloc(1, sizeof(Argument));
    *isar = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Use ISAR build directory layout",
        .data.optional.names = isar_names
    };
    return isar;
}

Argument *keep_config_unchanged_init(void) {
    static const char *keep_config_unchanged_names[] = {
        "-k",
        "--keep_config_unchanged",
        NULL
    };

    Argument *keep_config_unchanged = calloc(1, sizeof(Argument));
    *keep_config_unchanged = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Skip steps that change the configuration",
        .data.optional.names = keep_config_unchanged_names
    };
    return keep_config_unchanged;
}

Argument *indent_init(void) {
    static const char *indent_names[] = {
        "--indent",
        NULL
    };

    static const char *indent_metavar[] = {
        "INDENT",
        NULL
    };

    Argument *indent = calloc(1, sizeof(Argument));
    *indent = (Argument) {
        .type = ARG_OPTIONAL,
        .value_type = ARG_INT,
        .help = "Line indent (# of spaces, default: 4)", //TODO: check this out: automatic ?
        .metavar = indent_metavar,
        .default_value.integer = 4,
        .nargs = 1,
        .required = false,
        .data.optional.names = indent_names
    };
    return indent;
}

Argument *sort_init(void) {
    static const char *sort_names[] = {
        "--sort",
        NULL
    };

    Argument *sort = calloc(1, sizeof(Argument));
    *sort = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Alphanumerically sort keys in output",
        .data.optional.names = sort_names
    };
    return sort;
}

Argument *preserve_env_init(void) {
    static const char *preserve_env_names[] = {
        "-E",
        "--preserve-env",
        NULL
    };

    Argument *preserve_env = calloc(1, sizeof(Argument));
    *preserve_env = (Argument) {
        .type = ARG_OPTIONAL,
        .help = "Keep current user environment block",
        .data.optional.names = preserve_env_names
    };
    return preserve_env;
}

Argument *config_init(void) {
    Argument *config = calloc(1, sizeof(Argument));
    *config = (Argument) {
        .type = ARG_POSITIONAL,
        .value_type = ARG_STRING,
        .help = "Config file(s), separated by colon. Using .config.yaml in KAS_WORK_DIR if none is specified.",
        .nargs = 1,
        .required = false,
        .data.positional.name = "config"
    };
    return config;
}
