#ifndef MOTOR_H
#define MOTOR_H

#include "pico/types.h"

typedef struct {
  uint pwmGPIO;
  uint frontGPIO;
  uint backGPIO;
} MotorInfo;

enum Direction {
  FORWARD,
  STOP,
  BACKWARD,
};

void MotorInit(MotorInfo* info);

void SetMotor(MotorInfo* info, enum Direction d, float speed);

void MotorDeinit(MotorInfo* info);

#endif
