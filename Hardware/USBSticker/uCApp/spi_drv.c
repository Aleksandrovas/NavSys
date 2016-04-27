#include <avr/io.h>
#include "spi_drv.h"
void Init_SPI()
{

	/* Set MOSI and SCK, SS output, all others input */
	DDRB |= (1<<PB3)|(1<<PB1)|(1<<PB2)|(1<<PB0);
	PORTB|=(1<<SS);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|	//enable spi
		   (1<<MSTR)|	//master mode
		   (1<<CPOL)|	//sck idle high
		   (1<<SPR0);	//fosc/16 (1000000/16 hz)

}

char SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF))) ;
	return(SPDR);
}

char SPI_MasterRead(void)
{
	/* Start transmission */
	SPDR = 0;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF))) ;
	return(SPDR);

}


void SPI_sendbuf(unsigned char *buff,unsigned int len,unsigned char *rbuff)
{
		unsigned int i;
		PORTB&=~(1<<SS);
		for (i=0;i<len;i++)
			rbuff[i]=SPI_MasterTransmit(buff[i]);
		PORTB|=(1<<SS);
	
}
