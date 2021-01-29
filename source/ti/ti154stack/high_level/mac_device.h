/******************************************************************************

 @file  mac_device.h

 @brief Internal interface file for the MAC device module.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2005-2021, Texas Instruments Incorporated
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

#ifndef MAC_DEVICE_H
#define MAC_DEVICE_H

/* ------------------------------------------------------------------------------------------------
 *                                           Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

#define MAC_DATA_REQ_ASSOC  0x01      /* data request originated from associate procedure */
#define MAC_DATA_REQ_POLL   0x02      /* data request originated from poll procedure */
#define MAC_DATA_REQ_AUTO   0x04      /* data request originated from auto poll procedure */

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

typedef struct
{
  macTimer_t    frameTimer;             /* timer to wait for frame after data request */
  macTimer_t    responseTimer;          /* timer to wait before sending data request */
  ApiMac_sec_t  sec;                    /* associate req security setting */
  uint8         dataReqMask;
} macDevice_t;

/* ------------------------------------------------------------------------------------------------
 *                                           Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/* Action functions */
MAC_INTERNAL_API void macStartResponseTimer(macEvent_t *pEvent);
MAC_INTERNAL_API void macStartFrameResponseTimer(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiAssociateReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macAssocDataReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macAssocDataReqComplete(macEvent_t *pEvent);
MAC_INTERNAL_API void macRxAssocRsp(macEvent_t *pEvent);
MAC_INTERNAL_API void macAssocFrameResponseTimeout(macEvent_t *pEvent);
MAC_INTERNAL_API void macAssocFailed(macEvent_t *pEvent);
MAC_INTERNAL_API void macAssocRxDisassoc(macEvent_t *pEvent);
MAC_INTERNAL_API void macAssocDataRxInd(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiPollReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macPollDataReqComplete(macEvent_t *pEvent);
MAC_INTERNAL_API void macPollDataRxInd(macEvent_t *pEvent);
MAC_INTERNAL_API void macPollFrameResponseTimeout(macEvent_t *pEvent);
MAC_INTERNAL_API void macPollRxDisassoc(macEvent_t *pEvent);
MAC_INTERNAL_API void macPollRxAssocRsp(macEvent_t *pEvent);
MAC_INTERNAL_API void macRxCoordRealign(macEvent_t *pEvent);
MAC_INTERNAL_API void macPanConflictComplete(macEvent_t *pEvent);

MAC_INTERNAL_API void macDeviceReset(void);
/**************************************************************************************************
*/

#endif /* MAC_DEVICE_H */
