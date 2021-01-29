/**************************************************************************************************
  Filename:       gp_sink.h
  Revised:        $Date: 2016-05-23 11:51:49 -0700 (Mon, 23 May 2016) $
  Revision:       $Revision: - $

  Description:    This file contains the common Green Power stub definitions.


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

#if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)

#ifndef COMBOBASIC_H
#define COMBOBASIC_H


#ifdef __cplusplus
extern "C"
{
#endif



/*********************************************************************
 * INCLUDES
 */

#include "zcomdef.h"
#include "zmac.h"
#include "af.h"
#include "zcl_green_power.h"
#include "cgp_stub.h"
#include "gp_interface.h"


/*********************************************************************
 * MACROS
 */

 /*********************************************************************
 * ENUM
 */
enum
{
  GP_COMMAND_IDENTIFY_IDENTIFY,
  GP_COMMAND_RECALL_SCENE0 = 0x10,
  GP_COMMAND_RECALL_SCENE1,
  GP_COMMAND_RECALL_SCENE2,
  GP_COMMAND_RECALL_SCENE3,
  GP_COMMAND_RECALL_SCENE4,
  GP_COMMAND_RECALL_SCENE5,
  GP_COMMAND_RECALL_SCENE6,
  GP_COMMAND_RECALL_SCENE7,
  GP_COMMAND_STORE_SCENE0,
  GP_COMMAND_STORE_SCENE1,
  GP_COMMAND_STORE_SCENE2,
  GP_COMMAND_STORE_SCENE3,
  GP_COMMAND_STORE_SCENE4,
  GP_COMMAND_STORE_SCENE5,
  GP_COMMAND_STORE_SCENE6,
  GP_COMMAND_STORE_SCENE7,
  GP_COMMAND_OFF,
  GP_COMMAND_ON,
  GP_COMMAND_TOGGLE,
  GP_COMMAND_LEVEL_CONTROL_MOVE_UP = 0x30,
  GP_COMMAND_LEVEL_CONTROL_MOVE_DOWN,
  GP_COMMAND_LEVEL_CONTROL_STEP_UP,
  GP_COMMAND_LEVEL_CONTROL_STEP_DOWN,
  GP_COMMAND_LEVEL_CONTROL_STOP,
  GP_COMMAND_LEVEL_CONTROL_MOVE_UP_WITH_ON_OFF,
  GP_COMMAND_LEVEL_CONTROL_MOVE_DOWN_UP_WITH_ON_OFF,
  GP_COMMAND_LEVEL_CONTROL_STEP_UP_UP_WITH_ON_OFF,
  GP_COMMAND_LEVEL_CONTROL_STEP_DOWN_UP_WITH_ON_OFF,
  GP_COMMAND_MOVE_HUE_STOP = 0x40,
  GP_COMMAND_MOVE_HUE_UP,
  GP_COMMAND_MOVE_HUE_DOWN,
  GP_COMMAND_STEP_HUE_UP,
  GP_COMMAND_STEP_HUE_DOWN,
  GP_COMMAND_MOVE_SATURATION_STOP,
  GP_COMMAND_MOVE_SATURATION_UP,
  GP_COMMAND_MOVE_SATURATION_DOWN,
  GP_COMMAND_STEP_SATURATION_UP,
  GP_COMMAND_STEP_SATURATION_DOWN,
  GP_COMMAND_MOVE_COLOR,
  GP_COMMAND_STEP_COLOR,
  GP_COMMAND_LOCK_DOOR = 0x50,
  GP_COMMAND_UNLOCK_DOOR,
  GP_COMMAND_ATTR_REPORTING = 0xA0,
  GP_COMMAND_MFR_SPECIFIC_ATTR_REPORTING,
  GP_COMMAND_MULTICLUSTER_REPORTING,
  GP_COMMAND_MULTICLUSTER_MFR_SPECIFIC_REPORTING,
  GP_COMMAND_REQUEST_ATTR,
  GP_COMMAND_READ_ATTR_RSP,
  GP_COMMAND_ZCL_TUNNELING,
};

 /*********************************************************************
 * CONSTANTS
 */

 /*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
extern uint8_t  zgGP_SinkCommissioningMode;     // Global flag that states if in commissioning mode or in operational mode.

/*********************************************************************
 * FUNCTION MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn          pt_ZclReadGetSinkEntry
 *
 * @brief       Populate the given item data
 *
 * @param       nvId - NV entry to read
 *              pData - Pointer to OTA message payload
 *              len - Lenght of the payload
 *
 * @return
 */
