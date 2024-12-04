#include "utils.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LogLevel log_level = ERROR;

void SetLogLevel(LogLevel level) { log_level = level; }

void Logger(LogLevel level, const char* format, ...) {
  if (log_level < level) return;

  va_list args;
  va_start(args, format);

  char msg[strlen(format) + 10];
  memset(msg, '\0', strlen(format) + 10);
  if (level == ERROR) sprintf(msg, "ERROR | %s\n", format);
  if (level == INFO) sprintf(msg, "INFO  | %s\n", format);
  if (level == DEBUG) sprintf(msg, "DEBUG | %s\n", format);

  vprintf(msg, args);
  fflush(stdout);
  va_end(args);
}

void AssertErr(int status) {
  if (status < 0) {
    Logger(ERROR, strerror(errno));
    exit(1);
  }
}
