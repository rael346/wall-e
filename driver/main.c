#include <hardware/gpio.h>
#include <pico/error.h>
#include <pico/stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <tusb.h>

#include "motor.h"
#include "ultrasonic.h"
#include "utils.h"

#define MSG_BUF_SIZE 128

#define GPIO_34EN 11
#define GPIO_3A 13
#define GPIO_4A 14

#define GPIO_12EN 17
#define GPIO_1A 18
#define GPIO_2A 20

#define GPIO_ECHO 3
#define GPIO_TRIG 5

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

void handleMsg(char msg[MSG_BUF_SIZE + 1], MotorInfo* leftMotor,
               MotorInfo* rightMotor) {
  char buf[MSG_BUF_SIZE + 1] = {0};
  strncpy(buf, msg, strlen(msg));

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

  SetLogLevel(DEBUG);
  Logger(DEBUG, "Connected");

  MotorInit(&leftMotor);
  MotorInit(&rightMotor);
  UltrasonicInit(&sensorInfo);

  char msgBuf[MSG_BUF_SIZE + 1] = {0};
  int idx = 0;
  while (true) {
    Logger(INFO, "%d", GetCm(&sensorInfo));

    int chr = getchar_timeout_us(100000);
    if (chr == PICO_ERROR_TIMEOUT) {
      continue;
    }

    if (chr == '\n') {
      Logger(DEBUG, "Received msg %s", msgBuf);
      handleMsg(msgBuf, &leftMotor, &rightMotor);
      memset(msgBuf, '\0', MSG_BUF_SIZE);
      idx = 0;
      continue;
    }

    if (idx >= MSG_BUF_SIZE) {
      Logger(DEBUG, "Received msg %s", msgBuf);
      Logger(ERROR, "input too large, missing newline character");
      continue;
    }

    msgBuf[idx++] = chr;
  }
}
