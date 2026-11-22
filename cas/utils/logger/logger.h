#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_CRITICAL
} LogLevel;

#define DEFAULT_LOG_LEVEL LOG_INFO

typedef struct {
    const char *name;
    LogLevel level;
} Logger;

Logger *logger_init(const char *name);

void log_debug(Logger *logger, const char *message);

void log_info(Logger *logger, const char *message);

void log_warning(Logger *logger, const char *message);

void log_error(Logger *logger, const char *message);

void log_critical(Logger *logger, const char *message);

#endif /* ! LOGGER_H */
