#include <stdbool.h>
#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/assert.h"
#include "pico/error.h"
#include "pico/stdio.h"
#include "pico/time.h"

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

int main() {
  stdio_init_all();

  gpio_init(GPIO_1A);
  gpio_init(GPIO_2A);
  gpio_set_dir(GPIO_1A, GPIO_OUT);
  gpio_set_dir(GPIO_2A, GPIO_OUT);

  gpio_init(GPIO_3A);
  gpio_init(GPIO_4A);
  gpio_set_dir(GPIO_3A, GPIO_OUT);
  gpio_set_dir(GPIO_4A, GPIO_OUT);

  setup_pwm(GPIO_12EN, 100.0f, 0.5f);
  setup_pwm(GPIO_34EN, 100.0f, 0.5f);

  while (true) {
    char user_input = getchar();
    if (user_input == '5') {
      gpio_put(GPIO_3A, false);
      gpio_put(GPIO_4A, true);
      printf("Motor 2 on regular direction\n");
      continue;
    }
    if (user_input == '4') {
      gpio_put(GPIO_3A, true);
      gpio_put(GPIO_4A, false);
      printf("Motor 2 on reverse direction\n");
      continue;
    }

    if (user_input == '3') {
      gpio_put(GPIO_3A, false);
      gpio_put(GPIO_4A, false);
      printf("Motor 2 off\n");
      continue;
    }

    if (user_input == '2') {
      gpio_put(GPIO_1A, false);
      gpio_put(GPIO_2A, true);
      printf("Motor 1 on reverse direction\n");
      continue;
    }

    if (user_input == '1') {
      gpio_put(GPIO_1A, true);
      gpio_put(GPIO_2A, false);
      printf("Motor 1 on regular direction\n");
      continue;
    }

    if (user_input == '0') {
      gpio_put(GPIO_1A, false);
      gpio_put(GPIO_2A, false);
      printf("Motor 1 off\n");
      continue;
    }

    printf("Invalid input %c\n", user_input);
  }
}
