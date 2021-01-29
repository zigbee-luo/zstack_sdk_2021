/******************************************************************************

 @file  macTask.h

 @brief Mac Stack interface function definition

 Group: WCS LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2013-2021, Texas Instruments Incorporated
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

#ifndef MAC_STACK_H
#define MAC_STACK_H

#ifdef __cplusplus
extern "C" {
#endif


#include "mac_user_config.h"
#include "mac_api.h"

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint8_t                       srctaskid;
  uint8_t                      retransmit;
  uint8_t                     pendingMsg;
  uint8_t (*pMacCbackQueryRetransmit)();
  uint8_t (*pMacCbackCheckPending)();
} macStackInitParams_t;

/**************************************************************************************************
 * @fn          macTaskInit
 *
 * @brief       This function is called initialize the MAC task.
 *
 * input parameters
 *
 * @param       pUserCfg - MAC user config
 *
 * output parameters
 *
 * None.
 *
 * @return      MAC Task ID.
 **************************************************************************************************
 */
uint8_t macTaskInit(macUserCfg_t *pUserCfg);


/**************************************************************************************************
 * @fn          macTaskGetTaskHndl
 *
 * @brief       This function returns the TIRTOS Task handle of the MAC Task.
 *
 * input parameters
 *
 * @param       pUserCfg - MAC user config
 *
 * output parameters
 *
 *
 * @return      MAC Task ID.
 **************************************************************************************************
 */
void* macTaskGetTaskHndl(void);

#ifdef __cplusplus
};
#endif

#endif /* MAC_STACK_H */


