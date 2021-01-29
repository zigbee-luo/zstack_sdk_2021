/******************************************************************************

 @file fh_mgr.h

 @brief TIMAC 2.0 FH manager API

 Group: WCS LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2016-2021, Texas Instruments Incorporated
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

/******************************************************************************
 Includes
 *****************************************************************************/

#ifndef FH_MGR_H
#define FH_MGR_H

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_types.h"
#include "fh_pib.h"
#include "fh_data.h"
#include "mac_low_level.h"

/*! Clock tick period  */
#define CLOCK_TICK_PERIOD     (10)
/*! tick number for one ms  */
#define TICKPERIOD_MS_US      (1000/(CLOCK_TICK_PERIOD))

#define FH_UC_DWELLTIME_BUF		(1)
/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                           Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/*!
 * @brief       start the FH one shot timer
 *
 * @param pTimer - pointer to FH timer object. It can be either broadcast or unicast
 *

 */
void FHMGR_macStartOneShotTimer(FH_macFHTimer_t *pTimer);

/*!
 * @brief       start the FH timer
 *
 * @param pTimer - pointer to FH timer object. It can be either broadcast or unicast
 * @param dedicated - dedicated RAT timer (yes or no)
 *

 */
void FHMGR_macStartFHTimer(FH_macFHTimer_t *pTimer, bool dedicated);

/*!
 * @brief       stop the FH timer
 *
 * @param pTimer - pointer to FH timer object. It can be either broadcast or unicast
 *

 */
void FHMGR_macCancelFHTimer(FH_macFHTimer_t *pTimer);

/*!
 * @brief       get the elapsed time since the timer is started.
 *
 * @param pTimer - pointer to FH timer object. It can be either broadcast or unicast
 *

 */
uint32_t FHMGR_macGetElapsedFHTime(FH_macFHTimer_t *pTimer);

/*!
 * @brief       broadcast timer callback function.
 *
 * @param parameter - parameter passed by the ISR
 *
 */
void FHMGR_bcTimerIsrCb(uint8_t parameter);

/*!
 * @brief       unicast timer callback function.
 *
 * @param parameter - parameter passed by the ISR
 *
 */
void FHMGR_ucTimerIsrCb(uint8_t parameter);

/*!
 * @brief       update the Radio BC channel
 *
 * @param pData - pointer to FH handler
 *
 */
void FHMGR_updateRadioBCChannel(void *pData);

/*!
 * @brief       update the Radio UC channel
 *
 * @param pData - pointer to FH handler
 *
 */
void FHMGR_updateRadioUCChannel(void *pData);

/*!
 * @brief       update the UCchannel data channel pending flag
 *
 * @param pData - pointer to FH handler
 *
 */
void FHMGR_pendUcChUpdate(void *pData);

/*!
 * @brief       update the BC channel data pending flag
 *
 * @param pData - pointer to FH handler
 *
 */
void FHMGR_pendBcChUpdate(void *pData);

/*!
 * @brief       during hopping state, update the channel and packet
 *
 * @param pData - pointer to FH handler
 *
 */
void FHMGR_updateHopping(void *pData);


void FHMGR_BCTimerEventUpd(void);

#endif

