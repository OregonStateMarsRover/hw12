/*BogieController.c
 *Authors: Mike Fortner, Chris Newman
 */

#include "BogieController.h"



void bogie_controller_init(void)
{
	SREG = 0b1000000; //enable global interrupts
	PMIC.CTRL = 0b00000111;  //enable high, medium, and low priority interrupts
	
	PORTA.DIR = 0x00;
	PORTB.DIR = 0x00;
	PORTC.DIR = 0b00001010;
	PORTD.DIR = 0b00111110;

	//Motor Driver USART init
	USARTD0.CTRLA = 0b00010000; //enable RX interrupt with low priority
	USARTD0.CTRLB = 0b00001000; //enable TX, leave RX disabled
	USARTD0.CTRLC = 0b00000011; //Asychronous mode, no parity, 1 stop bit, 8 bit data
	USARTD0.BAUDCTRLA = 0x0C; //BSEL = 12
	USARTD0.BAUDCTRLB = 0x00; //BSCALE = 0
	
	//Mother Board USAR init
	USARTD0.CTRLA = 0x00;
	USARTD0.CTRLB = 0b00011000; //enable TX, RX 
	USARTD0.CTRLC = 0b00000011; //Asychronous mode, no parity, 1 stop bit, 8 bit data
	USARTD0.BAUDCTRLA = 0x0C; //BSEL = 12
	USARTD0.BAUDCTRLB = 0x00; //BSCALE = 0
	
	PORTD.OUTSET = 0b00010000;
}

#pragma vector=USARTC0_RXC_vect
__interrupt void message_handler(void)
{
	//handle message from mainboard
}

int main(void)
{
	int8_t speed = 0;
	
	bogie_controller_init();
	
	while(1)
	{
		PORTD.OUTTGL = 0b00110000;
		_delay_ms(18);
		drive_set(speed);
		if (speed == 0x7F) speed = -0x7F;
		else speed = speed+1;
	}
}