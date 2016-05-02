//==============================================================================
//
// Title:		Nav_CVI
// Purpose:		A short description of the application.
//
// Created on:	4/23/2016 at 20:55:47 by Taspats.
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#define VID 0x03EB
#define PID 0x2013
#define true  1
#define false 0
#define wait_delay 140
typedef char bool;

#include "windows.h"
#include <rtutil.h>
#include <ansi_c.h>
#include <utility.h>
#include <cvirte.h>		
#include <userint.h>
#include "Nav_CVI.h"
#include "toolbox.h"
#include "AtUsbHid.h"
#include <formatio.h>
#include <cvirte.h>


//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle = 0;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
static unsigned char buferis[64]={0};
static unsigned char DATA_OK=0;
static unsigned char Connected=false;
static unsigned char Switch=0; 

//==============================================================================
// Global functions
//--------------------------------------------------------------
//Ieskoma ir jungiamasi prie USB
//--------------------------------------------------------------
unsigned char connect(const UINT VendorID, const UINT ProductID)
{
  unsigned char ans=0;
  ans= findHidDevice(VendorID, ProductID);
  if (!ans)
  {
	    SetCtrlAttribute (panelHandle, PANEL_USB_MSG, ATTR_TEXT_COLOR , VAL_RED);
		SetCtrlVal(panelHandle, PANEL_USB_MSG, "USB device disconnected");
		Connected=false; 
  }
  else
  {
	  	SetCtrlAttribute (panelHandle, PANEL_USB_MSG, ATTR_TEXT_COLOR , VAL_GREEN);
		SetCtrlVal(panelHandle, PANEL_USB_MSG, "USB device connected");
		Connected=true; 

  }
  return  ans;
}
//==============================================================================
//---------------------------------------
//Atjungiamas USB
//---------------------------------------
void disconnect()
{
	if (Connected==true)  
	{
	    closeDevice();
		SetCtrlAttribute(panelHandle, PANEL_USB_TIMER,ATTR_ENABLED,false); 
		SetCtrlAttribute (panelHandle, PANEL_USB_MSG, ATTR_TEXT_COLOR , VAL_YELLOW);
		SetCtrlVal(panelHandle, PANEL_USB_MSG, "USB Disconnected");
	}
		
}
//============================================================================== 
//---------------------------------------
//Tikrinama ar USB prijungtas
//---------------------------------------
unsigned char test_alive()
{
	unsigned char buf[64];
	int k=0;
	buf[0]=254;
	writeData(buf);
		while((readData(buf)==false) && (k++<wait_delay)) Sleep(1);
	return (k<wait_delay);
}



//============================================================================== 
/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "Nav_CVI.uir", PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());
    
	//--------------------------------------------------------------------------
	connect(VID, PID);
	//--------------------------------------------------------------------------
	
Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	if (Connected==true)
	{
//		closeDevice();
	}
	
	return 0;
}
//==============================================================================
// Connect to Atmel USB HID Device
//==============================================================================
int CVICALLBACK Connect_USB_BUTTON (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			if (Connected==false)
			{
				connect(VID, PID);
				if (Connected==true)
					{
					SetCtrlAttribute(panelHandle, PANEL_USB_TIMER,ATTR_ENABLED,true);
					SetCtrlAttribute(panelHandle, PANEL_Connect_USB,ATTR_LABEL_TEXT,"Disconnect_USB");
					}
			}
			else if (Connected==true)
			{	
				SetCtrlAttribute(panelHandle, PANEL_USB_TIMER,ATTR_ENABLED,false); 
				SetCtrlAttribute(panelHandle, PANEL_Connect_USB,ATTR_LABEL_TEXT,"Connect_USB"); 
				disconnect();
				Connected=false; 
			}
			break;
	}
	return 0;
}
//==============================================================================
// Disconnect Atmel USB HID Device
//==============================================================================
int CVICALLBACK Disconnect_USB_BUTTON (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			disconnect();
			break;
	}
	return 0;
}