uint8_t pt_ZclReadGetSinkEntry( uint16_t nvId, uint8_t* pData, uint8_t* len );

/*
 * @brief       Populate the given item data with Auto-Commissioning GPDF
 */
extern ZStatus_t gp_autoCommissioningGpdfSinkTblUpdate(gpdID_t *gpdId, gp_DataInd_t *gpDataInd, uint8_t *pEntry, uint16_t nvIndex);

/*
 * @brief       Populate the given item data with commissioning GPDF
 */
extern ZStatus_t gp_commissioningGpdfSinkTblUpdate(gpdID_t *gpdId, gp_DataInd_t *gpDataInd, uint8_t *pEntry, uint16_t nvIndex);

/*
 * @brief   Callback from the ZCL GreenPower Cluster Library when
 *          it received an Gp Pairing Configuration Command for this application.
 */
extern void zclGp_GpPairingConfigurationCommandCB( zclGpPairingConfig_t *pCmd );

/*
 * @brief       Performs Security Operations according to Sink
 */
extern uint8_t gp_SecurityOperationSink( gp_SecReq_t* pInd, uint8_t* pKeyType, void* pKey);

/*
 * @brief       This passes the MCPS data indications received in MAC to the application
 */
extern void gp_dataIndSink(gp_DataInd_t *gp_DataInd);

/*
 * @brief       Parse Green Power Commissioning command payload
 */
extern void gp_parseCommisioningCmd(uint8_t *gpdASDU, gpdCommissioningCmd_t *pCmd);

/*
 * @brief       Callback from the ZCL GreenPower Cluster Library when
 *              it received an Gp notification Command for this application.
 */
extern void zclGp_GpNotificationCommandCB( zclGpNotification_t *pCmd );

/*
 * @brief       Callback from the ZCL GreenPower Cluster Library when
 *              it received an Gp commissioning notification Command for this application.
 */
extern void zclGp_GpCommissioningNotificationCommandCB( zclGpCommissioningNotification_t *pCmd );

/*
 * @brief       General function fill the sink table vector
 */
extern uint16_t gp_PairingConfigUpdateSinkTbl( gpPairingConfigCmd_t* payload );

/*
 * @brief       Create new entry from commissioning GPDF
 */
extern ZStatus_t gp_commissioningSinkTblUpdate(gpdID_t *gpdId, uint8_t ep, uint8_t deviceId, uint8_t *pEntry, uint16_t nvIndex, gpSinkTableOptions_t sinkOptions, gpdCommissioningCmd_t *pCommissioningCmd);

/*
 * @brief       Fill a sink table entry from a commissioning command
 */
extern void gp_commissioningSinkEntryParse(gpdID_t *gpdId, uint8_t ep, uint8_t deviceId, uint8_t *pEntry, gpSinkTableOptions_t sinkOptions, gpdCommissioningCmd_t *pCommissioningCmd);

/*
* @brief       General function to init the NV items for sink table
*/
extern uint8_t gp_SinkTblInit( uint8_t resetTable );

/*
 * @brief    General function to get sink table entry by NV index
 */
uint8_t gp_getSinkTableByIndex( uint16_t nvIndex, uint8_t *pEntry );

/*
 * @brief   General function to get sink table entry by gpdID (GP Src ID or Extended Adddress)
 */
uint8_t gp_getSinkTableByGpId(gpdID_t *gpdID, uint8_t *pEntry,uint16_t *NvSinkTableIndex);

/*
 * @brief       Populate the given item data
 */
extern uint8_t pt_ZclReadGetSinkEntry( uint16_t nvId, uint8_t* pData, uint8_t* len );


extern ZStatus_t zclGp_RegisterCBForGPDCommand(GpSink_AppCallbacks_t *pGpSink_AppCallbacks);

/*
* @brief       Get Green Power Sink commissioning mode
*/
extern bool gp_GetSinkCommissioningMode(void);

/*
 * @brief       Enable or disable Green Power Proxy commissioning mode
 */
extern void gp_SetSinkCommissioningMode(bool enabled);

#ifdef __cplusplus
}
#endif


#endif /* COMBOBASIC_H */

#endif // #if !defined (DISABLE_GREENPOWER_BASIC_PROXY) && (ZG_BUILD_RTR_TYPE)
