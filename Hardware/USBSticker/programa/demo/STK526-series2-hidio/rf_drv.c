#include <avr/io.h>
#include "rf_drv.h"

/************************************
PORTS initialise
*************************************/
void RF_PORT_int(void)
{
	RF_PORT |= (1<<nSEL);
	RF_DDR |= (1<<SDI)	| (1<<SCK) | (1<<nSEL);  
//	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//|(1<<SPR0);
}

void SPI_MasterInit(void)
{

}
unsigned char SPI_MasterTransmit(char cData)
{
	unsigned int j=0;
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF))&&(j++<10000))
	;
	return SPDR;
}


/************************************
Write command
*************************************/
void WriteCMD(unsigned int CMD)
{
	int n=0;
	for (n=0;n<50;n++);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//|(1<<SPR0);
	nSEL_LOW;
	SPI_MasterTransmit(CMD>>8);
	SPI_MasterTransmit(CMD);
	nSEL_HI;
	for (n=0;n<5;n++);
	SPCR=0;
	for (n=0;n<50;n++);
}



static unsigned char sdrssi, sgain;

void rf01_setbandwidth(unsigned char bandwidth)
{
	WriteCMD(0x8970|((bandwidth&7)<<1));
}

void rf01_setreceiver(unsigned char gain, unsigned char drssi)
{
	sdrssi=drssi;
	sgain=gain;
}

void rf01_setfreq(unsigned short freq)
{	if (freq<96)				// 430,2400MHz
		freq=96;
	else if (freq>3903)			// 439,7575MHz
		freq=3903;
	WriteCMD(0xA000|freq);
}

void rf01_setbaud(unsigned short baud)
{
	if (baud<336)
		return;
	if (baud<5400)				// Baudrate= 344827,58621/(R+1)/(1+nSEL*7)
		WriteCMD(0xC880|((43104/baud)-1));
	else
		WriteCMD(0xC800|((344828UL/baud)-1));

	WriteCMD(0xC806);
}


/************************************
Read FIFO
*************************************/
unsigned long RMFM01_readFIFO(void)
{
/*
0
128
132
*/


	unsigned long Result=0;
	nSEL_LOW;
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	Result=SPI_MasterTransmit(0);
	Result<<=8;
	Result+=SPI_MasterTransmit(0);
	Result<<=8;
	Result+=SPI_MasterTransmit(0);
	Result<<=8;
	Result+=SPI_MasterTransmit(0);
	SPCR = 0;
	nSEL_HI;
	WriteCMD(0xCEF8);	// Reset FIFO
	WriteCMD(0xCEFB);

	//WriteCMD(0xCE87);
	return Result;//(Result);
}


/************************************
RFM02 initialise
*************************************/
void RFM01_init(void)
{
	WriteCMD(0xC248); //3.0V

	WriteCMD(0x8983);
	WriteCMD(0xAE10);//frequency
	//WriteCMD(0xAE10);//frequency
	WriteCMD(0xC829);
	WriteCMD(0xC65B);
	//WriteCMD(0xC69B);

	WriteCMD(0xC42C);
	WriteCMD(0xC080);
	//WriteCMD(0xC080);

	WriteCMD(0xCEF8);
	//WriteCMD(0xCE87);
	WriteCMD(0xCEFB);

	WriteCMD(0xDA01);
	WriteCMD(0xC081);
	//WriteCMD(0xC081);
}
