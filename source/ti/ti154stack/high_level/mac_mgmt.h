/******************************************************************************

 @file  mac_mgmt.h

 @brief Internal interface file for the MAC management module.

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

#ifndef MAC_MGMT_H
#define MAC_MGMT_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "mac_main.h"
#include "mac_low_level.h"
#include "mac_high_level.h"

#include "fh_api.h"
#include "fh_pib.h"
#include "fh_ie.h"
#include "fh_nt.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

/* Mask this value with the srcAddrMode when source PAN ID is broadcast */
#define MAC_SRC_PAN_ID_BROADCAST    0x80

/* Macro to check if frequency hopping module is enabled or not */
#define MAC_FH_ENABLED                  (fhObject.fhEnabled)

/* Frequency hopping Reset function */
#define MAC_FH_RESET_FN                 fhObject.pFhResetFn
/* Frequency hopping Start function */
#define MAC_FH_START_FN                 fhObject.pFhStartFn
/* Frequency hopping Start BS function */
#define MAC_FH_START_BS_FN              fhObject.pFhStartBsFn
/* Frequency hopping Stop Async function */
#define MAC_FH_STOP_ASYNC_FN            fhObject.pFhStopAsyncFn
/* Frequency hopping Send data function */
#define MAC_FH_SEND_DATA_FN             fhObject.pFhSendDataFn
/* Frequency hopping Get IE length function */
#define MAC_FH_GET_IE_LEN_FN            fhObject.pFhGetIeLenFn
/* Frequency hopping Generate IE function */
#define MAC_FH_GENERATE_IE_FN           fhObject.pFhGenIeFn
/* Frequency hopping Parse Payload IE function */
#define MAC_FH_PARSE_PIE_FN              fhObject.pFhParsePieFn
/* Frequency hopping Extract header IE function */
#define MAC_FH_EXTRACT_HIE_FN           fhObject.pFhExtractHieFn
/* Frequency hopping Extract Payload IE function */
#define MAC_FH_EXTRACT_PIE_FN           fhObject.pFhExtractPieFn
/* Frequency hopping Parse Header IE function */
#define MAC_FH_PARSE_HIE_FN             fhObject.pFhParseHieFn
/* Frequency hopping Reset PIB function */
#define MAC_FH_RESET_PIB_FN             fhObject.pFhResetPibFn
/* Frequency hopping Set PIB function */
#define MAC_FH_SET_PIB_FN               fhObject.pFhSetPibFn
/* Frequency hopping Get PIB function */
#define MAC_FH_GET_PIB_FN               fhObject.pFhGetPibFn
/* Frequency hopping Get PIB length function */
#define MAC_FH_GET_PIB_LEN_FN           fhObject.pFhGetPibLenFn
/* Frequency hopping Get TX params function */
#define MAC_FH_GET_TX_PARAMS_FN         fhObject.pFhGetTxParamsFn
/* Frequency hopping Complete TX Callback function */
#define MAC_FH_COMPLETE_TX_CB_FN        fhObject.pFhCompleteTxCbFn
/* Frequency hopping Complete RX Callback function */
#define MAC_FH_COMPLETE_RX_CB_FN        fhObject.pFhCompleteRxCbFn
/* Frequency hopping Set state Callback function */
#define MAC_FH_SET_STATE_CB_FN          fhObject.pFhSetStateCbFn
/* Frequency hopping MAC Add Device Callback function */
#define MAC_FH_ADD_DEVICE_FN            fhObject.pFhAddDeviceCB
/* Frequency hopping Get ch hopping function of the target node */
#define MAC_FH_GET_TX_CH_HOP_FN         fhObject.pFhGetTxChHopFn
/* Frequency hopping Get remaining dwell time of the
 * current slot of the target node */
#define MAC_FH_GET_REM_DT_FN            fhObject.pFhGetRemDTFn
/* Frequency hopping MAC update CCA function */
#define MAC_FH_UPDATE_CCA_FN            fhObject.pFhUpdateCCA
/* Frequency hopping MAC compute TX off remaing time */
#define MAC_FH_CHECK_TX_OFF_FN          fhObject.pFhCheckTxOffTime
/* Frequency hopping MAC get CCCA time */
#define MAC_FH_GET_CCA_TIME_FN          fhObject.pFhGetCCATime

#define MAC_FH_GET_BIT_COUNT_FN         fhObject.pFhGetBitCount

