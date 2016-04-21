
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define Transmitter_ON	WriteCMD(0x8238);	//Enable transmitter; Enable synthesizer; Enable crystal oscillator
#define Receiver_ON		WriteCMD(0x82D8);	// Enable receiver; er, ebb, es, ex, dc - set to "1"

//#define Sleep			WriteCMD(0x82D8);	// 8 visada paskutinis skaicius!!!!!! ant 82 registro

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


/************************************
Global and local variables
************************************/
unsigned char TLow,THigh;
volatile unsigned short laikas=0;


/************************************
PORTS initialise
*************************************/
void PORTS_int(void)
{
	DDRA |= (1<<PA1) | (1<<PA2) | (1<<PA3) | (1<<PA4) | (1<<PA6);
	PORTA |= (1<<SDO);
}


/************************************
Write command
*************************************/
void WriteCMD(unsigned int CMD)
{
	unsigned char n = 16;

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


/************************************
ADC	 initialise
*************************************/
void ADC_int(void)
{
	ADMUX = (1<<REFS1) | (1<<MUX5) | (1<<MUX1);	// 8 ch; Internal 1.1V 
	ADCSRB = 0;
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);	// ADC on, start ADC conversion (measure temperature)	
}


/**************************************
Write FSK data
**************************************/
void Write_FSK_byte(unsigned char data)
{
	unsigned int CMD = 0xB800;
	unsigned char RGIT = 0;
	
	do{
		SCK_LOW;
		nSEL_LOW;
		SDI_LOW;

		SCK_HI;
		
		if(SDO_PIN)		//Polling SDO
			RGIT=1;		

		SCK_LOW;
		SDI_HI;
		nSEL_HI;

		}while(!RGIT);

	WriteCMD(CMD|data);
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
void RMFM12_send(unsigned short data)
{
	Write_FSK_byte(0xAA);	// Send Preamble
	Write_FSK_byte(0xAA);	// Send Preamble
	Write_FSK_byte(0xAA);	// Send Preamble
	Write_FSK_byte(0x2D);	// Send sync word
	Write_FSK_byte(0xD4);	// Send sync word
	Write_FSK_byte(data>>8);
	Write_FSK_byte(data&0xff);
	

}


/************************************
RFM12 initialise
*************************************/
void RFM12_init(void)
{
	unsigned int F;
	WriteCMD(0x80DF);		// 433MHz Band; Enable TX registere; Enable RX FIFO buffer, 16pF
	WriteCMD(0xC0E0);		// 10MHz output  
	asm("nop");
	asm("nop");
	asm("nop");
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


/************************************
Read FIFO
*************************************/
unsigned short RMFM12_readFIFO(void)
{
	unsigned int i=0;
	unsigned short Result=0;

	SCK_LOW;
	SDI_LOW;
	nSEL_LOW;

	for(i=0;i<16;i++)	// skip status bits
		{ 
		SCK_HI;
		SCK_LOW;
		}

	for(i=0;i<16;i++)	// read fifo data byte
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

void transmit_packet(unsigned char nr,unsigned char tl, unsigned short value)
{
	unsigned short packet;
	packet|=1<<15;
	packet|=(nr&0x03)<<13;
	packet|=(tl&0x01)<<12;
	packet|=(value&2047)<<1;
	Transmitter_ON;
	_delay_ms(10);
	RMFM12_send(packet);
	_delay_ms(10);
	Receiver_ON;
}

short Temp_measure(void)
{
	short temp;
	unsigned short k;
	long T;


	ADCSRA |= (1<<ADSC);	
	while(ADCSRA&(1<<ADSC));
	k = ADC;
//	k=278;
//	return k;

	if(k<300)
		T = (130*(unsigned long)k - 35500 + 7)/14;
	else
		T = (600*(unsigned long)k - 162500 + 35)/70;
	temp = (signed short)T;
	return(temp);
}

#define _time 			0
#define _temperature 	1
#define _number 		2

int main(void)
{
	unsigned short temp;
	unsigned short packet=0;
	unsigned char nr=3;
	unsigned short temperatura=0;
	PORTS_int();
	RFM12_init();
	ADC_int();
	Receiver_ON;
	
	for (int i=0;i<_number;i++)
	{
		LED1_ON;
		_delay_ms(200);
		LED1_OFF;
		_delay_ms(200);
	}
	if (_number==0)
		_delay_ms(1000);
	while(1)
	{ 
		if(!nIRQ_PIN)	// Wait for interupt from RFM12
		{		
			temp = RMFM12_readFIFO();
       
			if(temp == 0x1454)
			{
	    			// Indicate start signal		   	   		    
			    DDRA |= (1<<PA0);
				PORTA &= ~(1<<PA0);
				_delay_us(100);
				PORTA |= (1<<PA0);
				DDRA &= ~(1<<PA0);
			
				LED1_ON;
			    TCNT1 = 0;			// Reset Timer/Counter1
				TCCR1A=0; 
				TCCR1B=1<<CS11;		// div 8, start
				while(PINA&(1<<PA0));
		
				LED2_ON;		// Indicate start of UG
				
				TCCR1B=0;			//stop
				laikas=TCNT1/16;
				//temp=123;
				//laikas=600;
				//if (((nr++)%2)==0)
				_delay_ms(30*_number);
				temperatura = Temp_measure();
				if (((nr++)%20)==0)
				{
				//	temperatura/=20;
					transmit_packet(_number,_temperature,temperatura);
					temperatura=0;
				}
				transmit_packet(_number,_time,laikas);
				_delay_ms(5);
				//temperatura=1023;
			}
	     }
		_delay_us(50);
	  	LED1_OFF;
	 	LED2_OFF;
	

	}

}

