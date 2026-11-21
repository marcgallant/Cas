#include "context.h"

#define _GNU_SOURCE

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>

bool is_digit_string(const char *str)
{
    if (*str == '\0')
        return false;

    while (*str) {
        if (!isdigit((unsigned char)*str))
            return false;
        str++;
    }

    return true;
}

Context *context_init(void) {
    Context *context = calloc(1, sizeof(Context));

    char *work_dir = getenv("KAS_WORK_DIR");
    if (work_dir == NULL) {} //TODO

    context->kas_work_dir = realpath(work_dir, NULL);
    if (context->kas_work_dir == NULL) {} //TODO

    char *build_dir = getenv("KAS_BUILD_DIR");
    if (build_dir == NULL) {
        asprintf(&build_dir, "%s/build", context->kas_work_dir);
    }

    context->kas_build_dir = realpath(build_dir, NULL);
    free(build_dir);
    if (context->kas_build_dir == NULL) {} //TODO

    char *ref_dir = getenv("KAS_REPO_REF_DIR");
    if (ref_dir != NULL) {
        context->kas_repo_ref_dir = realpath(ref_dir, NULL);
        if (context->kas_repo_ref_dir == NULL) {} //TODO
    }

    char *clone_depth = getenv("KAS_CLONE_DEPTH");
    if (clone_depth == NULL) {
        clone_depth = "0";
    }

    if (!is_digit_string(clone_depth)) {} //TODO
    //raise KasUserError('KAS_CLONE_DEPTH must be a number')

    context->repo_clone_depth = MAX(atoi(clone_depth), 0);

    context->managed_paths = hash_set_init(8);

    if (getenv("KAS_BUILD_DIR") == NULL) {
        hash_set_insert(context->managed_paths, context->kas_build_dir);
    }

    return context;
}

void context_destroy(Context *context) {
    hash_set_destroy(context->managed_paths);
    free(context);
}

/*
self.setup_initial_environ()
self.check_container_call()
# Register the paths that kas created and exclusively owns
self.keyhandler = {}
self.config = None
self.args = args
*/
