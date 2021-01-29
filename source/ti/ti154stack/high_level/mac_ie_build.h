/******************************************************************************

 @file  mac_ie_build.h

 @brief This file includes typedefs and functions required for building header
        and payload IE.

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

#ifndef MAC_IE_BUILD_H
#define MAC_IE_BUILD_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "mac_high_level.h"
#include "mac_api.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

/* Payload IE byte 2 start position for individual fields */
#define MAC_PAYLOAD_GID_POS                       3 /* Group ID start Position */
#define MAC_PAYLOAD_TYPE_POS                      7 /* Type start Position */

/* First byte value of Payload Termination IE */
#define MAC_PAYLOAD_IE_TERM_FIRST_BYTE              0x00
/* Second byte value of Payload Termination IE */
#define MAC_PAYLOAD_IE_TERM_SECOND_BYTE_PAYIE       0xF8


/* Coexistence Specification IE, byte 1 */
#define MAC_COEXIST_IE_BEACON_ORDER_POS           0
#define MAC_COEXIST_IE_SUPERFRAME_ORDER_POS       4

/* Coexistence Specification IE, byte 2 */
#define MAC_COEXIST_IE_FINAL_CAP_SLOT_POS         0
#define MAC_COEXIST_IE_ENHANCED_BEACON_ORDER_POS  4

/* Coexistence Specification IE, byte 3 */
#define MAC_COEXIST_IE_OFFSET_TIME_SLOT_POS       0
#define MAC_COEXIST_IE_CAP_BACKOFF_OFFSET_POS     4

/* Internal -- use to indicate end of payload ie's */
#define MAC_PAYLOAD_IE_TERM                       0xFF

/* Internal -- use to indicate end of sub ie's */
#define MAC_PAYLOAD_SUB_IE_TERM                   0xFF

/* Internal -- use to indicate end of header ie's */
#define MAC_HEADER_IE_TERM                        0xFF

/* Coexistence IE content length */
#define MAC_PAYLOAD_MLME_SUB_IE_COEXIST_LEN         5

/* ------------------------------------------------------------------------------------------------
 *                                           Functions
 * ------------------------------------------------------------------------------------------------
 */

/**
 * @brief This function builds and inserts header ies in the
 *        transmit buffer.
 *
 * @param pBuf           pointer to the structure containing the
 *                       transmit message buffer.
 * @param pHeaderIes     pointer to the list of header ie's that
 *                       needs to be built and inserted in the
 *                       transmit buffer.
 *
 * @return uint8         returns MAC_SUCCESS if successful.
 *                       MAC_NO_DATA if it fails to build the
 *                       requested IE.
 */
MAC_INTERNAL_API uint8
macBuildHeaderIes( macTx_t* pBuf, headerIeInfo_t *pHeaderIes );

/**
 * @brief This function builds and inserts payload ies in the
 *        transmit buffer.
 *
 * @param pBuf           pointer to the structure containing the
 *                       transmit message buffer.
 * @param pPayloadIeInfo pointer to the list of payload ie's
 *                       that needs to be built and inserted in
 *                       the transmit buffer.
 * @param includeTermIe  if TRUE, payload termination IE will be included else not.
 * @param ieIdOnly       if TRUE no IE content will be populated and the length
 *                       field will be set to zero.
 *
 * @return uint8         returns MAC_SUCCESS if successful.
 *                       MAC_NO_RESOURCES if it fails to build the
 *                       requested IE.
 */
MAC_INTERNAL_API uint8
macBuildPayloadIes( macTx_t* pBuf, payloadIeInfo_t *pPayloadIeInfo, bool includeTermIe, bool ieIdOnly );

/**
 * @brief Builds the MLME payload co-existence sub IE.
 *
 * @param pBuf      pointer to the structure containing the transmitpBuf
 *                  message buffer.
 * @param ieIdOnly  if TRUE no IE content will be populated and the length
 *                  field will be set to zero.
 *
 * @return uint16 returns the length of the IE if successful,
 *         otherwise zero.
 */
uint16 macBuildPayloadCoexistIe( macTx_t* pBuf, bool ieIdOnly );

#endif /* MAC_IE_BUILD_H */
