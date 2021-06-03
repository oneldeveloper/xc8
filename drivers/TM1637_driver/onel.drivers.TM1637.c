#include "onel.drivers.TM1637.h"


#define DATA_COMMAND_RESET	0x40
#define DATA_COMMAND_READ_KEYS	0x42
#define DATA_COMMAND_WRITE_DISPLAY	0x40
#define DATA_COMMAND_FIXED_ADDR	0x44

#define ADDRESS_COMMAND_C0H		0xC0
#define ADDRESS_COMMAND_C1H		0xC1
#define ADDRESS_COMMAND_C2H		0xC2
#define ADDRESS_COMMAND_C3H		0xC3
#define ADDRESS_COMMAND_C4H		0xC4
#define ADDRESS_COMMAND_C5H		0xC5

#define DISPLAY_COMMAND_BRIGHT_1_16	0x80
#define DISPLAY_COMMAND_BRIGHT_2_16	0x81
#define DISPLAY_COMMAND_BRIGHT_4_16	0x82
#define DISPLAY_COMMAND_BRIGHT_10_16	0x83
#define DISPLAY_COMMAND_BRIGHT_11_16	0x84
#define DISPLAY_COMMAND_BRIGHT_12_16	0x85
#define DISPLAY_COMMAND_BRIGHT_13_16	0x86
#define DISPLAY_COMMAND_BRIGHT_14_16	0x86
#define DISPLAY_COMMAND_ON	0x88
#define DISPLAY_COMMAND_OFF	0x80

#define DISPLAY_DATA_0	0x3F
#define DISPLAY_DATA_1	0x06
#define DISPLAY_DATA_2	0x5B
#define DISPLAY_DATA_3	0x4F
#define DISPLAY_DATA_4	0x66
#define DISPLAY_DATA_5	0x6D
#define DISPLAY_DATA_6	0x7D
#define DISPLAY_DATA_7	0x07
#define DISPLAY_DATA_8	0x7F
#define DISPLAY_DATA_9	0x6F
#define DISPLAY_DATA_A	0x77
#define DISPLAY_DATA_B	0x7C
#define DISPLAY_DATA_C	0x5F
#define DISPLAY_DATA_D	0x17
#define DISPLAY_DATA_E	0x79
#define DISPLAY_DATA_F	0x78
#define DISPLAY_DP		0x80

enum _driver_machine_statuses { 
    DRIVER_IDLE = 0, 
    DRIVER_SEND_COMMAND_DATA = 1, 
    DRIVER_SEND_DATA = 2, 
	DRIVER_RECEIVE_DATA = 3, 
	DRIVER_DATA_HANDLE_COMPLETE = 4
};

static const uint8_t display_encode_table[16] = { 
	DISPLAY_DATA_0,
	DISPLAY_DATA_1,
	DISPLAY_DATA_2,
	DISPLAY_DATA_3,
	DISPLAY_DATA_4,
	DISPLAY_DATA_5,
	DISPLAY_DATA_6,
	DISPLAY_DATA_7,
	DISPLAY_DATA_8,
	DISPLAY_DATA_9,
	DISPLAY_DATA_A,
	DISPLAY_DATA_B,
	DISPLAY_DATA_C,
	DISPLAY_DATA_D,
	DISPLAY_DATA_E,
	DISPLAY_DATA_F};


void encode_data(struct _TM1637_driver_context *context);
void send_byte(uint8_t data,  struct _TM1637_driver_functions *functions);
void start (struct _TM1637_driver_functions *functions);
void stop (struct _TM1637_driver_functions *functions);



void TM1637_read_keys(struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions, struct _TM1637_driver_parameters *parameters)
{
}
void TM1637_write_display(struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions)
{
	context->command = DATA_COMMAND_WRITE_DISPLAY;
	encode_data(context);
	start(functions);
	send_byte(DATA_COMMAND_WRITE_DISPLAY, functions);
	stop(functions);
	start(functions);
	send_byte(ADDRESS_COMMAND_C0H, functions);
	send_byte(context->reserved_data[0], functions);
	send_byte(context->reserved_data[1], functions);
	send_byte(context->reserved_data[2], functions);
	send_byte(context->reserved_data[3], functions);
	send_byte(context->reserved_data[4], functions);
	send_byte(context->reserved_data[5], functions);
	stop(functions);
}
void TM1637_display_on(uint8_t brightness, struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions)
{
	start(functions);
	send_byte(DISPLAY_COMMAND_ON | brightness, functions);
	stop(functions);
}
	
