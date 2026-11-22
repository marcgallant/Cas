#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <colors.h>

static char *level_name[] = { "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL" };
static char *colors[] = { RESET, RESET, BOLD YELLOW, BOLD RED, BOLD RED };

static bool color_supported(void)
{
    char *force_color = getenv("FORCE_COLOR");
    if (force_color != NULL && force_color[0] != '\0')
        return true;

    char *no_color = getenv("NO_COLOR");
    if (no_color != NULL && no_color[0] != '\0')
        return false;

    if (!isatty(STDERR_FILENO))
        return false;

    return true;
}

Logger *logger_init(const char *name) {
    Logger *logger = calloc(1, sizeof(Logger));
    if (!logger) return NULL;

    logger->name = name ? name : "root" ;
    logger->level = DEFAULT_LOG_LEVEL;
    logger->color = color_supported();

    return logger;
}

static void print_log(Logger *logger, LogLevel level, const char *message) {
    if (level < logger->level) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (logger->color)
        printf("%s", colors[level]);

    printf("%04d-%02d-%02d %02d:%02d:%02d - %-8s - %s\n",
           t->tm_year + 1900,
           t->tm_mon + 1,
           t->tm_mday,
           t->tm_hour,
           t->tm_min,
           t->tm_sec,
           level_name[level],
           message);
}

void log_debug(Logger *logger, const char *message) {
    print_log(logger, LOG_DEBUG, message);
}

void log_info(Logger *logger, const char *message) {
    print_log(logger, LOG_INFO, message);
}

void log_warning(Logger *logger, const char *message) {
    print_log(logger, LOG_WARNING, message);
}

void log_error(Logger *logger, const char *message) {
    print_log(logger, LOG_ERROR, message);
}

void log_critical(Logger *logger, const char *message) {
    print_log(logger, LOG_CRITICAL, message);
}