#define MAC_FH_BC_TIMER_EVT             fhObject.pFhBcTimerEvent
/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

/* Management info struct */
typedef struct
{
  bool              networkStarted;
} macMgmt_t;


/* Frequency hopping module function pointer typedef's. Refer fh_api.h file */
typedef void (*fhResetFn_t)(void);
typedef void (*fhStartFn_t)(void);
typedef void (*fhStartBsFn_t)(void);
typedef FHAPI_status (*fhStopAsyncFn_t)(void);
typedef void (*fhSendDataFn_t)(void);
typedef uint16_t (*fhGetIeLenFn_t)(uint32_t);
typedef uint16_t (*fhGenIeFn_t)(uint8_t*, uint32_t, macTx_t*, FHIE_ieInfo_t*);
typedef FHAPI_status (*fhParsePieFn_t)(uint8_t*, sAddrExt_t*, uint16_t*, uint16_t*);
typedef FHAPI_status (*fhExtractHieFn_t)(uint8_t*, IE_TYPE_t, uint8_t*);
typedef FHAPI_status (*fhExtractPieFn_t)(uint8_t*, IE_TYPE_t, uint8_t*);
typedef FHAPI_status (*fhParseHieFn_t)(uint8_t*, sAddrExt_t*, uint32_t, uint16_t*, uint16_t*);
typedef void (*fhResetPibFn_t)(void);
typedef FHAPI_status (*fhSetPibFn_t)(uint16_t, void*);
typedef FHAPI_status (*fhGetPibFn_t)(uint16_t, void*);
typedef uint8_t (*fhGetPibLenFn_t)(uint16_t);
typedef FHAPI_status (*fhGetTxParamsFn_t)(uint32_t*, uint8_t*);
typedef void (*fhCompleteTxCbFn_t)(uint8_t);
typedef void (*fhCompleteRxCbFn_t)(macRx_t *pMsg);
typedef void (*fhStateCbFn_t)(FH_LMAC_STATUS_t);
typedef uint16_t (*fhAddDeviceCB)(sAddrExt_t *pEUI, uint16_t devIndex);
typedef uint16_t (*fhDelDeviceCB)(sAddrExt_t *pEUI, uint16_t devIndex);
typedef uint8_t (*fhGetTxChHopFn_t)(void);
typedef uint32_t (*fhGetRemDTFn_t)(void);
typedef void (*fhUpdateCCA)(uint32_t ccaTime);
typedef uint8_t (*fhGetBitCount_t)(void);
typedef void (*fhBcTimerEvt_t)(void);


/* Frequency hopping object type definition */
typedef struct _fhObject
{
  bool                  fhEnabled;
  fhResetFn_t           pFhResetFn;
  fhStartFn_t           pFhStartFn;
  fhStartBsFn_t         pFhStartBsFn;
  fhStopAsyncFn_t       pFhStopAsyncFn;
  fhSendDataFn_t        pFhSendDataFn;
  fhGetIeLenFn_t        pFhGetIeLenFn;
  fhGenIeFn_t           pFhGenIeFn;
  fhParsePieFn_t        pFhParsePieFn;
  fhExtractHieFn_t      pFhExtractHieFn;
  fhExtractPieFn_t      pFhExtractPieFn;
  fhParseHieFn_t        pFhParseHieFn;
  fhResetPibFn_t        pFhResetPibFn;
  fhSetPibFn_t          pFhSetPibFn;
  fhGetPibFn_t          pFhGetPibFn;
  fhGetPibLenFn_t       pFhGetPibLenFn;
  fhGetTxParamsFn_t     pFhGetTxParamsFn;
  fhCompleteTxCbFn_t    pFhCompleteTxCbFn;
  fhCompleteRxCbFn_t    pFhCompleteRxCbFn;
  fhStateCbFn_t         pFhSetStateCbFn;
  fhAddDeviceCB         pFhAddDeviceCB;
  fhDelDeviceCB         pFhDelDeviceCB;
  fhGetTxChHopFn_t      pFhGetTxChHopFn;
  fhGetRemDTFn_t        pFhGetRemDTFn;
  fhUpdateCCA           pFhUpdateCCA;
  fhGetBitCount_t       pFhGetBitCount;
  fhBcTimerEvt_t        pFhBcTimerEvent;
} fhObject_t ;

