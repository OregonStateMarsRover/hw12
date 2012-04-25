/*BogieController.c
 *Authors: Mike Fortner, Chris Newman
 */

#include "BogieController.h"

USART_data_t USART_motor;
USART_data_t USART_mainboard;

void bogie_controller_init(void)
{
	
	/***set I/O port directions***/
	PORTA.DIR = 0x00;
	PORTB.DIR = 0x00;
	PORTC.DIR = PIN1_bm | PIN3_bm;
	PORTD.DIR = PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm;

	/***Motor Driver USART init***/
		
	//enable DRE interrupt with low priority
	
	USART_InterruptDriver_Initialize(&USART_motor, &USARTD0, USART_DREINTLVL_LO_gc); 

	/* 8 Data bits, No Parity, 1 Stop bit. */
	USART_Format_Set(USART_motor.usart, USART_CHSIZE_8BIT_gc,
                     USART_PMODE_DISABLED_gc, false);

	/* Set Baudrate to 9600 bps:
	 * Use the default I/O clock frequency that is 2 MHz.
	 * Do not use the baudrate scale factor
	 *
	 * Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1)
	 *                 = 12
	 */
	USART_Baudrate_Set(&USARTD0, 12 , 0);

	/* Enable TX. */
	USART_Tx_Enable(USART_motor.usart);

	/***Mainboard USART init***/
	
	USART_InterruptDriver_Initialize(&USART_mainboard, &USARTC0, USART_DREINTLVL_LO_gc); 

	/* 8 Data bits, No Parity, 1 Stop bit. */
	USART_Format_Set(USART_mainboard.usart, USART_CHSIZE_8BIT_gc,
                     USART_PMODE_DISABLED_gc, false);
					 
	/* Enable RXC interrupt. */
	USART_RxdInterruptLevel_Set(USART_mainboard.usart, USART_RXCINTLVL_LO_gc);

	/* Set Baudrate to 9600 bps:
	 * Use the default I/O clock frequency that is 2 MHz.
	 * Do not use the baudrate scale factor
	 *
	 * Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1)
	 *                 = 12
	 */
	USART_Baudrate_Set(&USARTC0, 12 , 0);

	/* Enable RX and TX. */
	USART_Rx_Enable(USART_mainboard.usart);
	USART_Tx_Enable(USART_mainboard.usart);
	
	/*** Global Interrupt init***/	

	/* Enable PMIC interrupt level low. */
	PMIC.CTRL |= PMIC_LOLVLEX_bm;

	/* Enable global interrupts. */
	sei();
	
	
	/*** Initialize Sabertooth Motor Driver ***/
	
	sabertooth_init(&USART_motor);
}

ISR(USARTC0_RXC_vect)
{
	USART_RXComplete(&USART_mainboard);
}


ISR(USARTC0_DRE_vect)
{
	USART_DataRegEmpty(&USART_mainboard);
}

ISR(USARTD0_DRE_vect)
{
	USART_DataRegEmpty(&USART_motor);
}


int main(void)
{
	bogie_controller_init();
	PORTD.OUTSET = 0b00010000;
	while(1)
	{
		_delay_ms(500);
		PORTD.OUTTGL = 0b00110000;
		drive_set(0);
	}
}