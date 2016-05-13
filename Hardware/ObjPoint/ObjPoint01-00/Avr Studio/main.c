
#define F_CPU 10000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"


int main(void)
{
	/* Wait for Vcc stabilize */
	_delay_ms(100);

	/* Initialize Peripheral */
	PORTS_init();
	RF02_init();

	while(1)
	{
		_delay_ms(PRF_ms);

		RF_TXmode;
		_delay_us(300);

		LED1_ON;
		LED2_ON;

		RF02_Send(StartCode);
		Send_UG(20);

		_delay_us(300);
		RF_Iddle;

		LED1_OFF;
		LED2_OFF;
	}
}





/***************************************************************************
****************************************************************************
Functions
***************************************************************************/


/***************************************************************************
RF02_init - RFM02 initialize
***************************************************************************/
void RF02_init(void)
{
	uint16_t Fcarr;

	/* Configuration Setting Command: 433MHz band, +/-210kHz, CLK 10MHz, 16pF	*/
	WriteCMD16b(0x8000 | (RF_433MHz<<11) | (CLK10MHz<<8) | (0x0F<<4) | FreqDev210kHz);	
	_delay_ms(100);
	WriteCMD16b(0x8000 | (RF_433MHz<<11) | (CLK10MHz<<8) | (0x0F<<4) | FreqDev210kHz);	
	_delay_ms(100);

	/* Frequency Setting Command: Fcarrier = 439.00MHz */
	Fcarr = Fc(432.00);
	WriteCMD16b(0xA000|Fcarr);
	
	/* PLL Setting Command */
	WriteCMD16b(0xD282);
	
	/* Data Rate Command: BR 114.943Kbps */
	WriteCMD16b(0xC800|BR114_943kbs);

	/* Low Battery Detector and Tx bit Synchronization Command */
	WriteCMD16b(0xC2A0);	// ENABLE BIT SYNC ,dwc - set to "1"

	/* Power Setting Command */
	WriteCMD8b(0xB0|P_0dBm);
}


/***********************************
PORTS initialize
***********************************/
void PORTS_init(void)
{
	/* Configure RF module pins */
	PORTB |= (1<<nSEL);
	DDRB |= (1<<SCK) | (1<<nSEL);  
	DDRA |= (1<<SDI);

	/* Configure LEDS Pins */
	DDRA |= (1<<PA0) | (1<<PA1);

	/* Configure UG send Pins */
	DDRA |= (1<<PA5) | (1<<PA6);

	LED1_OFF;
	LED2_OFF;
}


/************************************
Write 16 bit command
*************************************/
void WriteCMD16b(uint16_t CMD)
{
	uint8_t n = 16;

	nSEL_LOW;
	
	while(n--)
		{
		SCK_LOW;

		if(CMD&0x8000)
			SDI_HI;
		else
			SDI_LOW;

		SCK_HI;	
			
		CMD = CMD<<1;
		}

	SCK_LOW;
	nSEL_HI;
}


/************************************
Write 8 bit command
*************************************/
void WriteCMD8b(uint8_t CMD)
{
	uint8_t n = 8;

	nSEL_LOW;
	
	while(n--)
		{
		SCK_LOW;

		if(CMD&0x80)
			SDI_HI;
		else
			SDI_LOW;

		SCK_HI;	
			
		CMD = CMD<<1;
		}

	SCK_LOW;
	nSEL_HI;
}


/**************************************
Write FSK data
**************************************/
void WriteFSKdata(uint8_t data)
{
	uint8_t n = 8;

	while(n--)
		{
		while(nIRQ_PIN);	
		while(!nIRQ_PIN);

		if(data&0x80)
			SDI_HI;
		else
			SDI_LOW;

		data = data<<1;
		}
}


/**************************************
Send data
**************************************/
void RF02_Send(uint16_t Data)
{
	uint8_t parity;
	uint8_t n = 8;
	uint8_t CMD = 0xC6;

	parity = GetParity(Data)&0x01;
	Data = Data | (parity<<15);

	nSEL_LOW;

	/* Send 8bit Data Transmit Command */
	while(n--)
		{
		SCK_LOW;

		if(CMD&0x80)
			SDI_HI;
		else
			SDI_LOW;

		SCK_HI;	
			
		CMD = CMD<<1;
		}
	SCK_LOW;

	/* Send Preamble and Send sync word */
	WriteFSKdata(0xAA);	// Send Preamble
	WriteFSKdata(0xAA);	// Send Preamble
	WriteFSKdata(0xAA);	// Send Preamble
	WriteFSKdata(0x2D);	// Send sync word
	WriteFSKdata(0xD4);	// Send sync word

	/* Send Data + Parity */
	WriteFSKdata(Data>>8);
	WriteFSKdata(Data&0xFF);
	
	while(nIRQ_PIN);	// wait until transfer done
	
	nSEL_HI;
}



/**************************************
Send Ug signals
**************************************/
void Send_UG(uint16_t n)
{
	uint8_t i=0;

	TIFR1=0xFF;
	TCNT1=0;
	ICR1 = 124;		// Top(12.5us)
	OCR1A = 59;		// 6us
	OCR1B = 64;		// 6.5us
	TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<COM1B0);
	TCCR1B = (1<<WGM13);		// Fcpu/1, start Timer/Counter1.
	TCCR1B |= (1<<CS10) ;		

	while(i<=n)
	{
		if ((TIFR1&(1<<TOV1))==(1<<TOV1))
		{
			TIFR1|=1<<TOV1;
			i++;
		}
	}
	TCCR1B = 0;
	TCCR1A = 0;
	PORTA &= ~(1<<PA5);
}


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
