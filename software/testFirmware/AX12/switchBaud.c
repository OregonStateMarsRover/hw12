#define F_CPU 16000000

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>


int main(void) {



	//Set up the USART for 1MBit
	CLKPR &= 1<<CLKPCE;
	DDRD = 0xFF;
	//UCSR1A |= 1<<U2X1;
	UCSR1C |= 1<<UCSZ11 || 1<<UCSZ10;
	UBRR1H = (unsigned char) (0>>8);
	UBRR1L = (unsigned char) 0;
	UCSR1B |= 1<<TXEN1;
	
	while(!(UCSR1A && 1<<UDRE1));
	UDR1 = 0xFF;
	while(!(UCSR1A && 1<<UDRE1));
	UDR1 = 0xFF;
	while(!(UCSR1A && 1<<UDRE1));
	UDR1 = 0x00;
	while(!(UCSR1A && 1<<UDRE1));
	UDR1 = 0x04;
	while(!(UCSR1A && 1<<UDRE1));
	UDR1 = 0x03;
	while(!(UCSR1A && 1<<UDRE1));
	UDR1 = 0x04;
	while(!(UCSR1A && 1<<UDRE1));
	UDR1 = 0x22;
	while(!(UCSR1A && 1<<UDRE1));
	UDR1 = 0xD2;

return 0;
}