/******************************************************************************

 @file fh_util.h

 @brief TIMAC 2.0 FH utility API

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

#ifndef FH_UTIL_H
#define FH_UTIL_H

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_types.h"
#include "fh_pib.h"
#include "fh_nt.h"
#include "mac_low_level.h"

 /* ------------------------------------------------------------------------------------------------
 *                                            Defines
 * ------------------------------------------------------------------------------------------------
 */


/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */



/* ------------------------------------------------------------------------------------------------
 *                                           Global Externs
 * ------------------------------------------------------------------------------------------------
 */



/* ------------------------------------------------------------------------------------------------
 *                                           Prototypes
 * ------------------------------------------------------------------------------------------------
 */

/*!
 * @brief       This function computes the backoff duration to be used for
 *              a broadcast transmission
 *
 * @param       pBackOffDur - pointer to current value of backoff duration
 * @param       pChIdx - The pointer to current chosen channel index
 *
 * @return      The status of the operation, as follows:<BR>
 *              [FHAPI_STATUS_SUCCESS](@ref FHAPI_STATUS_SUCCESS)
 *               - Operation successful<BR>
 *              [FHAPI_STATUS_ERR_OUT_SLOT] (@ref FHAPI_STATUS_ERR_OUT_SLOT)
 *               - the back off will be out of slot (unicast or broadcast)<BR>
 */
FHAPI_status FHUTIL_getBcTxParams(uint32_t *pBackOffDur, uint8_t *pChIdx);

/*!
 * @brief       Calcultaes the tx channel for unicast transmission
 *              at a given offset
 *
 * @param       pEntry - Pointer to neighbor table entry
 * @param       txUfsi - The current ufsi value for the neighbor
 * @param       offset - the desired offset after which the frame is to
 *              be transmitted
 *
 * @return      uint8_t - computed tx channel
 */
uint8_t FHUTIL_getTxChannel(NODE_ENTRY_s *pEntry, uint32_t txUfsi, uint32_t offset);

/*!
 * @brief       This function is called by low level to update backoffDuration
 *
 * @param       pEntry - Pointer to neighbor table entry
 * @param       pBackOffDur - The current value of chosen backoff duration.
 * @param       txUfsi - The current ufsi value for the neighbor
 */
FHAPI_status FHUTIL_adjBackOffDur(NODE_ENTRY_s *pEntry, uint32_t *pBackoffDur, uint32_t *txUfsi);

/*!
 * @brief       Asserts the exection based on error conditions
 *
 * @param       pData - pointer to data with error information
 */
void FHUTIL_assert(void *pData);

/*!
 * @brief       Performans no action. useful for state transisions with no special
 *              handling requirements
 *
 * @param       pData - pointer to data with relevant information
 */
void FHUTIL_noAction(void *pData);

/*!
 * @brief       This function gets the current value of broadcast fractional
 *              interval offset
 *
 * @param       bfio - pointer to bfio value
 * @param       slotIdx - pointer to value of brodcast slot id
 *
 * @return      The status of the operation, as follows:<BR>
 *              [FHAPI_STATUS_SUCCESS](@ref FHAPI_STATUS_SUCCESS)
 *               - Operation successful<BR>
 */
FHAPI_status FHUTIL_getCurBfio(uint32_t *bfio, uint16_t *slotIdx);

/*!
 * @brief       This function gets the current value of the ufsi
 *
 * @param       ufsi - pointer to unicast fractional offset index
 *
 * @return      The status of the operation, as follows:<BR>
 *              [FHAPI_STATUS_SUCCESS](@ref FHAPI_STATUS_SUCCESS)
 *               - Operation successful<BR>
 */
FHAPI_status FHUTIL_getCurUfsi(uint32_t *ufsi);

/*!
 * @brief       Calculates UFSI based on ufsi and dwell time
 *
 * @param       ufsi - unicast fractional slot interval
 * @param       ucDwellTime - unicast dwell time
 *
 * @return      uint32_t computed ufsi value
 */
