
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RFMODULE_H
#define __RFMODULE_H


/* Definitions ************************************************************/
#define StartCode		0x1454

#define RF_TXmode 		WriteCMD(0x8238)	// Enable transmitter; Enable synthesizer; Enable crystal oscillator
#define RF_RXmode		WriteCMD(0x82D8)	// Enable receiver; er, ebb, es, ex, dc - set to "1"
#define RF_Iddle		WriteCMD(0x8208)	// RX and TX off
#define RF_FIFORecog	WriteCMD(0xCAF1);WriteCMD(0xCAF3);	// FIFO int: 16bits, Sync-word; Enable FIFO fill,


#define Fc(freq)		(uint16_t)((freq - 430)*400)


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
  BR31_348kbs	= 0x0A,
  BR8_019kbs	= 0x2A
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

typedef enum 
{
  Gain_0dB		= 0x00,
  Gain_m3dB		= 0x01,
  Gain_m6dB		= 0x02,
  Gain_m9dB		= 0x03,
  Gain_m12dB	= 0x04,
  Gain_m15dB	= 0x05,
  Gain_m18dB	= 0x06,
  Gain_m21dB	= 0x07,
} LNA_Gain;



/* Functions prototypes ****************************************************/
void RF12_init(void);
uint16_t RF12_ReadFIFO(void);
void RF12_Send(uint16_t Data);
void WriteCMD(uint16_t CMD);
void Write_FSK_byte(uint8_t data);
uint8_t GetParity(uint16_t x);

#endif
