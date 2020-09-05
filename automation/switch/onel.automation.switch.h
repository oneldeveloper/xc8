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

enum switch_status { SWITCH_RELEASED = 0, SWITCH_PRESSED = 1};

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
	float second_function_time_ms;
    float repeat_time_ms;
    union _switch_flags flags;
};




typedef struct switch_functions_struct
{
    uint8_t(*p_get_state)(void);
	void(*p_r_edge_callback)(void);
	void(*p_f_edge_callback)(void);
    void(*p_pressed_state_callback)(void);
	void(*p_r_edge_second_function_callback)(void);
	void(*p_f_edge_second_function_callback)(void);
} switch_functions_t;

typedef struct switch_parameters 
{
	uint16_t ms_per_scan;
	float second_function_time_ms;
    float repeat_time_ms;
    bool autorepeat_state; 
} switch_parameters_t;


void switch_initialize(struct _switch_context  *context, switch_parameters_t *parameters, switch_functions_t *functions);

void switch_scan(struct _switch_context  *context, switch_parameters_t *parameters, switch_functions_t *functions);

#endif	/* ONEL_AUTOMATION_SWITCH_H */

