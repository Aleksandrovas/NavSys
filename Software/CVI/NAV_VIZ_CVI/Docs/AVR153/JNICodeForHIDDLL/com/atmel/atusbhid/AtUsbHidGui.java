package com.atmel.atusbhid;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.UIManager;
import javax.swing.border.TitledBorder;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.reflect.InvocationTargetException;

import com.atmel.atusbhidjni.AtUsbHidJni;;
/**
 * The Class AtUsbHidGui.
 */
public class AtUsbHidGui implements Runnable
{

  /** The usb device. */
  private AtUsbHidJni         usbDevice        = null;

  /** The j frame. */
  private JFrame              jFrame           = null;

  /** The j panel button. */
  private AtUsbHidPanelButton jPanelButton     = null;

  /** The j content pane. */
  private JPanel              jContentPane     = null;

  /** The j panel text. */
  private JPanel              jPanelText       = null;

  /** The j panel led. */
  private AtUsbHidPanelLed    jPanelLed        = null;

  /** The j scroll pane. */
  private JScrollPane         jScrollPane      = null;

  /** The j text area. */
  private JTextArea           jTextArea        = null;

  /** The j panel status. */
  private JPanel              jPanelStatus     = null;

  /** The j label status. */
  private JLabel              jLabelStatus     = null;

  /** The device connected. */
  private boolean             deviceConnected  = false;

  /** The auto connect. */
  private boolean             autoConnect      = false;

  /** The j label buffer info. */
  private JLabel              jLabelBufferInfo = null;

  /**
   * Instantiates a new at usb hid gui.
   */
  public AtUsbHidGui()
  {
    super();
    String libraryPath = System.getProperty("user.dir") + "\\AtUsbHidJni";
    try
    {
      usbDevice = new AtUsbHidJni();
    }
    catch (UnsatisfiedLinkError ex)
    {
      System.err.println("AtUsbHid.AtUsbHid():UnsatisfiedLinkError");
      JOptionPane.showMessageDialog(null,
          "AtUsbHid.AtUsbHid():UnsatisfiedLinkError\n" + "Can't find "
              + libraryPath + ".dll", "AtUsbHidJni.java error",
          JOptionPane.ERROR_MESSAGE);
      System.exit(0);
    } catch (SecurityException ex)
    {
      JOptionPane.showMessageDialog(null,
          "AtUsbHid.AtUsbHid():SecurityException", "AtUsbHidJni.java error",
          JOptionPane.ERROR_MESSAGE);
      System.exit(0);
    } catch (NullPointerException ex)
    {
      JOptionPane.showMessageDialog(null,
          "AtUsbHid.AtUsbHid():NullPointerException", "AtUsbHidJni.java error",
          JOptionPane.ERROR_MESSAGE);
      System.err.println("AtUsbHid.AtUsbHid():NullPointerException");
      System.exit(0);
    }

    if(usbDevice.loadLibraryUsbHid() == false)
    {
      JOptionPane.showMessageDialog(null, "DemoAtUsbHid.main():LoadLibrary",
          "AtUsbHidGui.java error", JOptionPane.ERROR_MESSAGE);
      System.exit(0);
    }
  }

  /*
   * (non-Javadoc)
   * 
   * @see java.lang.Runnable#run()
   */
  public void run()
  {
    getJFrame().pack();
    getJFrame().setVisible(true);
  }

  /**
   * This method initializes jFrame.
   * 
   * @return javax.swing.JFrame
   */
  private JFrame getJFrame()
  {
    if(jFrame == null)
    {
      jFrame = new JFrame();
      jFrame.setSize(new Dimension(510, 280));
      jFrame.setResizable(false);
      jFrame.setTitle("AtHidDll Jni demo code");
      jFrame.setContentPane(getJContentPane());
      jFrame.addWindowListener(new java.awt.event.WindowAdapter()
      {
        public void windowClosing(java.awt.event.WindowEvent e)
        {
          if(deviceConnected == true)
          {
            setDeviceConnected(false);
          }
          usbDevice.UnloadloadLibraryUsbHid();
          System.exit(0);
        }
      });
      centerJFrame();
    }
    return jFrame;
  }

  /**
   * Center J frame.
   */
  private void centerJFrame()
  {
    Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
    Dimension frameSize = getJFrame().getSize();
    if(frameSize.height > screenSize.height)
    {
      frameSize.height = screenSize.height;
    }
    if(frameSize.width > screenSize.width)
    {
      frameSize.width = screenSize.width;
    }

    getJFrame().setLocation((screenSize.width - frameSize.width) / 2,
        (screenSize.height - frameSize.height) / 2);
  }

