
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "main.h"


/***************************************************************************
Definitions
***************************************************************************/
#define Transmitter_ON	WriteCMD(0x98D0);WriteCMD(0x8238);	// Enable transmitter; Enable synthesizer; Enable crystal oscillator
#define Receiver_ON		WriteCMD(0x82D8);	// Enable receiver; er, ebb, es, ex, dc - set to "1"
#define RstFIFORecog	WriteCMD(0xCAF0);WriteCMD(0xCAF3);	// FIFO int: 16bits, Sync-word; Enable FIFO fill,

#define SleepRF			WriteCMD(0x82D8);	// 8 visada paskutinis skaicius!!!!!! ant 82 registro

#define Fc(freq)		(freq-430)*400

#define SDI 			PA6	// SDI 	-> 	RFM12 12
#define SDO				PA5	// SD0 	<- 	RFM12 01
#define SCK 			PA4	// SCK 	-> 	RFM12 13
#define nSEL			PA3	// nSEL -> 	RFM12 14
#define nIRQ 			PA7	// nIRQ <- 	RFM12 02

#define SDI_HI			PORTA |= (1<<SDI)
#define SDI_LOW			PORTA &= ~(1<<SDI)
#define SCK_HI			PORTA |= (1<<SCK)
#define SCK_LOW			PORTA &= ~(1<<SCK)
#define nSEL_HI			PORTA |= (1<<nSEL)
#define nSEL_LOW		PORTA &= ~(1<<nSEL)
#define nIRQ_PIN		(PINA & (1<<nIRQ))
#define SDO_PIN			(PINA & (1<<SDO))

#define LED1_ON			PORTA &= ~(1<<PA1)
#define LED1_OFF		PORTA |= (1<<PA1)
#define LED2_ON			PORTA &= ~(1<<PA2)
#define LED2_OFF		PORTA |= (1<<PA2)

#define RefPointNr 		1	// range: 0-7
//#define	RFTransmit_us	500
#define	RFTransmit_ms	10



/***************************************************************************
Functions prototypes
***************************************************************************/
void PORTS_int(void);
void WriteCMD(unsigned int CMD);
uint16_t RMFM12_readFIFO(void);
void RFM12_init(void);
void RFTransmit_packet(uint8_t Refnr, uint16_t Value);
void RMFM12_send(uint16_t data);
void Write_FSK_byte(uint8_t data);
void BlinkNumber(uint8_t Refnr);
void Timer1_int(void);


/***************************************************************************
Global variables
***************************************************************************/
volatile uint16_t laikas;


int main(void)
{
	uint8_t TimeOutFlag;
	uint16_t temp;

	_delay_ms(200);

	PORTS_int();
	RFM12_init();
	Timer1_int();
	BlinkNumber(RefPointNr);

	RstFIFORecog;
	Receiver_ON;

	while(1)
	{ 

	//_delay_ms(500);
	//RFTransmit_packet(5,0x1454);


		/* Wait for interupt from RFM12 (received Data) */
		if(!nIRQ_PIN)	
		{
			/* Start 16bit Timer1 immediately for ToF measurments */
			TCCR1B=1<<CS11;		// clk div 8, start (tres=0.8us, tmax=52,43ms, Lres=0,27mm, Lmax=17,83m c=340m/s)
			
			temp = RMFM12_readFIFO();

			/* Check received data 
			if(temp == 0xFFFE)//== 0xFFFF)
			{
				LED1_ON;
				LED2_ON;
				_delay_ms(50);
	  			LED1_OFF;
	 			LED2_OFF;
			}*/


			/* Check received data */
			if(temp == 0x1454)
			{
				/* RF Trasmiter/Receiver modes OFF to save power */
				//SleepRF;

	    		/* Send start signal to front-end (PW0268) */	   	   		    
			    DDRA |= (1<<PA0);	// Configure PA0 as Output
				PORTA &= ~(1<<PA0);	// Clear PA0
				_delay_us(100);	
				PORTA |= (1<<PA0);	// Set PA0
				DDRA &= ~(1<<PA0);	// Configure PA0 as Input
			
				LED1_ON;			// RF received

				/* Wait for UG signal */
				TimeOutFlag=0;
				while(PINA&(1<<PA0))
				{
					if (TIFR1&(1<<TOV1))	// Check for Overflow Flag
					{
						TimeOutFlag = 1;
						break;
					}
				}
				laikas=TCNT1/32;	// reduce bits number to 12bits (tres2=12.8us, Lres2=4.35mm)

				/* Check TimeOutFlag */
				if (TimeOutFlag == 0)
				{
					LED2_ON;		// UG signal received
					
					// Wait for Timer1 Overflow Flag to ensure all RefPoints allready received UG signal
				//	while( !(TIFR1&(1<<TOV1)) );

					/* Send ToF to Host */
					//_delay_us(RFTransmit_us*RefPointNr);
					//_delay_ms(RFTransmit_ms*RefPointNr);
					RFTransmit_packet(RefPointNr,laikas);
					//RFTransmit_packet(7,0xE055);


				}
	  			LED1_OFF;
	 			LED2_OFF;
			}

		/* Restart Timer1 Configuration */
		Timer1_int();

		/* Restart the synchron pattern recognition */
		RstFIFORecog;
		}

	}	// while
}		// main








