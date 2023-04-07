#ifndef CONSTANTS_H
#define CONSTANTS_H

/*******************************
 * DEVICE DESCRIPTOR CONSTANTS
********************************/
#define POLLING_INTERVAL_MS 1 // milliseconds
#define BCD_DEVICE 0x0100 // device release number
#define MAX_PACKET_SIZE sizeof(report_t) + 1
#define VENDOR_ID 0xCafe
#define PRODUCT_ID 0x0000
#define STRING_DESCRIPTOR_MANUFACTURER_INDEX 0x01
#define STRING_DESCRIPTOR_PRODUCT_INDEX 0x02
#define STRING_DESCRIPTOR_SERIAL_NUMBER_INDEX 0x03

/**************************************
 * CONFIGURATION DESCRIPTOR CONSTANTS
***************************************/
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define REPORT_DESCRIPTOR_LEN sizeof({ REPORT_DESCRIPTOR })
#define ENDPOINT_ADDRESS (TU_BIT(7) | TU_BIT(0)) // IN endpoint at address 1
#define MAX_POWER 500 // mA

#endif