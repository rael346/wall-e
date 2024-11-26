#ifndef UTILS_H
#define UTILS_H

typedef enum {
  ERROR,
  INFO,
  DEBUG,
} LogLevel;

void SetLogLevel(LogLevel level);

void Logger(LogLevel level, const char* format, ...);

#endif
