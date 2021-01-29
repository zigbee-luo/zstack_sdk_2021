/******************************************************************************

 @file  mac_settings.c

 @brief This file contains CM0 radio command variables.

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

//*********************************************************************************
// Default parameter summary
// Address: N/A
// Frequency: 920.00000 MHz
// Data Format: Serial mode disable
// Deviation: 25.000 kHz
// Packet Length Config: Variable
// Max Packet Length: 1040
// Packet Length: Variable
// RX Filter BW: 110 kHz
// Symbol Rate: 50.00000 kBaud
// Sync Word Length: 16 Bits
// Whitening: Whitening Enabled

/* ------------------------------------------------------------------------------------------------
 *                                           Includes
 * ------------------------------------------------------------------------------------------------
 */

#ifndef LIBRARY
#include "ti_154stack_features.h"
#endif

#include <stdint.h>
#include "mac_settings.h"
#include "rf_mac_api.h"


// CMD_RADIO_SETUP for IEEE 15.4
#if defined (DeviceFamily_CC13X2)
/* Use dynamic PA struct type for 13x2 */
rfc_CMD_RADIO_SETUP_PA_t RF_cmdRadioSetup =
#else
rfc_CMD_RADIO_SETUP_t RF_cmdRadioSetup =
#endif
{
    .commandNo = 0x0802,
    .status = 0x0000,
    .pNextOp = 0x00000000,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
    .mode = 0x01,
#if defined(DeviceFamily_CC13X0) || defined(DeviceFamily_CC26X2) || defined(DeviceFamily_CC13X2)
    .loDivider = 0x00,
#else
    .__dummy0 = 0x00,
#endif
    .config.frontEndMode = 0x0,
#if defined(DeviceFamily_CC13X2)
    .config.biasMode = 0x1,
#else
    .config.biasMode = 0x0,
#endif
#if defined (DeviceFamily_CC13X2)
    .config.analogCfgMode = 0x0,
#endif
    .config.bNoFsPowerUp = 0x0,
#if defined(DeviceFamily_CC13X0)
    .txPower = 0x3D3F,
#elif defined(DeviceFamily_CC26X0)
    .txPower = 0x9330,
#elif defined(DeviceFamily_CC13X2)
    .txPower = 0xAB3F,
#elif defined(DeviceFamily_CC26X2)
    .txPower = 0x7217,
#else
    .txPower = 0x7217,
#endif
    .pRegOverride = NULL,
#if defined (DeviceFamily_CC13X2)
    .pRegOverrideTxStd = NULL,
    .pRegOverrideTx20 = NULL,
#endif
};

/* IEEE TX Command */
rfc_CMD_IEEE_TX_t RF_cmdIEEETx =
{
    .commandNo = CMD_IEEE_TX,
    .status = 0,
    .pNextOp = 0,
    .startTime = 0,
    .startTrigger.triggerType = TRIG_NOW,
    .startTrigger.bEnaCmd = 0,
    .startTrigger.triggerNo = 0,
    .startTrigger.pastTrig = 1,
    .condition.rule = COND_NEVER,
    .condition.nSkip = 0,
    .txOpt.bIncludePhyHdr = 0,
    .txOpt.bIncludeCrc = 0,
    .txOpt.payloadLenMsb = 0,
    .payloadLen = 0,
    .pPayload = 0,
    .timeStamp = 0,
};

/* IEEE RX Command */
rfc_CMD_IEEE_RX_t RF_cmdIEEERx =
{
    .commandNo                  = CMD_IEEE_RX,
    .status                     = 0x0000,
    .pNextOp                    = 0, /*INSERT APPLICABLE POINTER: (uint8_t*)&xxx */
    .startTime                  = 0x00000000,
    .startTrigger.triggerType   = 0x0,
    .startTrigger.bEnaCmd       = 0x0,
    .startTrigger.triggerNo     = 0x0,
    .startTrigger.pastTrig      = 0x0,
    .condition.rule             = 0x1,
    .condition.nSkip            = 0x0,
    .channel                    = 0x00,
    .rxConfig.bAutoFlushCrc     = 0x1,
    .rxConfig.bAutoFlushIgn     = 0x0,
    .rxConfig.bIncludePhyHdr    = 0x1,
    .rxConfig.bIncludeCrc       = 0x0,
    .rxConfig.bAppendRssi       = 0x1,
    .rxConfig.bAppendCorrCrc    = 0x0,
    .rxConfig.bAppendSrcInd     = 0x0,
    .rxConfig.bAppendTimestamp  = 0x1,
    .pRxQ                       = 0x00000000,
    .pOutput                    = 0x00000000,
    .frameFiltOpt.frameFiltEn   = 0x0,
    .frameFiltOpt.frameFiltStop = 0x0,
    .frameFiltOpt.autoAckEn     = 0x0,
    .frameFiltOpt.slottedAckEn  = 0x0,
    .frameFiltOpt.autoPendEn    = 0x0,
    .frameFiltOpt.defaultPend   = 0x0,
    .frameFiltOpt.bPendDataReqOnly  = 0x0,
    .frameFiltOpt.bPanCoord         = 0x0,
    .frameFiltOpt.maxFrameVersion   = 0x3,
    .frameFiltOpt.fcfReservedMask   = 0x0,
    .frameFiltOpt.modifyFtFilter    = 0x0,
    .frameFiltOpt.bStrictLenFilter  = 0x0,
    .frameTypes.bAcceptFt0Beacon    = 0x1,
    .frameTypes.bAcceptFt1Data      = 0x1,
    .frameTypes.bAcceptFt2Ack       = 0x1,
    .frameTypes.bAcceptFt3MacCmd    = 0x1,
    .frameTypes.bAcceptFt4Reserved  = 0x1,
    .frameTypes.bAcceptFt5Reserved  = 0x1,
    .frameTypes.bAcceptFt6Reserved  = 0x1,
    .frameTypes.bAcceptFt7Reserved  = 0x1,
    .ccaOpt.ccaEnEnergy         = 0x0,
    .ccaOpt.ccaEnCorr           = 0x0,
    .ccaOpt.ccaEnSync           = 0x0,
    .ccaOpt.ccaCorrOp           = 0x1,
    .ccaOpt.ccaSyncOp           = 0x1,
    .ccaOpt.ccaCorrThr          = 0x0,
    .ccaRssiThr                 = 0x64,
    .__dummy0                   = 0x0,
    .numExtEntries              = 0x00,
    .numShortEntries            = 0x00,
    .pExtEntryList              = 0x00000000,
    .pShortEntryList            = 0x00000000,
    .localExtAddr               = 0x0000000000000000,
    .localShortAddr             = 0x0000,
    .localPanID                 = 0x0000,
    .__dummy1                   = 0x0000,
    .__dummy2                   = 0x00,
    .endTrigger.triggerType     = 0x1,
    .endTrigger.bEnaCmd         = 0x0,
    .endTrigger.triggerNo       = 0x0,
    .endTrigger.pastTrig        = 0x0,
    .endTime                    = 0x00000000,
};

