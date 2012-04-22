/*BogieCommon.c
 *Authors: Mike Fortner, Chris Newman
 */

#include "BogieCommon.h"

void usart_send_byte(USART_t usart, uint8_t data)
{
	while((usart.STATUS & 0b00100000) == 0){}
	usart.DATA = data;
}