/* Sabertooth.c
*  Authors: Mike Fortner, Chris Newman
*/

#include "Sabertooth.h"

#define MIN -127
#define MAX 127

#define SABERTOOTH_ADDRESS 130
#define DRIVE_FORWARD_CMD 0
#define DRIVE_REVERSE_CMD 1
#define ACTUATOR_FORWARD_CMD 3
#define ACTUATOR_REVERSE_CMD 4
#define TIMEOUT_CMD 14

USART_data_t *USART_sabertooth;

void sabertooth_init(USART_data_t *USART_data)
{
	USART_sabertooth = USART_data;
	uint8_t opcode = TIMEOUT_CMD;
	uint8_t timeout = 10; //10 * 100ms = 1s

	send_command(opcode, timeout);
}

void send_command(uint8_t opcode, uint8_t data)
{
	uint8_t address = SABERTOOTH_ADDRESS;
	USART_TXBuffer_PutByte(USART_sabertooth, address);
	USART_TXBuffer_PutByte(USART_sabertooth, opcode);
	USART_TXBuffer_PutByte(USART_sabertooth, data);
	USART_TXBuffer_PutByte(USART_sabertooth, (address + opcode + data) & 0x7F);
}

void motor_set(int8_t speed, uint8_t forward_cmd, uint8_t reverse_cmd)
{
	uint8_t opcode;

	if (speed < MIN) speed = MIN;
	if (speed > MAX) speed = MAX;

	if (speed >= 0)
	{
		opcode = forward_cmd;
	}
	else
	{
		speed = -speed;  //flip speed and use 'reverse' command
		opcode = reverse_cmd;
	}

	send_command(opcode, speed);
}

void actuator_set(int8_t speed)
{
	motor_set(speed, ACTUATOR_FORWARD_CMD, ACTUATOR_REVERSE_CMD);
}

void drive_set(int8_t speed)
{
	motor_set(speed, DRIVE_FORWARD_CMD, DRIVE_REVERSE_CMD);
}