int CVICALLBACK USB_TIMER (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	static unsigned char pos0=0;
	static unsigned char LINE_BUFF[64]; 
	static int kkk=0,i=0,j=0;
	static int laikas[8],temp[8],nr[8];
   	float atstumas=0;
	buferis[0]=0;
	buferis[1]=0;
	kkk=0;	
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
	for (i = 0; i < 4; i++) {
	 laikas[i]=0;
	 temp[i]=0;
	 nr[i]=0;
	}
	for (i = 0; i < 64; i++) {  
	buferis[i] = 0; }
   if (Connected==true)
   {
	//read data from USB, with wait_delay timeout in ms
		while((readData(buferis)==false) && (kkk++<wait_delay))
		{
			Sleep(1);
		}
	  //no timeout?
	  for  (int i=0;i<64;i++)
	  
	  
	  {
	  
	//   Fmt (LINE_BUFF,"%x \n",buferis[i]); 
	//	 SetCtrlVal(panelHandle, PANEL_TEXTBOX,LINE_BUFF);	
	  // buferis[i]=0;
	  }
		  if (kkk<wait_delay)
		  {   // check if first byte is 12
			  if (buferis[0]==12)
			  {
				  
				  
				  for (int i=0; i<32; i++)
				  {
					  Fmt (LINE_BUFF,"%x:",buferis[i]); 
		              SetCtrlVal(panelHandle, PANEL_TEXTBOX,LINE_BUFF);
				  }
				    Fmt (LINE_BUFF,"\n"); 
		              SetCtrlVal(panelHandle, PANEL_TEXTBOX,LINE_BUFF);
					//read data from the packet
					//for (int i=0;i<buferis[1];i++)
					//{
						 //	atstumas=(buferis[2+i*5+1]*256+buferis[2+i*5+2])*3.387*10000/61379+12.108;
					 		//	DATA_OK=true;
							  // if (buferis[2+0*5]==0) {
							//	nr[0]= buferis[2+0*5+1];
							//	laikas[0]=buferis[2+0*5+1]*256+buferis[2+0*5+2];
							//	temp[0]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
							//   }
							   /*
								if (buferis[2+i*5]==1) {
								nr[1]= buferis[2+i*5];
								laikas[1]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
								temp[1]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
							   }
							   if (buferis[2+i*5]==2) {
								nr[2]= buferis[2+i*5];
								laikas[2]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
								temp[2]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
							   }
							   if (buferis[2+i*5]==3) {
								nr[3]= buferis[2+i*5];
								laikas[3]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
								temp[3]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
							   }
							   if (buferis[2+i*5]==4) {
								nr[4]= buferis[2+i*5];
								laikas[4]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
								temp[4]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
							   }
							   if (buferis[2+i*5]==5) {
								nr[5]= buferis[2+i*5];
								laikas[5]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
								temp[5]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
							   }
							   if (buferis[2+i*5]==6) {
								nr[6]= buferis[2+i*5];
								laikas[6]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
								temp[6]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
							   }
							   if (buferis[2+i*5]==7) {
								nr[7]= buferis[2+i*5];
								laikas[7]=buferis[2+i*5+1]*256+buferis[2+i*5+2];
								temp[7]=buferis[2+i*5+3]*256+buferis[2+i*5+4];
							   }*/	
							
						
						   	    
					//}
					
			  }
			 
		  }
		 
		  else {break;}
   }
 // print recieved data in text box
   		  if (DATA_OK==true)
		  {
   			for (int i=0;i<8;i++)
				{
				   if (laikas[i]>0)
				   {   
					   SetCtrlVal(panelHandle, PANEL_TEXTBOX,"----------------------------------------\n");
					   atstumas=((float)laikas[i]*3.387*10000/61379+12.108);
					   Fmt (LINE_BUFF,"NR: %d L: %d T: %d A: %f \n",nr[i],laikas[i],temp[i],atstumas);  
					   SetCtrlVal(panelHandle, PANEL_TEXTBOX,LINE_BUFF);
				   }	   
				}
			DATA_OK=false;	
		  }	
   	     		
			break;
	}
	return 0;
}
