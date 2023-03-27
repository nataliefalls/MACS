#ifndef REPORT_IDS_H
#define REPORT_IDS_H

enum report_id_t {
    REPORT_ID_MODULE_CONNECTED = 1,
    REPORT_ID_MODULE_DISCONNECTED,
    REPORT_ID_BUTTON_DATA,
    REPORT_ID_DPAD_DATA,
    REPORT_ID_JOYSTICK_DATA,
    LAST
} __attribute__ ((__packed__));
static_assert( sizeof(enum report_id_t) == 1 );

typedef enum report_id_t report_id_t;

#endif