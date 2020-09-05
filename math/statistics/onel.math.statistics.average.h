/* 
 * File:   onel.math.statistics.average.h
 * Author: lucav
 *
 * Created on August 2, 2020, 2:39 PM
 */

#ifndef ONEL_MATH_STATISTICS_AVERAGE_H
#define	ONEL_MATH_STATISTICS_AVERAGE_H

#include <stdint.h>




struct _average_db {
    float *p_average_buffer;
    uint8_t bufferLength;
    uint8_t internal_sample_index;    
};

void initialize_average(struct _average_db *average_db);

//float get_average(float sample, struct _average_db *average_db, float (*p_average_type)(*struct _average_db));
float get_moving_average(float sample, struct _average_db *average_db);


#endif	/* ONEL_MATH_STATISTICS_AVERAGE_H */

