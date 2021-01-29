/**************************************************************************************************
  Filename:       bdb_reporting.h
  Revised:        $Date: 2016-07-04 10:15:00 -0800 (Mon, 4 Jul 2016) $
  Revision:       $Revision: - $

  Description:    This file contains the definitions used for the Bdb attribute
                  reporting.


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

#ifndef BDB_REPORTING_H
#define BDB_REPORTING_H

#ifdef BDB_REPORTING

#include "zcl.h"

/*********************************************************************
 * CONSTANTS
 */

// Errors, boolean and reporting constants definitions used in the bdb reporting code
#define BDBREPORTING_FALSE 0
#define BDBREPORTING_TRUE 1
#define BDBREPORTING_ERROR 1
#define BDBREPORTING_IGNORE 0xFF
#define BDBREPORTING_OUTOFMEMORYERROR 2
#define BDBREPORTING_SUCCESS 0
#define BDBREPORTING_INVALIDINDEX 0xFF

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */

/*
 * @fn      pBdb_SendReportCmdCallback
 *
 * @brief   Customer send report command callback. If had set this callback,bdb
 *          won't send report command
 * 
 * @param   dstAddr - destination address
 * @param   clusterID - cluster ID
 * @param   reportCmd - report command to be sent
 * @param   direction - direction of the command
 * @param   manuCode - manufacturer code for proprietary extensions to a profile
 * @param   seqNum - transaction sequence number
 * @param   isReqFromApp - Indicates where it comes from application thread or stack thread 
 *
 * @return  if false, send reporting in stack task, fixed by luoyiming 2019-11-22
 *
*/
extern bool (*pBdb_SendReportCmdCallback)( uint8_t srcEP, afAddrType_t *dstAddr, uint16_t clusterID,
                                           zclReportCmd_t *reportCmd, uint8_t direction, uint8_t disableDefaultRsp,
                                           uint16_t manuCode, uint8_t seqNum );

/*********************************************************************
 * FUNCTIONS
 */

void bdb_RepInit( void );
void bdb_RepConstructReportingData( void );
void bdb_RepProcessEvent( void );
void bdb_RepStartOrContinueReporting( void );
void bdb_RepMarkHasBindingInEndpointClusterArray( uint8_t endpoint, uint16_t cluster, uint16_t manuCode,
                                                  uint8_t direction, uint8_t unMark, uint8_t setNoNextIncrementFlag );
ZStatus_t bdb_ProcessInConfigReportCmd( zclIncomingMsg_t *pInMsg );
ZStatus_t bdb_ProcessInReadReportCfgCmd( zclIncomingMsg_t *pInMsg );
void bdb_RepUpdateMarkBindings( void );

#endif //BDB_REPORTING
#endif /* BDB_REPORTING_H */

