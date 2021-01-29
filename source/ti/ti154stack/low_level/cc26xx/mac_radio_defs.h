/**************************************************************************************************
  Filename:       mac_radio_def.h
  Revised:        $Date: 2015-02-12 15:17:06 -0800 (Thu, 12 Feb 2015) $
  Revision:       $Revision: 42536 $

  Description:    Describe the purpose and contents of the file.


  Copyright 2006-2015 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
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

#ifndef MAC_RADIO_DEFS_H
#define MAC_RADIO_DEFS_H

/* ------------------------------------------------------------------------------------------------
 *                                             Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_defs.h"
#include "hal_board_cfg.h"
#include "hal_mac_cfg.h"
#include "mac_spec.h"
#include "mac_mcu.h"
#include "mac.h"

/* ------------------------------------------------------------------------------------------------
 *                                      Target Specific Defines
 * ------------------------------------------------------------------------------------------------
 */

/* MAC RAT frequency in MHz */
#define MAC_RAT_MHZ                    (4)

/* Backoff to RAT ratio 320/0.25 = 1280 */
#define MAC_BACKOFF_TO_RAT_RATIO       (MAC_SPEC_USECS_PER_BACKOFF * MAC_RAT_MHZ)

/* Symbol to RAT ratio 16/0.25 = 64 */
#define MAC_SYMBOL_TO_RAT_RATIO        (MAC_SPEC_USECS_PER_SYMBOL * MAC_RAT_MHZ)

/* Past time determination window.
 * Note that this value has to be greater than RAT timer past determination
 * window size.
 * TBD: verify that RAT timer past determination window is smaller than 1sec.
 */ 
#define MAC_BACKOFF_PAST_WINDOW        (1000000ul*MAC_RAT_MHZ)

/* CC26xx rollover cannot exceed 0xFFFFFFFF / 1280 = 3355443 or 1073.74 seconds.
 * Past time determination window is also considered */
#define MAC_BACKOFF_MAXIMUM_ROLLOVER                                    \
  ((0xFFFFFFFF - MAC_BACKOFF_PAST_WINDOW)/MAC_BACKOFF_TO_RAT_RATIO)



/* ------------------------------------------------------------------------------------------------
 *                                    Unique Radio Define
 * ------------------------------------------------------------------------------------------------
 */
#define FEATURE_MAC_RADIO_HARDWARE_OVERFLOW_NO_ROLLOVER


/* ------------------------------------------------------------------------------------------------
 *                                    Common Radio Defines
 * ------------------------------------------------------------------------------------------------
 */
#define MAC_RADIO_CHANNEL_DEFAULT               11
#define MAC_RADIO_CHANNEL_INVALID               0xFF
#define MAC_RADIO_TX_POWER_INVALID              0xFFFF

#define MAC_RADIO_RECEIVER_SENSITIVITY_DBM      -100 /* dBm */
#define MAC_RADIO_RECEIVER_SATURATION_DBM       -5   /* dBm */

/* offset applied to hardware RSSI value to get RF power level in dBm units */
#define MAC_RADIO_RSSI_OFFSET                   HAL_MAC_RSSI_OFFSET

/* precise values for small delay on both receive and transmit side */
#define MAC_RADIO_RX_TX_PROP_DELAY_MIN_USEC     3.076  /* usec */
#define MAC_RADIO_RX_TX_PROP_DELAY_MAX_USEC     3.284  /* usec */


/* ------------------------------------------------------------------------------------------------
 *                                      Common Radio Macros
 * ------------------------------------------------------------------------------------------------
 */
#define MAC_RADIO_MCU_INIT()                          macMcuInit()

#define MAC_RADIO_TURN_ON_POWER()                     /* Nothing for CC26xx */
#define MAC_RADIO_TURN_OFF_POWER()                    /* Nothing for CC26xx */

#define MAC_RADIO_SET_RX_THRESHOLD(x)                 /* Nothing for CC26xx */
#define MAC_RADIO_ENABLE_RX_THRESHOLD_INTERRUPT()     /* Nothing for CC26xx */
#define MAC_RADIO_DISABLE_RX_THRESHOLD_INTERRUPT()    /* Nothing for CC26xx */
#define MAC_RADIO_CLEAR_RX_THRESHOLD_INTERRUPT_FLAG() /* Nothing for CC26xx */

#define MAC_RADIO_TX_ACK()                            MAC_RADIO_TURN_OFF_PENDING_OR()
#define MAC_RADIO_TX_ACK_PEND()                       MAC_RADIO_TURN_ON_PENDING_OR()

#define MAC_RADIO_RX_ON()                             st( macSendReceiveCmd(); )
#define MAC_RADIO_RXTX_OFF()                          st( macStopCmd(); )

#define MAC_RADIO_POWER_DOWN()                        st( macPowerDownCmd(); )

