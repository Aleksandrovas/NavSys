/**
 * 
 */
package com.atmel.atusbhid;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.text.ParseException;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.text.MaskFormatter;
import javax.swing.JCheckBox;


// TODO: Auto-generated Javadoc
/**
 * The Class AtUsbHidPanelButton.
 */
public class AtUsbHidPanelButton extends JPanel{
    
    /** The Constant VID. */
    static private final String VID            = "0x03EB";
    
    /** The Constant PID. */
    static private final String PID            = "0x2013";
    
    /** The Constant CONNECT. */
    static private final String CONNECT        = "Connect";
    
    /** The Constant DISCONNECT. */
    static private final String DISCONNECT     = "Disconnect";
    
    
    /** The Constant serialVersionUID. */
    private static final long serialVersionUID = 1381518020027414222L;
    
    /** The j panel button. */
    private JPanel              jPanelButton   = null;
    
    /** The j label VID. */
    private JLabel              jLabelVID      = null;
    
    /** The j text field VID. */
    private JFormattedTextField jTextFieldVID  = null;
    
    /** The j label PID. */
    private JLabel              jLabelPID      = null;
    
    /** The j text field PID. */
    private JFormattedTextField jTextFieldPID  = null;
    
    /** The j button connect. */
    private JButton             jButtonConnect = null;
    
    /** The j button ISP. */
    private JButton             jButtonISP     = null;
   
    /** The j parent. */
    private AtUsbHidGui         jParent         = null;
    
    /** The j check box auto connect. */
    private JCheckBox jCheckBoxAutoConnect = null;
    
    
    
    /**
     * Instantiates a new at usb hid panel button.
     * 
     * @param jParent the j parent
     */
    public AtUsbHidPanelButton(AtUsbHidGui jParent) {
        super();
        this.jParent = jParent;
    }
    
    /**
     * This method initializes jPanelButton.
     * 
     * @return javax.swing.JPanel
     */
    public JPanel getJPanelButton() {
        if (jPanelButton == null) {
            jLabelVID = new JLabel();
            jLabelVID.setText("VID");
            jPanelButton = new JPanel();
            jPanelButton.setLayout(new FlowLayout());
            jPanelButton.add(jLabelVID, null);
            jPanelButton.add(getJTextFieldVid(), null);
            jPanelButton.add(getJLabelPID(), null);
            jPanelButton.add(getJTextFieldPID(), null);
            jPanelButton.add(getJCheckBoxAutoConnect(), null);
            jPanelButton.add(Box.createHorizontalStrut(30));
            jPanelButton.add(getJButtonConnect(), null);
            jPanelButton.add(getJButtonISP(), null);
        }
        return jPanelButton;
    }
   
    
    /**
     * This method initializes jTextFieldVid.
     * 
     * @return javax.swing.JTextField
     */
    private JTextField getJTextFieldVid() {
        if (jTextFieldVID == null) {
            MaskFormatter formatter;
            try {
                formatter = new MaskFormatter("0x****");
                formatter.setValidCharacters("0123456789abcdefABCDEF");
                formatter.setPlaceholderCharacter('?');
                jTextFieldVID = new JFormattedTextField(formatter);
                jTextFieldVID.setAction(null);
            }
            catch (ParseException e) {
                jTextFieldVID = new JFormattedTextField();
                e.printStackTrace();
            }
            jTextFieldVID.setPreferredSize(new Dimension(50, 20));
            jTextFieldVID.setText(VID);

        }
        return jTextFieldVID;
    }

    /**
     * This method initializes jLabelPID.
     * 
     * @return javax.swing.JLabel
     */
    private JLabel getJLabelPID() {
        if (jLabelPID == null) {
            jLabelPID = new JLabel();
            jLabelPID.setText("PID");
        }
        return jLabelPID;
    }
    
    /**
     * This method initializes jTextFieldPID.
     * 
     * @return javax.swing.JTextField
     */
    private JTextField getJTextFieldPID() {
        if (jTextFieldPID == null) {
            MaskFormatter formatter;
            try {
                formatter = new MaskFormatter("0x****");
                formatter.setValidCharacters("0123456789abcdefABCDEF");
                formatter.setPlaceholderCharacter('?');
                jTextFieldPID = new JFormattedTextField(formatter);
            }
            catch (ParseException e) {
                jTextFieldPID = new JFormattedTextField();
                e.printStackTrace();
            }
            jTextFieldPID.setPreferredSize(new Dimension(50, 20));
            jTextFieldPID.setText(PID);
        }
        return jTextFieldPID;
    }
    
   
    
    /**
     * This method initializes jButtonConnect.
     * 
     * @return javax.swing.JButton
     */
    private JButton getJButtonConnect() {
        if (jButtonConnect == null) {
            jButtonConnect = new JButton();
            jButtonConnect.setPreferredSize(new Dimension(100, 26));
            jButtonConnect.setText(CONNECT);
            jButtonConnect.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    if(jButtonConnect.getText().contentEquals(CONNECT)) {
                        jParent.setDeviceConnected(true);
                    }
                    else {
                        jParent.setDeviceConnected(false);
                    }
                }
            });
        }
        return jButtonConnect;
    }
    
    /**
     * This method initializes jButtonISP.
     * 
     * @return javax.swing.JButton
     */
    private JButton getJButtonISP() {
        if (jButtonISP == null) {
            jButtonISP = new JButton();
            jButtonISP.setText("Firmware upgrade");
            jButtonISP.addActionListener(new java.awt.event.ActionListener() {
                public void actionPerformed(java.awt.event.ActionEvent e) {
                    jParent.startIsp();

                }
            });
        }
        return jButtonISP;
    }


    /**
     * Gets the vid.
     * 
     * @return the vid
     */
    public int getVid() {
        return  Integer.decode(getJTextFieldVid().getText());        
    }


    /**
     * Gets the pid.
     * 
     * @return the pid
     */
    public int getPid() {
        return Integer.decode(getJTextFieldPID().getText());
    }
    
    /**
     * Sets the communication enable.
     * 
     * @param b the new communication enable
     */
    public void setCommunicationEnable(boolean b) {
        getJButtonISP().setEnabled(b);
        if(b==false) {
            getJButtonConnect().setText(CONNECT);
        }
        else {
            getJButtonConnect().setText(DISCONNECT);
        }
    }

    /**
     * This method initializes jCheckBoxAutoConnect.
     * 
     * @return javax.swing.JCheckBox
     */
    private JCheckBox getJCheckBoxAutoConnect() {
        if (jCheckBoxAutoConnect == null) {
            jCheckBoxAutoConnect = new JCheckBox();
            jCheckBoxAutoConnect.setAction(null);
            jCheckBoxAutoConnect.setText("Auto-Connect");
            jCheckBoxAutoConnect.addItemListener(new java.awt.event.ItemListener() {
                public void itemStateChanged(java.awt.event.ItemEvent e) {
                    jParent.setAutoConnect(jCheckBoxAutoConnect.isSelected());
                }
            });
        }
        return jCheckBoxAutoConnect;
    }
    
    
}
