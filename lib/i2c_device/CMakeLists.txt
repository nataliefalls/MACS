add_library(i2c_device INTERFACE)

target_include_directories(i2c_device
    INTERFACE
    ./include)

target_sources(i2c_device
    INTERFACE
    i2c_device.cpp
    i2c_base.cpp
)

target_link_libraries(i2c_device
    INTERFACE
    hardware_i2c
    hardware_irq
)