#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void logger_init(const char *filename);
void logger_close(void);
void logger_log(LogLevel level, const char *file, int line, const char *fmt, ...);

#define LOG_DEBUG(fmt, ...) logger_log(LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) logger_log(LOG_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) logger_log(LOG_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger_log(LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif
