set(target_name usb_comms)
add_library(${target_name})

target_sources(${target_name} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/device_callbacks.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack/hid_callbacks.c
    ${CMAKE_CURRENT_LIST_DIR}/module/Module.cpp
    ${CMAKE_CURRENT_LIST_DIR}/queue/PicoQueueReportQueue.cpp
    ${CMAKE_CURRENT_LIST_DIR}/queue/ReportQueueHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/queue/ReportQueueController.cpp
    ${CMAKE_CURRENT_LIST_DIR}/queue/ReportQueueSingleton.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/Payload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/DigitalPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/AnalogPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/JoystickPayload.cpp
    ${CMAKE_CURRENT_LIST_DIR}/payload/PayloadFactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/util/PicoTimer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/usbTask.cpp
)

target_include_directories(${target_name} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/module
    ${CMAKE_CURRENT_LIST_DIR}/payload
    ${CMAKE_CURRENT_LIST_DIR}/queue
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack
    ${CMAKE_CURRENT_LIST_DIR}/util
    ${CMAKE_CURRENT_LIST_DIR}/..
)

target_include_directories(${target_name} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../include
    ${CMAKE_CURRENT_LIST_DIR}/../include/types
    ${CMAKE_CURRENT_LIST_DIR}/../include/queue
    ${CMAKE_CURRENT_LIST_DIR}/../include/payload
)

target_link_libraries(${target_name} PUBLIC
    pico_stdlib
    pico_multicore
    tinyusb_board
    tinyusb_device
)
