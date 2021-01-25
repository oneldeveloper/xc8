/* 
 * File:   onel.automation.switch.h
 * Author: lucav
 *
 * Created on August 9, 2020, 9:57 PM
 */

#ifndef ONEL_AUTOMATION_SWITCH_H
#define	ONEL_AUTOMATION_SWITCH_H

#include <stdint.h>
#include <stdbool.h>

enum switch_statuses { SWITCH_RELEASED = 0, SWITCH_PRESSED = 1};

union _switch_flags {
    struct{
        uint8_t bit0 : 1;
        uint8_t pressed_callback_done : 1;
        uint8_t bit2 : 1;
        uint8_t r_edge_second_function_done : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;        
    };
};


struct _switch_context
{
	uint8_t physical_status;
	float second_function_time_elapsed;
    float repeat_time_elapsed;
    union _switch_flags flags;
};




struct _switch_functions
{
    enum switch_statuses (*p_get_state)(void);
	void(*p_r_edge_callback)(void);
	void(*p_f_edge_callback)(void);
    void(*p_pressed_state_callback)(void);
	void(*p_r_edge_second_function_callback)(void);
	void(*p_f_edge_second_function_callback)(void);
};

struct _switch_parameters 
{
	uint16_t scan_time;
	float second_function_time;
    float repeat_time;
    bool autorepeat_state; 
} ;


void switch_initialize(struct _switch_context  *context, struct _switch_parameters *parameters, struct _switch_functions *functions);

void switch_scan(struct _switch_context  *context, struct _switch_parameters *parameters, struct _switch_functions *functions);

#endif	/* ONEL_AUTOMATION_SWITCH_H */

