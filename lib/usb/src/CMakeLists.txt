set(target_name usb_comms)
add_library(${target_name} INTERFACE)

target_sources(${target_name} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/device_callbacks.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/hid_callbacks.c
    ${CMAKE_CURRENT_LIST_DIR}/module/Module.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/PicoQueueReportQueue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/ReportQueueHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/ReportQueueController.cpp
    ${CMAKE_CURRENT_LIST_DIR}/report_queue/ReportQueueFactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/DigitalPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/AnalogPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/JoystickPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/PayloadFactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/PicoTimer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/usbTask.cpp
)

target_include_directories(${target_name} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/module
    ${CMAKE_CURRENT_LIST_DIR}/payload
    ${CMAKE_CURRENT_LIST_DIR}/report_queue
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack
    ${CMAKE_CURRENT_LIST_DIR}/util
    ${CMAKE_CURRENT_LIST_DIR}/..
    ${CMAKE_CURRENT_LIST_DIR}/../include
    ${CMAKE_CURRENT_LIST_DIR}/../include/types
    ${CMAKE_CURRENT_LIST_DIR}/../include/queue
    ${CMAKE_CURRENT_LIST_DIR}/../include/payload
)

target_link_libraries(${target_name} INTERFACE
    pico_stdlib
    pico_multicore
    tinyusb_board
    tinyusb_device
)
