/******************************************************************************

 @file  mac_ie_parse.h

 @brief This file includes typedefs and functions required for MAC header and
        payload IE parsing.

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

#ifndef MAC_IE_PARSE_H
#define MAC_IE_PARSE_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_mcu.h"
#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Constants
 * ------------------------------------------------------------------------------------------------
 */
/**
 * Header IEs
 */
/* Header IE header Length -- 2 bytes */
#define MAC_HEADER_IE_HEADER_LEN                 2
/* Type value for header IE */
#define MAC_HEADER_IE_TYPE_VAL                   0

/* IE to terminate list of IEs with no payload IE */
#define MAC_INTERNAL_HEADER_IE_TERM_NO_PAYIE     0x7f
/* IE to terminate list of IEs followed by payload IE */
#define MAC_INTERNAL_HEADER_IE_TERM_WITH_PAYIE   0x7e

/* First byte value of Header Termination IE */
#define MAC_HEADER_IE_TERM_SECOND_BYTE           0x3F
/* Second byte value of Header Termination IE with Payload IE  */
#define MAC_HEADER_IE_TERM_FIRST_BYTE_PAYIE      0x00
/* Second byte value of Header Termination IE without Payload IE  */
#define MAC_HEADER_IE_TERM_FIRST_BYTE_NO_PAYIE   0x80

/* WiSUN header IE, element ID */
#define MAC_HEADER_IE_WISUN                      0x2A

/**
 * Payload IEs
 */
/* Type value for payload IE */
#define MAC_PAYLOAD_IE_TYPE_VAL                  1
/* Type value for payload IE */
#define MAC_PAYLOAD_IE_HEADER_LEN                2


/* Payload IE Group IDs */
#define MAC_PAYLOAD_IE_ESDU_GID                  0x00   /* Payload ESDU IE Group ID*/
#define MAC_PAYLOAD_IE_MLME_GID                  0x01   /* Payload MLME IE Group ID*/
#define MAC_PAYLOAD_IE_WISUN_GID                 0x04   /* Payload MLME IE Group ID*/
#define MAC_PAYLOAD_IE_TERM_GID                  0x0f   /* Payload Termination IE Group ID*/

/* MLME Sub IEs -- short format */
#define MAC_PAYLOAD_MLME_SUB_IE_EBEACON_FILTER   0x1E   /* Enhanced beacon filter IE */
#define MAC_PAYLOAD_MLME_SUB_IE_COEXIST          0x21   /* Coexistence IE */
#define MAC_PAYLOAD_MLME_SUB_IE_SUN_DEV_CAP      0x22   /* SUN Device capabilities IE */
#define MAC_PAYLOAD_MLME_SUB_IE_SUN_FSK_GEN_PHY  0x23   /* SUN FSK Generic PHY IE */
#define MAC_PAYLOAD_MLME_SUB_IE_MODE_SWITCH      0x24   /* Mode Switch Parameter IE */

/* Payload IE's SubIe Header length */
#define MAC_PAYLOAD_SUB_IE_HEADER_LEN    2
/* Payload IE's SubIe Type short value  */
#define MAC_PAYLOAD_SUB_IE_ID_TYPE_SHORT 0
/* Payload IE's SubIe Type Long value */
#define MAC_PAYLOAD_SUB_IE_ID_TYPE_LONG  1


/* ------------------------------------------------------------------------------------------------
 *                                          Macros
 * ------------------------------------------------------------------------------------------------
 */
/* Macros for parsing Header IE header fields */
#define MAC_HEADER_IE_OFFSET                     0
#define MAC_HEADER_IE_CONTENT_LEN(p)             ((p)[MAC_HEADER_IE_OFFSET+0] & 0x7f)
#define MAC_HEADER_IE_ELEMENT_ID(p)              ((((p)[MAC_HEADER_IE_OFFSET+0] >> 7) & 0x01) + \
                                                  (((p)[MAC_HEADER_IE_OFFSET+1] << 1) & 0xfe))
#define MAC_HEADER_IE_TYPE(p)                    (((p)[MAC_HEADER_IE_OFFSET+1] >> 7) & 0x01)

/* Macros for parsing Payload IE header fields */
#define MAC_PAYLOAD_IE_OFFSET                    0
#define MAC_PAYLOAD_IE_SUBIE_OFFSET              0
#define MAC_PAYLOAD_IE_TYPE(p)                   (((p)[MAC_PAYLOAD_IE_OFFSET+1] >> 7) & 0x01)
#define MAC_PAYLOAD_IE_GROUP_ID(p)               (((p)[MAC_PAYLOAD_IE_OFFSET+1] >> 3) & 0x0f)
#define MAC_PAYLOAD_IE_CONTENT_LEN(p)            (((p)[MAC_PAYLOAD_IE_OFFSET+0] & 0x00ff) + \
                                                  (((p)[MAC_PAYLOAD_IE_OFFSET+1] & 0x0007)  \
                                                    << 8))

/* Macros for parsing SubIe of Payload IEs */
#define MAC_PAYLOAD_SHORT_SUBIE_LEN(p)           ((p)[MAC_PAYLOAD_IE_SUBIE_OFFSET+0])
#define MAC_PAYLOAD_LONG_SUBIE_LEN(p)            (((p)[MAC_PAYLOAD_IE_OFFSET+0] & 0x00ff) + \
                                                   (((p)[MAC_PAYLOAD_IE_OFFSET+1] & 0x0007)  \
                                                    << 8))
