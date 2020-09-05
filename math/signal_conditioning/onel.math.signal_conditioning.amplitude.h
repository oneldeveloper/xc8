/* 
 * File:   onel.math.signal_conditioning.amplitude.h
 * Author: lucav
 *
 * Created on August 2, 2020, 3:27 PM
 */

#ifndef ONEL_MATH_SIGNAL_CONDITIONING_AMPLITUDE_H
#define	ONEL_MATH_SIGNAL_CONDITIONING_AMPLITUDE_H

float limit_band(float input, float min, float max);

float scale_nomal(float input, float min, float max);

float normalize_scalar(float input, float min, float max);

#endif	/* ONEL_MATH_SIGNAL_CONDITIONING_AMPLITUDE_H */

