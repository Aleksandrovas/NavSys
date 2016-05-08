
#define F_CPU 10000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"


int main(void)
{
	volatile uint16_t ToF;
	uint16_t temp;
	volatile uint8_t TimeOutFlag;
	uint8_t parity;

	/* Wait for Vcc stabilize */
	_delay_ms(100);

	PORTS_init();
	RF12_init();
	Timer1_init();
	BlinkNumber(RefPointNr);

	RF_FIFORecog;
	RF_RXmode;
	RF12_ReadFIFO();

PW0268_ON;

	while(1)
	{
		/* Wait for interrupt from RF (received Data) */
		if(!nIRQ_PIN)	
		{
			/* Start 16bit Timer1 immediately for ToF measurements */
			TCCR1B=1<<CS11;		// clk div 8, start (tres=0.8us, tmax=52,43ms, Lres=0,27mm, Lmax=17,83m c=340m/s)

			temp = RF12_ReadFIFO();
			parity = temp>>15;
			temp = temp&0x7FFF;

			/* Check received data */		
			if ( (GetParity(temp) == parity) && (temp > 0) )
			{
				/* Start Timout Timer and clear Data Buffer */
				if(temp == StartCode)
				{
				LED1_ON;

					/* Turn ON PW0268 */
					//PW0268_ON;
					//_delay_us(300);
				
					/* RF Transmitter/Receiver modes OFF to save power */
					//RF_Iddle;

		    		/* Send start signal to front-end (PW0268) */	   	   		    
				    DDRA |= (1<<PA0);	// Configure PA0 as Output
					PORTA &= ~(1<<PA0);	// Clear PA0
					_delay_us(1000);	
					PORTA |= (1<<PA0);	// Set PA0
					DDRA &= ~(1<<PA0);	// Configure PA0 as Input
			
					//LED1_ON;			// RF received

					/* Wait for UG signal */
					TimeOutFlag = 0;
					while(PINA&(1<<PA0))
					{
						if (TIFR1&(1<<TOV1))	// Check for Overflow Flag
						{
							TimeOutFlag = 1;
							break;
						}
					}
					ToF = TCNT1/16;
								
					/* Turn OFF PW0268 */
					//PW0268_OFF;

					/* Check TimeOutFlag */
					if (!TimeOutFlag)
					{
						LED2_ON;		// UG signal received
					
						/* Wait for Timer1 Overflow Flag to ensure all RefPoints already received UG signal */
						if (!TimeOutFlag)
							{
								while( !(TIFR1&(1<<TOV1)) );
							}

						/* Send ToF to Host */
						_delay_us(RFTransmit_us*RefPointNr);
						RF_TXmode;
						_delay_us(300);
						RF12_Send(ToF);
						LED1_ON;
						_delay_us(100);
						RF_Iddle;
					}
		  			LED1_OFF;
		 			LED2_OFF;
				}
			}
			/* Restart the synchrony pattern recognition */
			RF_FIFORecog;
			RF_RXmode;
					
			/* Restart Timer1 Configuration */
			Timer1_init();		
		}

	}	// while
}		// main





/***************************************************************************
****************************************************************************
Functions
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
RFM12_init - RFM12 initialize
***************************************************************************/
void RF12_init(void)
{
	uint16_t Fcarr;

	/* Low Battery Detector and Micro controller Clock Divider Command: 10MHz output*/
	WriteCMD(0xC000 | (CLK10MHz<<5));
	_delay_ms(100);
	WriteCMD(0xC000 | (CLK10MHz<<5));
	_delay_ms(100);

	/* Configuration Setting Command  */
	WriteCMD(0x80DF);	// 433MHz Band; Enable TX registere; Enable RX FIFO buffer, 16pF
	
	/* Frequency Setting Command: F carrier = 432.00MHz */
	Fcarr = Fc(432.00);
	WriteCMD(0xA000|Fcarr);

	/* Data Rate Command: 114.943kbps */
	WriteCMD(0xC600|BR114_943kbs);

	/* Receiver Control Command */
	WriteCMD(0x9420);	// VDI, FAST, Bandwidth 400kHz, LNA gain 0dBm, -103dBm

	/* Data Filter Command */
	WriteCMD(0xC2AC);	// Auto-lock; Digital filter;
	
	/* FIFO and Reset Mode Command */
	WriteCMD(0xCA73);	// FIFO interrupt level: 7bits; FIFO fill start condition: Sync-word; Enable FIFO fill; dr - set to "1"
	
	/* AFC Command */
	WriteCMD(0xC49B);	// AFC setting: Keep offset when VDI hi; select range limit +15/-16; Enable AFC funcition; st,oe - set to "1"
	
	/* TX Configuration Control Command */
	WriteCMD(0x98D0|P_m6dBm);	// 210kHz deviation; MAX OUT

	/* Power Management Command */
	RF_Iddle;
}


