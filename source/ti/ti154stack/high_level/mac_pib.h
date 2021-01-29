/******************************************************************************

 @file  mac_pib.h

 @brief Internal interface file for the MAC PIB module.

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

#ifndef MAC_PIB_H
#define MAC_PIB_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "mac_api.h"
#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Defines
 * ------------------------------------------------------------------------------------------------
 */
#if !defined(MAC_PIB_MAX_BEACON_PAYLOAD_LEN)
#define MAC_PIB_MAX_BEACON_PAYLOAD_LEN  52
#endif

/* Attribute index constants, based on attribute ID values from spec */
#define MAC_ATTR_SET1_START       0x40
#define MAC_ATTR_SET1_END         0x64
#define MAC_ATTR_SET1_OFFSET      0
#define MAC_ATTR_SET2_START       0xE0
#define MAC_ATTR_SET2_END         0xF8
#define MAC_ATTR_SET2_OFFSET      (MAC_ATTR_SET1_END - MAC_ATTR_SET1_START + MAC_ATTR_SET1_OFFSET + 1)

/* frame response values */
#define MAC_MAX_FRAME_RESPONSE_MIN  143
#define MAC_MAX_FRAME_RESPONSE_MAX  32001

#if defined(COMBO_MAC) || !defined(FREQ_2_4G)
/* MR-FSK channel center frequencies */
#define MAC_915MHZ_BAND_MODE_1_CENTER_FREQ_KHZ          902200
#define MAC_915MHZ_BAND_MODE_2_CENTER_FREQ_KHZ          902400
#define MAC_868MHZ_BAND_MODE_1_CENTER_FREQ_KHZ          863125
#define MAC_433MHZ_BAND_MODE_1_CENTER_FREQ_KHZ          433300
#define MAC_5KBPS_915MHZ_BAND_MODE_1_CENTER_FREQ_KHZ    902200
#define MAC_5KBPS_433MHZ_BAND_MODE_1_CENTER_FREQ_KHZ    433300
#define MAC_5KBPS_868MHZ_BAND_MODE_1_CENTER_FREQ_KHZ    863125
#define MAC_915MHZ_BAND_MODE_3_CENTER_FREQ_KHZ          902400
#define MAC_868MHZ_BAND_MODE_2_CENTER_FREQ_KHZ          863225

/* MR-FSK channel spacing */
#define MAC_915MHZ_BAND_MODE_1_CHAN_SPACING_KHZ         200
#define MAC_915MHZ_BAND_MODE_2_CHAN_SPACING_KHZ         400
#define MAC_868MHZ_BAND_MODE_1_CHAN_SPACING_KHZ         200
#define MAC_433MHZ_BAND_MODE_1_CHAN_SPACING_KHZ         200
#define MAC_5KBPS_915MHZ_BAND_MODE_1_CHAN_SPACING_KHZ   200
#define MAC_5KBPS_433MHZ_BAND_MODE_1_CHAN_SPACING_KHZ   200
#define MAC_5KBPS_868MHZ_BAND_MODE_1_CHAN_SPACING_KHZ   200
#define MAC_915MHZ_BAND_MODE_3_CHAN_SPACING_KHZ         400
#define MAC_868MHZ_BAND_MODE_2_CHAN_SPACING_KHZ         400

/* MR-FSK Number of Channels */
#define MAC_915MHZ_BAND_MODE_1_TOTAL_CHANNELS           129
#define MAC_915MHZ_BAND_MODE_2_TOTAL_CHANNELS           64
#define MAC_868MHZ_BAND_MODE_1_TOTAL_CHANNELS           34
#define MAC_433MHZ_BAND_MODE_1_TOTAL_CHANNELS           7
#define MAC_5KBPS_915MHZ_BAND_MODE_1_TOTAL_CHANNELS     129
#define MAC_5KBPS_433MHZ_BAND_MODE_1_TOTAL_CHANNELS     7
#define MAC_5KBPS_868MHZ_BAND_MODE_1_TOTAL_CHANNELS     34
#define MAC_915MHZ_BAND_MODE_3_TOTAL_CHANNELS           64
#define MAC_868MHZ_BAND_MODE_2_TOTAL_CHANNELS           17

#endif

#if defined(COMBO_MAC) || defined(FREQ_2_4G)
/* MR-FSK channel center frequencies */
#define MAC_IEEE_CENTER_FREQ_KHZ              2405000
/* MR-FSK channel spacing */
#define MAC_IEEE_CHAN_SPACING_KHZ             5000
/* MR-FSK Number of Channels */
#define MAC_IEEE_TOTAL_CHANNELS               16

