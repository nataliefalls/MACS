# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)
# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
project(usb_comms_alt C CXX ASM)
# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH ${PICO_SDK_PATH})

set(PICO_BOARD pico CACHE STRING "Board type")

set(TINYUSB_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/tinyusb/")

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

#include_directories(include)

add_subdirectory(lib)
add_subdirectory(src)