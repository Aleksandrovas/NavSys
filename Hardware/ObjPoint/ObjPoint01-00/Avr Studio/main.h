
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


/* Definitions ************************************************************/
#define StartCode		0x1454
#define PRF_ms			1000

#define RF_TXmode 		WriteCMD16b(0xC038);	// Enable transmitter; Enable synthesizer; Enable crystal oscillator
#define RF_Iddle		WriteCMD16b(0xC020);	// TX off
#define RF_Sleep		WriteCMD16b(0xC40A);	

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

#define Fc(freq)		(uint16_t)((freq - 430)*400)


/* Functions prototypes ****************************************************/
void PORTS_init(void);
void RF02_init(void);
void Send_UG(uint16_t n);
void PrepareRFpacket(uint8_t* RFpacket, uint16_t Data, uint8_t RefNr);
void RF02_Send(uint16_t Data);
void WriteFSKdata(uint8_t data);
void WriteCMD16b(uint16_t CMD);
void WriteCMD8b(uint8_t CMD);
uint8_t GetParity(uint16_t x);

typedef enum 
{
  FreqDev30kHz 	= 0x00,
  FreqDev60kHz 	= 0x01,
  FreqDev90kHz 	= 0x02,
  FreqDev120kHz = 0x03,
  FreqDev150kHz = 0x04,
  FreqDev180kHz = 0x05,
  FreqDev210kHz = 0x06
} RF_FreqDeviation;

typedef enum 
{
  CLK1MHz 	= 0x00,
  CLK1_25MHz= 0x01,
  CLK1_66MHz= 0x02,
  CLK2MHz 	= 0x03,
  CLK2_5MHz = 0x04,
  CLK3_33MHz= 0x05,
  CLK5MHz 	= 0x06,
  CLK10MHz 	= 0x07
} RF_CLKOutput;

typedef enum 
{
  RF_433MHz	= 0x01,
  RF_868MHz	= 0x02,
  RF_915MHz	= 0x03
} RF_Band;

typedef enum 
{
  BR114_943kbs	= 0x02,
  BR86_207kbs	= 0x03,
  BR68_966kbs	= 0x04,
  BR57_471kbs	= 0x05,
  BR49_261kbs	= 0x06,
  BR43_103kbs	= 0x07,
  BR38_314kbs	= 0x08,
  BR34_483kbs	= 0x09,
  BR31_348kbs	= 0x0A
} RF_BaudRate;


typedef enum 
{
  P_0dBm	= 0x00,
  P_m3dBm	= 0x01,
  P_m6dBm	= 0x02,
  P_m9dBm	= 0x03,
  P_m12dBm	= 0x04,
  P_m15dBm	= 0x05,
  P_m18dBm	= 0x06,
  P_m21dBm	= 0x07,
  P_m24dBm	= 0x08,
} RF_Pout;


#endif
