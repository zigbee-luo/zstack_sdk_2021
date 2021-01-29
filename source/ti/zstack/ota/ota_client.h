
/******************************************************************************
  Filename:       ota_client.h
  Revised:        $Date: 2015-04-14 21:59:34 -0700 (Tue, 14 Apr 2015) $
  Revision:       $Revision: 43420 $

  Description:    Over-the-Air Upgrade Cluster client App definitions.


  Copyright 2015 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product. Other than for
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
******************************************************************************/

#ifndef OTA_CLIENT_APP_H
#define OTA_CLIENT_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 * INCLUDES
 */
#include "zcl.h"
#include "ota_common.h"
#include "zcl_ota.h"
#include "zd_object.h"
#ifndef CUI_DISABLE
#include "cui.h"
#endif
#include <ti/sysbios/knl/Semaphore.h>

/******************************************************************************
 * CONSTANTS
 */

// Callback events to application from OTA
#define ZCL_OTA_START_CALLBACK                        0
#define ZCL_OTA_DL_COMPLETE_CALLBACK                  1

#define SAMPLEAPP_BL_OFFSET                           0x1F001

#define ST_FULL_IMAGE                                 0x01
#define ST_APP_ONLY_IMAGE                             0x02
#define ST_STACK_ONLY_IMAGE                           0x03
#define ST_FULL_FACTORY_IMAGE                         0x04

// OTA_Storage_Status_t status codes
typedef enum {
    OTA_Storage_Status_Success, ///< Success
    OTA_Storage_Failed,         ///< Fail
    OTA_Storage_CrcError,       ///< Acknowledgment or Response Timed out
    OTA_Storage_FlashError,     ///< flash access error
    OTA_Storage_Aborted,        ///< Canceled by application
    OTA_Storage_Rejected,       ///< OAD request rejected by application
} OTA_Storage_Status_t;

/******************************************************************************
 * TYPEDEFS
 */

/******************************************************************************
 * GLOBAL VARIABLES
 */

extern uint8_t zclOTA_ClientPdState;
extern uint32_t zclOTA_DownloadedImageSize;  // Downloaded image size


/******************************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      otaClient_setAttributes
 *
 * @brief   Sets pointers to attributes used by OTA Client module
 *
 * @param   *attrs - set of attributes from the application
 * @param   numAttrs - number of attributes in the list
 *
 * @return  void
 */
extern void otaClient_setAttributes( const zclAttrRec_t *attrs, uint8_t numAttrs );

/******************************************************************************
 *
 * @fn      otaClient_CreateTask
 *
 * @brief   Call to pass generated endpoint information and initialize task
 *
 * @param   endpoint - Endpoint to register
 *          epDesc  - Endpoint Descriptor
 *          attrArraySize - Size of attribute array
 *          attrs - Array of endpoint attributes
 *          cmdsArraySize - Size of command array
 *          cmds  - Array of commands for endpoint
 *
 * @return  none
 */
extern void otaClient_CreateTask(uint8_t endpoint, endPointDesc_t epDesc, uint8_t attrArraySize,
                           const zclAttrRec_t attrs[], uint8_t cmdsArraySize, const zclCommandRec_t cmds[]);

/******************************************************************************
 *
 * @fn      otaClient_Init
 *
 * @brief   Call to initialize the OTA Client Task
 *
 * @param   task_id
 *
 * @return  none
 */
extern void otaClient_Init ( Semaphore_Handle appSem, uint8_t stEnt, uint32_t cuiHandle);

/******************************************************************************
 * @fn          otaClient_event_loop
 *
 * @brief       Event Loop Processor for OTA Client task.
 *
 * @param       task_id - TaskId
 *              events - events
 *
 * @return      Unprocessed event bits
 */
extern uint16_t otaClient_event_loop( void );

extern void otaClient_ProcessIEEEAddrRsp( ZDO_NwkIEEEAddrResp_t *pMsg );

extern bool otaClient_ProcessMatchDescRsp ( ZDO_MatchDescRsp_t *pMsg );

/******************************************************************************
 * @fn      otaClient_RequestNextUpdate
 *
 * @brief   Called by an application after discovery of the OTA server
 *          to initiate the query next image of the OTA server.
 *
 * @param   srvAddr - Short address of the server
 * @param   srvEndPoint - Endpoint on the server
 *
 * @return  ZStatus_t
 */
extern void otaClient_RequestNextUpdate(uint16_t srvAddr, uint8_t srvEndPoint);

/******************************************************************************
 * @fn      otaClient_DiscoverServer
 *
 * @brief   Call to discover the OTA Client
 *
 * @param   task_id
 *
 * @return  none
 */
extern void otaClient_DiscoverServer( void );//uint8_t task_id );

/******************************************************************************
 * @fn      otaClient_InitializeSettings
 *
 * @brief   Call to initialize the OTA Client
 *
 * @param   task_id
 *
 * @return  none
 */

extern void otaClient_InitializeSettings( void );//uint8_t task_id);

/******************************************************************************
 * @fn      otaClient_SetEndpoint
 *
 * @brief   Set OTA endpoint.
 *
 * @param   endpoint - endpoint ID from which OTA functions can be accessed
 *
 * @return  true if endpoint set, else false
 */
extern bool otaClient_SetEndpoint( uint8_t endpoint);


/******************************************************************************
 * @fn          otaClient_loadExtImage
 *
 * @brief       Load the image from external flash
 *              and reboot.
 *
 * @param       none
 *
 * @return      none
 */
extern void otaClient_loadExtImage(uint8_t imageSelect);


/*********************************************************************
 * @fn          otaClient_UpdateStatusLine
 *
 * @brief       Generate part of the OTA Info string
 *
 * @param       none
 *
 * @return      none
 */
extern void otaClient_UpdateStatusLine(void);



#ifdef FACTORY_IMAGE
/******************************************************************************
 * @fn          otaClient_hasFactoryImage
 *
 * @brief   This function check if the valid factory image exists on external
 *          flash
 *
 * @param   None
 *
 * @return  TRUE If factory image exists on external flash, else FALSE
 *
 */
extern bool otaClient_hasFactoryImage(void);


/*******************************************************************************
 * @fn      otaClient_saveFactoryImage
 *
 * @brief   This function creates factory image backup of current running image
 *
 * @return  rtn  OTA_Storage_Status_Success/OTA_Storage_FlashError
 */
extern uint8_t otaClient_saveFactoryImage(void);
#endif

#endif /*OTA_CLIENT_APP_H*/
