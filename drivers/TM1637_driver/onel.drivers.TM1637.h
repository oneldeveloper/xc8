/* 
 * File:   onel.drivers.TM1637.h
 * Author: lucav
 *
 * Created on May 27, 2021, 10:28 PM
 */

#ifndef ONEL_DRIVERS_TM1637_H
#define	ONEL_DRIVERS_TM1637_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#

struct _TM1637_driver_context {
    uint8_t command;
    uint8_t data[6];
    uint8_t dp;
    uint8_t data_lenght;
    uint8_t *reserved_p_byte;
    uint8_t reserved_data_handled;
    uint8_t reserved_driver_current_status;
    uint8_t reserved_driver_next_status;
    uint8_t reserved_data[6];
    bool reserved_driver_busy;
};

struct _TM1637_driver_parameters {
    uint8_t *keys;
};

struct _TM1637_driver_functions {
    void (*set_data_direction)(uint8_t);
    void (*drive_clock_pin)(uint8_t);  
    void (*drive_data_pin)(uint8_t);
    void (*p_operation_completed_callback)(struct _TM1637_driver_context *);
};


void TM1637_read_keys(struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions, struct _TM1637_driver_parameters *parameters);
void TM1637_write_display(struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions);
void TM1637_display_on(uint8_t brightness, struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions);
void TM1637_display_off(struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions);



void TM1637_operation_complete(struct _TM1637_driver_context *context);

void TM1637_scan_handler(struct _TM1637_driver_context *context, struct _TM1637_driver_parameters *parameters, struct _TM1637_driver_functions *functions);





#endif	/* ONEL_DRIVERS_TM1637_H */

