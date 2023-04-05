#ifndef REPORT_DESCRIPTOR_H
#define REPORT_DESCRIPTOR_H

#include <stdint.h>
#include "class/hid/hid_device.h"
#include "report_ids.h"

#define REPORT_ID_COUNT (LAST - 1)
// module connected
/**
 *  COLLECTION (Logical)
 *      REPORT_ID (1)
 *      USAGE_PAGE (Generic Desktop)
 *      USAGE (System Control)
 *      LOGICAL_MINIMUM (0)
 *      LOGICAL_MAXIMUM (255)
 *      REPORT_COUNT (4) // 1 byte for UID, 3 bytes for hexagonal coordinates
 *      REPORT_SIZE (8)
 *      INPUT (Data,Ary,Abs)
 *  END COLLECTION
*/
#define REPORT_DESC_MODULE_CONNECTED \
    HID_COLLECTION(HID_COLLECTION_LOGICAL),\
        HID_REPORT_ID(REPORT_ID_MODULE_CONNECTED)\
        HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),\
        HID_USAGE(HID_USAGE_DESKTOP_SYSTEM_CONTROL),\
        HID_LOGICAL_MIN(0),\
        HID_LOGICAL_MAX(255),\
        HID_REPORT_COUNT(3),\
        HID_REPORT_SIZE(8),\
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),\
    HID_COLLECTION_END\

/**
 *  USAGE_PAGE (Generic Desktop)
 *  USAGE (System Control)
 *  LOGICAL_MINIMUM (0)
 *  LOGICAL_MAXIMUM (255)
 *  REPORT_COUNT (1)
 *  REPORT_SIZE(8)
*/
#define REPORT_DESC_UID_ITEMS \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),\
    HID_USAGE(HID_USAGE_DESKTOP_SYSTEM_CONTROL),\
    HID_LOGICAL_MIN(0),\
    HID_LOGICAL_MAX(255),\
    HID_REPORT_COUNT(1),\
    HID_REPORT_SIZE(8),\
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE)\

// Module disconnected
/**
 *  COLLECTION (Logical)
 *      REPORT_ID (2)
 *      REPORT_DESC_UID_ITEMS // all we need to specify is the UID
 *      INPUT (Data,Ary,Abs)
 *  END COLLECTION
*/
#define REPORT_DESC_MODULE_DISCONNECTED \
    HID_COLLECTION(HID_COLLECTION_LOGICAL),\
        HID_REPORT_ID(REPORT_ID_MODULE_DISCONNECTED)\
        REPORT_DESC_UID_ITEMS,\
    HID_COLLECTION_END\

// 1-Button Data
/**
 *  COLLECTION (Physical)
 *      REPORT_ID (3)
 *      REPORT_DESC_UID_ITEMS
 *      // Data
 *      USAGE_Page (Button)
 *      USAGE (Button 1)
 *      LOGICAL_MINIMUM (0)
 *      LOGICAL_MAXIMUM (1)
 *      REPORT_COUNT (1)
 *      REPORT_SIZE (8)
 *      INPUT (Data,Var,Abs)
 *  END COLLECTION
*/
#define REPORT_DESC_BUTTON_DATA \
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),\
        HID_REPORT_ID(REPORT_ID_BUTTON_DATA)\
        REPORT_DESC_UID_ITEMS,\
        HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),\
        HID_USAGE_MIN(1),\
        HID_USAGE_MAX(1),\
        HID_LOGICAL_MIN(0),\
        HID_LOGICAL_MAX(1),\
        HID_REPORT_COUNT(1),\
        HID_REPORT_SIZE(8),\
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),\
    HID_COLLECTION_END\

// D-pad Data
/**
 *  COLLECTION (Physical)
 *      REPORT_ID (4)
 *      REPORT_DESC_UID_ITEMS
 *      // Data
 *      USAGE_Page (Button)
 *      USAGE_MINIMUM (Button 1)
 *      USAGE_MAXIMUM (Button 4)
 *      LOGICAL_MINIMUM (0)
 *      LOGICAL_MAXIMUM (1)
 *      REPORT_COUNT (4)
 *      REPORT_SIZE (2)
 *      INPUT (Data,Var,Abs)
 *  END COLLECTION
*/
#define REPORT_DESC_ANALOG_DATA \
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),\
        HID_REPORT_ID(REPORT_ID_ANALOG_DATA)\
        REPORT_DESC_UID_ITEMS,\
        HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),\
        HID_USAGE_MIN(1),\
        HID_USAGE_MAX(4),\
        HID_LOGICAL_MIN(0),\
        HID_LOGICAL_MAX(1),\
        HID_REPORT_COUNT(1),\
        HID_REPORT_SIZE(8),\
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),\
    HID_COLLECTION_END\

// Joystick Data
/**
 *  COLLECTION (Physical)
 *      REPORT_ID (5)
 *      REPORT_DESC_UID_ITEMS
 *      // Data
 *      USAGE (X)
 *      USAGE (Y)
 *      LOGICAL_MINIMUM (-127)
 *      LOGICAL_MAXIMUM (127)
 *      REPORT_SIZE (8)
 *      REPORT_COUNT (2)
 *      INPUT (Data,Var,Abs)
 *  END COLLECTION
*/
#define REPORT_DESC_JOYSTICK_DATA \
    HID_COLLECTION(HID_COLLECTION_PHYSICAL),\
        HID_REPORT_ID(REPORT_ID_JOYSTICK_DATA)\
        REPORT_DESC_UID_ITEMS,\
        HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),\
        HID_USAGE(HID_USAGE_DESKTOP_X),\
        HID_USAGE(HID_USAGE_DESKTOP_Y),\
        HID_LOGICAL_MIN(0),\
        HID_LOGICAL_MAX(255),\
        HID_REPORT_SIZE(8),\
        HID_REPORT_COUNT(3),\
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),\
    HID_COLLECTION_END\

/**
 *  USAGE_PAGE (Generic Desktop)
 *  USAGE (Game Pad)
 *  COLLECTION (Application)
 *      module connected message
 *      module disconnected message
 *      button data message
 *      d-pad data message
 *      joystick data message
 *  END COLLECTION
*/

// const uint8_t hid_report[] = REPORT_DESCRIPTOR;
#define REPORT_DESCRIPTOR \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),\
    HID_USAGE(HID_USAGE_DESKTOP_GAMEPAD),\
    HID_COLLECTION(HID_COLLECTION_APPLICATION),\
        REPORT_DESC_MODULE_CONNECTED,\
        REPORT_DESC_MODULE_DISCONNECTED,\
        REPORT_DESC_BUTTON_DATA,\
        REPORT_DESC_ANALOG_DATA,\
        REPORT_DESC_JOYSTICK_DATA,\
    HID_COLLECTION_END\

#endif