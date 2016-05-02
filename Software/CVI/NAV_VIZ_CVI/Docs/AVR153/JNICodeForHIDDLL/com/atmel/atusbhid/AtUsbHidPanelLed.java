package com.atmel.atusbhid;

import java.awt.FlowLayout;

import javax.swing.JCheckBox;
import javax.swing.JPanel;


/**
 * The Class AtUsbHidPanelLed.
 */
public class AtUsbHidPanelLed extends JPanel {
    
    /** The Constant serialVersionUID. */
    private static final long serialVersionUID = -9098183392513006046L;
    
    /** The j check box led1. */
    private JCheckBox   jCheckBoxLed1 = null;
    
    /** The j check box led2. */
    private JCheckBox   jCheckBoxLed2 = null;
    
    /** The j check box led3. */
    private JCheckBox   jCheckBoxLed3 = null;
    
    /** The j check box led4. */
    private JCheckBox   jCheckBoxLed4 = null;
    
    /** The j panel led. */
    private JPanel      jPanelLed     = null;

    /** The j parent. */
    private AtUsbHidGui jParent       = null;

    
    
    /**
     * Instantiates a new at usb hid panel led.
     * 
     * @param jParent
     *            the j parent
     */
    public AtUsbHidPanelLed(AtUsbHidGui jParent) {
        super();
        this.jParent = jParent;
    }

    /**
     * This method initializes jPanelLed.
     * 
     * @return javax.swing.JPanel
     */
    public JPanel getJPanelLed() {
        if (jPanelLed == null) {
            jPanelLed = new JPanel();
            jPanelLed.setLayout(new FlowLayout());
            jPanelLed.add(getJCheckBoxLed1(), null);
            jPanelLed.add(getJCheckBoxLed2(), null);
            jPanelLed.add(getJCheckBoxLed3(), null);
            jPanelLed.add(getJCheckBoxLed4(), null);
        }
        return jPanelLed;
    }

    /**
     * This method initializes jCheckBoxLed1.
     * 
     * @return javax.swing.JCheckBox
     */
    private JCheckBox getJCheckBoxLed1() {
        if (jCheckBoxLed1 == null) {
            jCheckBoxLed1 = new JCheckBox();
            jCheckBoxLed1.setText("Led 1");
            jCheckBoxLed1.addItemListener(new java.awt.event.ItemListener() {
                public void itemStateChanged(java.awt.event.ItemEvent e) {
                    jParent.setLed(1,jCheckBoxLed1.isSelected());
                    
                }
            });
        }
        return jCheckBoxLed1;
    }

    /**
     * This method initializes jCheckBoxLed2.
     * 
     * @return javax.swing.JCheckBox
     */
    private JCheckBox getJCheckBoxLed2() {
        if (jCheckBoxLed2 == null) {
            jCheckBoxLed2 = new JCheckBox();
            jCheckBoxLed2.setText("Led 2");
            jCheckBoxLed2.addItemListener(new java.awt.event.ItemListener() {
                public void itemStateChanged(java.awt.event.ItemEvent e) {
                    jParent.setLed(2,jCheckBoxLed2.isSelected());
                }
            });
        }
        return jCheckBoxLed2;
    }

    /**
     * This method initializes jCheckBoxLed3.
     * 
     * @return javax.swing.JCheckBox
     */
    private JCheckBox getJCheckBoxLed3() {
        if (jCheckBoxLed3 == null) {
            jCheckBoxLed3 = new JCheckBox();
            jCheckBoxLed3.setText("Led 3");
            jCheckBoxLed3.addItemListener(new java.awt.event.ItemListener() {
                public void itemStateChanged(java.awt.event.ItemEvent e) {
                    jParent.setLed(3,jCheckBoxLed3.isSelected());
                }
            });
        }
        return jCheckBoxLed3;
    }

    /**
     * This method initializes jCheckBoxLed4.
     * 
     * @return javax.swing.JCheckBox
     */
    private JCheckBox getJCheckBoxLed4() {
        if (jCheckBoxLed4 == null) {
            jCheckBoxLed4 = new JCheckBox();
            jCheckBoxLed4.setText("Led 4");
            jCheckBoxLed4.addItemListener(new java.awt.event.ItemListener() {
                public void itemStateChanged(java.awt.event.ItemEvent e) {
                    jParent.setLed(4,jCheckBoxLed4.isSelected());
                }
            });
        }
        return jCheckBoxLed4;
    }
    
    /**
     * Sets the communication enable.
     * 
     * @param b
     *            the new communication enable
     */
    public void setCommunicationEnable(boolean b) {
        jCheckBoxLed1.setEnabled(b);
        jCheckBoxLed2.setEnabled(b);
        jCheckBoxLed3.setEnabled(b);        
        jCheckBoxLed4.setEnabled(b);
        jCheckBoxLed1.setSelected(false);
        jCheckBoxLed2.setSelected(false);
        jCheckBoxLed3.setSelected(false);   
        jCheckBoxLed4.setSelected(false);
    }

}
