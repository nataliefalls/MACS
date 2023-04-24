target_sources(${lib_name} PRIVATE
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

target_include_directories(${lib_name} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/module
    ${CMAKE_CURRENT_LIST_DIR}/payload
    ${CMAKE_CURRENT_LIST_DIR}/queue
    ${CMAKE_CURRENT_LIST_DIR}/usb_stack
    ${CMAKE_CURRENT_LIST_DIR}/util
    ${CMAKE_CURRENT_LIST_DIR}/..
)