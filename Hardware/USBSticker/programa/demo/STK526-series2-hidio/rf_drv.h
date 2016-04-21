#define Synchro1		0x02		
#define Synchro2		0x03
#define LowBattery		0x01

#define RF01FREQ(freq)	((freq-430.0)/0.0025)

#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))  


#define Sleep_TX		WriteCMD(0xC40A);	// TX off after 1us 
#define Fc(freq)		(freq-430.5)/0.0025	
#define BR(baud)		(((10000000/29)/baud)-1)
#define Vlb(V)			(V-2.2)/0.1

/*#ifndef RF_PORT
# error "RF_PORT not defined for rf_drv.h"
#endif*/

#define	RF_PORT			PORTB
#define	RF_DDR			DDRB
#define RF_PIN			PINB
#define SDI 			PB2	// SDI 	-> 	RFM01 12
#define SDO				PB3	// SD0 	<- 	RFM01 01
#define SCK 			PB1	// SCK 	-> 	RFM01 13
#define nSEL			PB0	// nSEL -> 	RFM01 14
#define nIRQ 			PB4	// nIRQ <- 	RFM01 02


#define SDI_HI			RF_PORT |= (1<<SDI)
#define SDI_LOW			RF_PORT &= ~(1<<SDI)
#define SCK_HI			RF_PORT |= (1<<SCK)
#define SCK_LOW			RF_PORT &= ~(1<<SCK)
#define nSEL_HI			RF_PORT |= (1<<nSEL)
#define nSEL_LOW		RF_PORT &= ~(1<<nSEL)
#define nIRQ_PIN		(RF_PIN & (1<<nIRQ))
#define SDO_PIN			(RF_PIN & (1<<SDO))
#define LED1_ON			LED_PORT |= (1<<LED1)
#define LED1_OFF		LED_PORT &= ~(1<<LED1)
#define LED2_ON			LED_PORT |= (1<<LED2)
#define LED2_OFF		LED_PORT &= ~(1<<LED2)


void RFM01_init(void);
void RF_PORT_int(void);
void WriteCMD(unsigned int CMD);
void rf01_setbandwidth(unsigned char bandwidth);
void rf01_setreceiver(unsigned char gain, unsigned char drssi);
void rf01_setfreq(unsigned short freq);
void rf01_setbaud(unsigned short baud);
void makePulse(int numberOfPulses);
unsigned long RMFM01_readFIFO(void);
void WriteCMDs(unsigned int CMD);
