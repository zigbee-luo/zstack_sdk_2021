/**************************************************************************************************
  Filename:       zcl_samplefiredetector.h
  Revised:        $Date: 2015-08-19 17:11:00 -0700 (Wed, 19 Aug 2015) $
  Revision:       $Revision: 44460 $


  Description:    This file contains the Zigbee Cluster Library Home
                  Automation Sample Application.


  Copyright 2006-2013 Texas Instruments Incorporated. All rights reserved.

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

#ifndef ZCL_SAMPLESW_H
#define ZCL_SAMPLESW_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "zcl.h"
#include "nvintf.h"
#ifndef CUI_DISABLE
#include "cui.h"
#endif

/*********************************************************************
 * CONSTANTS
 */
#define SAMPLEFIREDETECTOR_ENDPOINT               8


// Events for the sample app
#define SAMPLEAPP_END_DEVICE_REJOIN_EVT   0x0001
#define SAMPLEAPP_AUTO_ENROLL_REQ_EVT     0x0002
#define SAMPLEAPP_WRITE_TO_CIE_DONE_EVT   0x0004

#define SAMPLEAPP_END_DEVICE_REJOIN_DELAY 1000
#define SAMPLEAPP_AUTO_ENROLL_REQ_DELAY   500

//Cluster NV items

// Application NV IDs:
// 0x0001 - 0x007F reserved
// 0x0080 - 0x03FF available for applications
#define IAS_ZONE_NV_ID                   0x0080
#define IAS_ZONE_ZONE_STATE_NV_SUBID     0x0001
#define IAS_ZONE_CIE_ADDRESS_NV_SUBID    0x0002
#define IAS_ZONE_ZONE_ID_NV_SUBID        0x0003

// Green Power Events
#define SAMPLEAPP_PROCESS_GP_DATA_SEND_EVT              0x0100
#define SAMPLEAPP_PROCESS_GP_EXPIRE_DUPLICATE_EVT       0x0200
#define SAMPLEAPP_PROCESS_GP_TEMP_MASTER_EVT            0x0400
/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * TYPEDEFS
 */

typedef enum
{
  TRIP_TO_PAIR = 0,
  // AUTO_ENROLL_RESPONSE, /* not supported */
  AUTO_ENROLL_REQUEST,
  ENROLLMENT_MODE_ENUM_LENGTH,
} IAS_Zone_EnrollmentModes_t;

/*********************************************************************
 * VARIABLES
 */
extern SimpleDescriptionFormat_t zclSampleZone_SimpleDesc;

extern SimpleDescriptionFormat_t zclSampleZone9_SimpleDesc;

extern CONST zclAttrRec_t zclSampleZone_Attrs[];

extern uint16_t zclSampleZone_IdentifyTime;
extern CONST uint8_t zclSampleZone_NumAttributes;

extern uint8_t zclSampleZone_ZoneState;
extern uint16_t zclSampleZone_ZoneType;
extern uint16_t zclSampleZone_ZoneStatus;
extern uint8_t zclSampleZone_CIE_Address[8];
extern uint8_t zclSampleZone_ZoneId;
/*********************************************************************
 * FUNCTIONS
 */

/*
 *  Reset all writable attributes to their default values.
 */
extern void zclSampleZone_ResetAttributesToDefaultValues(void); //implemented in zcl_samplefiredetector_data.c

#ifndef CUI_DISABLE
extern void zclSampleZone_UiActionChangeEnrollmentMode(const char _input, char* _pLines[3], CUI_cursorInfo_t* _pCurInfo);
extern void zclSampleZone_UiActionDiscoverCIE(const char _input, char* _pLines[3], CUI_cursorInfo_t* _pCurInfo);
extern void zclSampleZone_UiActionSendEnroll(const int32_t _itemEntry);
extern void zclSampleZone_UiActionToggleAlarm(const int32_t _itemEntry);
#endif
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ZCL_SAMPLEAPP_H */

