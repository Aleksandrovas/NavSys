// UsbHidSmallDemoCode.cpp : Defines the entry point for the console application.
//

////////////////////////////////////////////////////////////////////////////////
//  This demo program exercises the functions of the AtUsbHid library.
//  The library functions prototypes can be found in the last part of the
//   AtUsbHid.h file.
//
//    We perform the following:
//    1 - Load AtUsbHid Dll
//    2 - Load AtUsbHid function present in the DLL
//    3 - Opening the device using VID and PID define in the present source code
//    4 - Switch on and off Leds until button is pressed
//    5 - Close the Device once button as been pressed
//    6 - freed Dll


#include "stdafx.h"
#include <windows.h>

#include "AtUsbHid.h"

///////////////////////////////////////////
// USB HID device Vendor ID and Product ID.
#define VID 0x03EB
#define PID_1 0x2011
#define PID_2 0x2013

int main(int argc, char* argv[])
{
	
	// Read Buffer
	unsigned char sbuffer[256];

	// Write Buufer used to set leds
	unsigned char	leds[9];
	
	// Handle to AtUsbHid.dll
	HINSTANCE hLib = NULL;  // Handle to our USB HID DLL.
	
	printf("Atmel USB HID Library Test Program\n\n\n\n");


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
	while(DYNCALL(readData(sbuffer))==0)
	{
			// switch led 1 on
			leds[0]='1';
			leds[1]='1';
			DYNCALL(writeData)(leds);
			Sleep(100);

			// switch led 2 on
			leds[0]='1';
			leds[1]='2';		
			DYNCALL(writeData)(leds);
			Sleep(100);

			// switch led 3 on
			leds[0]='1';
			leds[1]='3';		
			DYNCALL(writeData)(leds);
			Sleep(100);

			// switch led 4 on
			leds[0]='1';
			leds[1]='4';		
			DYNCALL(writeData)(leds);
			Sleep(100);

			// switch led 1 off
			leds[0]='0';
			leds[1]='1';
			DYNCALL(writeData)(leds);
			Sleep(100);

			// switch led 2 off
			leds[0]='0';
			leds[1]='2';		
			DYNCALL(writeData)(leds);
			Sleep(100);

			// switch led 3 off
			leds[0]='0';
			leds[1]='3';		
			DYNCALL(writeData)(leds);
			Sleep(100);
			
			// switch led 4 off
			leds[0]='0';
			leds[1]='4';		
			DYNCALL(writeData)(leds);
			Sleep(100);
	}

	if(sbuffer[0]==1) {
		printf(">>> Button as been pressed, leave the application\n");
	}
	else {
		printf(">>> Button as been released\n");
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

