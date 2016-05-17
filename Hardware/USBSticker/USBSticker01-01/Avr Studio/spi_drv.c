
#include <avr/io.h>
#include "spi_drv.h"


void Init_SPI(void)
{
	/* Set MOSI and SCK, SS output, all others input */
	DDRB |= (1<<PB1)|(1<<PB2)|(1<<PB0);
	DDRB &= ~(1<<PB3);

	/* Chip Select */
	PORTCS|= (1<<CS);

	/* Enable SPI, Master, set clock rate fosc/2*/
	SPCR = (1<<SPE) | (1<<MSTR) | (SPI2X<<1);
}


uint8_t SPI_Transmit(uint8_t Data)
{
	/* Start transmission */
	SPDR = Data;

	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	return(SPDR);
}


uint8_t SPI_MasterRead(void)
{
	/* Start transmission */
	SPDR = 0;

	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	return(SPDR);
}


void SPI_SendBuff(uint8_t *buff,uint8_t len,uint8_t *rbuff)
{
	uint8_t i=0;

	/* CS Low */
	PORTCS &= ~(1<<CS);

	for (i=0;i<len;i++)
		rbuff[i]=SPI_Transmit(buff[i]);

	/* CS Higth */
	PORTCS |= (1<<CS);
}

