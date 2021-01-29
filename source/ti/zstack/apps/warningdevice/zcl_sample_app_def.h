/**************************************************************************************************
  Filename:       zcl_sample_app_def.h

  Description:    Generic sample app definitions for UI used by zcl_sampleapp_ui.c/.h


  Copyright 2006-2014 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

#include "zcl_samplewarningdevice.h"

#ifndef APPLICATION_UI_ZCL_SAMPLE_APP_DEF_H_
#define APPLICATION_UI_ZCL_SAMPLE_APP_DEF_H_


#ifndef CUI_DISABLE


#define SAMPLE_APP_MENUS  4

#define CUI_APP_MENU    CUI_MENU_ITEM_INT_ACTION("<ENROLLMENT MODE >", (CUI_pFnIntercept_t) zclSampleWarningDevice_UiActionChangeEnrollmentMode) \
                        CUI_MENU_ITEM_INT_ACTION("<  DISCOVER CIE  >", (CUI_pFnIntercept_t) zclSampleWarningDevice_UiActionDiscoverCIE) \
                        CUI_MENU_ITEM_ACTION    ("<SEND ENROLL REQ >", (CUI_pFnAction_t) zclSampleWarningDevice_UiActionSendEnroll) \
                        CUI_MENU_ITEM_ACTION    ("<  TOGGLE ALARM  >", (CUI_pFnAction_t) zclSampleWarningDevice_UiActionToggleAlarm)

#define APP_TITLE_STR "TI Sample Warning Device"

#endif  // #ifndef CUI_DISABLE


#endif /* APPLICATION_UI_ZCL_SAMPLEAPPDEF_H_ */
