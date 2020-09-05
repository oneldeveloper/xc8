#include <string.h>
#include <stdbool.h>
#include "onel.communication.standard_protocol.standard_ptotocol.h"


enum incoming_state_machine { RX_HEADER_WAITING,
	RX_RECEIVERID_WAITING, 
	RX_PACKET_LENGTH_WAITING, 
	RX_PACKETID_WAITING, 
	RX_SENDERID_WAITING,
	RX_FUNCTIONID_WAITING,
	RX_SPARE1_WAITING,
	RX_CONTROLBYTE_WAITING,
	RX_PAYLOAD_WAITING,
	RX_TERMINATOR_WAITING,
	RX_REQUEST_HANDLE};


volatile bool busy;
volatile bool valid_request;
volatile uint8_t rx_timeout_counter;
volatile uint8_t rx_timeout_time = 30;

volatile uint8_t rx_state_machine = RX_HEADER_WAITING;
volatile uint8_t rx_count = 0;
volatile uint8_t *p_rx_payload;

void (*p_write_function)(uint8_t byte);
uint8_t (*p_read_function)(void);
bool (*p_is_rx_ready_function)(void);
uint8_t (*p_get_last_status_function)(void);

extern uint8_t device_id;
extern struct _response_packet request_handler(struct _request_packet  request);



void initialize_communication_functions(
    uint8_t (*read_function)(void),
    void (*write_function)(uint8_t),
    bool (*is_rx_ready_function)(void),
    uint8_t (*get_last_status_function)(void))
{
	p_read_function = read_function;
	p_write_function = write_function;
	p_is_rx_ready_function = is_rx_ready_function;
	p_get_last_status_function = get_last_status_function;
}



void send_response(struct _response_packet response)
{
	p_write_function(response.header);
	p_write_function(response.receiver_id);
	p_write_function(response.packet_length);
	
	p_write_function((uint8_t)(response.packet_id / 0x100));
	p_write_function((uint8_t)response.packet_id);
	
	p_write_function(response.sender_id);
	p_write_function(response.errorByte);

	uint8_t payload_length = response.packet_length - EMPTY_RESPONSE_LENGTH;
	uint8_t *p_response_payload = response_payload;
	while (payload_length > 0)
	{
		p_write_function(*p_response_payload++);
		payload_length--;	
	}
	p_write_function(response.terminator);
	busy = false;
}

void error_response (uint8_t error)
{
	struct _response_packet response = new_response;
	response.packet_id = received_request.packet_id;
	response.receiver_id = received_request.sender_id;
	response.sender_id = device_id;
	response.errorByte = error;
	send_response(response);				
}




//check incoming packet on protocol rules
void incoming_packet_manager(void)
{		
	while(p_is_rx_ready_function() == true)
	{
		uint8_t rx_data = p_read_function();
		uint8_t rx_status = p_get_last_status_function();
        if(rx_status)
		{
			rx_state_machine = RX_HEADER_WAITING;
			continue;
		}

		switch (rx_state_machine)
		{
			case RX_HEADER_WAITING:
				if (rx_data == HEADER)
				{
					rx_timeout_counter = rx_timeout_time;
					rx_state_machine = RX_RECEIVERID_WAITING;
				}
				break;
			case RX_RECEIVERID_WAITING:
				if (rx_data != device_id)
				{
					rx_state_machine = RX_HEADER_WAITING;
				}
				else
					rx_state_machine = RX_PACKET_LENGTH_WAITING;
				break;
			case RX_PACKET_LENGTH_WAITING:
				received_request.packet_length = rx_data;
				rx_count = 0;
				rx_state_machine = RX_PACKETID_WAITING;
				break;
			case RX_PACKETID_WAITING:
				if (rx_count == 0)					
					received_request.packet_id = rx_data * 0x100;
				if (rx_count == 1)
					received_request.packet_id += rx_data;
				rx_count++;
				if (rx_count > 1)
					rx_state_machine = RX_SENDERID_WAITING;
				break;
			case RX_SENDERID_WAITING:
				received_request.sender_id = rx_data;
				rx_state_machine = RX_FUNCTIONID_WAITING;
				break;
			case RX_FUNCTIONID_WAITING:
				received_request.function_id = rx_data;
				rx_state_machine = RX_SPARE1_WAITING;
				break;
			case RX_SPARE1_WAITING:
				received_request.spare1 = rx_data;
				rx_state_machine = RX_CONTROLBYTE_WAITING;
				break;
			case RX_CONTROLBYTE_WAITING:
				received_request.control_byte = rx_data;
				rx_count = received_request.packet_length - EMPTY_REQUEST_LENGTH;
				if (rx_count > REQUEST_PAYLOAD_MAX_LENGTH)
				{
					error_response(BAD_REQUEST);
					rx_state_machine = RX_HEADER_WAITING;
					break;
				}
				if (rx_count > 0)
				{
					p_rx_payload = request_payload;
					rx_state_machine = RX_PAYLOAD_WAITING;					
				}
				else
					rx_state_machine = RX_TERMINATOR_WAITING;
				break;
			case RX_PAYLOAD_WAITING:
				if (rx_count > 0)
				{
					*p_rx_payload++ = rx_data;
					rx_count--;
					if (rx_count == 0)
						rx_state_machine = RX_TERMINATOR_WAITING;
				}
				break;
			case RX_TERMINATOR_WAITING:
				if (rx_data == TERMINATOR)
				{
					if (busy == true)
						error_response(BUSY);
					else
						valid_request = true;
				}
				else
					error_response(BAD_REQUEST);
				
				rx_state_machine = RX_HEADER_WAITING;
				break;
			default:
					rx_state_machine = RX_HEADER_WAITING;
					break;
		}
	}
}

uint8_t request_payload_length(struct _request_packet  *request)
{
	return request->packet_length - EMPTY_REQUEST_LENGTH; 
}

void request_timeout_set100ms(uint8_t timeoutX100ms)
{
	
}

void request_timeout_tick100ms(void)
{
	if (rx_state_machine == RX_HEADER_WAITING)
		return;
	if (rx_timeout_counter > 0)
		rx_timeout_counter--;
	else
	{
		rx_state_machine = RX_HEADER_WAITING;
	}
}

void protocol_handler(void)
{
	incoming_packet_manager();
	if (valid_request)
	{
		valid_request = false;
		busy = true;
		struct _response_packet response = request_handler(received_request);
		send_response(response);
	}
}
