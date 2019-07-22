/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2016. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_Disonnect_USB              2       /* control type: command, callback function: Disconnect_USB_BUTTON */
#define  PANEL_Connect_USB                3       /* control type: command, callback function: Connect_USB_BUTTON */
#define  PANEL_USB_MSG                    4       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTBOX                    5       /* control type: textBox, callback function: (none) */
#define  PANEL_USB_TIMER                  6       /* control type: timer, callback function: USB_TIMER */
#define  PANEL_TEXTMSG_2                  7       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG                    8       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_NR                 9       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_ATSTUMAS           10      /* control type: textMsg, callback function: (none) */
#define  PANEL_TAB                        11      /* control type: tab, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_T_STRIPCHART_TV         2       /* control type: strip, callback function: (none) */
#define  TABPANEL_T_RINGSLIDE             3       /* control type: slide, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_V_STRIPCHART_T6         2       /* control type: strip, callback function: (none) */
#define  TABPANEL_V_STRIPCHART_T5         3       /* control type: strip, callback function: (none) */
#define  TABPANEL_V_STRIPCHART_T4         4       /* control type: strip, callback function: (none) */
#define  TABPANEL_V_STRIPCHART_T3         5       /* control type: strip, callback function: (none) */
#define  TABPANEL_V_STRIPCHART_T2         6       /* control type: strip, callback function: (none) */
#define  TABPANEL_V_STRIPCHART_T1         7       /* control type: strip, callback function: (none) */
#define  TABPANEL_V_STRIPCHART_T8         8       /* control type: strip, callback function: (none) */
#define  TABPANEL_V_STRIPCHART_T7         9       /* control type: strip, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Connect_USB_BUTTON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Disconnect_USB_BUTTON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK USB_TIMER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