#define MAC_RADIO_FLUSH_RX_FIFO()                     /* Do not call macRxQueueFlushCmd()! */
#define MAC_RADIO_FLUSH_TX_FIFO()                     /* Nothing for CC26xx */

#define MAC_RADIO_SET_PAN_COORDINATOR(b)              SET_FRAME_FILTER_PAN_COORDINATOR( macRxEdScan.rxCmd.filterOpt, b )

#define MAC_RADIO_SET_CHANNEL(x)                      st( macRxEdScan.rxCmd.channel = (x == MAC_CHAN_27)?  \
                                                          MAC_CHAN_27_CC26XX : ((x == MAC_CHAN_28)?        \
                                                          MAC_CHAN_28_CC26XX : ((x == MAC_CHAN_29)?        \
                                                          MAC_CHAN_29_CC26XX : x));                        \
                                                        )

#define MAC_RADIO_SET_TX_POWER(x)                     st( macSetTxPowerVal(x); )

#define MAC_RADIO_SET_PAN_ID(x)                       st( macRxEdScan.rxCmd.localPanID = x; )
#define MAC_RADIO_SET_SHORT_ADDR(x)                   st( macRxEdScan.rxCmd.localShortAddr = x; )
#define MAC_RADIO_SET_IEEE_ADDR(p)                    osal_memcpy( macRxEdScan.rxCmd.localExtAddr, p, 8 )

#define MAC_RADIO_REQUEST_ACK_TX_DONE_CALLBACK()      /* Nothing for CC26xx */
#define MAC_RADIO_CANCEL_ACK_TX_DONE_CALLBACK()       /* Nothing for CC26xx */

#define MAC_RADIO_RANDOM_BYTE()                       macMcuRandomByte()
#define MAC_RADIO_RANDOM_WORD()                       macMcuRandomWord()

#define MAC_RADIO_TX_RESET()                          /* Nothing for CC26xx */
#define MAC_RADIO_TX_PREP_CSMA_UNSLOTTED()            macRadioTxPrepCsmaUnslotted()
#define MAC_RADIO_TX_PREP_CSMA_SLOTTED()              macRadioTxPrepCsmaSlotted()
#define MAC_RADIO_TX_PREP_SLOTTED()                   macRadioTxPrepSlotted()
#define MAC_RADIO_TX_PREP_GREEN_POWER()               macRadioTxPrepGreenPower()
#define MAC_RADIO_TX_GO_CSMA()                        macRadioTxGoCsma()
#define MAC_RADIO_TX_GO_SLOTTED()                     macRadioTxGoSlotted()
#define MAC_RADIO_TX_GO_SLOTTED_CSMA()                macRadioTxGoSlottedCsma()
#define MAC_RADIO_TX_GO_GREEN_POWER()                 macRadioTxGoGreenPower()

#define MAC_RADIO_TIMER_TICKS_PER_USEC()              MAC_RAT_MHZ
#define MAC_RADIO_TIMER_TICKS_PER_BACKOFF()          (MAC_RAT_MHZ * MAC_SPEC_USECS_PER_BACKOFF)
#define MAC_RADIO_TIMER_TICKS_PER_SYMBOL()           (MAC_RAT_MHZ * MAC_SPEC_USECS_PER_SYMBOL)

#define MAC_RADIO_TIMER_CAPTURE(x)                    macMcuTimerCapture(x)
#define MAC_RADIO_TIMER_FORCE_DELAY(x)                macMcuTimerForceDelay(x)

#define MAC_RADIO_TIMER_SLEEP()                       /* Nothing for CC26xx */
#define MAC_RADIO_TIMER_WAKE_UP()                     /* Nothing for CC26xx */

#define MAC_RADIO_TURN_ON_OSC()                       /* Nothing for CC26xx */
#define MAC_RADIO_TURN_OFF_OSC()                      /* Nothing for CC26xx */

#define MAC_RADIO_CLEAR_BACKOFF_COUNT()               st( MAC_RAT_ADJ = 0xFFFFFFFF - MAC_RAT_COUNT; )
#define MAC_RADIO_BACKOFF_COUNT()                     ( ( MAC_RAT_COUNT - macPrevPeriodRatCount ) / MAC_BACKOFF_TO_RAT_RATIO )
#define MAC_RADIO_BACKOFF_CAPTURE(x)                  macMcuOverflowCapture(x)
#define MAC_RADIO_BACKOFF_SET_COUNT(x)                macMcuOverflowSetCount(x)
#define MAC_RADIO_BACKOFF_SET_COMPARE(x)              macMcuOverflowSetCompare(x)

#define MAC_RADIO_BACKOFF_COMPARE_CLEAR_INTERRUPT()   /* obselete for CC253x and CC26xx */

#define MAC_RADIO_BACKOFF_SET_PERIOD(x)               macMcuOverflowSetPeriod(x)

