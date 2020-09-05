
#include "onel.io.persistance.h"
#include "../conversion/onel.conversion.bit_converter.h"




uint8_t (*p_read_byte)(uint16_t address);
void (*p_write_byte)(uint16_t address, uint8_t value);

void set_read_write_byte_functions(uint8_t (*read)(uint16_t bAdd),void (*write)(uint16_t, uint8_t))
{
	p_read_byte = read;
	p_write_byte = write;
}


int16_t read_int16(uint16_t address)
{	
	uint8_t b[2];	
	b[0] = p_read_byte(address);
	b[1] = p_read_byte(address +1);
	return  bit_converter_bytes_int16(b, 0);
}

uint16_t read_uint16(uint16_t address)
{	
	uint8_t b[2];
	b[0] = p_read_byte(address);
	b[1] = p_read_byte(address +1);
	return  bit_converter_bytes_uint16(b, 0);
}

int32_t read_int32(uint16_t address)
{	
	uint8_t b[4];
	b[0] = p_read_byte(address);
	b[1] = p_read_byte(address +1);
	b[2] = p_read_byte(address +2);
	b[3] = p_read_byte(address +3);
	return  bit_converter_bytes_int32(b, 0);
}

uint32_t read_uint32(uint16_t address)
{	
	uint8_t b[4];
	b[0] = p_read_byte(address);
	b[1] = p_read_byte(address +1);
	b[2] = p_read_byte(address +2);
	b[3] = p_read_byte(address +3);
	return  bit_converter_bytes_uint32(b, 0);
}
float read_float(uint16_t address)
{	
	uint8_t b[4];
	b[0] = p_read_byte(address);
	b[1] = p_read_byte(address +1);
	b[2] = p_read_byte(address +2);
	b[3] = p_read_byte(address +3);
	return  bit_converter_bytes_float(b, 0);
}

void write_int16(uint16_t address, int16_t value)
{
	uint8_t b[2];
	bit_converter_int16_bytes(value, b, 0);
	p_write_byte(address, b[0]);
	p_write_byte(address + 1, b[1]);
}
void write_uint16(uint16_t address, uint16_t value)
{
	uint8_t b[2];
	bit_converter_uint16_bytes(value, b, 0);
	p_write_byte(address, b[0]);
	p_write_byte(address + 1, b[1]);
}
void write_int32(uint16_t address, int32_t value)
{
	uint8_t b[4];
	bit_converter_int32_bytes(value, b, 0);
	p_write_byte(address, b[0]);
	p_write_byte(address + 1, b[1]);
	p_write_byte(address + 2, b[2]);
	p_write_byte(address + 3, b[3]);
}
void write_uint32(uint16_t address, uint32_t value)
{
	uint8_t b[4];
	bit_converter_uint32_bytes(value, b, 0);
	p_write_byte(address, b[0]);
	p_write_byte(address + 1, b[1]);
	p_write_byte(address + 2, b[2]);
	p_write_byte(address + 3, b[3]);
}
void write_float(uint16_t address, float value)
{
	uint8_t b[4];
	bit_converter_float_bytes(value, b, 0);
	p_write_byte(address, b[0]);
	p_write_byte(address + 1, b[1]);
	p_write_byte(address + 2, b[2]);
	p_write_byte(address + 3, b[3]);
}

