
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPIDRIVER_H
#define __SPIDRIVER_H


/***************************************************************************
Definitions
***************************************************************************/
#define PORTCS 		PORTB
#define CS 			PB0


/***************************************************************************
Functions prototypes
***************************************************************************/
void Init_SPI(void);
uint8_t SPI_Transmit(uint8_t Data);
uint8_t SPI_MasterRead(void);
void SPI_SendBuff(uint8_t *buff,uint8_t len,uint8_t *rbuff);


#endif
