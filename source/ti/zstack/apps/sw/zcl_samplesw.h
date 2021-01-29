/**************************************************************************************************
  Filename:       zcl_samplesw.h
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

/*********************************************************************
 * CONSTANTS
 */
#define SAMPLESW_ENDPOINT               8

#define LIGHT_OFF                       0x00
#define LIGHT_ON                        0x01
#define LIGHT_UNKNOWN                   0xFF

// Events for the sample app
#define SAMPLEAPP_END_DEVICE_REJOIN_EVT   0x0001
#define SAMPLEAPP_PROV_CONNECT_EVT        0x0002
#define SAMPLEAPP_PROV_DISCONNECT_EVT     0x0004
#define SAMPLEAPP_GET_NWK_INFO_EVT        0x0008
#define SAMPLEAPP_SYNC_ATTR_EVT           0x0010

#if defined (Z_POWER_TEST)
#define SAMPLEAPP_POWER_TEST_START_EVT    0x1000
#if defined (POWER_TEST_DATA_ACK)
#define SAMPLEAPP_POWER_TEST_TOGGLE_EVT   0x2000
#endif
#endif // Z_POWER_TEST

#if defined(DMM_ZCSWITCH) && defined(NWK_TOPOLOGY_DISCOVERY)
#define SAMPLEAPP_NWK_DISC_EVT            0x0080
#endif

// Green Power Events
#define SAMPLEAPP_PROCESS_GP_DATA_SEND_EVT              0x0100
#define SAMPLEAPP_PROCESS_GP_EXPIRE_DUPLICATE_EVT       0x0200
#define SAMPLEAPP_PROCESS_GP_TEMP_MASTER_EVT            0x0400

#define SAMPLEAPP_POLICY_UPDATE_EVT       0x0800

#define SAMPLEAPP_END_DEVICE_REJOIN_DELAY 1000
#define SAMPLEAPP_CONFIG_SYNC_TIMEOUT     500

#if defined (BDB_TL_TARGET) || defined (BDB_TL_INITIATOR)
#define TL_BDB_FB_EVT                     0x0100
#define TL_BDB_FB_DELAY                   16000
#endif // defined ( BDB_TL_TARGET ) || defined (BDB_TL_INITIATOR)

/*********************************************************************
 * MACROS
 */
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * VARIABLES
 */
extern SimpleDescriptionFormat_t zclSampleSw_SimpleDesc;

extern SimpleDescriptionFormat_t zclSampleSw9_SimpleDesc;

extern CONST zclAttrRec_t zclSampleSw_Attrs[];

extern uint8_t  zclSampleSw_OnOff;

extern uint16_t zclSampleSw_IdentifyTime;

extern CONST uint8_t zclSampleSw_NumAttributes;

/*********************************************************************
 * FUNCTIONS
 */

/*
 *  Reset all writable attributes to their default values.
 */
extern void zclSampleSw_ResetAttributesToDefaultValues(void); //implemented in zcl_samplesw_data.c

/*
 *  Function to toggle the remote light
 */
extern void zclSampleSw_actionToggleLight(const int32_t _itemEntry);

/*
 *  Function to allow switch be discovered by light device to get reports.
 */
extern void zclSampleSw_UiActionSwDiscoverable(const int32_t _itemEntry);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ZCL_SAMPLEAPP_H */
