#include "logger.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <colors.h>

static char *level_name[] = { "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL" };
static char *colors[] = { RESET, RESET, BOLD YELLOW, BOLD RED, BOLD RED };

static Logger logger;
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

static void logger_init(void) {
    memset(&logger, 0, sizeof(Logger));

    logger.level = DEFAULT_LOG_LEVEL;
    logger.color = color_supported();
}

Logger *logger_get(void) {
    if (!initialized) {
        logger_init();
        initialized = true;
    }
    return &logger;
}

static void print_log(LogLevel level, const char *fmt, va_list args) {
    Logger *logger = logger_get();
    if (level < logger->level) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    if (logger->color)
        fprintf(stderr, "%s", colors[level]);

    fprintf(stderr,
            "%04d-%02d-%02d %02d:%02d:%02d - %-8s - ",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec,
            level_name[level]);

    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    if (logger->color)
        fprintf(stderr, "%s", RESET);
}

void log_debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log(LOG_DEBUG, fmt, args);
    va_end(args);
}

void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log(LOG_INFO, fmt, args);
    va_end(args);
}

void log_warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log(LOG_WARNING, fmt, args);
    va_end(args);
}

void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log(LOG_ERROR, fmt, args);
    va_end(args);
}

void log_critical(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log(LOG_CRITICAL, fmt, args);
    va_end(args);
}

void logger_destroy(Logger *logger) {
    free(logger);
}