#endif

/* Invalid PIB table index used for error code */
//#define MAC_PIB_INVALID     ((uint8) (sizeof(macPibTbl) / sizeof(macPibTbl[0])))
#define MAC_PIB_INVALID     (0xFF)
/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

/* PIB access and min/max table type */
typedef struct
{
  uint8     offset;
  uint8     len;
  uint8     min;
  uint8     max;
} macPibTbl_t;

/* MAC PIB type */
typedef struct
{
  uint8             ackWaitDuration;
  bool              associationPermit;
  bool              autoRequest;
  bool              battLifeExt;
  uint8             battLifeExtPeriods;

  uint8             *pBeaconPayload;
  uint8             beaconPayloadLength;
  uint8             beaconOrder;
  uint32            beaconTxTime;
  uint8             bsn;

  sAddr_t           coordExtendedAddress;
  uint16            coordShortAddress;
  uint8             dsn;
  bool              gtsPermit;
  uint8             maxCsmaBackoffs;

  uint8             minBe;
  uint16            panId;
  bool              promiscuousMode;
  bool              rxOnWhenIdle;
  uint16            shortAddress;

  uint8             superframeOrder;
  uint16            transactionPersistenceTime;
  bool              associatedPanCoord;
  uint8             maxBe;
  uint16            maxFrameTotalWaitTime;

  uint8             maxFrameRetries;
  uint8             responseWaitTime;
  uint8             syncSymbolOffset;
  bool              timeStampSupported;
  bool              securityEnabled;

  uint8             ebsn;
  uint8             eBeaconOrder;
  uint16            eBeaconOrderNBPAN;
  uint8             offsetTimeSlot;
  bool              includeMPMIE;

  uint8             fskPreambleLen;
  uint8             mrFSKSFD;

  /* Proprietary */
  int8              phyTransmitPower;
  uint8             logicalChannel;
  sAddr_t           extendedAddress;
  uint8             altBe;
  uint8             deviceBeaconOrder;
  uint8             rf4cepowerSavings;
  uint8             frameVersionSupport;
  uint8             channelPage;
  uint8             curPhyID;
  bool              fcsType;

  /* Diagnostics */
  uint32            diagsRxCrcPass;
  uint32            diagsRxCrcFail;
  uint32            diagsRxBcast;
  uint32            diagsTxBcast;
  uint32            diagsRxUcast;
  uint32            diagsTxUcast;
  uint32            diagsTxUcastRetry;
  uint32            diagsTxUcastFail;
  uint32            diagsRxSecFail;
  uint32            diagsTxSecFail;

  /* LBT RSSI threshold */
  int8              rssiThreshold;

  /* Range Extender */
  uint8             rangeExt;
  /* enable Ack Pending for Data Pkts */
  uint8             enDataAckPending;

  /* RF freq */
  uint8             rfFreq;

  /* PA type */
  uint8             paType;
} macPib_t;


/* ------------------------------------------------------------------------------------------------
 *                                           Global Variables
 * ------------------------------------------------------------------------------------------------
 */


/* ------------------------------------------------------------------------------------------------
 *                                           Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

MAC_INTERNAL_API void macPibReset(void);

#if defined(COMBO_MAC) || !defined(FREQ_2_4G)
/**************************************************************************************************
 * @fn          macMRFSKGetPhyDesc
 *
 * @brief       This function retrieves PHY descriptor based on PHY ID
 *
 * input parameters
 *
 * @param       phyID - phy attribute identifier.
 *
 * @return      Pointer to macMRFSKPHYDescriptor for the particular PHY ID
 *
 **************************************************************************************************/
MAC_INTERNAL_API macMRFSKPHYDesc_t* macMRFSKGetPhyDesc(uint8 phyID);
#endif
#if defined(COMBO_MAC) || defined(FREQ_2_4G)
/**************************************************************************************************
 * @fn          macIEEEGetPhyDesc
 *
 * @brief       This function selects the IEEE PHY descriptor
 *
 * input parameters
 *
 * @param       phyID - index of the PHY descriptor
 *
 * @return      pValue - pointer to the PHY descriptor
 *
 **************************************************************************************************
 */
MAC_INTERNAL_API macIEEEPHYDesc_t* macIEEEGetPhyDesc(uint8 phyID);
#endif
/**************************************************************************************************
*/

#endif /* MAC_PIB_H */

