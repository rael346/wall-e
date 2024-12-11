#include "ultrasonic.h"

#include <pico/time.h>

#include "hardware/gpio.h"
#include "hardware/timer.h"

void UltrasonicInit(UltrasonicInfo* info) {
  gpio_init(info->trigGPIO);
  gpio_set_dir(info->trigGPIO, GPIO_OUT);

  gpio_init(info->echoGPIO);
  gpio_set_dir(info->echoGPIO, GPIO_IN);
}

void UltrasonicDeinit(UltrasonicInfo* info) {
  gpio_deinit(info->trigGPIO);
  gpio_deinit(info->echoGPIO);
}

int timeout = 26100;

// taken from https://github.com/Thomas-Kleist/Pico-Ultrasonic
uint64_t getPulse(uint trigPin, uint echoPin) {
  gpio_put(trigPin, 1);
  sleep_us(10);
  gpio_put(trigPin, 0);

  uint64_t width = 0;

  while (gpio_get(echoPin) == 0) tight_loop_contents();
  absolute_time_t startTime = get_absolute_time();
  while (gpio_get(echoPin) == 1) {
    width++;
    sleep_us(1);
    if (width > timeout) return 0;
  }
  absolute_time_t endTime = get_absolute_time();

  return absolute_time_diff_us(startTime, endTime);
}

int GetCm(UltrasonicInfo* info) {
  uint64_t pulseLength = getPulse(info->trigGPIO, info->echoGPIO);
  return pulseLength / 29 / 2;
}