void TM1637_display_off(struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions)
{
	start(functions);
	send_byte(DISPLAY_COMMAND_OFF, functions);
	stop(functions);
}






void TM1637_send_command_data(struct _TM1637_driver_context *context, struct _TM1637_driver_functions *functions)
{
//	if(context->reserved_driver_current_status == DRIVER_IDLE)
//	{
//		context->reserved_driver_busy = true;
//		context->reserved_driver_current_status = DRIVER_SEND_COMMAND_DATA;
//		context->reserved_p_byte = context->data;
//		if(context->command == DATA_COMMAND_READ_KEYS)
//			context->reserved_driver_next_status = DRIVER_RECEIVE_DATA;
//		if(context->data_lenght > 0)
//			encode_data(context);
//		functions->p_send_command_data(context);
//	}
}

void TM1637_operation_complete(struct _TM1637_driver_context *context)
{
	context->reserved_driver_busy = false;
	context->reserved_driver_current_status = DRIVER_IDLE;	
}


void TM1637_scan_handler(struct _TM1637_driver_context *context, struct _TM1637_driver_parameters *parameters, struct _TM1637_driver_functions *functions)
{
//	switch(context->reserved_driver_current_status)
//	{
//		case DRIVER_SEND_COMMAND_DATA:
//			context->reserved_p_byte = context->data;
//
//			functions->p_send_command_data(context);
//			if(context->command == DATA_COMMAND_READ_KEYS && context->data_lenght > 0)
//				context->reserved_driver_next_status = DRIVER_RECEIVE_DATA;
//			else if (context->command == DATA_COMMAND_WRITE_DISPLAY &&  context->data_lenght > 0)
//			{
//				encode_data(context);
//				context->reserved_driver_next_status = DRIVER_DATA_HANDLE_COMPLETE;
//			}
//			else 
//				context->reserved_driver_next_status = DRIVER_DATA_HANDLE_COMPLETE;
//			break;
//	case DRIVER_SEND_DATA:
//		if(context->reserved_driver_busy == true)
//			break;		
//		context->reserved_driver_busy = true;	
//		functions->p_send_command_data(*(context->reserved_p_byte + context->reserved_data_handled));		
//		context->reserved_data_handled++;
//		if(context->reserved_data_handled > context->data_lenght - 1 )
//			context->reserved_driver_next_status = DRIVER_DATA_HANDLE_COMPLETE;
//		break;
//	case DRIVER_RECEIVE_DATA:
//		if(context->reserved_driver_busy == true)
//			break;		
//		context->reserved_driver_busy = true;	
//		functions->p_receive_byte(context->reserved_p_byte + context->reserved_data_handled);
//		context->reserved_data_handled++;
//		if(context->reserved_data_handled > context->data_lenght - 1 )
//			context->reserved_driver_next_status = DRIVER_DATA_HANDLE_COMPLETE;
//		break;
//	case DRIVER_DATA_HANDLE_COMPLETE:
//		if(context->reserved_driver_busy == true)
//			break;	
//		functions->p_operation_completed_callback();
//		context->reserved_driver_current_status = DRIVER_IDLE;
//		context->reserved_driver_next_status = DRIVER_IDLE;
//		break;
//	}

}

void encode_data(struct _TM1637_driver_context *context)
{
	uint8_t *p_source = context->data;
	uint8_t *p_dest = context->reserved_data;
	for(uint8_t encode_count = 0; encode_count < 6; encode_count++)
	{
		*p_dest = display_encode_table[*p_source];
		if(context->dp && (1 << encode_count))
			*p_dest |= DISPLAY_DP;
		
		p_dest++;
		p_source++;		
	}
}
void start (struct _TM1637_driver_functions *functions)
{
	functions->set_data_direction(0);
	functions->drive_data_pin(0);
	functions->drive_clock_pin(0);
}

void stop (struct _TM1637_driver_functions *functions)
{
	functions->drive_clock_pin(1);
	functions->drive_data_pin(1);
	functions->set_data_direction(1);
}

void send_byte(uint8_t data,  struct _TM1637_driver_functions *functions)
{
	functions->set_data_direction(0);
	for (uint16_t mask = 0x1; mask < 0x100;mask = mask << 1)
	{
		functions->drive_data_pin((data & mask) != 0);
		functions->drive_clock_pin(1);
		//__delay_us(2);
		functions->drive_clock_pin(0);
		//__delay_us(2);	

	}
	functions->set_data_direction(1);
	functions->drive_clock_pin(1);
	//__delay_us(2);
	functions->drive_clock_pin(0);
	functions->set_data_direction(0);
	functions->drive_data_pin(0);
}

