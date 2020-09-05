/* 
 * File:   motor_driver.h
 * Author: lucav
 *
 * Created on July 24, 2020, 6:11 PM
 */

#ifndef MOTOR_DRIVER_H
#define	MOTOR_DRIVER_H

#include <xc.h>
#include <stdint.h>

#define CURRENT_MA_PER_DIGIT 34.44
enum _motor_driver_statuses { 
    MOTOR_DRIVER_IDLE = 0, 
    MOTOR_DRIVER_ENABLED = 1, 
    MOTOR_DRIVER_MOVING = 2, 
    MOTOR_DRIVER_ERROR = 100, 
    MOTOR_DRIVER_ERROR_OVERCURRENT = 100, 
    MOTOR_DRIVER_ERROR_UNDERCURRENT = 101, 
    MOTOR_DRIVER_ERROR_DEVICE = 102};

struct _motor_driver_context {
    uint8_t status;
    float current_ma;
    float undercurrent_time;
};

struct _motor_driver_parameters {
    float current_limit; 
    float scan_period;
    float undercurrent_timeout;
    float drive_strength_factor;
};

struct _motor_driver_functions {
    void (*p_enable)(void);
    void (*p_disable)(void);
    void (*p_move_forward)(void);
    void (*p_move_backward)(void);
    void (*p_set_drive_strength)(float drive_strength);
    void (*p_stop)(void);
    float (*p_get_current)(void);
    uint8_t (*p_get_status)(void);
    void (*p_error_callback)(void);
};



void motor_driver_disable(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions);

void motor_driver_enable(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions);

void motor_driver_forward(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions);

void motor_driver_backward(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions);

void motor_driver_stop(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions);

void motor_driver_set_speed(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions, float speed);

void motor_driver_scan_handler (struct _motor_driver_context   *context, struct _motor_driver_parameters  *parameters, struct _motor_driver_functions  *functions);

#endif	/* MOTOR_DRIVER_H */