uint32_t FHUTIL_calcUfsi(uint32_t ufsi, uint8_t ucDwellTime);

/*!
 * @brief       update btie based on bfio and slotIdx
 *
 * @param       bfio - new bfio value
 * @param       slotIdx - new broadcast slot index *
 *
 * @return      The status of the operation, as follows:<BR>
 *              [0]- Operation successful<BR>
 *              [1]- Operation failure<BR>
 */
uint8_t FHUTIl_updateBTIE(uint32_t bfio, uint16_t slotIdx);

/*!
 * @brief       Compute the physical channel cooresponding to 0 index based on
 *              PHY configurations
 *
 * @return      uint32_t - The physical channel value
 */
uint32_t FHUTIL_getCh0(void);

/*!
 * @brief       Compute the channel spacing to be used based on configuration
 *
 * @return      uint8_t - channel spacing value
 */
uint8_t FHUTIL_getChannelSpacing(void);

/*!
 * @brief       Compute the maximum number of channels based on configuration
 *
 * @return      uint8_t - total number of channels
 */
uint8_t FHUTIL_getMaxChannels(void);

/*!
 * @brief       Computes the number of ones in a exclude channel bit accounting
 *              for max possible channels
 *
 *@param        bitmap - the exclude channel bitmap
 *@param        maxChannels - maximum possible number of channels
 *
 * @return      uint8_t - total number of ones in bitmap
 */
uint8_t FHUTIL_getBitCounts(uint8_t *bitmap, uint8_t maxChannels);

/*!
 * @brief       Updates the total number of unicast channels based on
 *              exclude channel bitmap
 *
 *@param        bitmap - the exclude channel bitmap
 */
void FHUTIL_updateUcNumChannels(uint8_t *bitmap);

/*!
 * @brief       Updates the total number of broadcast channels based on
 *              exclude channel bitmap
 *
 *@param        bitmap - the exclude channel bitmap
 */
void FHUTIL_updateBcNumChannels(uint8_t *bitmap);

/*!
 * @brief       Computes the dynamic guardtime to be applied ffor a neighbor
 *
 * @param       pEntry - pointer to neighbor table entry
 *
 * @return      uint32_t the computed guard time to be applied
 */
uint32_t FHUTIL_getGuardTime(NODE_ENTRY_s *pEntry);

/*!
 * @brief       Computes the elapsed time since last known RTC time stamp
 *
 * @param       prevTime - time in tickts from which the elapsed time is to be
 *              computed
 * @param       slotIdx - pointer to value of brodcast slot id
 *
 * @return      uint32_t the elapsed time value in clock tick periods
 */
uint32_t FHUTIL_getElapsedTime(uint32_t prevTime);

/*!
 * @brief       Computes the elapsed time between two given rtc time stamps
 *
 * @param       curTime - the RTC time stamp to which elapsed time is to
 *              be computed
 * @param       oldTIme - the RTC time stamp from which elapsed time is to
 *              be computed
 *
 * @return      uint32_t the computed guard time to be applied
 */
uint32_t FHUTIL_elapsedTime(uint32_t curTime, uint32_t oldTime);
/*!
 * @brief       Computes the target node's ufsi
 *
 * @param       pEntry - neighbor table entry containing target's info.
 *
 * @return      uint32_t the computed txUfsi
 */
uint32_t FHUTIL_getTxUfsi(NODE_ENTRY_s *pEntry);

#if 1 //def FH_PRINT_DEBUG
/*!
 * @brief       Prints TX debug information.
 *
 */
void FHUTIL_printTxDebugInfo(void);

/*!
 * @brief       Prints CSMA debug information.
 *
 * @param       backOffDur - backoff duration
 * @param       maxTxCSMABackOddDelay - maximum allowed backoff duration
 */
void FHUTIL_printCsmaDebugInfo(uint32_t backOffDur, uint8_t macTxCsmaBackoffDelay);
/*!
 * @brief       Prints RX debug information.
 */
void FH_printDebugInfo(void);
#endif
#endif
