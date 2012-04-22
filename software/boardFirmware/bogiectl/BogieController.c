/*BogieController.c
 *Authors: Mike Fortner, Chris Newman
 */

#include "BogieController.h"



void bogie_controller_init(void)
{
	PORTA.DIR = 0x00;
	PORTB.DIR = 0x00;
	PORTC.DIR = 0b00001010;
	PORTD.DIR = 0b00111110;

	USARTD0.CTRLA = 0x00;
	USARTD0.CTRLB = 0b00001000; //enable TX, leave RX disabled
	USARTD0.CTRLC = 0b00000011; //Asychronous mode, no parity, 1 stop bit, 8 bit data
	USARTD0.BAUDCTRLA = 0x0B; //BSEL = 12
	USARTD0.BAUDCTRLB = 0x00; //BSCALE = 0
}

int main(void)
{
	int8_t speed = 0;
	while(1)
	{
		drive_set(speed);
		if (speed == 0x7F) speed = -0x7F;
		else speed = speed+1;
	}
}