#include <stdbool.h>
#include "onel.automation.axis.positioner.h"
#include "../../math/signal_conditioning/onel.math.signal_conditioning.amplitude.h"


void home_timed_handler(struct _positioner_context_struct* context, struct _positioner_parameters * parameters,  struct _positioner_functions  * functions);
void position_timed_handler( struct _positioner_context_struct* context,struct _positioner_parameters * parameters, struct _positioner_functions  * functions);
void jog_timed_handler (struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions);
void compute_speed(struct _positioner_context_struct* context, struct _positioner_parameters * parameters);
void compute_home_speed(struct _positioner_context_struct* context, struct _positioner_parameters * parameters);
//void (*timed_handler)(struct _positioner_context_struct*, struct _positioner_parameters *, struct _positioner_functions  *);

//void _handler(struct _positioner_parameters * parameters, struct _positioner_context_struct* context, struct _positioner_functions  * functions, int32_t pulses);

//void (*pulse_handler)(struct _positioner_parameters *, struct _positioner_context_struct*, struct _positioner_functions  *, int32_t);

/*INiziazlizza i valori attuali del posizionatore*/
void positioner_initialize( struct _positioner_context_struct* context, struct _positioner_parameters * parameters)
{
	context->inPositionRange = parameters->inPositionRange * parameters->pulsesPerUnit;
	context->positionPulses = 0;
	context->previous_position = 0;
	context->speedPulsesSec = parameters->homeSpeed * parameters->pulsesPerUnit;
	context->targetPositionPulses = 0;
	context->targetSpeedPulsesSec =  parameters->homeSpeed * parameters->pulsesPerUnit;
	context->status = POSITIONER_IDLE;
	context->home_valid = 0;
}


//Funzionalità ancora da sviluppare e da testare
//L'idea è quella di abilitare l'asse quando questo si trova in modalità IDLE,
//Se le condizioni lo consentono.

void positioner_enable(struct _positioner_context_struct* context, struct _positioner_functions  *functions)
{
	if (context->status == POSITIONER_IDLE) 
	{
		if (context->home_valid == 0)
			context->status = POSITIONER_ERROR_NO_REFERENCE;	
		else			
			if (abs(context->positionPulses - context->targetPositionPulses) <= (context->inPositionRange) / 2)
				context->status = POSITIONER_IN_POSITION;
			else
				context->status = POSITIONER_NOT_IN_POSITION;
		if (functions->enable != 0)
			functions->enable();
	}
	return;
}

void positioner_disable(struct _positioner_context_struct* context, struct _positioner_functions  *functions)
{
	context->status = POSITIONER_IDLE;
	if (functions->disable != 0)
	functions->disable();
}



//Richiama la routine home
void positioner_home(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions)
{
	if (context->status != POSITIONER_IDLE &&
		context->status != POSITIONER_MOVING &&
		context->status != POSITIONER_HOMING)
	{
		if (parameters->homeDirection == 0)
		{		
			functions->moveForward();
			context->direction = 1;
		} 
		else 
		{
			context->direction = 0;
			functions->moveBackward();
		}
		context->targetSpeedPulsesSec = parameters->homeSpeed * parameters->pulsesPerUnit;
		context->status = POSITIONER_HOMING;
		context->drive_speed_command = 0;
		context->speedPulsesSec = 0;
		context->timed_function_in_execution = &home_timed_handler;
		//timed_handler = home_timed_handler;	
	}
}

void positioner_start_absolute(struct _positioner_context_struct* context,struct _positioner_parameters * parameters, struct _positioner_functions  * functions, float newPosition, float speed)
{
	if (context->status == POSITIONER_NOT_IN_POSITION ||
		context->status == POSITIONER_IN_POSITION ||
		context->status == POSITIONER_MOVING)
	{
		context->targetPositionPulses = newPosition * parameters->pulsesPerUnit;

		float position_difference = context->targetPositionPulses - context->positionPulses;
		if (abs(position_difference) < parameters->inPositionRange / 2)
		{
			context->status = POSITIONER_IN_POSITION;
			return;
		}
		context->drive_speed_command = 0;
		if (position_difference > 0)
		{
			functions->moveForward();
			context->direction = 0;
		}
		else
		{
			functions->moveBackward();
			context->direction = 1;
		}
		context->status = POSITIONER_MOVING;		
		context->targetSpeedPulsesSec = speed * parameters->pulsesPerUnit;
		context->drive_speed_command = 0;
		context->speedPulsesSec = 0;
		context->timed_function_in_execution = &position_timed_handler;
		//timed_handler = position_timed_handler;
	}
}
void positioner_stop(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions)
{
	functions->brake();
	context->targetPositionPulses = context->positionPulses;
	context->speedPulsesSec = 0;
	if (context->status == POSITIONER_JOG_FORWARD || context->status == POSITIONER_JOG_BACKWARD)
		context->status = POSITIONER_IN_POSITION;
}

