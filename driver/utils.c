#include "utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

LogLevel log_level = ERROR;

void SetLogLevel(LogLevel level) { log_level = level; }

void Logger(LogLevel level, const char* format, ...) {
  if (log_level < level) return;

  va_list args;
  va_start(args, format);

  char msg[strlen(format) + 10];
  memset(msg, '\0', strlen(format) + 10);
  if (level == ERROR) sprintf(msg, "e %s\n", format);
  if (level == INFO) sprintf(msg, "i %s\n", format);
  if (level == DEBUG) sprintf(msg, "d %s\n", format);

  vprintf(msg, args);
  fflush(stdout);
  va_end(args);
}
