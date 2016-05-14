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
#define V_ug 338.7 

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
#include "USB2.h"
#include <formatio.h>
#include <cvirte.h>
#include "Trilateration.h" 

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables


static int panelHandle = 0;
static int TV_HANDLE = 0; 
static int T1_HANDLE = 0; 
//==============================================================================
// Static functions

//==============================================================================
// Global variables
static unsigned char buferis[64]={0};
static unsigned char DATA_OK=0;
static unsigned char Connected=false;
static unsigned char Switch=0; 
static unsigned char full=false; 
static unsigned char Skiped=false;  
//==============================================================================
// Global functions
//--------------------------------------------------------------
//Ieskoma ir jungiamasi prie USB
//--------------------------------------------------------------
unsigned char connect(const UINT VendorID, const UINT ProductID)
{
  unsigned char ans=0,i;
  
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
				full=false;
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
	static unsigned char LINE_BUFF[64]; 
	static unsigned char time_out=0,i=0,j=0;
	static int ring_POS;
	time_out=0;

	
	switch (event)
	{
		case EVENT_TIMER_TICK:
	
	    		
			
 /* If US is connected start data reading*/	
   if (Connected==true)
   {
	   	GetPanelHandleFromTabPage (panelHandle, PANEL_TAB, 1, &TV_HANDLE);
		GetPanelHandleFromTabPage (panelHandle, PANEL_TAB, 0, &T1_HANDLE);
		/*read data from USB, with wait_delay timeout in ms   */
		while((readData(buferis)==false) && (time_out++<wait_delay))
		{
			Sleep(1);
		}
	     	/*	if data from USB succesfully obtained   */ 
		  if (time_out<wait_delay)
		  {   
			  
				/*Write Raw data to text box*/
				 for (int i=0; i<32; i++)
				 {
				  Fmt (LINE_BUFF,"%x:",buferis[i]); 
		             SetCtrlVal(panelHandle, PANEL_TEXTBOX,LINE_BUFF);
				 }
				  Fmt (LINE_BUFF,"\n"); 
		          SetCtrlVal(panelHandle, PANEL_TEXTBOX,LINE_BUFF);  
					  
				/* Fill data structure with recieved data - first 2 bytes holds data from first reciever, 
					next two bytes from second and so on till 8 */	
				for (i=0;i<8;i++)
				{
				  T_data[full].point[i].flight_time=((buferis[i*2]&0xFF)<<8)+buferis[((i*2)+1)];
				  T_data[full].point[i].number=(unsigned char)i+1;
				}
				
				if (full<2)
				  {																	 
					  full++;
					  Skiped=false;
				  }
			    else
				{																				 
				 for (i=0;i<1;i++)
					 {
						
						for (j=0;j<8;j++)
						{
							if  ((abs((T_data[full].point[j].flight_time-T_data[1].point[j].flight_time))>0x1F)&&(Skiped==false)) 
							{
								T_data[full].point[j].flight_time=T_data[1].point[j].flight_time;	
								Skiped=true;
								full=false;
								
							}
						}
						
						if (Skiped==false)
						{ 
						T_data[1] =	T_data[full];
						}
						
					 }
				}
			  
				
		  }			  
		 /*Quit callback after time out*/
		  else
		  {
			  full=false;
			  break;
		  }
   }
 /* if data recieved calculate distances from recieved times and draw graphs(for debuging purpose) 		 */
   			for (int i=0;i<8;i++)
				{
				   if (T_data[full].point[i].flight_time>0)
				   {   
					   
						T_data[full].point[i].radius=(((float)T_data[full].point[i].flight_time*8*((1/(10000000.0/8.0))))*V_ug);
						
					 /* SetCtrlVal(panelHandle, PANEL_TEXTBOX,"----------------------------------------\n"); 
					   Fmt (LINE_BUFF,"NR: %d R:%f \n",T_data[full].point[i].number,T_data[full].point[i].radius);  
					   SetCtrlVal(panelHandle, PANEL_TEXTBOX,LINE_BUFF);	 */
					   if (i==0)
					   {
					   PlotStripChartPoint (TV_HANDLE, TABPANEL_V_STRIPCHART_T1, T_data[full].point[i].radius);
					   }
					   if (i==1)
					   {
					   PlotStripChartPoint (TV_HANDLE, TABPANEL_V_STRIPCHART_T2, T_data[full].point[i].radius);
					   }
					   if (i==2)
					   {
					   PlotStripChartPoint (TV_HANDLE, TABPANEL_V_STRIPCHART_T3, T_data[full].point[i].radius);
					   }
					   if (i==3)
					   {
					   PlotStripChartPoint (TV_HANDLE, TABPANEL_V_STRIPCHART_T4, T_data[full].point[i].radius);
					   }
					   if (i==4)
					   {
					   PlotStripChartPoint (TV_HANDLE, TABPANEL_V_STRIPCHART_T5, T_data[full].point[i].radius);
					   }
					   if (i==5)
					   {
					   PlotStripChartPoint (TV_HANDLE, TABPANEL_V_STRIPCHART_T6, T_data[full].point[i].radius);
					   }
					   if (i==6)
					   {
					   PlotStripChartPoint (TV_HANDLE, TABPANEL_V_STRIPCHART_T7, T_data[full].point[i].radius);
					   }
					   if (i==7)
					   {
					   PlotStripChartPoint (TV_HANDLE, TABPANEL_V_STRIPCHART_T8, T_data[full].point[i].radius);
					   }
				   }	   
			/* Drawing one selected point data on the graph*/	
			GetCtrlVal(T1_HANDLE, TABPANEL_T_RINGSLIDE,&ring_POS);
			if (T_data[full].point[ring_POS-1].flight_time>0)
				   { 
						PlotStripChartPoint (T1_HANDLE, TABPANEL_T_STRIPCHART_TV, T_data[full].point[(ring_POS-1)].radius);
						Fmt (LINE_BUFF,"Taskas: %d",T_data[full].point[(ring_POS-1)].number);  
						SetCtrlAttribute(T1_HANDLE, TABPANEL_T_STRIPCHART_TV, ATTR_LABEL_TEXT, LINE_BUFF);
				   }
		  }	
   	     		
			break;
	}
	return 0;
}
