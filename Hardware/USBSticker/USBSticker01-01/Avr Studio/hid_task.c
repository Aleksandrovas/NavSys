/*This file has been prepared for Doxygen automatic documentation generation.*/
//! \file *********************************************************************
//!
//! \brief This file manages the generic HID IN/OUT task.
//!
//! - Compiler:           IAR EWAVR and GNU GCC for AVR
//! - Supported devices:  AT90USB162, AT90USB82
//!
//! \author               Atmel Corporation: http://www.atmel.com \n
//!                       Support and FAQ: http://support.atmel.no/
//!
//! ***************************************************************************

/* Copyright (c) 2007, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf_usb.h"
#include "hid_task.h"
#include "lib_mcu/usb/usb_drv.h"
#include "usb_descriptors.h"
#include "../modules/usb/device_chap9/usb_standard_request.h"
#include "usb_specific_request.h"
#include "lib_mcu/util/start_boot.h"

#include <avr/io.h>
#include "spi_drv.h"
#include "RFmodule.h"
#include "main.h"
#include <avr/interrupt.h>

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________
U8 buferis[64];
const U8 alive[4]="\1\254\2\253";
extern U8 jump_bootloader;

bool USB_Send(U8 *buff,unsigned char len);
bool USB_Read(U8 *buff,unsigned char len);

void hid_task_init(void);
void UI_task(void);
void DATA_RECEIVED(void);
bool USB_Read(U8 *buff,unsigned char len);
bool USB_Send(U8 *buff,unsigned char len);

uint8_t GetParity(uint16_t x) ;
void Timer1_init(void);
volatile uint8_t SendDataFlag=0;
volatile uint8_t StartLogFlag=0;
uint8_t ToFData[64];;

//--------------------------------------------------------------------------


//! @brief This function initializes the target board ressources.
//!
void hid_task_init(void)
{
	/* Configure LEDS Pins */
	DDRD|=(1<<PD1)|(1<<PD2);

	/* Configure RF nIRQ PIN */
	DDRB&=~(1<<nIRQ);

	Init_SPI();
	RF01_init();
	RF_FIFORecog;
	RF_RXmode;
	RF01_ReadFIFO();
	sei();
}


//! @brief Entry point of the HID generic communication task
//! This function manages IN/OUT repport management.
//!
void hid_task(void)
{
	UI_task();

   	/*if(!Is_device_enumerated())          // Check USB HID is enumerated
      	return;

   	if (USB_Read(buferis,64))
   		DATA_RECEIVED();*/
} 


void UI_task(void)
{
	uint16_t temp;
	uint16_t ToF;
	uint8_t parity;
	uint8_t RefNr;
	uint8_t i;
	uint8_t indx;

	/* Wait for interrupt from RF (received Data) */
	if(!nIRQ_PIN)	
	{
		temp = RF01_ReadFIFO();
		parity = temp>>15;
		temp = temp&0x7FFF;

		/* Check received data */		
		if ( (GetParity(temp) == parity) && (temp > 0) )
		{
			/* Start Timout Timer and clear Data Buffer */
			if(temp == StartCode)
			{	
				LED2_ON;				// Received StartCode
				SendDataFlag = 0;
				StartLogFlag = 0;
				Timer1_init();
				for (i=0;i<64;i++)
					ToFData[i]=0;
			}
			/* Store Data to Buffer */
			else if (StartLogFlag)
			{
				LED1_ON;				// Received ToF from Reference Point
				RefNr = (temp>>12)&0x07;
				ToF = temp&0x0FFF;
				indx = RefNr*2;
				ToFData[indx] = ToF>>8;
				ToFData[indx+1] = ToF&0xFF;
			}
		}
		//while(!nIRQ_PIN);

		RF_FIFORecog;
		LED1_OFF;
		LED2_OFF;
	}

	if (SendDataFlag)
	{	
		if(Is_device_enumerated())  
			USB_Send(ToFData,64);
		SendDataFlag = 0;
		StartLogFlag = 0;
	}

}


void DATA_RECEIVED(void)
{	
	switch(buferis[0])
	{
		case 12:
			LED1_ON;
			LED2_ON;
			break;
		default:
			break;
	
	}
	buferis[0]=~buferis[0];
	USB_Send(buferis,64);
}


//! @brief Get data report from Host
//!
bool USB_Read(U8 *buff,unsigned char len)
{
	bool received = false;

  	Usb_select_endpoint(EP_HID_OUT);
  	received=Is_usb_receive_out();
   	if(received)
   	{
		usb_read_packet(EP_HID_OUT, buff, len);
      	Usb_ack_receive_out();
   	}

   // Check if we received DFU mode command from host
   	if(jump_bootloader)
   	{
		U32 volatile tempo;
      	Usb_detach();                          // Detach actual generic HID application
      	for(tempo=0;tempo<70000;tempo++);      // Wait some time before
      	start_boot();                          // Jumping to booltoader
   	}
   	return received;
}


//! @brief Send data report to Host
//!
bool USB_Send(U8 *buff,unsigned char len)
{
   Usb_select_endpoint(EP_HID_IN);
   if(!Is_usb_write_enabled())
      return false;                    		// Not ready to send report
   usb_send_packet(EP_HID_IN, buff, len);
   Usb_ack_in_ready();                     	// Send data over the USB                     // Send data over the USB
   return true;
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


/***************************************************************************
Timer1_init - restart Timer1
***************************************************************************/
void Timer1_init(void)
{
	/* Reset 16bit Timer1 for ToF measurements */
	TCCR1B=0;			// Stop Timer/Counter1
	TCNT1=0;			// Reset Timer/Counter1
	TCCR1A=0;			// Normal mode
	TIFR1 |= (1<<OCF1A) | (1<<OCF1B);
	OCR1A = 50000;		// set compare match register to 50ms
	OCR1B = 59000;		// set compare match register to 59ms
    TIMSK1 = (1<<OCIE1A) | (1<<OCIE1B);// enable timer interrupts
	TCCR1B = 1<<CS11;	// clk div 8, start (tres=1us, tmax=65.536ms)
}



ISR(TIMER1_COMPA_vect) 
{
	if (!StartLogFlag)
	{
		StartLogFlag = 1;
		TCNT1=0;			// Reset Timer/Counter1
	}
}

ISR(TIMER1_COMPB_vect) 
{
	if (StartLogFlag)
		{
		SendDataFlag = 1;
		TCCR1B=0;			// Stop Timer/Counter1
		}
}

