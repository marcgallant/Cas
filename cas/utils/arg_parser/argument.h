#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <stdbool.h>

typedef enum {
    ARG_POSITIONAL,
    ARG_OPTIONAL,
    ARG_SUBPARSER
} ArgType;

typedef enum {
    ARG_STRING,
    ARG_INT,
    ARG_FLOAT,
    ARG_BOOL
} ArgValueType;

typedef struct {
    const char *name;
} Positional;

typedef struct {
    const char **names; //contains short opt and long opt
} Optional;

typedef struct {
    struct dlist    *parsers;
} Subparser;

typedef struct {
    ArgType         type;
    ArgValueType    value_type;
    
    const char      *dest;
    const char      *help;

    const char      *metavar;

    int             nargs;
    bool            required;

    union {
        const char *string;
        int integer;
        float floating;
        bool boolean;
    } default_value;

    const char      **choices;

    union {
        Positional positional;
        Optional optional;
        Subparser subparser;
    } data;
} Argument;

Argument *help_init(void);
Argument *skip_init(void);
Argument *force_checkout_init(void);
Argument *update_init(void);
Argument *dry_run_init(void);
Argument *isar_init(void);
Argument *keep_config_unchanged_init(void);
Argument *indent_init(void);
Argument *sort_init(void);
Argument *preserve_env_init(void);
Argument *config_init(void);

#endif /* ! ARGUMENT_H */