/* ------------------------------------------------------------------------------------------------
 *                                           Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/* Utility functions */
MAC_INTERNAL_API void macGetCoordAddress(sAddr_t *pAddr);
MAC_INTERNAL_API uint8 macGetMyAddrMode(void);
MAC_INTERNAL_API bool macDestAddrCmp(uint8 *p1, uint8 *p2);
MAC_INTERNAL_API bool macDestSAddrCmp(sAddr_t *pAddr, uint16 panId, uint8 *p);
MAC_INTERNAL_API bool macCoordAddrCmp(sAddr_t *pAddr);
MAC_INTERNAL_API bool macCoordDestAddrCmp(uint8 *p);
MAC_INTERNAL_API uint8 macBuildHeader(macTx_t *pBuf, uint8 srcAddrMode, sAddr_t *pDstAddr,
                                      uint16 dstPanId, uint8 elideSeqNo);
MAC_INTERNAL_API uint8 macBuildDisassociateReq(macEvent_t *pEvent);
MAC_INTERNAL_API uint8 macBuildCommonReq(uint8 cmd, uint8 srcAddrMode, sAddr_t *pDstAddr,
                                        uint16 dstPanId, uint16 txOptions, ApiMac_sec_t *sec);
#ifdef FEATURE_ENHANCED_BEACON
MAC_INTERNAL_API uint8 macBuildCoexistEBeaconReq(macTx_t *pMsg, sAddr_t *pDstAddr, ApiMac_sec_t *sec);

MAC_INTERNAL_API uint8 macBuildEnhanceBeaconReq(macTx_t *pMsg, sAddr_t *pDstAddr, ApiMac_sec_t *sec,

                                                uint8 requestField, uint8 linkQuality,

                                                uint8 percentFilter);

MAC_INTERNAL_API uint8 macBuildEBeaconNotifyInd(macMlmeBeaconNotifyInd_t *pNotifyInd, macEvent_t *pEvent);
#endif /* FEATURE_ENHANCED_BEACON */

#if defined (FEATURE_BEACON_MODE) || defined (FEATURE_NON_BEACON_MODE)
MAC_INTERNAL_API void macBuildBeaconNotifyInd(macMlmeBeaconNotifyInd_t *pNotifyInd, macEvent_t *pEvent);
MAC_INTERNAL_API macTx_t *macBuildBeacon(uint8 beaconOrder, uint8 superframeOrder, bool battLifeExt);
#endif /* (FEATURE_BEACON_MODE) || (FEATURE_NON_BEACON_MODE) */
MAC_INTERNAL_API void macSecCpy(ApiMac_sec_t *pDst, ApiMac_sec_t *pSrc);

MAC_INTERNAL_API void macConflictSyncLossInd(void);
MAC_INTERNAL_API uint8 macGetCSMPhy(uint8 currPhyId);
MAC_INTERNAL_API void macUpdateCsmPhyParam(bool val);
MAC_INTERNAL_API bool macIsCsmOperational(void);
MAC_INTERNAL_API void macSetDefaultsByPhyID(void);
MAC_INTERNAL_API void macSetDefaultsByRE(void);

/* Action functions */
MAC_INTERNAL_API void macNoAction(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiUnsupported(macEvent_t *pEvent);
MAC_INTERNAL_API void macDefaultAction(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiBadState(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiPending(macEvent_t *pEvent);
MAC_INTERNAL_API void macCommStatusInd(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiDisassociateReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macDisassocComplete(macEvent_t *pEvent);
MAC_INTERNAL_API void macRxDisassoc(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiRxEnableReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macTxCsmaDelay(macEvent_t *pEvent);
#if defined (FEATURE_BEACON_MODE) || defined (FEATURE_NON_BEACON_MODE)
MAC_INTERNAL_API void macRxBeacon(macEvent_t *pEvent);
#endif /* (FEATURE_BEACON_MODE) || (FEATURE_NON_BEACON_MODE) */
#ifdef FEATURE_FREQ_HOP_MODE
MAC_INTERNAL_API void macBcTimerHandler(macEvent_t *pEvent);
#endif

void copyApiMacAddrToMacAddr(sAddr_t *pDst, ApiMac_sAddr_t *pSrc);

//bool macCheckPhyLRMMode(void);
//bool macCheckPhyFSKMode(void);


/**************************************************************************************************
*/

#endif /* MAC_MGMT_H */

