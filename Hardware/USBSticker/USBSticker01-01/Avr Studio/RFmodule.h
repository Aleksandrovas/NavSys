
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RFMODULE_H
#define __RFMODULE_H

/* Definitions ************************************************************/
#define StartCode		0x1454

#define Fc(freq)		(uint16_t)((freq - 430)*400)

#define RF_RXmode 		WriteCMD(0xC0C1)	// VDI always on, LNA gain 0dBm, -103dBm, RX on
#define RF_Iddle		WriteCMD(0xC0C0)	// RX off
#define RF_FIFORecog	WriteCMD(0xCEF4);WriteCMD(0xCEF7);	// FIFO int: 8bits, Sync-word Enable FIFO fill,

typedef enum 
{
  Bandwidth67kHz 	= 0x06,
  Bandwidth134kHz 	= 0x05,
  Bandwidth200kHz 	= 0x04,
  Bandwidth270kHz 	= 0x03,
  Bandwidth340kHz 	= 0x02,
  Bandwidth400kHz 	= 0x01
} RF_Bandwidth; 

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


/* Functions prototypes ****************************************************/
void RF01_init(void);
uint16_t RF01_ReadFIFO(void);
void WriteCMD(uint16_t CMD);

#endif
