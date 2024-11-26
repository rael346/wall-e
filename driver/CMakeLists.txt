cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(driver C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()
add_executable(${CMAKE_PROJECT_NAME}
  main.c
  motor.c
  ultrasonic.c
  utils.c
)

target_link_libraries(${CMAKE_PROJECT_NAME}
  pico_stdlib
  pico_multicore
  hardware_gpio
  hardware_pwm
  hardware_timer
)

pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
