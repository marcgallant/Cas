#ifndef PLUGINS_H
#define PLUGINS_H

#include <arg_parser/parser.h>

#include <stdlib.h>

Parser *build_parser_init(void);

Parser *checkout_parser_init(void);

Parser *clean_parser_init(void);
Parser *cleansstate_parser_init(void);
Parser *cleanall_parser_init(void);
Parser *purge_parser_init(void);

Parser *diff_parser_init(void);

Parser *dump_parser_init(void);

Parser *for_all_repos_parser_init(void);

Parser *lock_parser_init(void);

Parser *menu_parser_init(void);

Parser *shell_parser_init(void);

#endif /* ! PLUGIN_H */
