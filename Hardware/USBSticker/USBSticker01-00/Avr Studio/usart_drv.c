#include <avr/io.h>
#include "usart_drv.h"
void USART_Init( unsigned int baud )
{
/* Set baud rate */
UBRRHn = (unsigned char)(baud>>8);
UBRRLn = (unsigned char)baud;
/* Enable receiver and transmitter */
UCSRnB = (1<<RXENn)|(1<<TXENn);
/* Set frame format: 8data, 2stop bit */
UCSRnC = (1<<USBSn)|(3<<UCSZn0);
}
void USART_Transmit( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSRnA & (1<<UDREn)) )
;
/* Put data into buffer, sends the data */
UDRn = data;
}
unsigned char USART_Receive( void )
{
/* Wait for data to be received */
while ( !(UCSRnA & (1<<RXCn)) )
;
/* Get and return received data from buffer */
return UDRn;
}
void USART_Flush( void )
{
unsigned char dummy;
while ( UCSRnA & (1<<RXCn) ) dummy = UDRn;
}
