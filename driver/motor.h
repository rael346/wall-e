#ifndef MOTOR_H
#define MOTOR_H

#include "pico/types.h"

typedef struct {
  uint pwmGPIO;
  uint frontGPIO;
  uint backGPIO;
} MotorInfo;

enum Direction {
  STOP = 0,
  FORWARD = 1,
  BACKWARD = 2,
};

void MotorInit(MotorInfo* info);

void SetMotor(MotorInfo* info, enum Direction d, float speed);

void MotorDeinit(MotorInfo* info);

#endif