  /**
   * This method initializes jContentPane.
   * 
   * @return javax.swing.JPanel
   */
  private JPanel getJContentPane()
  {
    if(jContentPane == null)
    {
      jContentPane = new JPanel();
      jContentPane
          .setLayout(new BoxLayout(getJContentPane(), BoxLayout.Y_AXIS));
      jPanelButton = new AtUsbHidPanelButton(this);
      jContentPane.add(jPanelButton.getJPanelButton(), null);
      jContentPane.add(getJPanelText(), null);

      jPanelLed = new AtUsbHidPanelLed(this);
      jContentPane.add(jPanelLed.getJPanelLed(), null);
      jContentPane.add(getJPanelStatus(), null);
      setControlEnable(false); // Just enable Connect
      TaskReadText();
    }
    return jContentPane;
  }

  /**
   * This method initializes jPanelText.
   * 
   * @return javax.swing.JPanel
   */
  private JPanel getJPanelText()
  {
    if(jPanelText == null)
    {
      jPanelText = new JPanel();
      jPanelText.setLayout(new FlowLayout());
      jPanelText.add(getJScrollPane(), null);
    }
    return jPanelText;
  }

  /**
   * This method initializes jTextArea.
   * 
   * @return javax.swing.JTextArea
   */
  private JTextArea getJTextArea()
  {
    if(jTextArea == null)
    {
      jTextArea = new JTextArea();
    }
    return jTextArea;
  }

  /**
   * This method initializes jPanelStatus.
   * 
   * @return javax.swing.JPanel
   */
  private JPanel getJPanelStatus()
  {
    if(jPanelStatus == null)
    {
      jLabelBufferInfo = new JLabel("");
      jLabelStatus = new JLabel("Not Connected");
      jLabelStatus.setFont(new Font("Courier New", Font.BOLD, 14));
      jLabelStatus.setForeground(Color.blue);
      jPanelStatus = new JPanel();
      jPanelStatus.setLayout(new BorderLayout());
      jPanelStatus.setBorder(BorderFactory.createTitledBorder(null, "Status",
          TitledBorder.DEFAULT_JUSTIFICATION, TitledBorder.DEFAULT_POSITION,
          new Font("Dialog", Font.BOLD, 12), new Color(51, 51, 51)));
      jPanelStatus.add(jLabelStatus, BorderLayout.WEST);
      jPanelStatus.add(jLabelBufferInfo, BorderLayout.EAST);
    }
    return jPanelStatus;
  }

  /**
   * Sets the control enable.
   * 
   * @param b
   *                the new control enable
   */
  private void setControlEnable(boolean b)
  {
    jPanelLed.setCommunicationEnable(b);
    jPanelButton.setCommunicationEnable(b);
  }

  /**
   * Sets the led.
   * 
   * @param ledNum
   *                the led num
   * @param ledState
   *                the led state
   */
  public void setLed(int ledNum, boolean ledState)
  {
    byte[] byteArray = new byte[usbDevice.getOutputReportLength()];
    for (int i = 0; i < byteArray.length; i++)
    {
      byteArray[i] = (byte) 0;
    }

    byteArray[1] = (byte) (ledNum+'0');

    // Manage Led
    if(ledState == true)
    {
      byteArray[0] = (byte)'1';
      jLabelStatus.setText("Led " + ledNum + " is on");
    } else
    {
      byteArray[0] = (byte)'0';
      jLabelStatus.setText("Led " + ledNum + " is off");
    }
    if(usbDevice.writeData(byteArray) == false)
    {
      setDeviceConnected(false);
      jLabelStatus.setText("Connection to device lost during led setting");
    }
  }

  /**
   * Task read text.
   */
  private void TaskReadText()
  {
    Timer timer;
    timer = createTimer();
    timer.start();
  }

  /**
   * Creates the timer.
   * 
   * @return the timer
   */
  private Timer createTimer()
  {
    ActionListener action = new ActionListener()
    {
      public void actionPerformed(ActionEvent event)
      {
        if(deviceConnected == true)
        {
          // Check if there is new data to read
          byte data[] = usbDevice.readData();
          if(data.length > 0)
          {
            if(data[0] == 1)
            {
              getJTextArea().append("Button as been pressed\n");
            } else
            {
              getJTextArea().append("Button as been released\n");
            }
            jLabelStatus.setText("New action on target");
          }
          // Check if connection is still alive
          byte[] buffer = new byte[usbDevice.getOutputReportLength()];
          for (int i = 0; i < buffer.length; i++)
          {
            // Init Array
            buffer[i] = 0;
          }
          if(buffer.length >= 4)
          {
            buffer[0] = (byte) 0x55;
            buffer[1] = (byte) 0xAA;
            buffer[2] = (byte) 0x55;
            buffer[3] = (byte) 0x55;
          }
          if(usbDevice.setFeature(buffer) == false)
          {
            setDeviceConnected(false);
            jLabelStatus.setText("Connection to device lost");
          }
        } else
        {
          if(autoConnect == true)
          {
            int state = usbDevice.findHidDevice(jPanelButton.getVid(),
                jPanelButton.getPid());
            if(state == 1)
            {
              setDeviceConnected(true);
            }
          }
        }
      }
    };

    return new Timer(100, action);
  }

