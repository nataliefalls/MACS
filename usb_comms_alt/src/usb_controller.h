#ifndef USB_CONTROLLER_H
#define USB_CONTROLLER_H

#include "report_types.h"

/**
 * prepare a module connected report to be sent over usb
*/
bool register_module(module_connected_report_t report);

/**
 * prepare a module disconnected report to be sent over usb
*/
bool unregister_module(module_disconnected_report_t report);

/**
 * prepare a button report to be sent over usb
*/
bool send_button_report(button_report_t report);

/**
 * prepare a dpad report to be sent over usb
*/
bool send_dpad_report(dpad_report_t report);

/**
 * prepare a joystick report to be sent over usb
*/
bool send_joystick_report(joystick_report_t report);

#endif