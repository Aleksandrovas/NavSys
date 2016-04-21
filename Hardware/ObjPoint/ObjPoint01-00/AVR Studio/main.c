
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
 
#define Sleep_TX		WriteCMD(0xC402);	// TX off (sleep) after 1us 
#define Fc(freq)		( (freq/10) - 43 ) * 4000	

#define nIRQ 			PA2		// nIRQ <- 	RFM02 02
#define SDI 			PA7		// SDI 	-> 	RFM02 12
#define nSEL			PB1		// nSEL -> 	RFM02 14
#define SCK 			PB2		// SCK 	-> 	RFM02 13

#define SDI_HI			PORTA |= (1<<SDI)
#define SDI_LOW			PORTA &= ~(1<<SDI)
#define SCK_HI			PORTB |= (1<<SCK)
#define SCK_LOW			PORTB &= ~(1<<SCK)
#define nSEL_HI			PORTB |= (1<<nSEL)
#define nSEL_LOW		PORTB &= ~(1<<nSEL)
#define nIRQ_PIN		(PINA & (1<<nIRQ))



#define LED1_ON			PORTA &= ~(1<<PA0)
#define LED1_OFF		PORTA |= (1<<PA0)
#define LED2_ON			PORTA &= ~(1<<PA1)
#define LED2_OFF		PORTA |= (1<<PA1)


/***********************************
PORTS initialise
***********************************/
void PORTS_int(void)
{
	DDRA |= (1<<PA0) | (1<<PA1);
}


/************************************
Write command
*************************************/
void WriteCMD(unsigned int CMD)
{
	unsigned char n = 16;

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
void Write_FSK_byte(unsigned char data)
{
	unsigned char n = 8;

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
void Write_FSK_word(unsigned short data)
{
	unsigned char n = 15;

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
void RMFM02_send(unsigned short data)
{
	unsigned char n = 8;
	unsigned char CMD = 0xC6;

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
LED1_ON;
	Write_FSK_byte(0xAA);	// Send Preamble
	Write_FSK_byte(0xAA);	// Send Preamble
	Write_FSK_byte(0xAA);	// Send Preamble

	Write_FSK_byte(0x2D);	// Send sync word
	Write_FSK_byte(0xD4);	// Send sync word
	
	Write_FSK_word(data);	

					// Indicate end of data transmition.

	nSEL_HI;

	while(nIRQ_PIN);		// wait until transfer done

//	Sleep_TX;
}


/************************************
RFM02 initialise
*************************************/
void RFM02_init(void)
{
	unsigned int F;

	PORTB |= (1<<nSEL);
	DDRB |= (1<<SCK) | (1<<nSEL);  
	DDRA |= (1<<SDI);

	WriteCMD(0x8FF6);			// 433MHz band, +/-210kHz, CLK 10MHz, 16pF.	
	asm("nop");					// Wait for clock stabilize

	F = Fc(439.00);				// 2.5kHz step (430.0975MHz - 439.7575MHz).
	WriteCMD(0xA000|F);			// Fcarrier = 433.00MHz		
	WriteCMD(0xC829);			// 114.94Kbps
	WriteCMD(0xD282);			// Set PLL bandwidth, dr - set to "1".
	WriteCMD(0xC2A8);			// ENABLE BIT SYNC ,dwc - set to "1".
	// Pout = Pmax = 7dBm
	WriteCMD(0xC0E0);			// a0,a1,ex - set to "1".	
}


/**************************************
Initialise Ug 
**************************************/
void init_Ug()
{
	DDRA |= (1<<PA5) | (1<<PA6);
}


/**************************************
Send Ug signals
**************************************/
void send_Ug(unsigned char n)
{
	unsigned char i=0;
	TIFR1=0xFF;
	TCNT1=0;
	ICR1 = 124;		// Top(12.5us)
	OCR1A = 59;		// 6us
	OCR1B = 64;		// 6.5us
	TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<COM1B0);
	TCCR1B = (1<<WGM13);		// Fcpu/1, start Timer/Counter1.
	TCCR1B |= (1<<CS10) ;		
	LED2_ON;

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


/************************************
Interupt service
*************************************/
ISR(PCINT1_vect)
{
//	sleep_disable();
//	cli();			// disable interupt
}

//parity bito skaiciavimo paprograme
unsigned char parity(unsigned short word)
{
	unsigned char count=1;
	unsigned short i;
	for (i=1;i<16;i++)
	{
		if ((word&1)==1)
			count++;
		word>>=1;
	}
	return (count%2);
}


int main(void)
{
unsigned short n=0;
PORTS_int();
RFM02_init();
init_Ug();

set_sleep_mode(SLEEP_MODE_PWR_DOWN);						
PCMSK0 = (1<<PCINT2);
GIMSK = (1<<PCIE0);	// Pin Change on PCINT7..0.



while(1)
{
//	WriteCMD(0xE064);		// Twakeup = 100ms
//	WriteCMD(0xC0E2);		// Set wake-up timer.
//	WriteCMD(0xC40A);		// TX off (sleep) after 10 periods of CLK signal.
//	PORTB &= ~(1<<nSEL);	// Turn off all Pull up resistors.
//	sei();					// Enable wake up interupt. 		
//	sleep_enable();
//	sleep_cpu();

//	PORTB |= (1<<nSEL);		// Reset Pull up resistors.
	_delay_ms(500);

	WriteCMD(0xC038);


	RMFM02_send(0x1454);	// Sends only 15 bits!!!
	send_Ug(20);

	WriteCMD(0xC000);

	_delay_ms(1);
	LED1_OFF;
	LED2_OFF;
//	WriteCMD(0xC0E0);		// Clear wake-up timer.
	}
}
