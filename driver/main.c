#include <hardware/gpio.h>
#include <pico/multicore.h>
#include <pico/stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tusb.h>

#include "motor.h"
#include "ultrasonic.h"
#include "utils.h"

#define GPIO_34EN 11
#define GPIO_3A 13
#define GPIO_4A 14

#define GPIO_12EN 17
#define GPIO_1A 18
#define GPIO_2A 20

#define GPIO_ECHO 3
#define GPIO_TRIG 5
#define INPUT_BUF_SIZE 128

MotorInfo leftMotor = {
    .pwmGPIO = GPIO_12EN,
    .frontGPIO = GPIO_1A,
    .backGPIO = GPIO_2A,
};

MotorInfo rightMotor = {
    .pwmGPIO = GPIO_34EN,
    .frontGPIO = GPIO_4A,
    .backGPIO = GPIO_3A,
};

UltrasonicInfo sensorInfo = {
    .echoGPIO = GPIO_ECHO,
    .trigGPIO = GPIO_TRIG,
};

void core1_entry() {
  while (true) {
    Logger(INFO, "%d", GetCm(&sensorInfo));
    sleep_ms(100);
  }
}

void handleMsg(char msg[INPUT_BUF_SIZE], MotorInfo* leftMotor,
               MotorInfo* rightMotor) {
  char buf[INPUT_BUF_SIZE] = {0};
  strncpy(buf, msg, strlen(msg));

  Logger(DEBUG, "buffer %s", buf);

  char* curr = strtok(buf, " ");
  if (curr == NULL) {
    Logger(ERROR, "Missing left Motor direction");
    return;
  }
  enum Direction leftMotorDirection = atoi(curr);
  Logger(DEBUG, "Left Motor Direction %d", leftMotorDirection);

  curr = strtok(NULL, " ");
  if (curr == NULL) {
    Logger(ERROR, "Missing Left Motor Speed");
    return;
  }
  float leftMotorSpeed = atof(curr);
  Logger(DEBUG, "Left Motor Speed %.2f", leftMotorSpeed);

  curr = strtok(NULL, " ");
  if (curr == NULL) {
    Logger(ERROR, "Missing Right Motor Direction");
    return;
  }
  enum Direction rightMotorDirection = atoi(curr);
  Logger(DEBUG, "Right Motor Direction %d", rightMotorDirection);

  curr = strtok(NULL, " ");
  if (curr == NULL) {
    Logger(ERROR, "Missing Right Motor Speed");
    return;
  }
  float rightMotorSpeed = atof(curr);
  Logger(DEBUG, "Right Motor Speed %.2f", rightMotorSpeed);

  SetMotor(leftMotor, leftMotorDirection, leftMotorSpeed);
  SetMotor(rightMotor, rightMotorDirection, rightMotorSpeed);
}

int main() {
  stdio_init_all();
  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }

  multicore_launch_core1(core1_entry);

  MotorInit(&leftMotor);
  MotorInit(&rightMotor);
  UltrasonicInit(&sensorInfo);
  SetLogLevel(DEBUG);

  char input[INPUT_BUF_SIZE] = {0};
  int idx = 0;
  while (true) {
    char chr = getchar();
    if (chr == 13) {
      Logger(DEBUG, "input %s", input);
      handleMsg(input, &leftMotor, &rightMotor);
      memset(input, 0, sizeof(input));
      idx = 0;
      continue;
    }

    if (idx >= INPUT_BUF_SIZE - 1) {
      Logger(ERROR, "input too large, end message now");
      continue;
    }

    input[idx] = chr;
    idx++;
  }
}
