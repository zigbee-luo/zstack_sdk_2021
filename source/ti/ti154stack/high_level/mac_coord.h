/******************************************************************************

 @file  mac_coord.h

 @brief Internal interface file for the MAC coordinator module.

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

#ifndef MAC_COORD_H
#define MAC_COORD_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "mac_high_level.h"
#include "mac_timer.h"
#include "api_mac.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Typedefs
 * ------------------------------------------------------------------------------------------------
 */
/* Coordinator info type */
typedef struct
{
  macTimer_t             indirectTimer;      /* indirect frame timer */
  ApiMac_mlmeStartReq_t* pStartReq;         /* buffer allocated to store copy of start parameters */
  bool                   beaconing;          /* TRUE if beaconing network has been started */
  ApiMac_sec_t           realignSec;         /* Security parameters for the coordinator realignment frame */
  ApiMac_sec_t           beaconSec;          /* Security parameters for the beacon frame */
#ifdef FEATURE_ENHANCED_BEACON
  uint8*                 pIEIDs;             /* buffer containing IE IDs */
  uint8                  numIEs;             /* number of IE IDs */
#endif /* FEATURE_ENHANCED_BEACON */
} macCoord_t;

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */


/* ------------------------------------------------------------------------------------------------
 *                                           Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/* Action functions */
MAC_INTERNAL_API void macIndirectExpire(macEvent_t *pEvent);
MAC_INTERNAL_API void macRxAssociateReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiAssociateRsp(macEvent_t *pEvent);
MAC_INTERNAL_API void macRxOrphan(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiOrphanRsp(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiPurgeReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiStartReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macStartContinue(macEvent_t *pEvent);
MAC_INTERNAL_API void macStartComplete(macEvent_t *pEvent);
#if defined (FEATURE_BEACON_MODE) || defined (FEATURE_NON_BEACON_MODE)
MAC_INTERNAL_API void macRxBeaconReq(macEvent_t *pEvent);
#endif /* (FEATURE_BEACON_MODE) || (FEATURE_NON_BEACON_MODE) */
MAC_INTERNAL_API void macRxDataReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macRxPanConflict(macEvent_t *pEvent);
#ifdef FEATURE_ENHANCED_BEACON
MAC_INTERNAL_API void macRxEnhancedBeaconReq(macEvent_t *pEvent);
#endif /* FEATURE_ENHANCED_BEACON */

/* utility functions */
MAC_INTERNAL_API uint8 macStartBegin(macEvent_t *pEvent);
MAC_INTERNAL_API uint8 macBuildAssociateRsp(macEvent_t *pEvent);

MAC_INTERNAL_API void macStartSetParams(ApiMac_mlmeStartReq_t *pStartReq);
MAC_INTERNAL_API uint8 macBuildRealign(macTx_t *pMsg, sAddr_t *pDstAddr, uint16 coordShortAddr,
                                      uint16 newPanId, uint8 logicalChannel);
MAC_INTERNAL_API void macCoordReset(void);
MAC_INTERNAL_API bool macIndirectSend(sAddr_t *pAddr, uint16 panId);
#ifdef FEATURE_ENHANCED_BEACON
MAC_INTERNAL_API macTx_t *macBuildEnhancedBeacon(uint8 beaconOrder, uint8 superframeOrder,
                                                 bool battLifeExt, uint8 coexistIe);
#endif /* FEATURE_ENHANCED_BEACON */


#if defined (FEATURE_BEACON_MODE) || defined (FEATURE_NON_BEACON_MODE)
uint8 macPendAddrLen(uint8 *maxPending);
uint8 *macBuildPendAddr(uint8 *p, uint8 maxPending, bool *pBroadcast);
#endif /* (FEATURE_BEACON_MODE) || (FEATURE_NON_BEACON_MODE) */

void macIndirectMark(macTx_t *pMsg);
void macIndirectTxFrame(macTx_t *pMsg);
void macIndirectRequeueFrame(macTx_t *pMsg);

#ifdef FEATURE_ENHANCED_BEACON
bool macProcessEBeaconFilterIe(macEvent_t* pEvent, uint8* pContent, uint32* pAttribIds);
#endif /* FEATURE_ENHANCED_BEACON */

/**************************************************************************************************
*/

#endif /* MAC_COORD_H */