/***************************************************************************
Timer1_init - restart Timer1
***************************************************************************/
void Timer1_init(void)
{
	/* Reset 16bit Timer1 for ToF measurements */
	TCCR1B=0;			// Stop Timer/Counter1
	TIFR1|=(1<<TOV1); 	// Clear Overflow Flag
	TCNT1=0;			// Reset Timer/Counter1
	TCCR1A=0;			// Normal mode
}


/***************************************************************************
PORTS_int - PORTS initialize
***************************************************************************/
void PORTS_init(void)
{
	/* Configure PW0268 Pin */
	PORTB &= ~(1<<PB2);	
	DDRB |= (1<<PB2);
	
	/* Configure RF module pins */
	DDRA |= (1<<PA3) | (1<<PA4) | (1<<PA6);
	PORTA |= (1<<SDO);
	
	/* Configure LEDS Pins */
	DDRA |= (1<<PA1) | (1<<PA2);

	PW0268_OFF;
	LED1_OFF;
	LED2_OFF;
}


/************************************
Write 16 bit command
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


/***************************************************************************
Write_FSK_byte - Write FSK 8bit data
***************************************************************************/
void Write_FSK_byte(uint8_t data)
{
	uint16_t CMD = 0xB800;
	uint8_t RGIT = 0;
	
	/* Wait for previous data latch */
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
		}while(!RGIT);

	/* Transmitter Register Write Command */
	WriteCMD(CMD|data);
}


/***************************************************************************
RF12_Send - Send data 
***************************************************************************/
void RF12_Send(uint16_t Data)
{
	uint8_t parity;

	Data = Data | (RefPointNr<<12);
	parity = GetParity(Data)&0x01;
	Data = Data | (parity<<15);

	/* Send Preamble and Send sync word */
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0x2D);		// Send sync word
	Write_FSK_byte(0xD4);		// Send sync word

	/* Send Data */
	Write_FSK_byte(Data>>8);
	Write_FSK_byte(Data&0xFF);
}


/***************************************************************************
RF12_ReadFIFO - Read FIFO
***************************************************************************/
uint16_t RF12_ReadFIFO(void)
{
	uint16_t Result=0;

	SCK_LOW;
	SDI_LOW;
	nSEL_LOW;

	/* Skip status bits */
	for(uint8_t i=0;i<16;i++)	
		{ 
		SCK_HI;
		SCK_LOW;
		}

	/* Read Fifo Data byte */
	for(uint8_t i=0;i<16;i++)	
		{ 
		Result = Result<<1;
		
		if(SDO_PIN)
			Result|=1;

		SCK_HI;
		SCK_LOW;
		}

	nSEL_HI;

	return(Result);
}


/***************************************************************************
BlinkNumber - 
***************************************************************************/
void BlinkNumber(uint8_t RefNr)
{
	LED2_ON;
	LED1_OFF;
	_delay_ms(500);

	for (uint8_t i=0;i<RefNr;i++)
	{
		LED1_ON;
		_delay_ms(250);
		LED1_OFF;
		_delay_ms(250);
	}
	if (RefNr==0)
		_delay_ms(1000);
	
	LED1_OFF;
	LED2_OFF;
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
