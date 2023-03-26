set(target_name usb_comms_alt)
add_executable(${target_name})
# add_library(${target_name})

# pico_enable_stdio_usb(${target_name} 1)

# for picoprobe stuff
pico_enable_stdio_usb(usb_comms_alt 0)
pico_enable_stdio_uart(usb_comms_alt 1)

target_sources(${target_name} PRIVATE
    usb_stack/usb_descriptors.c
    usb_stack/device_callbacks.c
    usb_stack/hid_callbacks.c
    demo/demo.cpp
    report_queue/BufferedReportQueue.cpp
    report_queue/MulticoreFifoReportQueue.cpp
    report_queue/PicoQueueReportQueue.cpp
    report_queue/handler/ReportQueueHandler.cpp
    report_queue/controller/ReportQueueController.cpp
    # module/Module.cpp
    payload/ButtonPayload.cpp
    payload/DpadPayload.cpp
    payload/JoystickPayload.cpp
    util/PicoTimer.cpp
    usbTask.cpp
)

target_include_directories(${target_name} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/demo
    ${CMAKE_CURRENT_LIST_DIR}/module
    ${CMAKE_CURRENT_LIST_DIR}/payload
    ${CMAKE_CURRENT_LIST_DIR}/report_queue
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/controller
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/handler
    ${CMAKE_CURRENT_LIST_DIR}/types
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack
    ${CMAKE_CURRENT_LIST_DIR}/util
    ${CMAKE_CURRENT_LIST_DIR}/..
)

target_link_libraries(${target_name} PUBLIC
    pico_stdlib
    pico_multicore
    tinyusb_board
    tinyusb_device
    hardware_adc # only for demo
)

pico_add_extra_outputs(${target_name})