  /**
   * Sets the device connected.
   * 
   * @param b
   *                the new device connected
   */
  public void setDeviceConnected(boolean b)
  {
    jLabelBufferInfo.setText("");
    if(b == true)
    {
      int state = usbDevice.findHidDevice(jPanelButton.getVid(), jPanelButton
          .getPid());
      switch (state)
      {
      case 1: // Find Device
        jPanelButton.setCommunicationEnable(false);
        setControlEnable(true);
        // New connection set led state to off
        setLed(1, false);
        setLed(2, false);
        setLed(3, false);
        setLed(4, false);
        if(autoConnect == false)
        {
          jLabelStatus.setText("Connected to the device");
        } else
        {
          jLabelStatus.setText("Auto-connected to the device");
        }
        jLabelBufferInfo.setText("Buffer Size, In:"
            + usbDevice.getInputReportLength() + "Byte, Out="
            + usbDevice.getOutputReportLength() + "Byte, Feature"
            + usbDevice.getFeatureReportLength() + "Byte");
        deviceConnected = true;
        break;
      case -1: // ERROR_USB_DEVICE_NOT_FOUND
        if(autoConnect == false)
        {
          JOptionPane.showMessageDialog(getJFrame(), "Usb Device Not Found",
              "AtUsbHidGui.java error", JOptionPane.ERROR_MESSAGE);
        }
        jLabelStatus.setText("Usb Device Not Found");
        deviceConnected = false;
        break;
      case -2: // ERROR_USB_DEVICE_NO_CAPABILITIES:
        if(autoConnect == false)
        {
          JOptionPane.showMessageDialog(getJFrame(), "Usb no capabilities",
              "AtUsbHidGui.java error", JOptionPane.ERROR_MESSAGE);
        }
        jLabelStatus.setText("Usb no capabilities");
        deviceConnected = false;
        break;
      case 0: // unknown Error
      default:
        if(autoConnect == false)
        {
          JOptionPane.showMessageDialog(getJFrame(), "Usb Unknown Error",
              "AtUsbHidGui.java error", JOptionPane.ERROR_MESSAGE);
        }
        jLabelStatus.setText("Usb Unknown Error");
        deviceConnected = false;
        break;
      }

    } else
    {
      usbDevice.closeDevice();
      setControlEnable(false);
      jLabelStatus.setText("Disconneted from the device");
      deviceConnected = false;
    }
  }

  /**
   * Sets the auto connect.
   * 
   * @param b
   *                the new auto connect
   */
  public void setAutoConnect(boolean b)
  {
    autoConnect = b;
  }

  /**
   * Start isp.
   */
  public void startIsp()
  {
    // Check if connection is still alive
    byte[] buffer = new byte[usbDevice.getOutputReportLength()];
    for (int i = 0; i < buffer.length; i++)
    {
      // Init Array
      buffer[i] = 0;
    }
    if(buffer.length >= 4)
    {
      buffer[0] = (byte) 0x55;
      buffer[1] = (byte) 0xAA;
      buffer[2] = (byte) 0x55;
      buffer[3] = (byte) 0xAA;
    }
    if(usbDevice.setFeature(buffer) == true)
    {
      setDeviceConnected(false);
      jLabelStatus.setText("Isp mode enable, you can run flip");
    } else
    {
      JOptionPane.showMessageDialog(getJFrame(), "Not able to start ISP mode",
          "AtUsbHidGui.java error", JOptionPane.ERROR_MESSAGE);
    }
  }

  /**
   * This method initializes jScrollBar.
   * 
   * @return javax.swing.JScrollBar
   */
  private JScrollPane getJScrollPane()
  {
    if(jScrollPane == null)
    {
      jScrollPane = new JScrollPane(getJTextArea());
      jScrollPane.setPreferredSize(new Dimension(490, 120));
    }
    return jScrollPane;
  }

  /**
   * The main method.
   * 
   * @param args
   *                the args
   */
  public static void main(String[] args)
  {
    Runnable gui = new AtUsbHidGui();

    try
    {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
    } catch (Exception e)
    {
      e.printStackTrace();
    }

    try
    {
      SwingUtilities.invokeAndWait(gui);
    } catch (InvocationTargetException ex)
    {
      ex.printStackTrace();
    } catch (InterruptedException ex)
    {
      ex.printStackTrace();
    }

  }

}