rfc_CMD_IEEE_CSMA_t RF_cmdIEEECsma =
{
    .commandNo                  = CMD_IEEE_CSMA,
    .status                     = 0x0000,
    .pNextOp                    = (rfc_radioOp_t *)&RF_cmdIEEETx,
    .startTime                  = 0,
    .startTrigger.triggerType   = 0x0,
    .startTrigger.bEnaCmd       = 0x0,
    .startTrigger.triggerNo     = 0x0,
    .startTrigger.pastTrig      = 0x1, // command can be delayed and appended to the end of the rf_cmdQ.
    .condition.rule             = COND_STOP_ON_FALSE,
    .condition.nSkip            = 0,
    .randomState                = 0,
    .macMaxBE                   = 5,
    .macMaxCSMABackoffs         = 4,
    .csmaConfig.initCW          = 1,
    .csmaConfig.bSlotted        = 0,
    .csmaConfig.rxOffMode       = 0,
    .NB                         = 0,
    .BE                         = 3,
    .remainingPeriods           = 0,
    .lastRssi                   = 0,
    .endTrigger.triggerType     = 0x1,
    .endTrigger.bEnaCmd         = 0x0,
    .endTrigger.triggerNo       = 0x0,
    .endTrigger.pastTrig        = 0x0,
    .lastTimeStamp              = 0,
    .endTime                    = 0,
};

rfc_CMD_IEEE_RX_ACK_t RF_cmdRxAckCmd =
{
    .commandNo                  = CMD_IEEE_RX_ACK,
    .status                     = IDLE,
    .startTrigger.triggerType   = TRIG_NOW,
    .startTrigger.pastTrig      = 1, // XXX: workaround for RF scheduler
    .endTrigger.triggerType     = TRIG_REL_START,
    .endTrigger.pastTrig        = 1,
    .condition.rule             = COND_NEVER,
    .pNextOp                    = NULL,
    /* number of RAT ticks to wait before claiming we haven't received an
     * ack
     */
    .endTime                    = ((IEEE802154_MAC_ACK_WAIT_DURATION * PLATFORM_RADIO_RAT_TICKS_PER_SEC)/IEEE802154_SYMBOLS_PER_SEC),
};

// CMD_NOP
rfc_CMD_NOP_t RF_cmdNop =
{
    .commandNo = 0x0801,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x0,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
};

// CMD_NOP Foreground command 0x0400 = FG
rfc_CMD_NOP_t RF_cmdNopFg =
{
    .commandNo = 0x0C01,
    .status = 0x0000,
    .pNextOp = 0,
    .startTime = 0x00000000,
    .startTrigger.triggerType = 0x0,
    .startTrigger.bEnaCmd = 0x0,
    .startTrigger.triggerNo = 0x0,
    .startTrigger.pastTrig = 0x1,
    .condition.rule = 0x1,
    .condition.nSkip = 0x0,
};

// CMD_SET_RAT_CMP
rfc_CMD_SET_RAT_CMP_t RF_cmdRat =
{
    .commandNo = 0x000A,
    .ratCh = 0,
    .__dummy0 = 0,
    .compareTime = 0,
};


// CMD_SET_RAT_CMP
rfc_CMD_SET_RAT_CMP_t RF_cmdRat1 =
{
    .commandNo = 0x000A,
    .ratCh = 1,
    .__dummy0 = 0,
    .compareTime = 0,
};
