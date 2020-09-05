#include "onel.conversion.bit_converter.h"


void bit_converter_int16_bytes(int16_t value, uint8_t * buffer, uint8_t offset)
{
	uint8_t * pBuffer = buffer + offset;
	*pBuffer++ = (uint8_t)(value >> 8);
	*pBuffer = (uint8_t)(value);		
}

int16_t bit_converter_bytes_int16(uint8_t * buffer, uint8_t offset)
{
	uint8_t * pBuffer = buffer + offset;
	int16_t value = (int16_t)(*pBuffer++) << 8;
	value += (int16_t)(*pBuffer);
	return value;	
}

void bit_converter_uint16_bytes(uint16_t value, uint8_t * buffer, uint8_t offset)
{
	uint8_t * pBuffer = buffer + offset;
	*pBuffer++ = (uint8_t)(value >> 8);
	*pBuffer = (uint8_t)(value);		
}

uint32_t bit_converter_bytes_uint16(uint8_t * buffer, uint8_t offset)
{
	uint8_t * pBuffer = buffer + offset;
	uint16_t value = (uint32_t)(*pBuffer++) << 8;
	value += (uint32_t)(*pBuffer);
	return value;	
}



void bit_converter_int32_bytes(int32_t value, uint8_t * buffer, uint8_t offset)
{
	uint8_t * pBuffer = buffer + offset;
	*pBuffer++ = (uint8_t)(value >> 24);
	*pBuffer++ = (uint8_t)(value >> 16);
	*pBuffer++ = (uint8_t)(value >> 8);
	*pBuffer = (uint8_t)(value);		
}

int32_t bit_converter_bytes_int32(uint8_t * buffer, uint8_t offset)
{
	uint8_t * pBuffer = buffer + offset;
	int32_t value = (int32_t)(*pBuffer++) << 24;
	value += (int32_t)(*pBuffer++) << 16;
	value += (int32_t)(*pBuffer++) << 8;
	value += (int32_t)(*pBuffer);
	return value;	
}

void bit_converter_uint32_bytes(uint32_t value, uint8_t * buffer, uint8_t offset)
{
	uint8_t * pBuffer = buffer + offset;
	*pBuffer++ = (uint8_t)(value >> 24);
	*pBuffer++ = (uint8_t)(value >> 16);
	*pBuffer++ = (uint8_t)(value >> 8);
	*pBuffer = (uint8_t)(value);		
}

uint32_t bit_converter_bytes_uint32(uint8_t * buffer, uint8_t offset)
{
	uint8_t * pBuffer = buffer + offset;
	uint32_t value = (uint32_t)(*pBuffer++) << 24;
	value += (uint32_t)(*pBuffer++) << 16;
	value += (uint32_t)(*pBuffer++) << 8;
	value += (uint32_t)(*pBuffer);
	return value;	
}

void bit_converter_float_bytes(float value, uint8_t * buffer, uint8_t offset)
{
	union {
		float f;
		uint8_t b[4];
	} v;	
	v.f = value;
	uint8_t * pBuffer = buffer + offset;
	*pBuffer++ = v.b[3];
	*pBuffer++ = v.b[2];
	*pBuffer++ = v.b[1];
	*pBuffer = v.b[0];		
}

float bit_converter_bytes_float(uint8_t * buffer, uint8_t offset)
{
	union {
		float f;
		uint8_t b[4];
	} value;	
	uint8_t * pBuffer = buffer + offset;
	value.b[3] = *pBuffer++;
	value.b[2] = *pBuffer++;
	value.b[1] = *pBuffer++;
	value.b[0] = *pBuffer;
	return value.f;	
}
