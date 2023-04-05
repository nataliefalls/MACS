set(target_name usb_comms)
add_library(${target_name})

pico_enable_stdio_usb(${target_name} 0)

target_sources(${target_name} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/device_callbacks.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/hid_callbacks.c
    ${CMAKE_CURRENT_LIST_DIR}/module/Module.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/BufferedReportQueue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/MulticoreFifoReportQueue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/PicoQueueReportQueue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/handler/ReportQueueHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/controller/ReportQueueController.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/DigitalPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/AnalogPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/JoystickPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/demo/demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/PicoTimer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/usbTask.cpp
)

target_include_directories(${target_name} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/demo/
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