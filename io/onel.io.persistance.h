/* 
 * File:   persistance.h
 * Author: lucav
 *
 * Created on August 2, 2020, 12:41 PM
 */

#ifndef PERSISTANCE_H
#define	PERSISTANCE_H
#include <xc.h>
#include <stdint.h>

int16_t read_int16(uint16_t address);
uint16_t read_uint16(uint16_t address);
int32_t read_int32(uint16_t address);
uint32_t read_uint32(uint16_t address);
float read_float(uint16_t address);

void write_int16(uint16_t address, int16_t value);
void write_uint16(uint16_t address, uint16_t value);
void write_int32(uint16_t address, int32_t value);
void write_uint32(uint16_t address, uint32_t value);
void write_float(uint16_t address, float value);

void set_read_write_byte_functions(uint8_t (*read)(uint16_t bAdd),void (*write)(uint16_t, uint8_t));

#endif	/* PERSISTANCE_H */

