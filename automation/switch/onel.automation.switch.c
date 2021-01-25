
#include <stdint.h>
#include <stdbool.h>
#include "onel.automation.switch.h"



void switch_scan(struct _switch_context  *context, struct _switch_parameters *parameters, struct _switch_functions *functions)
{
	uint8_t state = functions->p_get_state();
	if(state == SWITCH_PRESSED)
	{
		if(!context->flags.pressed_callback_done)
		{
			context->flags.pressed_callback_done = true;
			context->repeat_time_elapsed = 0;
			if(functions->p_pressed_state_callback != 0)
				functions->p_pressed_state_callback();			
		}
		else if (parameters->autorepeat_state)
		{
		if(context->repeat_time_elapsed > parameters->repeat_time)
		{
			context->repeat_time_elapsed = 0;
			if(functions->p_pressed_state_callback != 0)
				functions->p_pressed_state_callback();
		}
		else
			context->repeat_time_elapsed += parameters->scan_time;
		}
	}
	else
	{
		context->repeat_time_elapsed = 0;
		context->flags.pressed_callback_done = false;
	}
	if(context->physical_status ^ state)
	{
		context->flags.r_edge_second_function_done = false;
		context->second_function_time_elapsed = 0;
		if (state == SWITCH_PRESSED)			
		{
			if (functions->p_r_edge_callback != 0)
				functions->p_r_edge_callback();
		}
		else
		{
			if (functions->p_f_edge_callback)
				functions->p_f_edge_callback();			
		}
	}
	else
	{		
		if(context->second_function_time_elapsed > parameters->second_function_time)
		{
			if (state == SWITCH_PRESSED && !context->flags.r_edge_second_function_done)
			{
				context->flags.r_edge_second_function_done = true;
				if (functions->p_r_edge_second_function_callback != 0)
					functions->p_r_edge_second_function_callback();
			}

		}
		else
			context->second_function_time_elapsed += parameters->scan_time;
	}

	context->physical_status = state;
}

void switch_initialize(struct _switch_context  *context, struct _switch_parameters *parameters, struct _switch_functions *functions)
{
	context->flags.r_edge_second_function_done = 0;
	context->second_function_time_elapsed = 0;
}