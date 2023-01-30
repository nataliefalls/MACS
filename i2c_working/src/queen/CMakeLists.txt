add_executable(i2c_queen)

target_sources(i2c_queen PRIVATE
        i2cqueen.cpp)

pico_set_program_name(i2c_queen "i2c_queen")
pico_set_program_version(i2c_queen "0.1")

pico_enable_stdio_uart(i2c_queen 0)
pico_enable_stdio_usb(i2c_queen 1)

# Add the standard library to the build
target_link_libraries(i2c_queen
        pico_stdlib
        i2c_device
        hardware_i2c
        hardware_pwm
)

# Add the standard include files to the build
target_include_directories(i2c_queen PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

pico_add_extra_outputs(i2c_queen)