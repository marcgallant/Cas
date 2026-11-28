#include "commands.h"

#define _GNU_SOURCE
#define TMP_TEMPLATE "XXXXXX"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../context.h"

#include <cwalk/cwalk.h>
#include <dlist/dlist.h>
#include <logger/logger.h>

#include <uv.h>

static struct dlist *get_default_tempdir_list(void) {
    struct dlist *dirlist = dlist_init();

    const char *envnames[] = {
        "TMPDIR",
        "TEMP",
        "TMP",
        NULL
    };

    for (size_t i = 0; envnames[i] != NULL; i++) {
        const char *dirname = getenv(envnames[i]);

        if (dirname && dirname[0] != '\0')
            dlist_push_back(dirlist, strdup(dirname));
    }

    dlist_push_back(dirlist, strdup("/tmp"));
    dlist_push_back(dirlist, strdup("/var/tmp"));

    return dirlist;
}

char *tempdir_create(void) {
    struct dlist *dirlist = get_default_tempdir_list();

    if (!dirlist)
        return NULL;

    struct dlist_item *tmp = dirlist->head;

    char *path = NULL;
    while (tmp) {
        size_t required = cwk_path_join(
            tmp->data,
            TMP_TEMPLATE,
            NULL,
            0
        );

        char *path = malloc(required);

        cwk_path_join(
            tmp->data,
            TMP_TEMPLATE,
            path,
            required + 1
        );

        uv_fs_t req;

        int err = uv_fs_mkdtemp(
            NULL,
            &req,
            path,
            NULL
        );

        uv_fs_req_cleanup(&req);

        if (err == 0)
            break;

        free(path);
        tmp = tmp->next;
    }

    dlist_destroy(dirlist);
    return path;
}

static char *path_from_env(const char *name) {
    char *path = getenv(name);

    if (!path || !path[0])
        return NULL;

    struct stat st;

    if (stat(path, &st) != 0)
        return NULL;

    if (!S_ISREG(st.st_mode))
        return NULL;

    return path;
}

void setup_netrc(Command *command) {
    size_t required = cwk_path_join(
        command->tmpdirname,
        ".netrc",
        NULL,
        0
    );

    char *path = malloc(required);

    cwk_path_join(
        command->tmpdirname,
        ".netrc",
        path,
        required + 1
    );

    uv_fs_t req;
    char *netrc_file = path_from_env("NETRC_FILE");
    if (netrc_file) {
        uv_fs_copyfile(
            NULL,
            &req,
            netrc_file,
            path,
            UV_FS_COPYFILE_EXCL,
            NULL
        );

        uv_fs_req_cleanup(&req);
    }

    char *ci_server_host = getenv("CI_SERVER_HOST");
    char *ci_job_token = getenv("CI_JOB_TOKEN");

    if (ci_server_host && ci_job_token) {
        uv_file file = uv_fs_open(
            NULL,
            &req,
            path,
            O_WRONLY | O_APPEND | O_CREAT,
            0600,
            NULL
        );

        uv_fs_req_cleanup(&req);

        if (file < 0)
            return;

        char buffer[4096];
        int length = snprintf(
            buffer,
            sizeof(buffer),
            "machine %s\n"
            "login gitlab-ci-token\n"
            "password %s\n",
            ci_server_host,
            ci_job_token
        );

        if (length > 0 && (size_t)length < sizeof(buffer)) {
            uv_buf_t buf = uv_buf_init(buffer, length);

            uv_fs_write(
                NULL,
                &req,
                file,
                &buf,
                1,
                -1,
                NULL
            );

            uv_fs_req_cleanup(&req);
        }
        uv_fs_close(NULL, &req, file, NULL);
        uv_fs_req_cleanup(&req);
    }
}

void setup_npmrc(Command *command) {
    char *npmrc_file = path_from_env("NPMRC_FILE");
    if (!npmrc_file)
        return;

    char path[4096];
    cwk_path_join(
        command->tmpdirname,
        ".npmrc",
        path,
        sizeof(path)
    );

    uv_fs_t req;
    uv_fs_copyfile(
        NULL,
        &req,
        npmrc_file,
        path,
        0,
        NULL
    );

    uv_fs_req_cleanup(&req);
}

void setup_registry_auth(Command *command) {
    (void)command;
}

void setup_gitconfig(Command *command) {
    (void)command;
}

void setup_aws_creds(Command *command) {
    (void)command;
}

void exec(Command *command) {
    ManagedEnvironment managed_env = context_get()->managed_env;
    if (managed_env != NONE)
        log_info("Running on %s", managed_env_to_str(managed_env));

    if (!command->tmpdirname)
        command->tmpdirname = tempdir_create();

    setup_netrc(command);
    setup_npmrc(command);
    setup_registry_auth(command);
    setup_gitconfig(command);
    setup_aws_creds(command);

    hash_map_insert(context_get()->environ, "HOME", command->tmpdirname, NULL);
}
