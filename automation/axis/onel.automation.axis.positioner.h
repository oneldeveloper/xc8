/* 
 * File:   positioner.h
 * Author: lucav
 *
 * Created on July 15, 2020, 10:08 PM
 */

#ifndef POSITIONER_H
#define	POSITIONER_H

#include <xc.h>
#include <stdint.h>

enum _positioner_statuses {  
            POSITIONER_UNINITIALIZED = 0,
            POSITIONER_IDLE = 1, 
            POSITIONER_IN_POSITION = 10, 
            POSITIONER_NOT_IN_POSITION = 11,
            POSITIONER_END_OF_MOVEMENT = 12,        
            POSITIONER_MOVING = 13,
            POSITIONER_JOG_FORWARD = 20,
            POSITIONER_JOG_BACKWARD = 25,
            POSITIONER_HOMING = 50, 
            POSITIONER_ERROR = 100, 
            POSITIONER_ERROR_NO_REFERENCE = 101, 
            POSITIONER_ERROR_POSITION = 102};
            
struct _positioner_functions
{
    bool (*homeSignal)(void);
    bool (*limitSwitchForward)(void);
    bool (*limitSwitchBackward)(void);
    void (*enable)(void);
    void (*disable)(void);
    void (*moveForward)(void);
    void (*moveBackward)(void);
    void (*set_speed)(float);
    void (*brake)(void);
};

struct _positioner_parameters
{
    float scan_time;
    float pulsesPerUnit;
    float positionMin;
    float positionMax;
    float inPositionRange;
    float homeOffset;
    float homeSpeed;
    float min_speed;
    float decel_distance;
    uint8_t homeDirection;
};

struct _positioner_context_struct 
{
    int32_t positionPulses;
    int32_t previous_position;
    int32_t targetPositionPulses;    
    float speedPulsesSec;
    float targetSpeedPulsesSec;
    float drive_speed_command;
    int32_t inPositionRange;
    uint8_t status;
    uint8_t direction;   
    uint8_t home_valid;
    void (*timed_function_in_execution)(struct _positioner_context_struct*, struct _positioner_parameters *,  struct _positioner_functions *);
    };



void positioner_initialize(struct _positioner_context_struct* context, struct _positioner_parameters * parameters);
void positioner_enable(struct _positioner_context_struct* context, struct _positioner_functions  *functions);
void positioner_disable(struct _positioner_context_struct* context, struct _positioner_functions  *functions);

void positioner_home(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions);
void positioner_start_absolute(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions, float newPosition, float speed);
void positioner_stop(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions);
void positioner_jog_forward(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions, float speed);
void positioner_jog_backward(struct _positioner_context_struct* context, struct _positioner_parameters * parameters, struct _positioner_functions  * functions, float speed);


void positioner_scan_handler (struct _positioner_context_struct* context, struct _positioner_parameters * parameters,  struct _positioner_functions * functions);
void positioner_pulses_handler(struct _positioner_context_struct* context,struct _positioner_parameters * parameters, struct _positioner_functions * functions, int32_t pulses);

#endif	/* POSITIONER_H */

