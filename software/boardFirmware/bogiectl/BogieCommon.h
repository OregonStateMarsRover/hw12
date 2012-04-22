/*BogieCommon.h
 *Authors: Mike Fortner, Chris Newman
 */

#include <avr/io.h>
#include <stdint.h>

void usart_send_byte(USART_t usart, uint8_t data);