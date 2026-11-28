#ifndef CONTEXT_H
#define CONTEXT_H

#include <hash_map/hash_map.h>
#include <hash_set/hash_set.h>

typedef enum {
    GITHUB_ACTIONS,
    GITLAB_CI,
    VSCODE_REMOTE_CONTAINERS,
    NONE
} ManagedEnvironment;

const char *managed_env_to_str(ManagedEnvironment);

typedef struct {
    const char *kas_work_dir;
    const char *kas_build_dir;
    const char *kas_repo_ref_dir;
    struct hash_map *environ;
    struct hash_set *managed_paths;
    struct hash_map *keyhandler;
    void *config;
    int repo_clone_depth;
    ManagedEnvironment managed_env;
} Context;

Context *context_get(void);
void context_destroy();

#endif /* ! CONTEXT_H*/
