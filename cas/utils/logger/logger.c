#include "logger.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

Logger *logger_init(const char *name) {
    Logger *logger = calloc(1, sizeof(Logger));
    if (!logger) return NULL;

    logger->name = name ? name : "root" ;
    logger->level = DEFAULT_LOG_LEVEL;

    return logger;
}

static char *level_name[] = { "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL" };

static void print_log(Logger *logger, LogLevel level, const char *message) {
    if (level < logger->level) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

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
