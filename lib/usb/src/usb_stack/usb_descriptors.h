#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#include "device/usbd.h"
#include "tusb.h"
#include "report_descriptor.h"
#include "constants.h"

/*******************************
 * DEVICE DESCRIPTOR DEFINITION
********************************/
// tusb_desc_device_t const device_descriptor = { DEVICE_DESCRIPTOR };
#define DEVICE_DESCRIPTOR \
    sizeof(tusb_desc_device_t),\
    TUSB_DESC_DEVICE,\
    0x200,\
    0x00,\
    0x00,\
    0x00,\
    CFG_TUD_ENDPOINT0_SIZE,\
    VENDOR_ID,\
    PRODUCT_ID,\
    BCD_DEVICE,\
    STRING_DESCRIPTOR_MANUFACTURER_INDEX,\
    STRING_DESCRIPTOR_PRODUCT_INDEX,\
    STRING_DESCRIPTOR_SERIAL_NUMBER_INDEX,\
    0x01

/**************************************
 * CONFIGURATION DESCRIPTOR DEFINITION
***************************************/
// uint8_t const configuration_descriptor[] = CONFIGURATION_DESCRIPTOR
#define CONFIGURATION_DESCRIPTOR {\
    TUD_CONFIG_DESCRIPTOR(1, 1, 1, CONFIG_TOTAL_LEN, TU_BIT(7), 500),\
    TUD_HID_DESCRIPTOR(1, 1, HID_ITF_PROTOCOL_NONE, REPORT_DESCRIPTOR_LEN,\
        ENDPOINT_ADDRESS, MAX_PACKET_SIZE, POLLING_INTERVAL_MS)\
};

#endif