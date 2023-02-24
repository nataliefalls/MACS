add_executable(module)

target_sources(module PRIVATE
        module.cpp)

pico_set_program_name(module "module")
pico_set_program_version(module "0.1")

pico_enable_stdio_uart(module 1)
pico_enable_stdio_usb(module 1)

# Add the standard library to the build
target_link_libraries(module
        pico_stdlib
        i2c_device
        hardware_i2c
        hardware_adc
)

# Add the standard include files to the build
target_include_directories(module PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

pico_add_extra_outputs(module)