void positioner_jog_forward(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions, float speed)
{
	if (context->positionPulses * parameters->pulsesPerUnit <= parameters->positionMax)
	{
		if (context->status == POSITIONER_NOT_IN_POSITION ||
			context->status == POSITIONER_IN_POSITION ||
			(context->status == POSITIONER_END_OF_MOVEMENT && context->positionPulses < parameters->positionMax * parameters->pulsesPerUnit) ||
			context->status == POSITIONER_MOVING)
		{
			context->targetPositionPulses = context->positionPulses;
			context->targetSpeedPulsesSec = speed * parameters->pulsesPerUnit;
			context->drive_speed_command = 0;
			context->speedPulsesSec = 0;
			functions->moveForward();
			context->direction = 0;
			context->status = POSITIONER_JOG_FORWARD;
		}
	}
	//context->status = POSITIONER_IN_POSITION;
	context->timed_function_in_execution = &jog_timed_handler;
	//timed_handler = &jog_timed_handler;
}
void positioner_jog_backward(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions, float speed)
{
	if (context->positionPulses * parameters->pulsesPerUnit <= parameters->positionMax)
	{
		if (context->status == POSITIONER_NOT_IN_POSITION ||
			context->status == POSITIONER_IN_POSITION ||
			(context->status == POSITIONER_END_OF_MOVEMENT && context->positionPulses > parameters->positionMin * parameters->pulsesPerUnit) ||
			context->status == POSITIONER_MOVING)
		{
			context->targetPositionPulses = context->positionPulses;
			context->targetSpeedPulsesSec = speed * parameters->pulsesPerUnit;
			context->drive_speed_command = 0; 
			context->speedPulsesSec = 0;
			functions->moveBackward();

			context->direction = 1;
			context->status = POSITIONER_JOG_BACKWARD;
		}
	}
	//context->status = POSITIONER_IN_POSITION;
	context->timed_function_in_execution = &jog_timed_handler;
	//timed_handler = &jog_timed_handler;
}

void positioner_scan_handler (struct _positioner_context_struct* context, struct _positioner_parameters * parameters,  struct _positioner_functions * functions)
{
	if (context->timed_function_in_execution != 0)
		context->timed_function_in_execution(context, parameters, functions);
}

/*Questa funzione è da chiamare ogni volta che vi è un trigger di cambio della posizione corrente ( impulso encoder)
 *che cosa farà la funzione dipende dall'assegnazione del puntatore */
void positioner_pulses_handler(struct _positioner_context_struct* context, struct _positioner_parameters * parameters,  struct _positioner_functions * functions, int32_t pulses)
{
	if(context->status == POSITIONER_HOMING)
		return;
	if (context->direction > 0)
	{
		context->positionPulses -=  pulses;
		if (context->positionPulses < parameters->positionMin * parameters->pulsesPerUnit)
		{
			functions->brake();		
			context->status = POSITIONER_END_OF_MOVEMENT;
		}
	}
	else
	{
		context->positionPulses +=  pulses;	
		if (context->positionPulses > parameters->positionMax * parameters->pulsesPerUnit)
		{
			functions->brake();		
			context->status = POSITIONER_END_OF_MOVEMENT;
		}
	}
	
	float position_difference = context->targetPositionPulses - context->positionPulses;
	if (abs(position_difference) <= parameters->inPositionRange / 2)
	{
		functions->brake();
		context->speedPulsesSec = 0;
	}
}


