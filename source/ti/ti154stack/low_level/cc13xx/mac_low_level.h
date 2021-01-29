/******************************************************************************

 @file  mac_low_level.h

 @brief Describe the purpose and contents of the file.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2006-2021, Texas Instruments Incorporated
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

#ifndef MAC_LOW_LEVEL_H
#define MAC_LOW_LEVEL_H

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_types.h"
#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                            Defines
 * ------------------------------------------------------------------------------------------------
 */
#define MAC_SYMBOL_TIMER_DEFAULT_ROLLOVER  (((uint32) MAC_A_BASE_SUPERFRAME_DURATION) << 14)

/* 15,728,640 symbols or 314.57 Seconds */
#define MAC_SYMBOL_TIMER_DEFAULT_NONBEACON_ROLLOVER  MAC_SYMBOL_TIMER_DEFAULT_ROLLOVER

/* Overhead delay in symbols incurred by radio driver and RTOS ~ 12ms
 * This delay should only be added for RX related timeout such as
 * ACK timeout. 75 symbols added to account for BT-IE.
 */
#define MAC_PROCESS_DELAY                   675
#define MAC_LRM_PROCESS_DELAY               3000
#define MAC_IEEE_PROCESS_DELAY              150

/* macTxFrame() parameter values for txType */
#define MAC_TX_TYPE_SLOTTED_CSMA            0x00
#define MAC_TX_TYPE_UNSLOTTED_CSMA          0x01
#define MAC_TX_TYPE_SLOTTED                 0x02
#define MAC_TX_TYPE_NO_CSMA                 0x03
#define MAC_TX_TYPE_FH_CSMA                 0x04
#define MAC_TX_TYPE_LBT                     0x05
#define MAC_TX_TYPE_GREEN_POWER             0x06

/* FH enhanced ACK, there is CCA in UTIE or BTIE */
#define MAC_TX_TYPE_FH_ENH_ACK              0xFF

/* macSleep() parameter values for sleepState */
#define MAC_SLEEP_STATE_OSC_OFF             0x01
#define MAC_SLEEP_STATE_RADIO_OFF           0x02

#ifdef FH_BENCHMARK_BUILD_TIMES
#define NUM_MAC_BENCHMARK_CNT 4
#endif

/* Slotted delay value for beacon mode transmission */
#ifndef HAL_MAC_TX_SLOTTED_DELAY
#define HAL_MAC_TX_SLOTTED_DELAY    48
#endif

#define MAC_MRFSK_20_KSPS_BEACON_DELAY_FACTOR     1 /* 20K symbol rate factor for beacon tx */
#define MAC_MRFSK_50_KSPS_BEACON_DELAY_FACTOR     1 /* 50K symbol rate factor for beacon tx */
#define MAC_MRFSK_150_KSPS_BEACON_DELAY_FACTOR    2 /* 150K symbol rate factor for beacon tx */
#define MAC_MRFSK_200_KSPS_BEACON_DELAY_FACTOR    5 /* 200K symbol rate factor for beacon tx */

