/******************************************************************************

 @file fh_ie.h

 @brief TIMAC 2.0 FH IE API

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

#ifndef _FHIE_H_
#define _FHIE_H_

/******************************************************************************
 Includes
 *****************************************************************************/

#include "hal_types.h"
#include "hal_mcu.h"


/******************************************************************************
 Constants and definitions
 *****************************************************************************/

/*! Network Name Size */
#define FHIE_NETNAME_SIZE                   (32)
/*! default channel 0 in FCC */
#define FHIE_DEFAULT_CH0                    (902200)
/*! default channel spacing */
#define FHIE_DEFAULT_CHANNEL_SPACING        (0)
/*! default number of channels */
#define FHIE_DEFAULT_NO_OF_CHANNELS         (129)

/*! Channel Plan Domain tag */
#define FHIE_CP_REGULATORY_DOMAIN           (0)
/*! Channel Plan vendor tag */
#define FHIE_CP_VENDOR_SPECIFIED            (1)
/*! Channel function: single channel function (fixed channel) */
#define FHIE_CF_SINGLE_CHANNEL              (0)
/*! Channel function: TR51CF */
#define FHIE_CF_TR51CF                      (1)
/*! Channel function: DH1CF */
#define FHIE_CF_DH1CF                       (2)
/*! Exclude channel control: NO EC */
#define FHIE_ECC_NO_EC                      (0)
/*! Exclude channel control: RANGE */
#define FHIE_ECC_RANGE                      (1)
/*! Exclude channel control: MASK */
#define FHIE_ECC_BIT_MASK                   (2)

/* long type */
/*! Sub ID: US IE */
#define FHIE_PIE_SUB_IE_SUB_ID_US_IE        0x01
/*! Sub ID: BS IE */
#define FHIE_PIE_SUB_IE_SUB_ID_BS_IE        0x02
/* short type */
/*! Sub ID: PAN IE */
#define FHIE_PIE_SUB_IE_SUB_ID_PAN_IE       0x04
/*! Sub ID: NETNAME IE */
#define FHIE_PIE_SUB_IE_SUB_ID_NETNAME_IE   0x05
/*! Sub ID: PANVER IE */
#define FHIE_PIE_SUB_IE_SUB_ID_PANVER_IE    0x06
/*! Sub ID: GTKHASH IE */
#define FHIE_PIE_SUB_IE_SUB_ID_GTKHASH_IE   0x07

/*! Sub ID: UT IE */
#define FHIE_HIE_SUB_ID_UT_IE               0x01
/*! Sub ID: BT IE */
#define FHIE_HIE_SUB_ID_BT_IE               0x02
/*! Sub ID: FC IE */
#define FHIE_HIE_SUB_ID_FC_IE               0x03
/*! Sub ID: RSL IE */
#define FHIE_HIE_SUB_ID_RSL_IE              0x04


#define NO_OF_CHANNEL_PLAN                  (7)

/* #define FHIE_DBG */
/******************************************************************************
 Typedefs
 *****************************************************************************/

/*! HIE UTIE definiton */
typedef struct {
    uint8_t frameTypeId;            /*!< FH frame type */
    uint32_t ufsi;                  /*!< ufsi */
} FHIE_utIE_t;

/*! HIE BTIE definiton */
typedef struct {
    uint16_t bsn;                   /*!< BSN */
    uint32_t bfio;                  /*!< bfio*/
} FHIE_btIE_t;

/*! HIE FCIE definiton */
typedef struct {
    uint8_t txFlowControl;          /*!< TX flow control */
    uint8_t rxFlowControl;          /*!< RX flow control */
} FHIE_fcIE_t;

/*! HIE RSLIE definiton */
typedef struct {
    uint8_t rsl;                    /*!< RSL value */
} FHIE_rslIE_t;

/*! PIE schedule IE definiton */
typedef struct {
    uint8_t dwellInterval;          /*!< dwell interval */
    uint8_t clockDrift;             /*!< clock drift */
    uint8_t timingAccuracy;         /*!< timing accuracy */
    uint8_t channelPlan;            /*!< channle plan */
    uint8_t channelFunction;        /*!< channel function */
    uint8_t excludedChannelControl; /*!< excluded channel control */
} schedInfo_t;