void home_timed_handler(struct _positioner_context_struct* context, struct _positioner_parameters * parameters,  struct _positioner_functions  * functions)
{
	if (functions->homeSignal() == true) 
	{
		functions->brake();
		context->positionPulses = - (parameters->homeOffset * parameters->pulsesPerUnit);
		context->previous_position = context->positionPulses;
		context->status = POSITIONER_NOT_IN_POSITION;
		positioner_start_absolute(context, parameters, functions, 0, parameters->homeSpeed);
		return;
	}
	compute_home_speed(context, parameters);
	functions->set_speed(context->drive_speed_command);
}

void position_timed_handler(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions)
{
	float position_difference = context->targetPositionPulses - context->positionPulses;
	if (abs(position_difference) <= parameters->inPositionRange / 2)
	{
		context->status = POSITIONER_IN_POSITION;
		return;
	}

	if (position_difference > 0)
	{
		functions->moveForward();
		context->direction = 0;
	}
	else
	{
		functions->moveBackward();
		context->direction = 1;
	}
	compute_speed(context, parameters);
	functions->set_speed(context->drive_speed_command);
	context->status = POSITIONER_MOVING;	
}

void jog_timed_handler (struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions)
{
	context->speedPulsesSec = abs(context->positionPulses - context->previous_position) / parameters->scan_time;
	context->previous_position = context->positionPulses;
	context->drive_speed_command += (context->targetSpeedPulsesSec - context->speedPulsesSec);
	functions->set_speed(context->drive_speed_command);
}

void compute_speed(struct _positioner_context_struct* context, struct _positioner_parameters * parameters)
{
	float remaining_distance = abs(context->targetPositionPulses - context->positionPulses);
	float min_speed = parameters->min_speed * parameters->pulsesPerUnit;
	float decel_distance =  parameters->decel_distance * parameters->pulsesPerUnit;
	
	float max_speed = remaining_distance > decel_distance * parameters->pulsesPerUnit ? context->targetSpeedPulsesSec : remaining_distance / decel_distance * context->targetSpeedPulsesSec;
	float calculated_speed = limit_band(context->targetSpeedPulsesSec, min_speed, max_speed);
	context->speedPulsesSec = abs(context->positionPulses - context->previous_position) / parameters->scan_time;
	context->previous_position = context->positionPulses;
	context->drive_speed_command += (calculated_speed - context->speedPulsesSec);
}

void compute_home_speed(struct _positioner_context_struct* context, struct _positioner_parameters * parameters)
{
	context->speedPulsesSec = abs(context->positionPulses - context->previous_position) / parameters->scan_time;
	context->previous_position = context->positionPulses;
	context->drive_speed_command += (context->targetSpeedPulsesSec - context->speedPulsesSec);
}









/*Questa routine è privata e gestisce il posizionamento dell'asse*/



//void handlePosition(struct _positioner_parameters * parameters, positioner_context_t* context, struct _positioner_functions * functions)
//{
//	if (abs(context->positionPulses - context->targetPositionPulses) > (context->inPositionRange / 2)) {
//		context->status = POSITIONER_MOVING;
//		if (context->positionPulses < context->targetPositionPulses) {
//			context->speedPulsesSec = context->targetSpeedPulsesSec;
//			functions->moveForward(context->speedPulsesSec);
//		} else {
//			context->speedPulsesSec = -context->targetSpeedPulsesSec;
//			functions->moveBackward(-context->speedPulsesSec);
//		}
//		positionerHandler(parameters, context, functions, 0);
//	} else {
//		context->speedPulsesSec = 0;
//		functions->brake();
//		context->status = POSITIONER_IN_POSITION;
//	}
//}
//
///*Questa routine è privata ed è assegnasta al puntatore di funzioni ricorrente per gestire il normale posizionamento dell'asse*/
//void handlePulses(struct _positioner_parameters * parameters, positioner_context_t* context, struct _positioner_functions * functions, int32_t pulses)
//{
//
//	if (context->speedPulsesSec > 0)
//		context->positionPulses += pulses;
//	else
//		if (context->speedPulsesSec < 0)
//		context->positionPulses -= pulses;
//	handlePosition(parameters, context, functions);
//
//}









