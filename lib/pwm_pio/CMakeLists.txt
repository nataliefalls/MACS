add_library(pwm_pio INTERFACE)

pico_generate_pio_header(pwm_pio ${CMAKE_CURRENT_LIST_DIR}/PwmIn.pio)

target_include_directories(pwm_pio
    INTERFACE
    ./include
)

target_sources(pwm_pio
    INTERFACE
    PwmIn.cpp
)

target_link_libraries(pwm_pio
    INTERFACE
    pico_stdlib
    hardware_pio
    hardware_irq
)