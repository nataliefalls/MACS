#ifndef USB_CONTROLLER_H
#define USB_CONTROLLER_H

#include "payload_types.h"

/**
 *  prepare a module connected report to be sent over usb
 *  false return indicates that the report could not be queued,
 *      and this report should be attempted again
 *  true return indicates success
*/
bool module_connected_report(uint8_t moduleID, module_coordinates_t coordinates);

/**
 *  prepare a module disconnected report to be sent over usb
 *  false return indicates that the report could not be queued,
 *      and this report should be attempted again
 *  true return indicates success
*/
bool module_disconnected_report(uint8_t moduleID);

/**
 *  prepare a button report to be sent over usb
 *  false return indicates that the report could not be queued,
 *      and this report should be attempted again
 *  true return indicates success
*/
bool button_report(uint8_t moduleID, button_data_t report);

/**
 *  prepare a dpad report to be sent over usb
 *  false return indicates that the report could not be queued,
 *      and this report should be attempted again
 *  true return indicates success
*/
bool dpad_report(uint8_t moduleID, dpad_data_t report);

/**
 *  prepare a joystick report to be sent over usb
 *  false return indicates that the report could not be queued,
 *      and this report should be attempted again
 *  true return indicates success
*/
bool joystick_report(uint8_t moduleID, joystick_data_t report);

/**
 * the number of reports that can be queued before it's at maximum capacity
*/
uint8_t queue_space_remaining();

#endif