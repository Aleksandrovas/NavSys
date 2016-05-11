
#include "stdafx.h"
#include <windows.h>

#include "AtUsbHid.h"

///////////////////////////////////////////
// USB HID device Vendor ID and Product ID.
#define VID		0x03EB
#define PID_1	0x2011
#define PID_2	0x2013

#define Vug			338.7		// Ultrasound Speed in Air @ 20C [m/s]
#define TimerClk	10000000	// Timer base Clock [Hz]
#define TimerClkDiv	8			// Timer base Clock devider
#define Prescaler	16			// Prescaler used to reduce ToF bits


int main(int argc, char* argv[])
{
	UINT8 sbuffer[64];
	UINT16 temp;
	FLOAT ToF[8];
	FLOAT Distance[8];
	
	// Handle to AtUsbHid.dll
	HINSTANCE hLib = NULL;  // Handle to our USB HID DLL.
	
	///////////////////////////////////////////////////////////////////////////
    // Explicitely load the AtUsbHid library.
    printf(">>> Loading USB HID Dll.\n");

    hLib = LoadLibrary(AT_USB_HID_DLL);
    if (hLib != NULL) {
        printf(">>> USB HID Dll loaded\n");
	}
    else {
        if(GetLastError()==ERROR_MOD_NOT_FOUND) {
			printf("Error: Could not find the Atmel USB HID Dll.\n");
			printf("       Please update the PATH variable.\n");
		}
		else {
			printf("Error: While opening Dll.\n");
		}
        return 1;
    }

	///////////////////////////////////////////////////////////////////////////
    // Get USB HID library functions addresses.
    printf(">>> Loading all Dll functions.\n");
    if (loadFuncPointers(hLib)) {
        printf(">>> All function of the Dll has been loaded\n");
	}
    else {
		printf("Error: Could not find load function of the Dll.\n");
        return 1;
	}

	///////////////////////////////////////////////////////////////////////////
    // Open our USB HID device.
    printf(">>> Opening USB HID device with Vendor ID= 0x%04X and Product ID=0x%04X or ID=0x%04X.\n", VID, PID_1,PID_2);
    
	if (DYNCALL(findHidDevice)(VID, PID_2)) {
		printf(">>> USB HID device  VID=0x%04X, PID=0x%04X opened.\n", VID, PID_1);
	}
    else {
		switch(GetLastError()) {
			case ERROR_USB_DEVICE_NOT_FOUND :
				printf("Error: Could not open the device.\n");
				break;
			case ERROR_USB_DEVICE_NO_CAPABILITIES:
				printf("Error: Could not get USB device capabilities.\n");
				break;
			default:
				printf("Error: While opening device.\n");
				break;
		}
		if (DYNCALL(findHidDevice)(VID, PID_2)) {
			printf(">>> USB HID device  VID=0x%04X, PID=0x%04X opened.\n", VID, PID_2);
		}
		else {
			switch(GetLastError()) {
				case ERROR_USB_DEVICE_NOT_FOUND :
					printf("Error: Could not open the device.\n");
					break;
				case ERROR_USB_DEVICE_NO_CAPABILITIES:
					printf("Error: Could not get USB device capabilities.\n");
					break;
				default:
					printf("Error: While opening device.\n");
					break;
			}
        
	    }    
    }

	// Print Some information about the connected device
	printf(">>> USB HID Input   Buffer size is %dByte.\n",DYNCALL(getInputReportLength()));
	printf(">>> USB HID Output  Buffer size is %dByte.\n",DYNCALL(getOutputReportLength()));
	printf(">>> USB HID Feature Buffer size is %dByte.\n",DYNCALL(getFeatureReportLength()));


	// While button has not been pressed 
	while(1)	//DYNCALL(readData(sbuffer))==0)
	{
		Sleep(100);

		if (DYNCALL(readData(sbuffer)))
		{
			for(UINT8 indx=0;indx<8;indx++)
			{
				temp = (sbuffer[2*indx]<<8) | (sbuffer[2*indx+1]);
				ToF[indx] = ((FLOAT)temp*(FLOAT)Prescaler) * (1/((FLOAT)TimerClk/(FLOAT)TimerClkDiv));
				Distance[indx] = ToF[indx]*(FLOAT)Vug;
				
				printf("%d\t",temp);		// print Raw data				
				//printf("%2.3fms\t",ToF[indx]*1e3);	// print ToF [,s]		
				//printf("%2.4fm\t",Distance[indx]);	// print Distance [m]
			}
			printf("\n");
		}
	}

	// Close our USB HID device.	
    DYNCALL(closeDevice)();
    printf(">>> USB HID device VID=0x%04X closed.\n", VID);


	// Wait thread to be close before fee DLL
	Sleep(1000);
    FreeLibrary(hLib);
	printf(">>> Please press a key to exit");
	getchar();
	
	return 0;	
}

