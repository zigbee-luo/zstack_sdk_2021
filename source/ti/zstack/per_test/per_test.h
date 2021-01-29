/**************************************************************************************************
  Filename:       per_test.h
  Revised:        $Date: 2016-05-23 11:51:49 -0700 (Mon, 23 May 2016) $
  Revision:       $Revision: - $

  Description:    This file contains the Packet Error Rate test interface definitions.


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

#ifndef PER_TEST_H
#define PER_TEST_H

#ifdef PER_TEST

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef PER_TEST


#include "rom_jt_154.h"
#include <ti/sysbios/knl/Semaphore.h>
#include "zstackmsg.h"
#include "zcl_sampleapps_ui.h"
#include "cui.h"

/*********************************************************************
 * INCLUDES
 */



/*********************************************************************
 * CONSTANTS
 */
#if 0
/* These flags must be use as compile flags */
#define PER_TEST                             //Enable per test code code
#define PER_TEST_DISABLE_FINDING_BINDING     //Disable F&B to allow easy commissioning of the intended devices
#define PER_TEST_SEND_TO_PARENT              //Force Switch device to send commands only to parent
#define PER_TEST_SEND_USE_FINDING_BINDING    //Force Switch device to use Binds entry to send commands
#define PER_TEST_SET_FINDING_BINDING_1_MIN   //Change F&B timeout to 1 minute to allow easy commissioning
#define PER_TEST_ENABLE_FWD_NOTIFICATION     //Enable router feature to notify about frames being forwarded
#endif

/*********************************************************************
 * MACROS
 */


#ifndef SAMPLEAPP_PER_TEST_SEND_CMD_DELAY
    #define SAMPLEAPP_PER_TEST_SEND_CMD_DELAY     5000
#endif
#define SAMPLEAPP_PER_TEST_SEND_CMD_EVT           0x1000
#ifndef PER_TEST_APS_ACKS
    #define PER_TEST_APS_ACKS                     3
#endif
#ifndef PER_TEST_APS_ACKS_WAIT_TIME
    #define PER_TEST_APS_ACKS_WAIT_TIME           3000
#endif
#ifndef PER_TEST_POLL_RATE
    #define PER_TEST_POLL_RATE                    500
#endif

/*********************************************************************
 * TYPEDEFS
 */




/*********************************************************************
 * VARIABLES
 */



/*********************************************************************
 * FUNCTIONS
 */


extern void PERTest_init(Semaphore_Handle appSem, uint8_t stEnt, CUI_clientHandle_t gCuiHandle);
extern void PERTest_process(void);
extern void PERTest_processZStackMsg(zstackmsg_genericReq_t *pMsg);
extern void PERTest_uiRefresh(uint8_t UiState, char * line[3]);

#endif


#ifdef __cplusplus
}

#endif

#endif /* PER_TEST */
#endif /* PER_TEST_H */
