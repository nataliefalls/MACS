set(target_name usb_comms_alt)
add_executable(${target_name})

pico_enable_stdio_usb(${target_name} 1)

target_sources(${target_name} PRIVATE
    main.c
    usb_descriptors.c
    report_queue_handler.c
    usb_controller.c
    util/report_queue.c
)

target_include_directories(${target_name} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/util
    ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(${target_name} PUBLIC
    pico_stdlib
    tinyusb_board
    tinyusb_device
    hardware_adc
)

pico_add_extra_outputs(${target_name})