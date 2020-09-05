/* 
 * File:   pid_c.h
 * Author: lucav
 *
 * Created on March 7, 2020, 9:22 PM
 */

#ifndef PID_C_H
#define	PID_C_H

typedef enum pidModeEnum {IDLE, STEADY, P_MODE, PI_MODE, PID_MODE} pidModeDef; 

void PidInitialize(pidModeDef mode, float kpFactor, float kiFactor, float kdFactor, float iPeriodPar, float dPeriodPar);
void PidCharacterize (float inputMin, float inputMax, float feedbackMin, float feedbackMax, float outputMin, float outputMax);
void ChangePidStatus(pidModeDef mode);

float PidCompute(float input, float feedback);

#endif	/* PID_H */

