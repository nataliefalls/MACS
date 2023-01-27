# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)
# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
project(i2c_potentiometer C CXX ASM)
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/usr/share/pico_sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

set(PICO_PIO_USB_DIR "${CMAKE_CURRENT_LIST_DIR}/external/pico_pio_usb/")

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Add executable. Default name is the project name, version 0.1

add_subdirectory(queen)
add_subdirectory(worker)
include_directories(./)