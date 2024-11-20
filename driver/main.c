#include <stdbool.h>
#include <stdio.h>

#include "motor.h"
#include "pico/stdio.h"

#define GPIO_34EN 11
#define GPIO_3A 13
#define GPIO_4A 14

#define GPIO_12EN 17
#define GPIO_1A 18
#define GPIO_2A 20

int main() {
  stdio_init_all();

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

  MotorInit(&leftMotor);
  MotorInit(&rightMotor);

  while (true) {
    char user_input = getchar();
    if (user_input == '5') {
      SetMotor(&rightMotor, FORWARD, 0.5f);
      printf("Right Motor forward\n");
      continue;
    }
    if (user_input == '4') {
      SetMotor(&rightMotor, BACKWARD, 0.5f);
      printf("Right Motor backward\n");
      continue;
    }

    if (user_input == '3') {
      SetMotor(&rightMotor, STOP, 0.5f);
      printf("Right Motor off\n");
      continue;
    }

    if (user_input == '2') {
      SetMotor(&leftMotor, BACKWARD, 0.5f);
      printf("Left Motor backward\n");
      continue;
    }

    if (user_input == '1') {
      SetMotor(&leftMotor, FORWARD, 0.5f);
      printf("Left Motor Forward\n");
      continue;
    }

    if (user_input == '0') {
      SetMotor(&leftMotor, STOP, 0.5f);
      printf("Left Motor off\n");
      continue;
    }

    printf("Invalid input %c\n", user_input);
  }
}
