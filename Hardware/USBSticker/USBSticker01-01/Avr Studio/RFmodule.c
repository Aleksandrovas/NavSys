
#include <avr/io.h>
#include "spi_drv.h"
#include "RFmodule.h"


/***************************************************************************
RFM12_init - RFM12 initialize
***************************************************************************/
void RF12_init(void)
{
	uint16_t Fcarr;

	/* Low Battery Detector and Micro controller Clock Divider Command: 10MHz output*/
	WriteCMD(0xC000 | (CLK10MHz<<5));

	/* Configuration Setting Command  */
	WriteCMD(0x80DF);	// 433MHz Band; Enable TX registere; Enable RX FIFO buffer, 16pF
	
	/* Frequency Setting Command: F carrier = 432.00MHz */
	Fcarr = Fc(438.00);
	WriteCMD(0xA000|Fcarr);

	/* Data Rate Command: 114.943kbps */
	WriteCMD(0xC600|BR114_943kbs);

	/* Receiver Control Command */
	WriteCMD(0x9420|Gain_0dB);	// VDI, FAST, Bandwidth 400kHz, LNA gain 0dBm, -103dBm

	/* Data Filter Command */
	WriteCMD(0xC2AC);	// Auto-lock; Digital filter;
	
	/* FIFO and Reset Mode Command */
	WriteCMD(0xCAF3);	// FIFO interrupt level: 16bits; FIFO fill start condition: Sync-word; Enable FIFO fill; dr - set to "1"
	
	/* AFC Command */
	WriteCMD(0xC49B);	// AFC setting: Keep offset when VDI hi; select range limit +15/-16; Enable AFC funcition; st,oe - set to "1"
	
	/* TX Configuration Control Command */
	WriteCMD(0x98D0|P_0dBm);	// 210kHz deviation; MAX OUT

	/* Power Management Command */
	RF_Iddle;
}



/***************************************************************************
Write command
***************************************************************************/
void WriteCMD(uint16_t CMD)
{	
	uint8_t DataTX[2];
	uint8_t DataRX[2];

	DataTX[0] = CMD>>8;
	DataTX[1] = CMD&0xFF;

	SPI_SendBuff(DataTX,2,DataRX);
}


/***************************************************************************
Write_FSK_byte - Write FSK 8bit data
***************************************************************************/
void Write_FSK_byte(uint8_t data)
{
	uint16_t CMD = 0xB800;
	//uint8_t RGIT = 0;
	
	/* Wait for previous data latch 
	do{
		SCK_LOW;
		nSEL_LOW;
		SDI_LOW;
		SCK_HI;
		
		if(SDO_PIN)
			RGIT=1;		

		SCK_LOW;
		SDI_HI;
		nSEL_HI;
		}while(!RGIT);*/

	/* Transmitter Register Write Command */
	WriteCMD(CMD|data);
}


/***************************************************************************
RF12_Send - Send data 
***************************************************************************/
void RF12_Send(uint16_t Data)
{
	/*uint8_t parity;

	Data = Data | (RefPointNr<<12);
	parity = GetParity(Data)&0x01;
	Data = Data | (parity<<15);

	// Send Preamble and Send sync word 
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0x2D);		// Send sync word
	Write_FSK_byte(0xD4);		// Send sync word

	// Send Data
	Write_FSK_byte(Data>>8);
	Write_FSK_byte(Data&0xFF);
	*/
}


/***************************************************************************
RF12_ReadFIFO - Read FIFO
***************************************************************************/
uint16_t RF12_ReadFIFO(void)
{
	uint16_t Data;
	uint8_t temp[2];

	/* CS Low */
	PORTCS &= ~(1<<CS);

	/* Skip Status Bits */
	SPI_Transmit(0x00);
	SPI_Transmit(0x00);

	/* Read Fifo Data */
	temp[0] = SPI_Transmit(0x00);
	temp[1] = SPI_Transmit(0x00);

	Data = (temp[0]<<8) | (temp[1]);

	/* CS Higth */
	PORTCS |= (1<<CS);

	return (Data);
}


/***************************************************************************
PrepareRFpacket - 
***************************************************************************/
void PrepareRFpacket(uint8_t* RFpacket, uint16_t Data, uint8_t RefNr)
{
	uint8_t MSB;
	uint8_t LSB;
	uint8_t ChkSum;
	
	RefNr = RefNr&0x0F;	// max number of Ref points is 16
	MSB = Data>>8;
	LSB = Data&0xFF;
	ChkSum = MSB + LSB + RefNr;
	ChkSum = ChkSum&0x0F;		// use only 4 LSB bits of checksum

	/* Check sum just 4 bits */
	RFpacket[0] = (ChkSum<<4) | RefNr;
	RFpacket[1] = Data>>8;
	RFpacket[2] = Data&0xFF;
}


/***************************************************************************
GetParity - 
***************************************************************************/
uint8_t GetParity(uint16_t x) 
{
	uint8_t parity=0;
    
	while (x > 0) 
	{
       parity = (parity + (x & 1)) % 2;
       x >>= 1;
    }
	return parity;
}
