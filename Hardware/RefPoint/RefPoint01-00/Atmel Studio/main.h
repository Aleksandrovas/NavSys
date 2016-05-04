
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


#define RF_TXmode 		WriteCMD(0x8238)	// Enable transmitter; Enable synthesizer; Enable crystal oscillator
#define RF_RXmode		WriteCMD(0x82D8)	// Enable receiver; er, ebb, es, ex, dc - set to "1"
#define RF_Iddle		WriteCMD(0x8208)	// RX and TX off
#define RF_FIFORecog	WriteCMD(0xCAF0);WriteCMD(0xCAF3);	// FIFO int: 16bits, Sync-word; Enable FIFO fill,


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

#define PW0268_OFF		PORTB &= ~(1<<PB2)
#define PW0268_ON		PORTB |= (1<<PB2)


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


/* 433band: Fc=430+F*0.0025 MHz */
#define Fc(freq)		(uint16_t)((freq - 430)*400)

#endif
