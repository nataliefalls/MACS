set(target_name usb_comms_alt)
add_executable(${target_name})

pico_enable_stdio_usb(${target_name} 1)

target_sources(${target_name} PRIVATE
    main.c
)

target_include_directories(${target_name} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${TINYUSB_DIR}
)

target_link_libraries(${target_name} PUBLIC
    tinyusb
)

pico_add_extra_outputs(${target_name})