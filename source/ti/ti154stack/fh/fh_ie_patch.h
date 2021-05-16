/******************************************************************************

 @file fh_ie_patch.h

 @brief TIMAC 2.0 FH IE PATCH API

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

#ifndef _FHIE_PATCH_H_
#define _FHIE_PATCH_H_

/******************************************************************************
 Includes
 *****************************************************************************/

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

/******************************************************************************
 Typedefs
 *****************************************************************************/

/******************************************************************************
 Global Externs
 *****************************************************************************/

/******************************************************************************
 Prototypes
 *****************************************************************************/
uint16_t FHNT_delTempIndex(uint16_t eui_index);
FHAPI_status FHIEPATCH_extractHie(uint8_t subIeId, uint16_t ieLength, uint8_t *pBuf, uint8_t *pOut);
FHAPI_status FHIEPATCH_extractPie(uint8_t subIeId, uint16_t ieLength, uint8_t *pBuf, uint8_t *pOut);
uint16_t FHIEPATCH_getHieLen(uint32_t ieBitmap);
uint8_t FHIEPATCH_getPieId(uint32_t ieBitmap, uint8_t *pType);
uint16_t FHIEPATCH_getPieContentLen(uint8_t id);
uint16_t FHIEPATCH_genPieContent(uint8_t *pData, uint8_t id);
uint16_t FHIEPATCH_genHie(uint8_t *pData, uint32_t bitmap, macTx_t *pMacTx,
                            FHIE_ieInfo_t *pIeInfo);
FHAPI_status FHIEPATCH_parsePie(uint8_t subIeId, uint16_t subIeLength, uint8_t *pBuf, sAddrExt_t *pSrcAddr);
FHAPI_status FHIEPATCH_parseHie(uint8_t subIeId, uint16_t ieLength, uint8_t *pBuf, uint32_t ts, sAddrExt_t *pSrcAddr);
#endif
