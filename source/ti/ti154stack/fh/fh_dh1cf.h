/******************************************************************************

 @file fh_dh1cf.h

 @brief TIMAC 2.0 FH DH1CF API

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

#ifndef _FH_DH1CF_H
#define _FH_DH1CF_H

#include "hal_types.h"
#include "fh_nt.h"

#define FH_DH1CF
/******************************************************************************
 * MACROS
 ******************************************************************************/
/*
 * The defines below are from WiSUN spec. They thus do not follow
 * TI Coding guidelines
 */
#define hashsize(n) ((uint32_t)1<<(n))
#define hashmask(n) (hashsize(n)-1)
#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

#define mix(a,b,c) \
{ \
  a -= c;  a ^= rot(c, 4);  c += b; \
  b -= a;  b ^= rot(a, 6);  a += c; \
  c -= b;  c ^= rot(b, 8);  b += a; \
  a -= c;  a ^= rot(c,16);  c += b; \
  b -= a;  b ^= rot(a,19);  a += c; \
  c -= b;  c ^= rot(b, 4);  b += a; \
}

#define final(a,b,c) \
{ \
  c ^= b; c -= rot(b,14); \
  a ^= c; a -= rot(c,11); \
  b ^= a; b -= rot(a,25); \
  c ^= b; c -= rot(b,16); \
  a ^= c; a -= rot(c,4);  \
  b ^= a; b -= rot(a,14); \
  c ^= b; c -= rot(b,24); \
}

/******************************************************************************
 * CONSTANTS
 ******************************************************************************/

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/*!
 * @brief       This function calculates hash index.
 *              Taken from the WiSUN standard.
 *
 * @param       k - pointer to key, an array of uint32_t values
 * @param       length -  the length of the key
 * @param       initval - the previous hash, or an arbitrary value
 *
 * @return       uint32_t  - hash value
 */
uint32_t FHDH1CF_hashword(
        uint32_t        *k,
        uint32_t        length,
        uint32_t        initval);

/*!
 * @brief     This function calculates the channel at a given slot.
 *            Taken from WiSUN standard.
 *
 * @param       slotNum - The slot at which channel has to be computed
 * @param       devExtAddr - The EUI of the node for whom tart channel is to
 *              be computed
 * @param       numChannels - The total number of channels
 *
 * @return      uint8_t - Channel number
 */
uint8_t FHDH1CF_getCh(uint16_t slotNum,
                      sAddrExt_t  devExtAddr,
                      uint16_t numChannels);

/*!
 * @brief   This function maps hash index to actual channel number
 *
 * @param   chIdx - the index for the channel
 * @param   exclChList - The exclude channel list
 * @param   nChannels - total number of possible channels
 *
 * @return  uint8_t - mapped channel number
 */
uint8_t FHDH1CF_mapChIdxChannel(uint8_t  chIdx,
                                uint8_t  *exclChList,
                                uint8_t  nChannels);


/*!
 * @brief       This function calculates broadcast channel at a given slot
 *
 * @param       slotNum - the broadcast slot number
 * @param       bsi - broadcast slot index
 * @param       numChannels - total number of broadcast channels
 *
 * @return      uint8_t - the broadcast channel number
 */
uint8_t FHDH1CF_getBcCh(uint32_t slotNum,
                          uint16_t bsi,
	                      uint16_t numChannels);

/*!
 * @brief       Wrapper function to get broadcast channel for a given slot
 *
 * @param       slotIdx - Slot index
 * @param       numChannels - total number of channels
 *
 * @return      uint8_t - correspoding broadcast channel number.
 */
uint8_t FHDH1CF_getBCChannelNum(uint16_t slotIdx, uint8_t numChannels);

/*!
 * @brief       Wrapper function to get node's channel at a given time
 *
 * @param       pFH_hnd - pointer to FH handle
 *
 * @return      uint8_t - correspoding channel number.
 */
uint8_t FHDH1CF_getChannelNum(FH_HND_s *pFH_hnd);

/*!
 * @brief       Wrapper function to get target's channel at a given slot
 *
 * @param       slotIdx - slot index
 * @param       pEntry - pointer to target node's neighbor table entry
 *
 * @return      uint8_t - correspoding channel number.
 */
uint8_t FHDH1CF_getTxChannelNum(uint16_t slotIdx, NODE_ENTRY_s *pEntry);

#endif





