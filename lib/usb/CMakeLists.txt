# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

set(lib_name usb_comms)
add_library(${lib_name})

target_link_libraries(${lib_name} PUBLIC
    pico_stdlib
    pico_multicore
    tinyusb_board
    tinyusb_device
)

add_subdirectory(src)
add_subdirectory(include)