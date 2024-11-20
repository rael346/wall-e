#include "motor.h"

#include "hardware/gpio.h"
#include "hardware/pwm.h"

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

void MotorInit(MotorInfo* info) {
  gpio_set_function(info->pwmGPIO, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(info->pwmGPIO);
  pwm_set_enabled(slice_num, true);

  gpio_init(info->frontGPIO);
  gpio_set_dir(info->frontGPIO, GPIO_OUT);

  gpio_init(info->backGPIO);
  gpio_set_dir(info->backGPIO, GPIO_OUT);
}

void SetMotor(MotorInfo* info, enum Direction d, float speed) {
  switch (d) {
    case FORWARD:
      gpio_put(info->frontGPIO, true);
      gpio_put(info->backGPIO, false);
      break;
    case BACKWARD:
      gpio_put(info->frontGPIO, false);
      gpio_put(info->backGPIO, true);
      break;
    case STOP:
      gpio_put(info->frontGPIO, false);
      gpio_put(info->backGPIO, false);
      break;
  }

  setPWM(info->pwmGPIO, 100.0f, speed);
}

void MotorDeinit(MotorInfo* info) {
  gpio_deinit(info->pwmGPIO);
  gpio_deinit(info->frontGPIO);
  gpio_deinit(info->backGPIO);
}
