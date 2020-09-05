
#include "onel.math.statistics.average.h"


void initialize_average(struct _average_db *average_db)
{
	average_db->internal_sample_index = 0;
	for(uint8_t i = 0; i< average_db->bufferLength; i++)
		*(average_db->p_average_buffer + i) = 0.0;
}


//float get_average(float sample, struct _average_db *average_db, float (*p_average_type)(float, *struct _average_db))
//{
//	return p_average_type(sample, average_db);
//}

float get_moving_average(float sample, struct _average_db *average_db)
{
	*(average_db->p_average_buffer + average_db->internal_sample_index) = sample;
	if (++average_db->internal_sample_index > average_db->bufferLength - 1)
		average_db->internal_sample_index = 0;
	float sum = 0.0;
	for(uint8_t i = 0; i< average_db->bufferLength; i++)
		sum += *(average_db->p_average_buffer + i);
	return sum/average_db->bufferLength;
}