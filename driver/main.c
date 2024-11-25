#include <hardware/gpio.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <tusb.h>

#include "motor.h"
#include "pico/stdio.h"
#include "ultrasonic.h"

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

int main() {
  stdio_init_all();
  while (!tud_cdc_connected()) {
    sleep_ms(100);
  }
  printf("USB Connected\n");

  /* MotorInit(&leftMotor); */
  /* MotorInit(&rightMotor); */

  /* char input[128] = {0}; */
  /* int idx = 0; */

  UltrasonicInfo sensorInfo = {
      .echoGPIO = GPIO_ECHO,
      .trigGPIO = GPIO_TRIG,
  };

  UltrasonicInit(&sensorInfo);
  while (true) {
    printf("\n %d cm", GetCm(&sensorInfo));
    sleep_ms(100);
    /* char chr = getchar(); */
    /* if (chr == 13) { */
    /*   printf("MSG: %s\n", input); */
    /*   memset(input, 0, sizeof(input)); */
    /*   idx = 0; */
    /*   continue; */
    /* } */
    /**/
    /* if (idx >= 128) { */
    /*   printf("ERROR: input too large, press Enter now\n"); */
    /*   continue; */
    /* } */
    /**/
    /* input[idx] = chr; */
    /* idx++; */
  }
}

void runMotor(char user_input, MotorInfo* leftMotor, MotorInfo* rightMotor) {
  if (user_input == '5') {
    SetMotor(rightMotor, FORWARD, 0.5f);
    printf("Right Motor forward\n");
  }
  if (user_input == '4') {
    SetMotor(rightMotor, BACKWARD, 0.5f);
    printf("Right Motor backward\n");
  }

  if (user_input == '3') {
    SetMotor(rightMotor, STOP, 0.5f);
    printf("Right Motor off\n");
  }

  if (user_input == '2') {
    SetMotor(leftMotor, BACKWARD, 0.5f);
    printf("Left Motor backward\n");
  }

  if (user_input == '1') {
    SetMotor(leftMotor, FORWARD, 0.5f);
    printf("Left Motor Forward\n");
  }

  if (user_input == '0') {
    SetMotor(leftMotor, STOP, 0.5f);
    printf("Left Motor off\n");
  }

  printf("Invalid input %c\n", user_input);
}