#define MAC_RADIO_RECORD_MAX_RSSI_START()             macMcuRecordMaxRssiStart()
#define MAC_RADIO_RECORD_MAX_RSSI_STOP()              macMcuRecordMaxRssiStop()

#define MAC_RADIO_TURN_ON_RX_FRAME_FILTERING()        st( SET_FRAME_FILTER_ON(macRxEdScan.rxCmd.filterOpt); )
#define MAC_RADIO_TURN_OFF_RX_FRAME_FILTERING()       st( SET_FRAME_FILTER_OFF(macRxEdScan.rxCmd.filterOpt); )

/*--------Source Matching------------*/

#define MAC_RADIO_TURN_ON_AUTO_ACK()                  st( SET_AUTO_ACK_ENABLE(macRxEdScan.rxCmd.filterOpt); )
#define MAC_RADIO_CANCEL_TX_ACK()                     /* Nothing for CC26xx */

#define MAC_RADIO_TURN_ON_SRC_MATCH()                 /* Nothing to do for CC26xx */
#define MAC_RADIO_TURN_ON_AUTOPEND()                  st( SET_AUTO_PEND_ENABLE(macRxEdScan.rxCmd.filterOpt); )
#define MAC_RADIO_TURN_ON_PEND_DATA_REQ()             st( SET_DATA_REQ_AUTO_PEND(macRxEdScan.rxCmd.filterOpt); )
                                                            
#define MAC_RADIO_TURN_ON_PENDING_OR()                st( SET_DEFAULT_PEND_ON(macRxEdScan.rxCmd.filterOpt); )
#define MAC_RADIO_TURN_OFF_PENDING_OR()               st( SET_DEFAULT_PEND_OFF(macRxEdScan.rxCmd.filterOpt); )

#define MAC_RADIO_SRC_MATCH_GET_EN(x)                 macSrcMatchGetEnableBit(x)
#define MAC_RADIO_SRC_MATCH_GET_PENDEN(x)             macSrcMatchGetPendEnBit(x)

#define MAC_RADIO_GET_SRC_SHORTPENDEN(x)              st( x = macRxEdScan.rxCmd.pShortEntryList->srcPendEn; )
#define MAC_RADIO_GET_SRC_EXTENPEND(x)                st( x = macRxEdScan.rxCmd.pExtEntryList->srcPendEn; )
#define MAC_RADIO_GET_SRC_SHORTEN(x)                  st( x = macRxEdScan.rxCmd.pShortEntryList->srcMatchEn; )
#define MAC_RADIO_GET_SRC_EXTEN(x)                    st( x = macRxEdScan.rxCmd.pExtEntryList->srcMatchEn; )

#define MAC_RADIO_SRC_MATCH_SET_SHORTPENDEN(x)        st( macRxEdScan.rxCmd.pShortEntryList->srcPendEn = x; )
#define MAC_RADIO_SRC_MATCH_SET_EXTPENDEN(x)          st( macRxEdScan.rxCmd.pExtEntryList->srcPendEn = x; )
#define MAC_RADIO_SRC_MATCH_SET_SHORTEN(x)            st( macRxEdScan.rxCmd.pShortEntryList->srcMatchEn = x; )
#define MAC_RADIO_SRC_MATCH_SET_EXTEN(x)              st( macRxEdScan.rxCmd.pExtEntryList->srcMatchEn = x; )
#define MAC_RADIO_SRC_MATCH_RESULT()                  MAC_SrcMatchCheckResult()

#define MAC_RADIO_SRC_MATCH_INIT_EXTPENDEN()         /* Nothing for CC26xx */ 
#define MAC_RADIO_SRC_MATCH_INIT_SHORTPENDEN()       /* Nothing for CC26xx */
                                                            
#define MAC_RADIO_SRC_MATCH_TABLE_SHORT_WRITE(offset, p, len) osal_memcpy( (macRxEdScan.rxCmd.pShortEntryList->shortAddrEntry + (offset)), (p), (len) )
#define MAC_RADIO_SRC_MATCH_TABLE_EXT_WRITE(offset, p, len)   osal_memcpy( (macRxEdScan.rxCmd.pExtEntryList->extAddrEntry + (offset)), (p), (len) )
                                                            
#define MAC_RADIO_SRC_MATCH_TABLE_SHORT_READ(offset, p, len) osal_memcpy( (p), (macRxEdScan.rxCmd.pShortEntryList->shortAddrEntry + (offset)), (len))
#define MAC_RADIO_SRC_MATCH_TABLE_EXT_READ(offset, p, len)   osal_memcpy( (p), (macRxEdScan.rxCmd.pExtEntryList->extAddrEntry + (offset)), (len))



/* ------------------------------------------------------------------------------------------------
 *                                    Common Radio Externs
 * ------------------------------------------------------------------------------------------------
 */


/**************************************************************************************************
 */
#endif
