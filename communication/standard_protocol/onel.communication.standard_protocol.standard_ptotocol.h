/* 
 * File:   standardProtocol.h
 * Author: lucav
 *
 * Created on May 31, 2020, 1:44 AM
 */

#ifndef STANDARDPROTOCOL_H
#define	STANDARDPROTOCOL_H

#include <stdint.h>

#define HEADER 0x02
#define TERMINATOR 0x03


//IMPORTANTE:
//verificare che il buffer di ricezione della seriale sia capace di contenere tutto il pachetto di richiesta.


#define REQUEST_PAYLOAD_MAX_LENGTH 128ul
#define RESPONSE_PAYLOAD_MAX_LENGTH 128ul
#define EMPTY_REQUEST_LENGTH (sizeof(struct _request_packet )+1-sizeof(uint8_t*))
#define EMPTY_RESPONSE_LENGTH (sizeof(struct _response_packet)+1-sizeof(uint8_t*))

enum _control_bytes { EXECUTE = 0, EXECUTE_VERIFY = 1 } ;

enum _error_bytes { OK = 0, BUSY = 1, BAD_REQUEST = 2, FUNCTION_NOT_FOUND = 3  } ;

enum _control_bytes CONTROLBYTES;
enum _error_bytes ERRORBYTES;
uint8_t request_payload[REQUEST_PAYLOAD_MAX_LENGTH], response_payload[RESPONSE_PAYLOAD_MAX_LENGTH];


struct _request_packet {
    uint8_t header;
    uint8_t receiver_id;
    uint8_t packet_length;
    uint16_t packet_id;
    uint8_t sender_id;
    uint8_t function_id;
    uint8_t spare1;
    enum _control_bytes control_byte;
    uint8_t *payload;
    uint8_t terminator;           
};

struct _response_packet {
    uint8_t header;
    uint8_t receiver_id;
    uint8_t packet_length;
    uint16_t packet_id;
    uint8_t sender_id;
    enum _error_bytes errorByte;
    uint8_t *payload;
    uint8_t terminator;           
};

struct _request_packet  new_request = {
    .header = HEADER,
    .receiver_id = 0x00,
    .packet_length = 0x000A,
    .packet_id = 0x0000,
    .sender_id = 0x00,
    .function_id = 0x00,
    .spare1 = 0x00,
    .control_byte = EXECUTE,
    .payload = 0,
    .terminator = TERMINATOR          
};

struct _response_packet new_response = {
    .header = HEADER,
    .receiver_id = 0x00,
    .packet_length = 0x08,
    .packet_id = 0x0000,
    .sender_id = 0x00,
    .errorByte = OK,
    .payload = 0,
    .terminator = TERMINATOR,   
};

struct _request_packet  received_request = {
    .header = HEADER,
     .receiver_id = 0x00,
    .packet_length = 0x000C,
    .packet_id = 0x0000,
    .sender_id = 0x00,
    .function_id = 0x00,
    .spare1 = 0x00,
    .control_byte = EXECUTE,
    .payload = 0,
    .terminator = TERMINATOR          
};


void protocol_handler(void);
void request_timeout_tick100ms(void);
uint8_t request_payload_length(struct _request_packet  *request);
void initialize_communication_functions(
    uint8_t (*read_function)(void),
    void (*write_function)(uint8_t),
    bool (*is_rx_ready_function)(void),
    uint8_t (*get_last_status)(void));


#endif	/* STANDARDPROTOCOL_H */