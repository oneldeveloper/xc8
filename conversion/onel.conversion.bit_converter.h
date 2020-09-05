/* 
 * File:   bit_converter.h
 * Author: lucav
 *
 * Created on August 2, 2020, 12:12 PM
 */

#ifndef BIT_CONVERTER_H
#define	BIT_CONVERTER_H

#include <xc.h>
#include <stdint.h>


void bit_converter_int16_bytes(int16_t value, uint8_t * buffer, uint8_t offset);

int16_t bit_converter_bytes_int16(uint8_t * buffer, uint8_t offset);

void bit_converter_uint16_bytes(uint16_t value, uint8_t * buffer, uint8_t offset);

uint32_t bit_converter_bytes_uint16(uint8_t * buffer, uint8_t offset);


void bit_converter_int32_bytes(int32_t value, uint8_t * buffer, uint8_t offset);

int32_t bit_converter_bytes_int32(uint8_t * buffer, uint8_t offset);

void bit_converter_uint32_bytes(uint32_t value, uint8_t * buffer, uint8_t offset);

uint32_t bit_converter_bytes_uint32(uint8_t * buffer, uint8_t offset);

void bit_converter_float_bytes(float value, uint8_t * buffer, uint8_t offset);


float bit_converter_bytes_float(uint8_t * buffer, uint8_t offset);

#endif	/* BIT_CONVERTER_H */

