/***************************************************************************************************
  Filename:       MT_APP.h
  Revised:        $Date: 2014-11-19 13:29:24 -0800 (Wed, 19 Nov 2014) $
  Revision:       $Revision: 41175 $

  Description:

  Copyright 2008-2014 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License"). You may not use this
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

 ***************************************************************************************************/
#ifndef MT_APP_H
#define MT_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************************************
 * INCLUDES
 ***************************************************************************************************/
#include "rom_jt_154.h"
#include "af.h"


/***************************************************************************************************
 * CONSTANTS
 ***************************************************************************************************/

#define MT_APP_PB_ZCL_CMD_MSG 0x00 // MT_APP_PB_ZCL_CMD message
#define MT_APP_PB_ZCL_CMD_CFG 0x01 // MT_APP_PB_ZCL_CMD config

#define MT_APP_PB_ZCL_MSG_HDR_LEN 13 // sizeof (uint8_t)  + // AppEndPoint
                                     // sizeof (uint16_t) + // DestAddress
                                     // sizeof (uint8_t)  + // DestEndpoint
                                     // sizeof (uint16_t) + // ClusterID
                                     // sizeof (uint8_t)  + // CommandID
                                     // sizeof (uint8_t)  + // Specific
                                     // sizeof (uint8_t)  + // Direction
                                     // sizeof (uint8_t)  + // DisableDefaultRsp
                                     // sizeof (uint16_t) + // ManuCode
                                     // sizeof (uint8_t)  + // TransSeqNum

#define MT_APP_PB_ZCL_IND_HDR_LEN 13 // sizeof (uint8_t)  + // AppEndPoint
                                     // sizeof (uint16_t) + // SrcAddress
                                     // sizeof (uint8_t)  + // SrcEndpoint
                                     // sizeof (uint16_t) + // ClusterID
                                     // sizeof (uint8_t)  + // CommandID
                                     // sizeof (uint8_t)  + // Specific
                                     // sizeof (uint8_t)  + // Direction
                                     // sizeof (uint8_t)  + // DisableDefaultRsp
                                     // sizeof (uint16_t) + // ManuCode
                                     // sizeof (uint8_t)  + // TransSeqNum

#define MT_APP_PB_ZCL_CFG_HDR_LEN  2 // sizeof (uint8_t)  + // AppEndPoint
                                     // sizeof (uint8_t)  + // Mode


/***************************************************************************************************
 * TYPEDEF
 ***************************************************************************************************/

typedef struct
{
  OsalPort_EventHdr  hdr;
  uint8_t             endpoint;
  uint8_t             appDataLen;
  uint8_t             *appData;
} mtSysAppMsg_t;

typedef struct
{
  OsalPort_EventHdr hdr;
  uint8_t            type;
} mtAppPB_ZCLCmd_t;

typedef struct
{
  OsalPort_EventHdr hdr;
  uint8_t            type;
  uint8_t            appEP;
  afAddrType_t     dstAddr;
  uint16_t           clusterID;
  uint8_t            commandID;
  uint8_t            specific;
  uint8_t            direction;
  uint8_t            disableDefRsp;
  uint16_t           manuCode;
  uint8_t            transSeqNum;
  uint8_t            appPBDataLen;
  uint8_t            *appPBData;
} mtAppPB_ZCLMsg_t;

typedef struct
{
  OsalPort_EventHdr hdr;
  uint8_t            type;
  uint8_t            mode;
} mtAppPB_ZCLCfg_t;

typedef struct
{
  uint8_t  appEP;
  uint16_t srcAddr;
  uint8_t  srcEP;
  uint16_t clusterID;
  uint8_t  commandID;
  uint8_t  specific;
  uint8_t  direction;
  uint8_t  disableDefRsp;
  uint16_t manuCode;
  uint8_t  transSeqNum;
  uint8_t  appPBDataLen;
  uint8_t  *appPBData;
} mtAppPB_ZCLInd_t;


/***************************************************************************************************
 * EXTERNAL FUNCTIONS
 ***************************************************************************************************/
#if defined (MT_APP_FUNC)
/*
 * Process MT_APP commands
 */
extern uint8_t MT_AppCommandProcessing(uint8_t *pBuf);
#endif

/*
 * Send an MT_APP_PB_ZCL_IND command
 */
extern void MT_AppPB_ZCLInd( mtAppPB_ZCLInd_t *ind );


#ifdef __cplusplus
}
#endif

#endif /* MTEL_H */

/***************************************************************************************************
 ***************************************************************************************************/
