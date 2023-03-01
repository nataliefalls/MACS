add_library(usb INTERFACE)

target_include_directories(usb
    INTERFACE
    ./include
)

target_sources(usb
    INTERFACE
    usb.c
)