#include "config.h"
#include "conf_usb.h"
#include "hid_task.h"
#include "lib_mcu/usb/usb_drv.h"
#include "usb_descriptors.h"
#include "modules/usb/device_chap9/usb_standard_request.h"
#include "usb_specific_request.h"
#include "lib_mcu/util/start_boot.h"
#include "rf_drv.h"
#define F_CPU 8000000UL
#include <util/delay.h>

U8 buferis[64];

struct Tref_point
{
	unsigned char number;
	unsigned short temperature;
	unsigned short time;
};
struct Tref_point refPoints[10];

unsigned char ref_number=0;
volatile U8    cpt_sof=0;
extern            U8    jump_bootloader;

void hid_report_out  (void);
void hid_report_in   (void);
bool USB_Send(U8 *buff,unsigned char len);
bool USB_Read(U8 *buff,unsigned char len);


void hid_task_init(void)
{
	int i,j;
	//sviesos diodu isvadai nustatomi kaip isejimai
	DDRD|=(1<<PD1)|(1<<PD2);
	//isjungiami abu sviesos diodai
	PORTD&=~((1<<PD1)|(1<<PD2));

	RF_PORT_int();//initializuojami isvadai radijo moduliui
	RFM01_init();//initializuojamas radijo modulis
	TCCR1A=0;//taimeris nenaudoja i/o portu
	TCCR1B=0;//sustabdomas taimeris
	TCNT1=0;//apnulinamas skaitliukas

		for (i=0;i<10;i++)
				{
					refPoints[i].number=0;
					refPoints[i].time=0;
					refPoints[i].temperature=0;
				}
				ref_number=0;
}

//parity bito skaiciavimo paprograme
unsigned char parity(unsigned short word)
{
	unsigned char count=1;
	unsigned short i;
	for (i=0;i<16;i++)
    {
                if ((word&1)==1)
			count++;
                word>>=1;
    }
	return (count%2);
}



inline void test_timeout(void)
{
	unsigned char buff[64],i=0;
	//Ar praejo nustatytas laikas? (90ms) ar gauti duomenys is visu atraminiu tasku?
	if (((TIFR1&1<<TOV1)>0))//||(ref_number==4))
	{
		//trumpam ijungiamas geltonas sviesos diodas
	//	PORTD|=1<<PD1;
	//	_delay_ms(10);
		PORTD&=~(1<<PD1);

		PORTD&=~(1<<PD2);
		TIFR1=1<<TOV1;
		TCCR1B=0;//sustabdomas taimeris
		buff[0]=12;//uzrasomas status baitas
		buff[1]=ref_number;//uzrasomas atraminiu tasku kiekis
		//sudedami atraminiu tasku duomenys
		for (i=0;i<ref_number;i++)
		{
			buff[2+i*5]=refPoints[i].number;
			buff[2+i*5+1]=refPoints[i].time>>8;
			buff[2+i*5+2]=refPoints[i].time;
			buff[2+i*5+3]=refPoints[i].temperature>>8;
			buff[2+i*5+4]=refPoints[i].temperature;
		}
		//pripildytas masyvas issiunciamas per USB sasaja
		USB_Send(buff,64);
		//apnulinamas atraminiu tasku kiekis
	/*	ref_number=0;
		//isvalomi atraminiu tasku duomenys
		for (i=0;i<10;i++)
		{
			refPoints[i].number=0;
			refPoints[i].time=0;
			refPoints[i].temperature=0;
		}*/
	}	
}
inline void blink_led(void)
{
	static unsigned int i=0;
	//jei tai 50 tukstantasis ciklas
	if (i++>50000)
	{
		//apnulinamas skaitliukas
		i=0;
	//	PORTD&=~(1<<PD2);	//isjungiamas raudonas sviesos diodas
	}
	//jei tai daugiau nei 49950 ciklas
	if (i>49950);
	//	PORTD|=(1<<PD2);	//ijungiamas raudonas sviesos diodas
}

void UI_task(void)
{
	unsigned char buff[64],j,i;
	char found_number=-1;
	unsigned short packet;


//	blink_led();

	test_timeout();

	//ar gauti duomenys radijo bangomis?
	if(!nIRQ_PIN)
	{
		//nuskaitomi status bitai ir gauti duomenys
		unsigned long temp = RMFM01_readFIFO();
	


		unsigned char a,b;
		a=temp&0xFF;
		b=(temp>>8)&0xFF;
		packet=(b<<8)|a;

		
		buff[0]=14;
		buff[1]=temp; //low
		buff[2]=temp>>8; //high
		buff[3]=temp>>16;
		buff[4]=temp>>24;
		buff[5]=packet>>8;
		buff[6]=packet;
				
	//is priimtu duomenu isskiriamas informacija nesantis paketas
	  	unsigned char nr,tl,ao;
        unsigned short value;
        ao=(packet>>15)&0x01;
        nr=(packet>>13)&0x03;
        tl=(packet>>12)&0x01;
        value=(packet>>1)&2047;


		buff[7]=0;
		buff[8]=ao;
		buff[9]=nr;
		buff[10]=tl;
		buff[11]=((value&2047)>>8);
		buff[12]=value&0xFF;

		if (((temp>>24)&128)!=0)
		{
			//ar tai objektas?
			if (packet==0x1454)		
			{
				TCCR1A=0;//taimeris nenaudoja i/o portu
				TCCR1B=0;//sustabdomas taimeris
				TCNT1=65536-12500-6250;//apnulinamas skaitliukas
				TIFR1|=1<<TOV1;
				TCCR1B=(1<<CS11)|(1<<CS10); //clkio/64
				PORTD|=(1<<PD2);
				ref_number=0;
			}	
			else
			//ar tai atraminis taskas?
			if (ao==1)
			{
				found_number=-1;
				//ieskoma, ar masyve dar nera duomenu apie si atramini taska
				for (j=0;j<ref_number;j++)
				{
					//ar masyvo j elemento numeris sutampa su gautu atraminio tasko numeriu?
					if (refPoints[j].number==nr)
					{
						found_number=j;
						//stabdomas paieskos ciklas
						break;
					}
				}
				buff[0]=4;
				//jei masyve duomenu apie si atramini taska nerasta
				if (found_number<0)
				{
					//i masyva irasomas atraminio tasko numeris
					refPoints[ref_number].number=nr;
					//jei siunciamas skaicius laikas
					if (tl==0)
						//i masyva issaugomas atraminio tasko laikas
						refPoints[ref_number].time=value;
					else
						//i masyva issaugoma atraminio tasko temperatura
						refPoints[ref_number].temperature=value;
					//padidinamas turimu atraminiu tasku duomenu kiekio masyvas
					ref_number++;	
					buff[0]=5;
				}
				//jei masyve duomenu apie si atramini taska rasta
				else
				{
					//jei siunciamas skaicius - laikas
					if (tl==0)
						//i masyva issaugomas atraminio tasko laikas
						refPoints[j].time=value;
					else
						//i masyva issaugoma atraminio tasko temperatura
						refPoints[j].temperature=value;
					buff[0]=6;
				}
			}
		}
	//	USB_Send(buff,64);
		
	}
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
		if(buferis[0]==1)
		{
		
			WriteCMD(0xC0B1|(buferis[1]<<1));
			buferis[0]=33;
			buferis[1]=11;
			USB_Send(buferis,64);
		}
		if (buferis[0]=2)
		{
			WriteCMD((buferis[1]<<8)|(buferis[0]));//frequency
		}
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
