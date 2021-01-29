/******************************************************************************

 @file  macs.h

 @brief Mac common definitions between Stack and App

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2015-2021, Texas Instruments Incorporated
 All rights reserved.

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

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef MACS_H
#define MACS_H

#include <stdint.h>


#define ICALL_MAC_CMD_EVENT_START   0xD0

enum
{
  MAC_STACK_INIT_PARAMS = ICALL_MAC_CMD_EVENT_START,
  MAC_INIT_DEVICE,
  MAC_INIT_COORD,
  MAC_INIT_BEACON_COORD,
  MAC_INIT_BEACON_DEVICE,
  MAC_INIT_FH,
  MAC_ENABLE_FH,
  MAC_START_FH,
  MAC_SET_REQ,
  MAC_GET_REQ,
  MAC_SET_SECURITY_REQ,
  MAC_GET_SECURITY_REQ,
  MAC_RESET_REQ,
  MAC_SRC_MATCH_ENABLE,
  MAC_SRC_MATCH_ADD_ENTRY,
  MAC_SRC_MATCH_DELETE_ENTRY,
  MAC_SRC_MATCH_ACK_ALL_PENDING,
  MAC_SRC_MATCH_CHECK_ALL_PENDING,
  MAC_SET_RADIO_REG,
  MAC_UPDATE_PANID,
  MAC_MCPS_DATA_REQ,
  MAC_MCPS_DATA_ALLOC,
  MAC_MCPS_PURGE_REQ,
  MAC_MLME_ASSOCIATE_REQ,
  MAC_MLME_ASSOCIATE_RSP,
  MAC_MLME_DISASSOCIATE_REQ,
  MAC_MLME_ORPHAN_RSP,
  MAC_MLME_POLL_REQ,
  MAC_MLME_SCAN_REQ,
  MAC_START_REQ,
  MAC_SYNC_REQ,
  MAC_RANDOM_BYTE,
  MAC_RESUME_REQ,
  MAC_YIELD_REQ,
  MAC_MSG_DEALLOCATE,
  MAC_GET_SECURITY_PTR_REQ,
  MAC_FH_SET_REQ,
  MAC_FH_GET_REQ,
  MAC_MLME_WS_ASYNC_REQ,
  MAC_SEC_ADD_DEVICE,
  MAC_SEC_DEL_DEVICE,
  MAC_SEC_DEL_KEY_AND_DEVICES,
  MAC_SEC_DEL_ALL_DEVICES,
  MAC_SEC_GET_DEFAULT_SOURCE_KEY,
  MAC_SEC_ADD_KEY_INIT_FC
};

#endif /* MACS_H */
