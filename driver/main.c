#include <stdbool.h>
#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/stdio.h"

#define LED_DELAY_MS 100

#define GPIO_34EN 11
#define GPIO_3A 13
#define GPIO_4A 14

#define GPIO_12EN 17
#define GPIO_1A 18
#define GPIO_2A 20

// Function to set up the PWM
void setup_pwm(uint gpio, float freq, float duty_cycle) {
  // Set the GPIO function to PWM
  gpio_set_function(gpio, GPIO_FUNC_PWM);

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
  pwm_set_gpio_level(gpio, (uint16_t)(duty_cycle * 65536));

  // Enable the PWM
  pwm_set_enabled(slice_num, true);
}

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

  // Enable the PWM
  pwm_set_enabled(slice_num, true);
}

typedef struct {
  uint pwmGPIO;
  uint frontGPIO;
  uint backGPIO;
} MotorInfo;

void MotorInit(MotorInfo* info) {
  gpio_set_function(info->pwmGPIO, GPIO_FUNC_PWM);

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

  /* gpio_init(GPIO_1A); */
  /* gpio_init(GPIO_2A); */
  /* gpio_set_dir(GPIO_1A, GPIO_OUT); */
  /* gpio_set_dir(GPIO_2A, GPIO_OUT); */
  /**/
  /* gpio_init(GPIO_3A); */
  /* gpio_init(GPIO_4A); */
  /* gpio_set_dir(GPIO_3A, GPIO_OUT); */
  /* gpio_set_dir(GPIO_4A, GPIO_OUT); */
  /**/
  /* setup_pwm(GPIO_12EN, 100.0f, 0.5f); */
  /* setup_pwm(GPIO_34EN, 100.0f, 0.5f); */
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
  setPWM(leftMotor.pwmGPIO, 100.0f, 1.0f);
  MotorInit(&rightMotor);
  setPWM(rightMotor.pwmGPIO, 100.0f, 1.0f);

  while (true) {
    char user_input = getchar();
    if (user_input == '5') {
      /* gpio_put(GPIO_3A, false); */
      /* gpio_put(GPIO_4A, true); */
      SetMotor(&rightMotor, FORWARD);
      printf("Right Motor forward\n");
      continue;
    }
    if (user_input == '4') {
      /* gpio_put(GPIO_3A, true); */
      /* gpio_put(GPIO_4A, false); */
      SetMotor(&rightMotor, BACKWARD);
      printf("Right Motor backward\n");
      continue;
    }

    if (user_input == '3') {
      /* gpio_put(GPIO_3A, false); */
      /* gpio_put(GPIO_4A, false); */
      SetMotor(&rightMotor, STOP);
      printf("Right Motor off\n");
      continue;
    }

    if (user_input == '2') {
      /* gpio_put(GPIO_1A, false); */
      /* gpio_put(GPIO_2A, true); */
      SetMotor(&leftMotor, BACKWARD);
      printf("Left Motor backward\n");
      continue;
    }

    if (user_input == '1') {
      /* gpio_put(GPIO_1A, true); */
      /* gpio_put(GPIO_2A, false); */
      SetMotor(&leftMotor, FORWARD);
      printf("Left Motor Forward\n");
      continue;
    }

    if (user_input == '0') {
      /* gpio_put(GPIO_1A, false); */
      /* gpio_put(GPIO_2A, false); */
      SetMotor(&leftMotor, STOP);
      printf("Left Motor off\n");
      continue;
    }

    printf("Invalid input %c\n", user_input);
  }
}
