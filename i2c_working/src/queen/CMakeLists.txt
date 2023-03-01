add_executable(i2c_queen)

pico_set_program_name(i2c_queen "i2c_queen")
pico_set_program_version(i2c_queen "0.1")

pico_enable_stdio_uart(i2c_queen 0)
pico_enable_stdio_usb(i2c_queen 1)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Add the standard include files to the build
target_include_directories(i2c_queen INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

target_sources(i2c_queen PUBLIC
        i2cqueen.cpp)

# Add the standard library to the build
target_link_libraries(i2c_queen PUBLIC
        pico_stdlib
        i2c_device
        hardware_i2c
        hardware_pwm
        pico_multicore
        pico_pio_usb
)

pico_add_extra_outputs(i2c_queen)