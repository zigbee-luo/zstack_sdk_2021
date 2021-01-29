/******************************************************************************

 @file  mac_hl_patch.h

 @brief This file includes typedefs and functions required for high level mac patches.

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

#ifndef MAC_HL_PATCH_H
#define MAC_HL_PATCH_H
#if defined(TIMAC_ROM_PATCH)
#include "mac_main.h"

#if defined (FEATURE_BEACON_MODE) || defined (FEATURE_NON_BEACON_MODE)
#include "mac_timer.h"
#include "mac_beacon_device.h"
#include "mac_symbol_timer.h"
#endif //defined (FEATURE_BEACON_MODE) || defined (FEATURE_NON_BEACON_MODE)
#endif

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                          Constants
 * ------------------------------------------------------------------------------------------------
 */
#define PHY_MODE_STD            (1)
#define PHY_MODE_GEN            (2)
#define PHY_MODE_FSK_50K        (1)
#define PHY_MODE_FSK_150K       (2)
#define PHY_MODE_FSK_200K       (3)
#define PHY_MODE_SLR_5K         (4)
#define PHY_MODE_UNDEF          (0)
/* ------------------------------------------------------------------------------------------------
 *                                          Macros
 * ------------------------------------------------------------------------------------------------
 */

/*--------------------------------------------------------------------------------------------------
*           Typedef
*---------------------------------------------------------------------------------------------------
*/
typedef struct AESCCM_Transaction {
    uint8 securityLevel;
    uint8 *pKey;
    uint8 *header;
    uint8 *data;
    uint8 *mic;
    uint8 *nonce;
    uint16 headerLength;
    uint16 dataLength;
    uint16 micLength;
} AESCCM_Transaction;
/*--------------------------------------------------------------------------------------------------
*           Functions
*---------------------------------------------------------------------------------------------------
*/

uint8 macCheckPhyRate(uint8 phyID);
uint8 macCheckPhyMode(uint8 phyID);
uint8 macPibIndex(uint8 pibAttribute);
uint8 macPibCheckByPatch(uint8 pibAttribute, void *pValue);
void macSetDefaultsByPatch(uint8 pibAttribute);
uint8 macCcmEncrypt(AESCCM_Transaction *trans);
uint8 macCcmDecrypt(AESCCM_Transaction *trans);

#if defined(TIMAC_ROM_PATCH)

#if defined (FEATURE_BEACON_MODE) || defined (FEATURE_NON_BEACON_MODE)
void macTrackTimeoutCallback(uint8 param);
void macTrackStartCallback(uint8 param);
void macBeaconStopTrack(void);
#endif

MAC_INTERNAL_API void macScanRxCoordRealign(macEvent_t *pEvent);
MAC_INTERNAL_API void macScanNextChan(macEvent_t *pEvent);
MAC_INTERNAL_API void macEdScanISR(void *arg);
MAC_INTERNAL_API void macBeaconSyncLoss(void);

uint8 macWrapperAddDevice(ApiMac_secAddDevice_t *param);
uint8 macWrapperDeleteDevice(ApiMac_sAddrExt_t *param);
uint8 macWrapperAddKeyInitFCtr( ApiMac_secAddKeyInitFrameCounter_t *param );

MAC_INTERNAL_API uint8 macIncomingFrameSecurity( macRx_t *pMsg );
MAC_INTERNAL_API uint8 macCcmStarTransform( uint8    *pKey,
                                            uint32   frameCounter,
                                            uint8    securityLevel,
                                            uint8    *pAData,
                                            uint16   aDataLen,
                                            uint8    *pMData,
                                            uint16   mDataLen );
MAC_INTERNAL_API uint8 macOutgoingFrameSecurity( macTx_t  *pBuf,
                                                 sAddr_t  *pDstAddr,
                                                 uint16   dstPanId,
                                                 keyDescriptor_t **ppKeyDesc );

MAC_INTERNAL_API uint8 macBeaconCheckStartTime(macEvent_t *pEvent);

MAC_INTERNAL_API uint8_t getLostBeaconCount(void);

uint8 macCheckPendAddr(uint8 pendAddrSpec, uint8 *pAddrList);

void MAC_ResumeReq(void);
void macMgmtReset(void);
#ifdef FEATURE_FREQ_HOP_MODE
extern MAC_INTERNAL_API void macApiWSAsyncReq(macEvent_t *pEvent);
#endif /* FEATURE_FREQ_HOP_MODE */

MAC_INTERNAL_API void macTxBackoffHandler(macEvent_t *pEvent);
MAC_INTERNAL_API void macRxBackoffHandler(macEvent_t *pEvent);

MAC_INTERNAL_API uint8 macBuildDataFrame(macEvent_t *pEvent);

#endif /* defined(TIMAC_ROM_PATCH) */

#endif /* MAC_HL_PATCH_H */

