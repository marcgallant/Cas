#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t LogLevel;

enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
};

#define DEFAULT_LOG_LEVEL LOG_INFO

typedef struct {
    const char *name;
    LogLevel level;
    bool color;
} Logger; //TODO: add handler and formatter

Logger *logger_init(const char *name);

void log_debug(Logger *logger, const char *message);

void log_info(Logger *logger, const char *message);

void log_warning(Logger *logger, const char *message);

void log_error(Logger *logger, const char *message);

void log_critical(Logger *logger, const char *message);

#endif /* ! LOGGER_H */
