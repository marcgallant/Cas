#ifndef CONTEXT_H
#define CONTEXT_H

#include <hash_set/hash_set.h>

typedef struct {
    const char *kas_work_dir;
    const char *kas_build_dir;
    const char *kas_repo_ref_dir;
    struct hash_set *managed_paths;
    int repo_clone_depth;
} Context;

Context* context_init(void);

void context_destroy(Context *context);

#endif /* ! CONTEXT_H*/
