add_subdirectory(lib)

add_executable(central_hub)

pico_set_program_name(central_hub "central_hub")
pico_set_program_version(central_hub "0.1")

pico_enable_stdio_uart(central_hub 1)
pico_enable_stdio_usb(central_hub 1)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Add the standard include files to the build
target_include_directories(central_hub INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}
  #${CMAKE_CURRENT_LIST_DIR}/include
  #${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

target_sources(central_hub PUBLIC
        src/central_hub.cpp)

# Add the standard library to the build
target_link_libraries(central_hub PUBLIC
        pico_stdlib
        i2c_device
        hardware_i2c
        hardware_pwm
        pico_multicore
        pwm_pio
        #usb
)

pico_add_extra_outputs(central_hub)