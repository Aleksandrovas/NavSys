/*
 * 
 */
package com.atmel.atusbhid;

import com.atmel.atusbhidjni.AtUsbHidJni;


/**
 * The Class AtUsbHidSimple.
 */
public class AtUsbHidSimple
{

  /** The Constant VID.  Vendor Id: 0x03EB (Atmel Corporation) */
  static final int VID  = 0x03EB;
  
  /** The Constant PID1.  Product Id: 0x2013*/
  static final int PID1 = 0x2013;

  /** The Constant PID2. Product Id: 0x2011*/
  static final int PID2 = 0x2011;

  /**
   * The main method.
   * 
   * @param args the arguments
   */
  public static void main(String[] args)
  {
    
    // Create usbDevice object
    AtUsbHidJni usbDevice = new AtUsbHidJni();

    // Load the DLL AtUsbHidJni
    if(usbDevice.loadLibraryUsbHid() == true)
    {
      System.out.println("AtUsbHidSimple.main():LoadLibrary");
    } else
    {
      System.out.println("AtUsbHidSimple.main():not able to Load Library");
      System.exit(0);
    }

    // Connect to the device using VID and PID
    System.out.println("AtUsbHidSimple.main():Find device (VID=0x2013)");
    int state = usbDevice.findHidDevice(VID, PID1);
    switch (state)
    {
    case 1: // Find Device
      System.out.println("AtUsbHidSimple.main():Connected to the device");
      break;
    case -1: // ERROR_USB_DEVICE_NOT_FOUND
      System.err.println("AtUsbHidSimple.main():Usb Device Not Found");
      break;
    case -2: // ERROR_USB_DEVICE_NO_CAPABILITIES:
      System.err.println("AtUsbHidSimple.main():Usb no capabilities");
      break;
    case 0: // unknown Error
    default:
      System.err.println("AtUsbHidSimple.main():Usb Unknown Error");
      break;
    }

    if(1 != state)
    {
      // Connect to the device using VID and PID
      System.out.println("AtUsbHidSimple.main():Find device (VID=0x2011)");
      state = usbDevice.findHidDevice(VID, PID2);
      switch (state)
      {
      case 1: // Find Device
        System.out.println("AtUsbHidSimple.main():Connected to the device");
        break;
      case -1: // ERROR_USB_DEVICE_NOT_FOUND
        System.err.println("AtUsbHidSimple.main():Usb Device Not Found");
        System.exit(0);
        break;
      case -2: // ERROR_USB_DEVICE_NO_CAPABILITIES:
        System.err.println("AtUsbHidSimple.main():Usb no capabilities");
        System.exit(0);
        break;
      case 0: // unknown Error
      default:
        System.err.println("AtUsbHidSimple.main():Usb Unknown Error");
        System.exit(0);
        break;
      }
    }

    boolean stop = false;

    while (stop == false)
    {
      byte[] writeByteArray = new byte[usbDevice.getOutputReportLength()];
      byte[] readByteArray = null;

      // All Led Off
      delay(100);

      // Led1 on
      writeByteArray[0] = '1';
      writeByteArray[1] = '1';
      usbDevice.writeData(writeByteArray);
      delay(100);

      // Led1 on
      writeByteArray[1] = '2';
      usbDevice.writeData(writeByteArray);
      delay(100);

      // Led1 on
      writeByteArray[1] = '3';
      usbDevice.writeData(writeByteArray);
      delay(100);

      // Led1 on
      writeByteArray[1] = '4';
      usbDevice.writeData(writeByteArray);
      delay(100);

      // Led1 on
      writeByteArray[0] = '0';
      writeByteArray[1] = '1';
      usbDevice.writeData(writeByteArray);
      delay(100);

      // Led1 on
      writeByteArray[1] = '2';
      usbDevice.writeData(writeByteArray);
      delay(100);

      // Led1 on
      writeByteArray[1] = '3';
      usbDevice.writeData(writeByteArray);
      delay(100);

      // Led1 on
      writeByteArray[1] = '4';
      usbDevice.writeData(writeByteArray);
      delay(100);

      // Read Device
      readByteArray = usbDevice.readData();
      if(readByteArray.length != 0)
      {
        System.out.println("AtUsbHidSimple.main():Key pressed");
        stop = true;
      }

    }

    // Close Device before leave application
    System.out.println("AtUsbHidSimple.main():Close device");
    usbDevice.closeDevice();

  }

  /**
   * Delay.
   * 
   * @param delay the delay
   */
  static void delay(int delay)
  {
    try
    {
      Thread.sleep(delay);
    } catch (InterruptedException e)
    {
      e.printStackTrace();
    }
  }

}