#define MAC_PAYLOAD_SUBIE_TYPE(p)                (((p)[MAC_PAYLOAD_IE_SUBIE_OFFSET+1] >> 7) \
                                                  & 0x01)
#define MAC_PAYLOAD_SHORT_SUBIE_ID(p)            ((p)[MAC_PAYLOAD_IE_SUBIE_OFFSET+1] & 0x7f)
#define MAC_PAYLOAD_LONG_SUBIE_ID(p)             (((p)[MAC_PAYLOAD_IE_OFFSET+1] >> 3) & 0x0f)


/*--------------------------------------------------------------------------------------------------
*           Typedef
*---------------------------------------------------------------------------------------------------
*/

/* Type definition for mac header information element */
typedef struct _macHeaderIeInfo
{
  struct _macHeaderIeInfo *pNext; /* pointer to the next element */
  uint8  ieType;                  /* 1 bit, Type (always short form, value 0) */
  uint8  ieElementId;             /* 8 bits, Element ID */
  uint8  ieContentLen;            /* 7 bits, IE Content Length */
  uint8* ieContentPtr;            /* max size 127 bytes */

} macHeaderIeInfo_t;

/* Type definition for the Payload information element */
typedef struct _macPayloadIeInfo
{
  struct _macPayloadIeInfo *pNext;          /* pointer to the next element */
  uint8                    ieType;          /* 1 bit, type of payload IE */
  uint8                    ieGroupId;       /* 7 bits, group id */
  uint16                   ieContentLen;    /* max 16 bits, IE Content Length */
  uint8*                   ieContentPtr;    /* max size 2047 bytes */

} macPayloadIeInfo_t;


struct _macRx;
/*--------------------------------------------------------------------------------------------------
*           Functions
*---------------------------------------------------------------------------------------------------
*/

/*!
 * @brief Parses the header information element.
 *        This function creates the link list of the header IEs.
 *        The caller is responsible to release the memory for
 *        the same. It also updates the payload pointer and the
 *        length.
 *
 * @param pMsg pointer to the macRx_t structure containing the
 *             payload data.
 *
 * @return returns MAC_SUCCESS if successful in parsing the
 *         payload ie's. MAC_NO_DATA if it does not find any
 *         payload ie's. MAC_NO_RESOURCES if memory allocation
 *         fails.
 */
uint8 macParseHeaderIes(struct _macRx *pMsg);

/*!
 * @brief Parses the payload information element.
 *        This function creates the link list of the Payload IE.
 *        The caller is responsible to release the memory for
 *        the same. It also updates the payload pointer and the
 *        length.
 *
 * @param pMsg pointer to the macRx_t structure containing the
 *             payload data.
 *
 * @return returns MAC_SUCCESS if successful in parsing the
 *         payload ie's. MAC_NO_DATA if it does not find any
 *         payload ie's. MAC_NO_RESOURCES if memory allocation
 *         fails.
 */
uint8 macParsePayloadIes(struct _macRx *pMsg );

/*!
 * @brief Updates the header IE pointer if header IE is present in macRx_t
 *        structure with the start position of header IE.
 *        It also updates the headerIeLen fields.
 *        Call this function only if the payload IE to precess exists.
 *
 * @param pMsg pointer to the macRx_t containing the incoming decrypted message.
 * @return None
 */
void
macUpdateHeaderIEPtr(struct _macRx  *pMsg);

/*!
 * @brief Updates the payload IE pointer if payload IE's are present in macRx_t
 *        structure with the start position of payload IE in the buffer.
 *        It also updates the payloadIeLen fields.
 *
 * @param pMsg pointer to the macRx_t containing the incoming decrypted message.
 * @return None
 */
void
macUpdatePayloadIEPtr(struct _macRx *pMsg);

/*!
 * @brief Cycles through the MLME IE content to find the Sub-IE indicated by the
 *        param mlmeSubIeType. If found set the value of the field pointed by
 *        pIePresent value to 1 and if the content of the IE is present returns
 *        the pointer to the content.
 *
 * @param pPayloadIe    pointer to the buffer containing the MLME IE content.
 * @param mlmeSubIeType Sub-IE to find within the MLME IE content.
 * @param pIePresent    pointer to the IE present field which is updated by this
 *                      function.
 *
 * @return uint8*       pointer to the Sub-IE content if present otherwise NULL.
 */
uint8* macMlmeSubIePresent( macPayloadIeInfo_t* pPayloadIe, uint8 mlmeSubIeType,
                            uint8* pIePresent );

/*!
 * @brief Frees the memory associated with each element in the link list
 * pointed by the pList
 *
 * @param pList pointer to the payload IE information element.
 *
 * @return None
 */
void macIeFreeList(macPayloadIeInfo_t* pList);

/*!
 * @brief Sets the value of the location pointed by the pIePresent parameter
 *        to 1 and returns the pointer to the start of the coexistence IE content
 *        if the coexistence is found in the list pointed by pPayloadIE param.
 *
 * @param pPayloadIe pointer to the payload IE list.
 * @param pIePresent pointer to the location of the iePresent field.
 *
 * @return uint8* pointer to the start of coexistence IE content if available
 *                otherwise NULL.
 */
uint8* macIeCoexistIePresent( macPayloadIeInfo_t* pPayloadIe, uint8* pIePresent );

#endif /* MAC_IE_PARSE_H */