/*! channle info definiton */
typedef struct {
    uint16_t noOfChannels;              /*!< number of channels */
    uint16_t fixedChannel;              /*!< fixed channel */
    uint8_t excludedChannelMask[32];    /*!< excluded channles */
} chanInfo_t;

/*! USIE definiton */
typedef struct {
    schedInfo_t schedInfo;              /*!< unicast schedule info */
    chanInfo_t chanInfo;                /*!< unicast channel info */
} FHIE_usIe_t;

/*! BSIE definiton */
typedef struct {
    uint32_t broadcastInterval;         /*!< broadcast interval */
    uint16_t broadcastSchedId;          /*!< broadcast schedule ID */
    schedInfo_t schedInfo;              /*!< broadcast schedule info */
    chanInfo_t chanInfo;                /*!< broadcast channel info */
} FHIE_bsIE_t;

/*! PANIE definiton */
typedef struct {
    uint16_t panSize;                   /*!< PAN size */
    uint8_t routingCost;                /*!< routing cost */
    uint8_t useParentBSIE;              /*!< use parent BSIE flag */
    uint8_t routingMethod;              /*!< routing method */
    uint8_t eapolReady;                 /*!< EAPOL ready flag */
    uint8_t fanTpsVersion;              /*!< FAN TPS version */
} FHIE_panIE_t;

/*! NETNAMEIE definiton */
typedef struct {
    uint8_t netName[32];                /*!< network name */
} FHIE_netNameIE_t;

/*! PANVERIE definiton */
typedef struct {
    uint16_t panVersion;                /*!< PAN version*/
} FHIE_panVerIE_t;

/*! GTK HASH definiton */
typedef struct {
    uint8_t gtk0Hash[8];                /*!< get hash-0*/
    uint8_t gtk1Hash[8];                /*!< get hash-1*/
    uint8_t gtk2Hash[8];                /*!< get hash-2*/
    uint8_t gtk3Hash[8];                /*!< get hash-3*/
} FHIE_gtkHashIE_t;

/*! IE Info definiton */
typedef union {
    FHIE_fcIE_t fcIe;                   /*!< FC IE */
    FHIE_rslIE_t rslIe;                 /*!< RSL IE */
} FHIE_ieInfo_t;

typedef struct {
    uint8_t regulatoryDomain;
    uint8_t operatingClass;
    uint32_t ch0;
    uint8_t channelSpacing; //0:200kHz, 1:400kHz, 2:600kHz
    uint16_t noOfChannels;
} FHIE_channelPlan_t;

typedef struct {
    uint8_t type;
    uint8_t id;
    uint16_t len;
    uint8_t *pVal;
} FHIE_ie_t;

#ifdef FHIE_DBG
typedef struct {
    uint16_t pie_getlen_try;
    uint16_t pie_getlen_fail;
    uint16_t hie_getlen_try;
    uint16_t hie_getlen_fail;
    uint16_t pie_gen_try;
    uint16_t pie_gen_fail;
    uint16_t pie_gen_not_supported;
    uint16_t hie_gen_try;
    uint16_t hie_gen_fail;
    uint16_t pie_parse_try;
    uint16_t pie_parse_invalid;
    uint16_t pie_parse_not_supported;
    uint16_t hie_parse_try;
    uint16_t hie_parse_invalid;
    uint16_t hie_parse_not_supported;
    uint16_t pie_extract_try;
    uint16_t pie_extract_invalid;
    uint16_t pie_extract_not_supported;
    uint16_t hie_extract_try;
    uint16_t hie_extract_invalid;
    uint16_t hie_extract_not_supported;
} FHIE_dbg_t;
#endif

/******************************************************************************
 Global Externs
 *****************************************************************************/

/******************************************************************************
 Prototypes
 *****************************************************************************/

/*!
 * @brief       get the cumulative length of the IEs indicated by the bitmap
 *
 * @param bitmap - bitmap of IEs (each bit corresponds to type of IE) <BR>
 *
 * @return      length of the IEs<BR>

 */
extern MAC_INTERNAL_API uint16_t FHIE_getLen(uint32_t bitmap);

/*!
 * @brief       generate the IE bit stream
 *
 * @param bitmap - bitmap of IEs (each bit corresponds to type of IE)
 * @param pdata - pointer to the buffer where IE bitstream is stored
 * @param pMacTx - pointer to MAC TX packet
 * @param pIeInfo - pointer to IE info data structure
 *
 * @return      length of the IEs<BR>

 */
