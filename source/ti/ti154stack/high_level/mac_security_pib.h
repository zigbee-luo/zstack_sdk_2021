/******************************************************************************

 @file  mac_security_pib.h

 @brief Internal interface file for the Security-related MAC PIB module.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2010-2021, Texas Instruments Incorporated
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

#ifndef MAC_SECURITY_PIB_H
#define MAC_SECURITY_PIB_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "mac_api.h"
#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

/* Invalid security PIB table index used for error code */
//#define MAC_SECURITY_PIB_INVALID     ((uint8) (sizeof(macSecurityPibTbl) / sizeof(macSecurityPibTbl[0])))
#define MAC_SECURITY_PIB_INVALID     (0xFF)
/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

/* Security related PIB access and min/max table type */
typedef struct
{
  uint8     offset;
  uint16     len;
  uint8     min;
  uint16     max;
} macSecurityPibTbl_t;


/* Security-related MAC PIB type */
typedef struct
{
  uint16             keyTableEntries;
  uint16             deviceTableEntries;
  uint8              securityLevelTableEntries;

  uint8              autoRequestSecurityLevel;
  uint8              autoRequestKeyIdMode;
  uint8              autoRequestKeySource[MAC_KEY_SOURCE_MAX_LEN];
  uint8              autoRequestKeyIndex;

  uint8              defaultKeySource[MAC_KEY_SOURCE_MAX_LEN];
  sAddr_t            panCoordExtendedAddress;
  uint16             panCoordShortAddress;

  /* Propriority Security PIBs */
  keyDescriptor_t            *macKeyTable;
  //keyDescriptor_t            macKeyTable[MAX_KEY_TABLE_ENTRIES];

  keyIdLookupDescriptor_t    *macKeyIdLookupList;
  //keyIdLookupDescriptor_t    **macKeyIdLookupList;
  //keyIdLookupDescriptor_t    macKeyIdLookupList[MAX_KEY_TABLE_ENTRIES][MAX_KEY_ID_LOOKUP_ENTRIES];
  keyDeviceDescriptor_t       *macKeyDeviceList;
  //keyDeviceDescriptor_t      macKeyDeviceList[MAX_KEY_TABLE_ENTRIES][MAX_KEY_DEVICE_TABLE_ENTRIES];
  //keyDeviceDescriptor_t *      macKeyDeviceList[MAX_KEY_TABLE_ENTRIES];
  keyUsageDescriptor_t       *macKeyUsageList;
  //keyUsageDescriptor_t       macKeyUsageList[MAX_KEY_TABLE_ENTRIES][MAX_KEY_USAGE_TABLE_ENTRIES];

  deviceDescriptor_t         *macDeviceTable;
  //deviceDescriptor_t         macDeviceTable[MAX_DEVICE_TABLE_ENTRIES];
  securityLevelDescriptor_t  macSecurityLevelTable[MAX_SECURITY_LEVEL_TABLE_ENTRIES];

} macSecurityPib_t;

/* ------------------------------------------------------------------------------------------------
 *                                           Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

MAC_INTERNAL_API void macSecurityPibReset(void);
MAC_INTERNAL_API uint8 MAC_MlmeGetPointerSecurityReq(uint8 pibAttribute, void **pValue);
MAC_INTERNAL_API uint8 macSecurityPibIndex(uint8 pibAttribute);

/**************************************************************************************************
*/

#endif /* MAC_SECURITY_PIB_H */

