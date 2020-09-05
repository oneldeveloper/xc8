
#include "onel.drivers.motor_driver.h"



void motor_driver_scan_handler (struct _motor_driver_context   *context, struct _motor_driver_parameters  *parameters, struct _motor_driver_functions  *functions)
{	
	context->current_ma = functions->p_get_current();
	if(functions->p_get_status() != 1)
	{
		context->status = MOTOR_DRIVER_ERROR_DEVICE;
		if (functions->p_error_callback != 0)
			functions->p_error_callback();
		return;	
	}		
	if(context->current_ma > parameters->current_limit)
	{
		context->status = MOTOR_DRIVER_ERROR_OVERCURRENT;
		if (functions->p_error_callback != 0)
			functions->p_error_callback();
		return;
	}
	if ((context->status == MOTOR_DRIVER_MOVING || context->status == MOTOR_DRIVER_ERROR_UNDERCURRENT) && functions->p_get_current() < 10 )
	{
		if (context->undercurrent_time > parameters->undercurrent_timeout)
		{
		context->status = MOTOR_DRIVER_ERROR_UNDERCURRENT;
		if (functions->p_error_callback != 0)
			functions->p_error_callback();
		return;			
		}
		else
			context->undercurrent_time += parameters->scan_period;
	}
	else
		context->undercurrent_time = 0;
		

}

void motor_driver_disable(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions)
{
	if (functions->p_disable != 0)
		functions->p_disable();
	context->status = MOTOR_DRIVER_IDLE;	
}
void motor_driver_enable(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions)
{
	if (functions->p_enable != 0)
		functions->p_enable();
	context->status = MOTOR_DRIVER_ENABLED;	
}

void motor_driver_forward(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions)
{
	if (context->status != MOTOR_DRIVER_IDLE)
	{
		if (functions->p_move_forward != 0)
			functions->p_move_forward();		
		context->status = MOTOR_DRIVER_MOVING;
	}
}
void motor_driver_backward(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions)
{
	if (context->status != MOTOR_DRIVER_IDLE)
	{
		if(functions->p_move_backward != 0)
			functions->p_move_backward();
		context->status = MOTOR_DRIVER_MOVING;
	}
}

void motor_driver_set_speed(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions, float drive_strength)
{
	if(functions->p_set_drive_strength != 0)
		functions->p_set_drive_strength(drive_strength);
}

void motor_driver_stop(struct _motor_driver_context   *context,struct _motor_driver_functions  *functions)
{
	if (context->status != MOTOR_DRIVER_IDLE && context->status < MOTOR_DRIVER_ERROR)
	{
		if (functions->p_stop != 0)
			functions->p_stop();
		context->status = MOTOR_DRIVER_ENABLED;
	}
}