extern MAC_INTERNAL_API uint16_t FHIE_gen(uint8_t *pData,
                                          uint32_t bitmap,
                                          macTx_t *pMacTx,
                                          FHIE_ieInfo_t *pIeInfo);

/*!
 * @brief       parse the PIEs, USIE, BSIE, PANVER IE etc
 *
 * @param pData - pointer to input payload IE byte stream
 * @param pSrcAddr - pointer to source address of the received packet
 * @param pLen1 - pointer of number of bytes of parsed WiSUN PIEs (output)
 * @param pLen2 - pointer of number of bytes of unsupported WiSUN PIEs (output)
 *
 *
 * @return      status of operation<BR>

 */

extern MAC_INTERNAL_API FHAPI_status FHIE_parsePie(uint8_t *pData,
                                                   sAddrExt_t *pSrcAddr,
                                                   uint16_t *pLen1,
                                                   uint16_t *pLen2);


/*!
 * @brief       This function is used by LMAC to parse for header IEs.
 *              It parses UTIE, BTIE, FCIE, RSLIE
 *
 * @param pData - pointer to input header IE byte stream
 * @param pSrcAddr - pointer to source address of the received packet
 * @param ts  - SFD time stamp of the received packet in miliseconds
 * @param pLen1  - pointer of number of bytes of parsed WiSUN HIEs
 * @param pLen2  - pointer of number of bytes of unsupported WiSUN HIEs
 *
  * @return      The status of the operation, as follows:<BR>
 *              [FH_STATUS_SUCCESS](@ref FH_STATUS_SUCCESS)
 *               - Operation successful<BR>
 *              [FH_STATUS_ERR_INVALID_FORMAT]
 *              (@ref FH_STATUS_ERR_INVALID_FORMAT)
 *               - invalid IE format
 *              [FH_STATUS_ERR_NOT_SUPPORTED_IE]
 *              (@ref FH_STATUS_ERR_NOT_SUPPORTED_IE)
 *               - IE is not supported
 *
 */
extern MAC_INTERNAL_API FHAPI_status FHIE_parseHie(uint8_t *pData,
                                                   sAddrExt_t *pSrcAddr,
                                                   uint32_t ts,
                                                   uint16_t *pLen1,
                                                   uint16_t *pLen2);

/*!
 * @brief       extract the header IEs
 *
 * @param pdata - pointer to the received packet buffer
 * @param ieType - type of HIE - UTIE, BTIE
 * @param pOut  - pointer to extracted values (output)
 *
  * @return      The status of the operation, as follows:<BR>
 *              [FH_STATUS_SUCCESS](@ref FH_STATUS_SUCCESS)
 *               - Operation successful<BR>
 *              [FH_STATUS_ERR_INVALID_FORMAT]
 *              (@ref FH_STATUS_ERR_INVALID_FORMAT)
 *               - invalid IE format
 *              [FH_STATUS_ERR_NOT_SUPPORTED_IE]
 *              (@ref FH_STATUS_ERR_NOT_SUPPORTED_IE)
 *               - IE is not supported
 *
 */

extern MAC_INTERNAL_API FHAPI_status FHIE_extractHie(uint8_t *pData,
                                                     IE_TYPE_t ieType,
                                                     uint8_t *pOut);


/*!
 * @brief       extract the payload IE of type ieType
 *
 * @param pdata - pointer to the received packet buffer
 * @param ieType - type of HIE - UTIE, BTIE
 * @param pOut  - pointer to extracted values (output)
 *
  * @return      The status of the operation, as follows:<BR>
 *              [FH_STATUS_SUCCESS](@ref FH_STATUS_SUCCESS)
 *               - Operation successful<BR>
 *              [FH_STATUS_ERR_INVALID_FORMAT]
 *              (@ref FH_STATUS_ERR_INVALID_FORMAT)
 *               - invalid IE format
 *              [FH_STATUS_ERR_NOT_SUPPORTED_IE]
 *              (@ref FH_STATUS_ERR_NOT_SUPPORTED_IE)
 *               - IE is not supported
 *
 */

extern MAC_INTERNAL_API FHAPI_status FHIE_extractPie(uint8_t *pData,
                                                     IE_TYPE_t ieType,
                                                     uint8_t *pOut);
#endif
