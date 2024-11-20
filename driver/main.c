#include <stdbool.h>
#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdio.h"

#define GPIO_34EN 11
#define GPIO_3A 13
#define GPIO_4A 14

#define GPIO_12EN 17
#define GPIO_1A 18
#define GPIO_2A 20

void setPWM(uint gpio, float freq, float duty_cycle) {
  // Find out which PWM slice is connected to the specified GPIO
  uint slice_num = pwm_gpio_to_slice_num(gpio);

  // Calculate the PWM frequency and set the PWM wrap value
  float clock_freq = 125000000.0f;  // Default Pico clock frequency in Hz
  uint32_t divider =
      clock_freq / (freq * 65536);  // Compute divider for given frequency
  pwm_set_clkdiv(slice_num, divider);

  // Set the PWM wrap value (maximum count value)
  pwm_set_wrap(slice_num, 65535);  // 16-bit counter (0 - 65535)

  // Set the duty cycle
  pwm_set_gpio_level(gpio, (uint16_t)(duty_cycle * 65535));
}

typedef struct {
  uint pwmGPIO;
  uint frontGPIO;
  uint backGPIO;
} MotorInfo;

void MotorInit(MotorInfo* info) {
  gpio_set_function(info->pwmGPIO, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(info->pwmGPIO);
  pwm_set_enabled(slice_num, true);

  gpio_init(info->frontGPIO);
  gpio_set_dir(info->frontGPIO, GPIO_OUT);

  gpio_init(info->backGPIO);
  gpio_set_dir(info->backGPIO, GPIO_OUT);
}

enum Direction {
  FORWARD,
  STOP,
  BACKWARD,
};

void SetMotor(MotorInfo* info, enum Direction d) {
  switch (d) {
    case FORWARD:
      gpio_put(info->frontGPIO, true);
      gpio_put(info->backGPIO, false);
      printf("foward set front gpio %u back gpio %u", info->frontGPIO,
             info->backGPIO);
      break;
    case BACKWARD:
      gpio_put(info->frontGPIO, false);
      gpio_put(info->backGPIO, true);
      printf("backward set front gpio %u back gpio %u", info->frontGPIO,
             info->backGPIO);
      break;
    case STOP:
      gpio_put(info->frontGPIO, false);
      gpio_put(info->backGPIO, false);
      printf("stop set front gpio %u back gpio %u", info->frontGPIO,
             info->backGPIO);
      break;
  }
}

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
      SetMotor(&rightMotor, FORWARD);
      setPWM(rightMotor.pwmGPIO, 100.0f, 1.0f);
      printf("Right Motor forward\n");
      continue;
    }
    if (user_input == '4') {
      SetMotor(&rightMotor, BACKWARD);
      setPWM(rightMotor.pwmGPIO, 100.0f, 0.5f);
      printf("Right Motor backward\n");
      continue;
    }

    if (user_input == '3') {
      SetMotor(&rightMotor, STOP);
      setPWM(rightMotor.pwmGPIO, 100.0f, 0.5f);
      printf("Right Motor off\n");
      continue;
    }

    if (user_input == '2') {
      SetMotor(&leftMotor, BACKWARD);
      setPWM(leftMotor.pwmGPIO, 100.0f, 0.5f);
      printf("Left Motor backward\n");
      continue;
    }

    if (user_input == '1') {
      SetMotor(&leftMotor, FORWARD);
      setPWM(leftMotor.pwmGPIO, 100.0f, 1.0f);
      printf("Left Motor Forward\n");
      continue;
    }

    if (user_input == '0') {
      SetMotor(&leftMotor, STOP);
      setPWM(leftMotor.pwmGPIO, 100.0f, 0.5f);
      printf("Left Motor off\n");
      continue;
    }

    printf("Invalid input %c\n", user_input);
  }
}
