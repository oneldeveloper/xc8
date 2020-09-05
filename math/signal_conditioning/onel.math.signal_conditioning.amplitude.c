
#include "onel.math.signal_conditioning.amplitude.h"


float limit_band(float input, float min, float max)
{
	if (input < min)
		return min;
	if (input > max)
		return max;
	return input;
}

float scale_nomal(float input, float min, float max)
{
    return (input * (max - min)) + min;
}

float normalize_scalar(float input, float min, float max)
{
    if (max == min)
        return 0;
	if (input > max)
		return 1;
	if(input < min)
		return 0;
    return (input - min) / (max - min); 
}