#ifdef COMBO_MAC
#define SELECT_CALL(name, ...)                  \
  uint8 rfFreq = pMacPib->rfFreq;               \
                                                \
  if (rfFreq == MAC_RF_FREQ_SUBG)               \
  {                                             \
      (name ## SubG(__VA_ARGS__));              \
  }                                             \
  else                                          \
  {                                             \
      (name ## Ieee(__VA_ARGS__));              \
  }

#define SELECT_RCALL(name, ...)                 \
  uint8 rfFreq = pMacPib->rfFreq;               \
                                                \
  if (rfFreq == MAC_RF_FREQ_SUBG)               \
  {                                             \
      return (name ## SubG(__VA_ARGS__));       \
  }                                             \
  else                                          \
  {                                             \
      return (name ## Ieee(__VA_ARGS__));       \
  }
#else
#ifndef FREQ_2_4G
#define SELECT_CALL(name, ...)                  \
  (name ## SubG(__VA_ARGS__));

#define SELECT_RCALL(name, ...)                 \
  return (name ## SubG(__VA_ARGS__));
#else
#define SELECT_CALL(name, ...)                  \
  (name ## Ieee(__VA_ARGS__));

#define SELECT_RCALL(name, ...)                 \
  return (name ## Ieee(__VA_ARGS__));
#endif
#endif

#define RF_SWITCH_TO_2_4G   0
#define RF_SWITCH_TO_SUBG   1
/* ------------------------------------------------------------------------------------------------
 *                                           Global Externs
 * ------------------------------------------------------------------------------------------------
 */
extern uint8 macTxSlottedDelay;
extern uint8 macTxSlottedDelayFactor;

/* beacon interval margin */
extern uint16 macBeaconMargin[];

/* MAC_A_UNIT_BACKOFF_PERIOD in symbols */
extern uint8 macUnitBackoffPeriod;


/* ------------------------------------------------------------------------------------------------
 *                                           Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/* mac_low_level.c */
MAC_INTERNAL_API void macLowLevelInit(void);
MAC_INTERNAL_API void macLowLevelBufferInit(void);
MAC_INTERNAL_API void macLowLevelReset(void);
MAC_INTERNAL_API void macLowLevelResume(void);
MAC_INTERNAL_API bool macLowLevelYield(void);
MAC_INTERNAL_API void macLowLevelDiags(uint8 pibAttribute);

/* mac_sleep.c */
MAC_INTERNAL_API void macSleepWakeUp(void);
MAC_INTERNAL_API uint8 macSleep(uint8 sleepState);
#if defined USE_ICALL || defined OSAL_PORT2TIRTOS
MAC_INTERNAL_API uint8 macCanSleep(void);
#endif /* defined USE_ICALL || defined OSAL_PORT2TIRTOS */

/* mac_radio.c */
MAC_INTERNAL_API uint8 macRadioRandomByte(void);
MAC_INTERNAL_API void macRadioSetPanCoordinator(uint8 panCoordinator);
MAC_INTERNAL_API void macRadioSetPanID(uint16 panID);
MAC_INTERNAL_API void macRadioSetShortAddr(uint16 shortAddr);
MAC_INTERNAL_API void macRadioSetIEEEAddr(uint8 * pIEEEAddr);
MAC_INTERNAL_API uint8 macRadioSetTxPower(int8 txPower);
MAC_INTERNAL_API uint32 macGetRadioTxPowerReg(int8 txPower);
MAC_INTERNAL_API bool macRadioSetChannel(uint8 channel);
MAC_INTERNAL_API bool macRadioSetChannelRx(uint8 channel);
MAC_INTERNAL_API void macRadioStartScan(uint8 scanType);
MAC_INTERNAL_API void macRadioStopScan(void);
MAC_INTERNAL_API uint8 macRadioEnergyDetectStop(void);

/* mac_radio_tx.c */
MAC_INTERNAL_API void macRadioTxEnhAckCb(macTx_t *pMsg);

/* mac_symbol_timer.c */
MAC_INTERNAL_API void macSymbolTimerSetRollover(uint32 rolloverSymbol);
MAC_INTERNAL_API void macSymbolTimerSetCount(uint32 symbol);
MAC_INTERNAL_API uint32 macSymbolTimerCount(void);
MAC_INTERNAL_API uint32 macSymbolTimerGetTrigger(void);
MAC_INTERNAL_API void macSymbolTimerSetTrigger(uint32 triggerSymbol);
MAC_INTERNAL_API void macSymbolTimerTriggerCallback(void);
MAC_INTERNAL_API void macSymbolTimerRolloverCallback(void);
MAC_INTERNAL_API int32 macSymbolTimerRealign(macRx_t *pMsg);

/* mac_tx.c */
MAC_INTERNAL_API void macTxFrame(uint8 txType);
MAC_INTERNAL_API void macTxFrameRetransmit(void);
MAC_INTERNAL_API void macTxCompleteCallback(uint8 status);

/* mac_rx.c */
MAC_INTERNAL_API bool macRxCheckPendingCallback(void);
MAC_INTERNAL_API bool macRxCheckMACPendingCallback(void);
MAC_INTERNAL_API void macRxCompleteCallback(macRx_t * pMsg);
MAC_INTERNAL_API uint8 macRxSequenceNum(void);

/* mac_rx_onoff.c */
MAC_INTERNAL_API bool macRxEnable(uint8 flags);
MAC_INTERNAL_API void macRxSoftEnable(uint8 flags);
MAC_INTERNAL_API void macRxDisable(uint8 flags);
MAC_INTERNAL_API void macRxSoftDisable(uint8 flags);
MAC_INTERNAL_API void macRxHardDisable(void);

/**************************************************************************************************
 */
#endif
