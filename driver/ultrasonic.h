#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "pico/types.h"

typedef struct {
  uint trigGPIO;
  uint echoGPIO;
} UltrasonicInfo;

void UltrasonicInit(UltrasonicInfo* info);

int GetCm(UltrasonicInfo* info);

#endif