/***************************************************************************
****************************************************************************
Functions
***************************************************************************/


/***************************************************************************
RFTransmit_packet - Transmit data to RF
***************************************************************************/
void RFTransmit_packet(uint8_t Refnr, uint16_t Value)
{
	uint16_t packet=0;

	/* Prepare 16bit packet */
	packet|=0x8000;				// MSB always set to 1 ??????????????????
	packet|=(Refnr&0x07)<<12;	// Refnr 3 bits
	packet|=(Value&2047);		// Value
	
	/* Send 16bit packet */
	Transmitter_ON;
	_delay_ms(10);
	//RMFM12_send(packet);
	RMFM12_send(packet);
	_delay_ms(10);
	Receiver_ON;
	//Transmitter_OFF;
}


/***************************************************************************
Timer1_int - restart Timer1
***************************************************************************/
void Timer1_int(void)
{
	/* Reset 16bit Timer1 for ToF measurments */
	TCCR1B=0;			// Stop Timer/Counter1
	TIFR1|=(1<<TOV1); 	// Clear Overflow Flag
	TCNT1=0;			// Reset Timer/Counter1
	TCCR1A=0;			// Normal mode
}


/***************************************************************************
PORTS_int - PORTS initialise
***************************************************************************/
void PORTS_int(void)
{
	DDRA |= (1<<PA1) | (1<<PA2) | (1<<PA3) | (1<<PA4) | (1<<PA6);
	PORTA |= (1<<SDO);
	LED1_OFF;
	LED2_OFF;
}


/***************************************************************************
WriteCMD - Write command
***************************************************************************/
void WriteCMD(unsigned int CMD)
{
	uint8_t n = 16;

	SCK_LOW;
	nSEL_LOW;
	
	while(n--)
		{
		SCK_LOW;

		if(CMD&0x8000)
			SDI_HI;
		else
			SDI_LOW;

		SCK_HI;	
			
		CMD=CMD<<1;
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
	
	/* Wait for previous data lacth */
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
RMFM12_send - Send FSK data 
***************************************************************************/
void RMFM12_send(uint16_t data)
{
	/* RF packet [0xAA 0xAA 0xAA 0x2D 0xD4 Data_MSB Data_LSB] */
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0xAA);		// Send Preamble
	Write_FSK_byte(0x2D);		// Send sync word
	Write_FSK_byte(0xD4);		// Send sync word
	Write_FSK_byte(data>>8);	// Send Data
	Write_FSK_byte(data&0xFF);	// Send Data
}


/***************************************************************************
RFM12_init - RFM12 initialise
***************************************************************************/
void RFM12_init(void)
{
	uint16_t F;

	WriteCMD(0x80DF);		// 433MHz Band; Enable TX registere; Enable RX FIFO buffer, 16pF
	WriteCMD(0xC0E0);		// 10MHz output  
	asm("nop");
	_delay_ms(200);
	asm("nop");
	WriteCMD(0x82D8);		// Enable receiver; er, ebb, es, ex - set to "1"
	F = Fc(439.00);
	WriteCMD(0xA000|F);		// Fcarrier = 433.00MHz
	WriteCMD(0xC629);		// 114.94Kbps
	WriteCMD(0x9420);		// VDI, FAST, Bandwidth 400kHz, LNA gain 0dBm, -103dBm
	WriteCMD(0xC2AC);		// Auto-lock; Digital filter;	
	WriteCMD(0xCAF3);		// FIFO interrupt level: 16bits; FIFO fill start condition: Sync-word; Enable FIFO fill; dr - set to "1"
	WriteCMD(0xC49B);		// AFC setting: Keep offset when VDI hi; select range limit +15/-16; Enable AFC funcition; st,oe - set to "1"
	WriteCMD(0x98D0);		// 210kHz deviation,MAX OUT
}


/***************************************************************************
RMFM12_readFIFO - Read FIFO
***************************************************************************/
uint16_t RMFM12_readFIFO(void)
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
	
	WriteCMD(0xCA80);	// reset FIFO
	WriteCMD(0xCAF3);

	return(Result);
}


/***************************************************************************
BlinkNumber - 
***************************************************************************/
void BlinkNumber(uint8_t Refnr)
{
	LED2_ON;
	LED1_OFF;
	_delay_ms(500);

	for (uint8_t i=0;i<Refnr;i++)
	{
		LED1_ON;
		_delay_ms(500);
		LED1_OFF;
		_delay_ms(500);
	}
	if (Refnr==0)
		_delay_ms(1000);
	
	LED1_OFF;
	LED2_OFF;
}

