#include "logger.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <colors.h>

static char *level_name[] = { "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL" };
static char *colors[] = { RESET, RESET, BOLD YELLOW, BOLD RED, BOLD RED };

static Logger *logger;
static bool initialized = false;

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

static Logger *logger_init() {
    Logger *logger = calloc(1, sizeof(Logger));
    if (!logger) return NULL;

    logger->level = DEFAULT_LOG_LEVEL;
    logger->color = color_supported();

    return logger;
}

Logger *get_logger(void) {
    if (!initialized) {
        logger = logger_init(NULL);
        initialized = true;
    }
    return logger;
}

static void print_log(LogLevel level, const char *message) {
    Logger *logger = get_logger();
    if (level < logger->level) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (logger->color)
        fprintf(stderr, "%s", colors[level]);

    fprintf(stderr,
           "%04d-%02d-%02d %02d:%02d:%02d - %-8s - %s\n",
           t->tm_year + 1900,
           t->tm_mon + 1,
           t->tm_mday,
           t->tm_hour,
           t->tm_min,
           t->tm_sec,
           level_name[level],
           message);
}

void log_debug(const char *message) {
    print_log(LOG_DEBUG, message);
}

void log_info(const char *message) {
    print_log(LOG_INFO, message);
}

void log_warning(const char *message) {
    print_log(LOG_WARNING, message);
}

void log_error(const char *message) {
    print_log(LOG_ERROR, message);
}

void log_critical(const char *message) {
    print_log(LOG_CRITICAL, message);
}

void logger_destroy(Logger *logger) {
    free(logger);
}
