add_executable(button_module_1)

pico_set_program_name(button_module_1 "button_module_1")
pico_set_program_version(button_module_1 "0.1")

pico_enable_stdio_uart(button_module_1 1)
pico_enable_stdio_usb(button_module_1 1)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

target_compile_definitions(button_module_1 PRIVATE 
        BUTTON
)

# Add the standard include files to the build
target_include_directories(button_module_1 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/include
  #${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

target_sources(button_module_1 PUBLIC
        src/button_module_1.cpp
        src/inputs.cpp
)

# Add the standard library to the build
target_link_libraries(button_module_1 PUBLIC
        pico_stdlib
        i2c_device
        hardware_i2c
        hardware_pwm
        pico_multicore
        pwm_pio
)

pico_add_extra_outputs(button_module_1)