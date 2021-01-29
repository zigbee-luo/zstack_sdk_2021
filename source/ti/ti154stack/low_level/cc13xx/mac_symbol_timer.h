/******************************************************************************

 @file  mac_symbol_timer.h

 @brief Describe the purpose and contents of the file.

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

#ifndef MAC_BACKOFF_TIMER_H
#define MAC_BACKOFF_TIMER_H

/* ------------------------------------------------------------------------------------------------
 *                                           Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "mac_high_level.h"
#include "rf_mac_api.h"

/* ------------------------------------------------------------------------------------------------
 *                                         Defines
 * ------------------------------------------------------------------------------------------------
 */
/* Synchronization events */
#define MAC_SYMBOL_TIMER_EVENT_POWER_WAKEUP              0x01
#define MAC_SYMBOL_TIMER_EVENT_POWER_TIMER_EXP           0x02
#define MAC_SYMBOL_TIMER_EVENT_POWER_DOWN                0x04

#define MAC_CSMA_TIMER_EVENT_EXPIRY                      0x08

/* ------------------------------------------------------------------------------------------------
 *                                         Global Variables
 * ------------------------------------------------------------------------------------------------
 */
extern uint32 macSymbolTimerRollover;
extern uint32 macPrevPeriodRatCount;
extern volatile uint8 macSymbolTimerImpending;
extern uint8  macSpecUsecsPerSymbol;
extern uint32 macSymbolTimerTrigger;

/* ------------------------------------------------------------------------------------------------
 *                                           Prototypes
 * ------------------------------------------------------------------------------------------------
 */
#if defined USE_ICALL || defined OSAL_PORT2TIRTOS
MAC_INTERNAL_API void macSymbolSetupPwrMgmt(void);
MAC_INTERNAL_API void macSymbolTimerPowerDown(void);
#endif /* defined USE_ICALL || defined OSAL_PORT2TIRTOS */

MAC_INTERNAL_API void macSymbolTimerInit(void);
MAC_INTERNAL_API void macSymbolTimerReset(void);
MAC_INTERNAL_API void macSymbolTimerCompareIsr(void);
MAC_INTERNAL_API void macSymbolTimerPeriodIsr(void);
MAC_INTERNAL_API void macSymbolTimerRestore(void);
MAC_INTERNAL_API uint32 macGetBackOffTimerRollover(void);
MAC_INTERNAL_API void macSymbolTimerSetTriggerRollover(void);
MAC_INTERNAL_API void macCsmaTimerExpiry(void);
MAC_INTERNAL_API void macSymbolTimerBCWakeUp(void);
MAC_INTERNAL_API void macSymbolTimerChkPowerDown(void);
MAC_INTERNAL_API void macSymbolBCTimerPowerUp(void);
#ifndef OSAL_PORT2TIRTOS
static
#endif /* !OSAL_PORT2TIRTOS */
MAC_INTERNAL_API void macSymbolTimerEventHandler(void);
MAC_INTERNAL_API void macSymbolTimerUpdateWakeup(void);
extern void macSymbolTimerICallTimerCback(void *arg);
/**************************************************************************************************
 */
#endif
