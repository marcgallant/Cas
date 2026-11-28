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
    LogLevel level;
    bool color;
} Logger; //TODO: add handler and formatter

Logger *logger_get();

void log_debug(const char *fmt, ...);

void log_info(const char *fmt, ...);

void log_warning(const char *fmt, ...);

void log_error(const char *fmt, ...);

void log_critical(const char *fmt, ...);

void logger_destroy(Logger *logger);

#endif /* ! LOGGER_H */
