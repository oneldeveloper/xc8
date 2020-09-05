#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "pid_c.h"


float LimitBand(float input, float min, float max)
{
    float n = input;
    if (n < min)
    {
        n = min;
        return n;
    }
    if (n > max)
        n = max;
    return n;
}

float ScaleNormal(float input, float min, float max)
{
    return (input * (max - min)) + min;
}

float Normalize(float input, float min, float max)
{
    if (max == min)
        return min;
    float out = (input - min) / (max - min); 
    if (out < 0)
        return 0;
    if (out > 1)
        return 1;
	return out;
}


 
float pvPrevious, pvOlder; //provess variable

float kp, ki, kd;  //Constants parameters
float iPeriod, dPeriod; //time constants
float coPrevious; //control output

float inMin, inMax, fbMin, fbMax, outMin, outMax;


pidModeDef pidMode = IDLE;


float ProportionalComponent(float pv, float pvPrevious, float kp)
{
	return (pv - pvPrevious) * kp;
}

float IntegralComponent(float sp, float pv, float ki, float period)
{
    float a = sp;
    float b = pv;
    float c= ki;
    float d = period;
	return (sp - pv) * ki * period;
}

float DerivativeComponent(float pv, float pvPrevious, float pvOlder, float kd, float period)
{
	return (pv - (2 * pvPrevious) + pvOlder) * kd / period;
}

void PidInitialize(pidModeDef mode, float kpFactor, float kiFactor, float kdFactor, float iPeriodPar, float dPeriodPar)
{
	pidMode = mode;
	kp = kpFactor;
	ki = kiFactor;
	kd = kdFactor;
	iPeriod = iPeriodPar;
	dPeriod = dPeriodPar;
	pvPrevious = 0;
	pvOlder = 0;
	coPrevious = 0;
	
}
void PidCharacterize (float inputMin, float inputMax, float feedbackMin, float feedbackMax, float outputMin, float outputMax)
{
	inMin = inputMin;
	inMax = inputMax;
	fbMin = feedbackMin;
	fbMax = feedbackMax;
	outMin = outputMin;
	outMax = outputMax;
}

void ChangePidStatus(pidModeDef mode)
{
	pidMode = mode;
}


//float PidComputation()
//{
//	float proportional = ProportionalComponent (pv, pvPrevious, kp);
//	float integral = IntegralComponent(sp, pv, ki, iPeriod);
//	float derivative = DerivativeComponent(pv, pvPrevious, pvOlder, kd, dPeriod);
//	float pidOutput = coPrevious - proportional + integral - derivative;
//	return pidOutput;
//}

float PidCompute(float input, float feedback)
{
    float co = 0;
    if (pidMode > IDLE)
	{
    	float sp = Normalize(input, inMin, inMax);
    	float pv = Normalize(feedback, fbMin, fbMax);
    	float pComponent = 0;
    	float iComponent = 0;
    	float dComponent = 0;
    
    
    
    	if (pidMode >= P_MODE)
    	{		
    		pComponent = ProportionalComponent(pv, pvPrevious, kp);
    	}
    	if (pidMode >= PI_MODE)
    	{
    		iComponent = IntegralComponent(sp, pv, ki, iPeriod);
    	}
    	if (pidMode >= PID_MODE)
    	{
    		dComponent = DerivativeComponent(pv, pvPrevious, pvOlder, kd, dPeriod);
    	}	
    	co = coPrevious - pComponent + iComponent - dComponent;
    	coPrevious =  co;
		coPrevious = LimitBand(co, 0.0, 1.0);
    	pvOlder = pvPrevious;
    	pvPrevious = pv;
		printf("%.2f %.2f %.2f %.2f %.2f\r\n", co, coPrevious, pComponent, iComponent, dComponent);
	}
	float coLimited = LimitBand(co, 0.0, 1.0);
	float output = ScaleNormal(coLimited, outMin, outMax);
	return output;
}


















































































