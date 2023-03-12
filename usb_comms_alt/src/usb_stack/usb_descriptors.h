/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#include "report_descriptor.h"
#include "device/usbd.h"
#include "tusb.h"

/*********************
 * STUFF TO DETERMINE
**********************/

// for device descriptor
#define POLLING_INTERVAL_MS 1 // milliseconds
#define BCD_DEVICE 0x0100 // device release number

// for configuration descriptor
#define MAX_PACKET_SIZE sizeof(report_t) + 1 // don't know until we define the report descriptor

/*******************************
 * DEVICE DESCRIPTOR DEFINITION
********************************/
#define VENDOR_ID 0xCafe
#define PRODUCT_ID 0x0000
#define STRING_DESCRIPTOR_MANUFACTURER_INDEX 0x01
#define STRING_DESCRIPTOR_PRODUCT_INDEX 0x02
#define STRING_DESCRIPTOR_SERIAL_NUMBER_INDEX 0x03

// device descriptor
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
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define REPORT_DESCRIPTOR_LEN sizeof({ REPORT_DESCRIPTOR })
#define ENDPOINT_ADDRESS (TU_BIT(7) | TU_BIT(0)) // IN endpoint at address 1
#define MAX_POWER 500 // mA

// configuration descriptor
// uint8_t const configuration_descriptor[] = CONFIGURATION_DESCRIPTOR
#define CONFIGURATION_DESCRIPTOR {\
    TUD_CONFIG_DESCRIPTOR(1, 1, 1, CONFIG_TOTAL_LEN, TU_BIT(7), 500),\
    TUD_HID_DESCRIPTOR(1, 1, HID_ITF_PROTOCOL_NONE, REPORT_DESCRIPTOR_LEN,\
        ENDPOINT_ADDRESS, MAX_PACKET_SIZE, POLLING_INTERVAL_MS)\
};

#endif