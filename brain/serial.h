#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>

void ConfigSerial();

void WriteMsg(const char msg[]);

ssize_t ReadChar(char* c);

void CloseSerial();

#endif
