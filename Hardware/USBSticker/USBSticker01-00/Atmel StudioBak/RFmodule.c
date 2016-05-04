
#include <avr/io.h>
#include "spi_drv.h"
#include "RFmodule.h"

/***************************************************************************
RFM01_init - RFM01 initialise
***************************************************************************/
void RFM01_init(void)
{
	uint16_t Fcarr;

	/* Configuration Setting Command: 433MHz band, 16pF, Bandwidth 400kHz Disable CLK output */
	WriteCMD(0x8000 | (RF_433MHz<<11) | (0x01<<8) | (0x0F<<4) | (Bandwidth400kHz<<1) | 0x01);

	/* Frequency Setting Command: Fcarrier = 439.00MHz */
	Fcarr = Fc(439.00);
	WriteCMD(0xA000|Fcarr);
	
	/* Data Rate Command: BR 114.943Kbps */
	WriteCMD(0xC800|BR114_943kbs);	

	/* AFC Command: eep offset when VDI hi; select range limit +15/-16; */
	WriteCMD(0xC69B);

	/* Data Filter Command:  Auto-lock; Digital filter */
	WriteCMD(0xC42A);

	/* Reset Mode Command: Disable RESET mode */
	WriteCMD(0xDA01);
}



/************************************
Write command
*************************************/
void WriteCMD(uint16_t CMD)
{	
	uint8_t DataTX[2];
	uint8_t DataRX[2];

	DataTX[0] = CMD>>8;
	DataTX[1] = CMD&0xFF;

	SPI_SendBuff(DataTX,2,DataRX);
}



uint16_t RMFM12_ReadFIFO(void)
{
	uint8_t DataRX[2];
	uint16_t DATA;

	/* CS Low */
	PORTCS &= ~(1<<CS);

	/* Skip Status Bits */
	SPI_Transmit(0x00);
	SPI_Transmit(0x00);


	/* Read Data */
	DataRX[0]=SPI_Transmit(0x00);
	DataRX[1]=SPI_Transmit(0x00);
	DATA = (DataRX[0]<<8) | DataRX[1];

	/* CS Higth */
	PORTCS |= (1<<CS);

	return (DATA);
}





