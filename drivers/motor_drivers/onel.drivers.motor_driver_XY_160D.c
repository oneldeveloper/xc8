#include <stdbool.h>
#include  "onel.drivers.motor_driver.h"



void M1_Forward(void)
{
	M1_Fw_SetHigh();
	M1_Bw_SetLow();
}
void M1_Backward(void)
{
	M1_Fw_SetLow();
	M1_Bw_SetHigh();
}
void M1_Brake(void)
{
	M1_Fw_SetLow();
	M1_Bw_SetLow();
}
void M1_Free(void)
{
	M1_Fw_SetHigh();
	M1_Bw_SetHigh();
}

uint8_t M1_GetMode (void)
{
	if (M1_Fw_GetValue() && M1_Bw_GetValue())
		return MOTOR_FREE;
	if (M1_Fw_GetValue() && !M1_Bw_GetValue())
		return MOTOR_FORWARD;
	if (!M1_Fw_GetValue() && M1_Bw_GetValue())
		return MOTOR_BACKWARD;
	return MOTOR_BRAKE;
}

void M2_Forward(void)
{
	M2_Fw_SetHigh();
	M2_Bw_SetLow();
}
void M2_Backward(void)
{
	M2_Fw_SetLow();
	M2_Bw_SetHigh();
}
void M2_Brake(void)
{
	M2_Fw_SetLow();
	M2_Bw_SetLow();
}
void M2_Free(void)
{
	M2_Fw_SetHigh();
	M2_Bw_SetHigh();
}
uint8_t M2_GetMode (void)
{
	if (M2_Fw_GetValue() && M2_Bw_GetValue())
		return MOTOR_FREE;
	if (M2_Fw_GetValue() && !M2_Bw_GetValue())
		return MOTOR_FORWARD;
	if (!M2_Fw_GetValue() && M2_Bw_GetValue())
		return MOTOR_BACKWARD;
	return MOTOR_BRAKE;
}
