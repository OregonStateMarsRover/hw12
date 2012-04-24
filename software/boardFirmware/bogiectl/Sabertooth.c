/* Sabertooth.c
*  Authors: Mike Fortner, Chris Newman
*/

#include "Sabertooth.h"

#define MIN -127
#define MAX 127

#define SABERTOOTH_ADDRESS 130
#define DRIVE_FORWARD_CMD 0
#define DRIVE_REVERSE_CMD 1
#define TIMEOUT_CMD 14

USART_data_t *USART_sabertooth;

void sabertooth_init(USART_data_t *USART_data)
{
	USART_sabertooth = USART_data;
	
	uint8_t address = SABERTOOTH_ADDRESS;
	uint8_t opcode = TIMEOUT_CMD;
	uint8_t timeout = 10; //10 * 100ms = 1s

	send_command(address, opcode, timeout);
}

void send_command(uint8_t address, uint8_t opcode, uint8_t data)
{
	USART_TXBuffer_PutByte(USART_sabertooth, address);
	USART_TXBuffer_PutByte(USART_sabertooth, opcode);
	USART_TXBuffer_PutByte(USART_sabertooth, data);
	USART_TXBuffer_PutByte(USART_sabertooth, (address + opcode + data) & 0x7F);
}

void drive_set(int8_t speed)
{
	uint8_t address = SABERTOOTH_ADDRESS;
	uint8_t opcode;

	if (speed < MIN) speed = MIN;
	if (speed > MAX) speed = MAX;

	if (speed >= 0)
	{
		opcode = DRIVE_FORWARD_CMD;
	}
	else
	{
		speed = -speed;  //flip speed and use 'reverse' command
		opcode = DRIVE_REVERSE_CMD;
	}

	send_command(address, opcode, speed);
}
