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

typedef struct {
    const char *kas_work_dir;
    const char *kas_build_dir;
    const char *kas_repo_ref_dir;
    struct hash_map *environ;
    struct hash_set *managed_paths;
    struct hash_map *keyhandler;
    void *config;
    int repo_clone_depth;
} Context;

Context* context_init(void);

void context_destroy(Context *context);

#endif /* ! CONTEXT_H*/
