
#define F_CPU 10000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"


/***************************************************************************
Definitions
***************************************************************************/
#define RF_StartCode	0x1454
#define PRF				1000		// xx ms


/***************************************************************************
Functions prototypes
***************************************************************************/
void RFM02_init(void);
void Send_UG(uint16_t n);
void RMFM02_send(uint16_t data);
void Write_FSK_word(uint16_t data);
void Write_FSK_byte(uint8_t data);
void WriteCMD(uint16_t CMD);
void PORTS_int(void);


int main(void)
{
	/* Wait for Vcc stabilize */
	_delay_ms(100);

	PORTS_int();
	RFM02_init();

	while(1)
	{
		_delay_ms(PRF);

		RF_TXmode;
		_delay_us(500);

		LED1_ON;
		LED2_ON;

		RMFM02_send(RF_StartCode);
		Send_UG(20);

		_delay_us(500);
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
RFM02_init - RFM02 initialize
***************************************************************************/
void RFM02_init(void)
{
	uint16_t Fcarr;

	/* Configuration Setting Command: 433MHz band, +/-210kHz, CLK 10MHz, 16pF	*/
	WriteCMD(0x8000 | (RF_433MHz<<11) | (CLK10MHz<<8) | (0x0F<<4) | FreqDev210kHz);	
	_delay_ms(100);
	WriteCMD(0x8000 | (RF_433MHz<<11) | (CLK10MHz<<8) | (0x0F<<4) | FreqDev210kHz);	
	_delay_ms(100);

	/* Frequency Setting Command: Fcarrier = 439.00MHz */
	Fcarr = Fc(439.00);
	WriteCMD(0xA000|Fcarr);
	
	/* PLL Setting Command */
	WriteCMD(0xD282);
	
	/* Data Rate Command: BR 114.943Kbps */
	WriteCMD(0xC800|BR114_943kbs);	

	/* Power Setting Command */
	//WriteCMD(0xB0xx);

	/* Low Battery Detector and Tx bit Synchronization Command */
	WriteCMD(0xC2A0);	// ENABLE BIT SYNC ,dwc - set to "1".
}


/***********************************
PORTS initialize
***********************************/
void PORTS_int(void)
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
Write command
*************************************/
void WriteCMD(uint16_t CMD)
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


/**************************************
Write FSK data
**************************************/
void Write_FSK_byte(uint8_t data)
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
Write FSK data
**************************************/
void Write_FSK_word(uint16_t data)
{
	uint8_t n = 15;

	while(n--)
		{
		while(nIRQ_PIN);	
		while(!nIRQ_PIN);

		if(data&0x8000)
			SDI_HI;
		else
			SDI_LOW;

		data = data<<1;
		}
}


/**************************************
Send FSK data
**************************************/
void RMFM02_send(uint16_t data)
{
	uint8_t n = 8;
	uint8_t CMD = 0xC6;

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

	Write_FSK_byte(0xAA);	// Send Preamble
	Write_FSK_byte(0xAA);	// Send Preamble
	Write_FSK_byte(0xAA);	// Send Preamble
	Write_FSK_byte(0x2D);	// Send sync word
	Write_FSK_byte(0xD4);	// Send sync word	
	Write_FSK_word(data);	

	nSEL_HI;

	while(nIRQ_PIN);		// wait until transfer done
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