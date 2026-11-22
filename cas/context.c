#include "context.h"

#define _GNU_SOURCE

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include <distro/distro.h>

#include "version.h"

extern char **environ;

static char *to_lower_string(const char *str) {
    if (*str == '\0')
        return NULL;

    char *str_lower = calloc(strlen(str), sizeof(char));

    size_t i = 0;
    while (str[i]) {
        str_lower[i] = tolower(str[i]);
        i++;
    }

    return str_lower;
}

static bool is_digit_string(const char *str) {
    if (*str == '\0')
        return false;

    size_t i = 0;
    while (str[i]) {
        if (!isdigit(str[i]))
            return false;
        i++;
    }

    return true;
}

static ManagedEnvironment get_managed_env(void) {
    char *tmp;
    if ((tmp = getenv("GITHUB_ACTIONS")) && strcmp(tmp, "true") == 0)
        return GITHUB_ACTIONS;
    else if ((tmp = getenv("GITHUB_CI")) && strcmp(tmp, "true") == 0)
        return GITLAB_CI;
    else if ((tmp = getenv("REMOTE_CONTAINERS")) && strcmp(tmp, "true") == 0)
        return VSCODE_REMOTE_CONTAINERS;

    return NONE;
}

static void setup_initial_environ(Context *context) {
    context->environ = hash_map_init(8);
    Distro distro;
    distro_load(&distro);

    char *distro_name = to_lower_string(distro.id_like);

    if (strcmp(distro_name, "fedora") == 0 ||
        strcmp(distro_name, "suse") == 0 ||
        strcmp(distro_name, "opensuse") == 0) {
        hash_map_insert(context->environ, "LC_ALL", "en_US.utf8", NULL);
        hash_map_insert(context->environ, "LANG", "en_US.utf8", NULL);
        hash_map_insert(context->environ, "LANGUAGE", "en_US", NULL);
    }
    else if (strcmp(distro_name, "debian") == 0 ||
             strcmp(distro_name, "ubuntu") == 0 ||
             strcmp(distro_name, "gentoo") == 0) {
        hash_map_insert(context->environ, "LC_ALL", "en_US.UTF-8", NULL);
        hash_map_insert(context->environ, "LANG", "en_US.UTF-8", NULL);
        hash_map_insert(context->environ, "LANGUAGE", "en_US:en", NULL);
    }
    else {} //TODO

    free(distro_name);

    char *keys[7] = { "http_proxy", "https_proxy", "ftp_proxy", "no_proxy",
                  "SSH_AUTH_SOCK", "BB_NUMBER_THREADS", "PARALLEL_MAKE" };
    for (size_t i = 0; i < 7; i++) {
        char *val = getenv(keys[i]);
        if (val)
            hash_map_insert(context->environ, keys[i], val, NULL);
    }

    if (get_managed_env() == VSCODE_REMOTE_CONTAINERS) {
        char **tmp = environ;
        while (*tmp) {
            if (strcmp(*tmp, "REMOTE_CONTAINERS_") == 0)
                hash_map_insert(context->environ, *tmp, getenv(*tmp), NULL);
            tmp++;
        }
    }
}

static void check_container_call(void) {
    char *container_v = getenv("KAS_CONTAINER_SCRIPT_VERSION");
    if (!container_v)
        return;
    if (strcmp(container_v, VERSION_STRING) != 0) {} //TODO
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

    setup_initial_environ(context);
    check_container_call();
    context->keyhandler = hash_map_init(8);
    context->config = NULL;
    //self.args = args

    context->managed_paths = hash_set_init(8);

    if (getenv("KAS_BUILD_DIR") == NULL) {
        hash_set_insert(context->managed_paths, context->kas_build_dir);
    }

    return context;
}

void context_destroy(Context *context) {
    hash_map_destroy(context->environ);
    hash_set_destroy(context->managed_paths);
    free(context);
}
