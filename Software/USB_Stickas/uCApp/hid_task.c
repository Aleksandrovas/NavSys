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
#include "spi_drv.h"

U8 buferis[64];
const U8 alive[4]="\1\254\2\253";

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________

volatile U16    cpt_sof=0, cold=0;
extern            U8    jump_bootloader;
bool blinker=true;


void hid_report_out  (void);
void hid_report_in   (void);
bool USB_Send(U8 *buff,unsigned char len);
bool USB_Read(U8 *buff,unsigned char len);
	#define interv 500
	#define max 2100

//--------------------------------------------------------------------------
void delay_ms(unsigned int tim)
{
	while (tim >0)
	{
		TIFR0|= 1<<TOV0;//tifr
		TCNT0=131;
		TCCR0B = (1<<CS01)|(1<<CS00);//fclk/64 0.008ms 125/ms
		while (  (TIFR0&(1<<TOV0)) ==0) ;
		tim--;
	}
}
ISR(TIMER0_OVF_vect)
{
	TCNT1=131;
	if (cpt_sof<max)
	cpt_sof++;
	else
	cpt_sof=0;
}

//! @brief This function initializes the target board ressources.
//!
void hid_task_init(void)
{
	DDRB|=(1<<PB4)|(1<<PB6);
	PORTB|=(1<<PB4)|(1<<PB6);
	PORTB|=((1<<PB4)|(1<<PB6));
	Init_SPI();

	TCNT0=131;
	TCCR0B = (1<<CS01)|(1<<CS00);//fclk/64 0.008ms 125/ms
	TIMSK0|=1<<TOIE0;
	sei();

}
void UI_task(void)
{
	if (blinker)
		if ((((cold+interv)<max)&&(cold+interv<=cpt_sof)) ||
			((cold+interv-max)<cpt_sof) && (cpt_sof < cold))
		{
				PORTB^=1<<PB6;
				cold=cpt_sof;
		}
	


}
void DATA_RECEIVED(void)
{
	U8 inbuf[64];
	switch(buferis[0])
	{
		case 12:
			PORTB^=(1<<PB4);
			buferis[1]=cpt_sof;
		break;
		case 1:
			SPI_sendbuf(&buferis[2],buferis[1],&inbuf);
			memcpy(&buferis[0],inbuf,buferis[1]);
		break;
		case 2:
			blinker=~blinker;
		default:
		break;
	
	}
	//	buferis[0]=~buferis[0];
		USB_Send(buferis,64);
}

//! @brief Entry point of the HID generic communication task
//! This function manages IN/OUT repport management.
//!
void hid_task(void)
{
	UI_task();
   if(!Is_device_enumerated())          // Check USB HID is enumerated
      return;

   if (USB_Read(buferis,64))
   {
   	DATA_RECEIVED();
	}
} 


//! @brief Get data report from Host
//!
bool USB_Read(U8 *buff,unsigned char len)
{
	bool received=false;
  Usb_select_endpoint(EP_HID_OUT);
  received=Is_usb_receive_out();
   if(received)
   {
	  usb_read_packet(EP_HID_OUT, buff, len);
      Usb_ack_receive_out();
   }
   //** Check if we received DFU mode command from host
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
      return false;                                // Not ready to send report
   usb_send_packet(EP_HID_IN, buff, len);
   Usb_ack_in_ready();                       // Send data over the USB                     // Send data over the USB
   return true;
}
