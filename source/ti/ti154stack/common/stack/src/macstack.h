/******************************************************************************

 @file  macstack.h

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

#include "mac_api.h"

typedef struct
{
  macEventHdr_t               hdr;
  uint8                       paramID;
  void                        *paramValue;
} macSetParam_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint8                       paramID;
  uint8                       len;
  uint8                       *pValue;
} macGetParam_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint8                       paramID;
  uint8                       len;
  uint8                       **pValue;
} macGetSecurityPtrParam_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint8                       srctaskid;
  uint8                       retransmit;
  uint8                       pendingMsg;
  uint8 (*pMacCbackQueryRetransmit)();
  uint8 (*pMacCbackCheckPending)();
} macStackInitParams_t;

typedef struct
{
  macEventHdr_t               hdr;
  sAddr_t                     *addr;
  uint16                      panID;
} macSrcMatchEntry_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint8                       options;
} macSrcMatchAckPend_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint8                       pend;
} macSrcMatchCheckPend_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint8                       txPwrTblIdx;
  uint8                       rssiAdjIdx;
} macSetRadioTable_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint16                      panId;
} macUpdatePanId_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint8                       setDefaultPib;
  uint8                       status;
}macResetReq_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint16                      len;
  uint8                       securityLevel;
  uint8                       keyIdMode;
  uint32                      includeFhIEs;
  uint16                      payloadIeLen;
  macMcpsDataReq_t            *pDataReq;
}macMcpsDataAlloc_t;


typedef struct
{
  macEventHdr_t               hdr;
  uint8                       msduhandle;
}macPurgeReq_t;

typedef struct
{
  macEventHdr_t               hdr;
  uint8                       randByte;
}randomByte_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mlmeAssociateReq_t   associateReq;
}macMlmeAssociateReqEvt_t;

typedef struct
{
  macEventHdr_t              hdr;
  ApiMac_mlmeAssociateRsp_t  associateRsp;
}macMlmeAssociateRspEvt_t;

typedef struct
{
  macEventHdr_t                hdr;
  ApiMac_mlmeDisassociateReq_t disAssociateReq;
}macMlmeDisassociateReqEvt_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mlmeOrphanRsp_t      orphanRsp;
}macMlmeOrphanRspEvt_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mlmePollReq_t        pollReq;
}macMlmePollReqEvt_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mlmeScanReq_t        scanReq;
}macMlmeScanReqEvt_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mlmeStartReq_t       startReq;
}macMlmeStartReqEvt_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mlmeSyncReq_t        syncReq;
}macMlmeSyncReqEvt_t;

typedef struct
{
  macEventHdr_t               hdr;
  uint16                      paramID;
  void                        *paramValue;
} macFHSetParam_t;

typedef struct
{
  macEventHdr_t               hdr; //!< hdr event field must be set as ICALL_CMD_EVENT
  uint16                      paramID;
  uint8                       len;
  uint8                       *pValue;
} macFHGetParam_t;


typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mlmeWSAsyncReq_t     asyncReq;
}macMlmeWSAsyncReqEvt_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_secAddDevice_t      *param;
} macSecAddDevice_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_sAddrExt_t          *param;
} macSecDelDevice_t;

typedef struct
{
  macEventHdr_t               hdr;
  uint8                       keyIndex; /* mac secuirty key table index of the key to be removed */
} macSecDelKeyAndDevices_t;

typedef struct
{
  macEventHdr_t               hdr;
  uint8                       keyId; /* key ID */
  uint32                      frameCounter; /* outgoing frame counter of the key */
} macSecGetDefaultSrcKey_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_secAddKeyInitFrameCounter_t  *param;
} macSecAddKeyInitFC_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mcpsDataReq_t        dataReq;
} macMcpsDataReqEvt_t;

typedef union
{
  macEventHdr_t               hdr;
  macStackInitParams_t        macInit;
  macSetParam_t               setParam;
  macGetParam_t               getParam;
  macSrcMatchEntry_t          srcMatchEntry;
  macSrcMatchAckPend_t        srcMatchAckPend;
  macSrcMatchCheckPend_t      srcMatchCheckPend;
  macSetRadioTable_t          setRadioTable;
  macUpdatePanId_t            updatePanId;
  macResetReq_t               resetReq;
  macMcpsDataAlloc_t          mcpsDataAlloc;
  macMcpsDataReqEvt_t         mcpsDataReq;
  randomByte_t                randomByte;
  macPurgeReq_t               purgeReq;
  macMlmeAssociateReqEvt_t    associateReqEvt;
  macMlmeAssociateRspEvt_t    associateRspEvt;
  macMlmeDisassociateReqEvt_t disassociateReqEvt;
  macMlmeOrphanRspEvt_t       orphanRspEvt;
  macMlmePollReqEvt_t         pollReqEvt;
  macMlmeScanReqEvt_t         scanReqEvt;
  macMlmeStartReqEvt_t        startReqEvt;
  macMlmeSyncReqEvt_t         syncReqEvt;
  macGetSecurityPtrParam_t    getPtrParam;
  macFHSetParam_t             fhSetParam;
  macFHGetParam_t             fhGetParam;
  macMlmeWSAsyncReqEvt_t      asyncReqEvt;
  macSecAddDevice_t           secAddDevice;
  macSecDelDevice_t           secDelDevice;
  macSecDelKeyAndDevices_t    secDelKeyAndDevices;
  macSecGetDefaultSrcKey_t    secGetDefaultSrcKey;
  macSecAddKeyInitFC_t        secAddKeyInitFC;
} macCmd_t;


void macStackTaskInit(uint8 taskId);
uint16 macStackEventLoop(uint8 taskId, uint16 events);

#ifdef __cplusplus
};
#endif

#endif /* MAC_STACK_H */


