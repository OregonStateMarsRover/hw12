/*BogieController.c
 *Authors: Mike Fortner, Chris Newman
 */

#include "BogieController.h"


//global vars
USART USART_motor;
USART USART_mainboard;

uint8_t desired_speed = 0;
uint8_t desired_angle = 0;

PIDobject act_pid;


void bogie_controller_init(void)
{
	USART_InitPortStructs();
	
	/***set I/O port directions***/
	PORTA.DIR = 0x00;
	PORTB.DIR = 0x00;
	PORTC.DIR = PIN1_bm | PIN3_bm;
	PORTD.DIR = PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm;

	/***Motor Driver USART init***/
		
	
	USART_Open(&bogie_controller.motor_port, 2, USART_BAUD_9600, 10, 10, false, true);

	//enable DRE interrupt with low priority
// 	USART_InterruptDriver_Initialize(&USART_motor, &USARTD0, USART_DREINTLVL_LO_gc); 
// 
// 	/* 8 Data bits, No Parity, 1 Stop bit. */
// 	USART_Format_Set(USART_motor.usart, USART_CHSIZE_8BIT_gc,
//                      USART_PMODE_DISABLED_gc, false);
// 
// 	/* Set Baudrate to 9600 bps:
// 	 * Use the default I/O clock frequency that is 2 MHz.
// 	 * Do not use the baudrate scale factor
// 	 *
// 	 * Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1)
// 	 *                 = 12
// 	 */
// 	USART_Baudrate_Set(&USARTD0, 12 , 0);
// 
// 	/* Enable TX. */
// 	USART_Tx_Enable(USART_motor.usart);

	/***Mainboard USART init***/
	USART_Open(&bogie_controller.mainboard_port, 0, USART_BAUD_38400, 10, 10, false, true);
	CommInterfaceInit(&bogie_controller.mainboard_inf, &bogie_controller.mainboard_port);
	
	PacketQueueInit2(&bogie_controller.pktQueue, 6, 20, bogie_controller.queuedPackets, bogie_controller.queuedData);
// 	USART_InterruptDriver_Initialize(&USART_mainboard, &USARTC0, USART_DREINTLVL_LO_gc); 
// 
// 	/* 8 Data bits, No Parity, 1 Stop bit. */
// 	USART_Format_Set(USART_mainboard.usart, USART_CHSIZE_8BIT_gc,
//                      USART_PMODE_DISABLED_gc, false);
// 					 
// 	/* Enable RXC interrupt. */
// 	USART_RxdInterruptLevel_Set(USART_mainboard.usart, USART_RXCINTLVL_LO_gc);
// 
// 	/* Set Baudrate to 9600 bps:
// 	 * Use the default I/O clock frequency that is 2 MHz.
// 	 * Do not use the baudrate scale factor
// 	 *
// 	 * Baudrate select = (1/(16*(((I/O clock frequency)/Baudrate)-1)
// 	 *                 = 12
// 	 */
// 	USART_Baudrate_Set(&USARTC0, 12 , 0);
// 
// 	/* Enable RX and TX. */
// 	USART_Rx_Enable(USART_mainboard.usart);
// 	USART_Tx_Enable(USART_mainboard.usart	
	/*** Global Interrupt init***/	

	/* Enable PMIC interrupt level low. */
	PMIC.CTRL |= PMIC_LOLVLEX_bm;

	/* Enable global interrupts. */

	
	
	/*** Initialize Sabertooth Motor Driver ***/
	
	sabertooth_init(&USART_motor);
	
	/*** Initialize Quadrature Decoder for Actuator encoder***/
	
	QDEC_Total_Setup(&PORTC,                    /*PORT_t * qPort*/
	                 6,                         /*uint8_t qPin*/
	                 false,                     /*bool invIO*/
	                 0,                         /*uint8_t qEvMux*/
	                 EVSYS_CHMUX_PORTC_PIN6_gc, /*EVSYS_CHMUX_t qPinInput*/
	                 false,                     /*bool useIndex*/
	                 EVSYS_QDIRM_00_gc,         /*EVSYS_QDIRM_t qIndexState*/
	                 &TCC0,                     /*TC0_t * qTimer*/
	                 TC_EVSEL_CH0_gc,           /*TC_EVSEL_t qEventChannel*/
	                 ACTUATOR_QUADRATURE_LINECOUNT);   /*uint8_t lineCount*/
	
	/*** Initialize Counter for Drive encoder***/
	
	PORTC.DIRCLR = PIN4_bm;				  //set PC4/Pin 14 to input
	PORTC.PIN4CTRL |= PORT_ISC_RISING_gc;  //set PC4/Pin 14 to trigger events on rising edges
	
	EVSYS.CH2MUX = EVSYS_CHMUX_PORTC_PIN4_gc;  //set PC4/Pin 14 to input for event channel 2
	
	TC1_t *motor_counter = &TCC1;
	TC1_ConfigClockSource( motor_counter, TC_CLKSEL_EVCH2_gc );  //set TCC1 to count events on channel 2
	
	/*** Initialize Timer for PID loop***/
		

	TC0_t *loop_timer = &TCD0;
	TC_SetPeriod( loop_timer, 195U); //set period to (2000000/1024)(ticks/sec)/10(loops/sec) = 195 ticks/loop
	TC0_ConfigClockSource( loop_timer,  TC_CLKSEL_DIV1024_gc);  //set TCD0 to count the system clock. frequency should be 2,000,000 ticks/sec
	TC0_SetOverflowIntLevel( loop_timer, TC_OVFINTLVL_LO_gc);  //set TCD0 to trigger an interrupt when every overflow (100ms)


	/*** initialize min and max for actuator encoder ***/
	
	sei();
}


ISR(TCD0_OVF_vect)
{
	PORTD.OUTTGL = 0b00110000;
	drive_set(desired_speed); 
	actuator_set(pid(&act_pid, desired_angle, get_actuator_pos()));
}

void parse_command(CommPacket *pkt)
{
	drive_set((int8_t)pkt->data[0]);
}

int main(void)
{
	bogie_controller_init();
	PORTD.OUTSET = 0b00010000;

	act_pid.p= 1;
	while(1)
	{
		//wait for communication
		if(CommRXPacketsAvailable(&bogie_controller.mainboard_inf))
		{	
			CommPacket pkt;
			CommGetPacket(&bogie_controller.mainboard_inf, &pkt, 10);
			parse_command(&pkt);
		}
	}
}