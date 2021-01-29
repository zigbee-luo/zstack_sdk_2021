/******************************************************************************

 @file fh_nt.h

 @brief TIMAC 2.0 FH neighbor table API

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

#ifndef _FHNT_H_
#define _FHNT_H_

/******************************************************************************
 Includes
 *****************************************************************************/

#include "hal_types.h"
#include "hal_mcu.h"
#include "fh_pib.h"
#include "fh_data.h"

#ifdef FEATURE_MAC_SECURITY
#include "mac_security_pib.h"
#include "mac_security.h"
#endif

#define FHNT_SPLIT_TABLE

//#define FHNT_MAX_DEVICE_TABLE_ENTRIES           (100)
#define FHNT_TABLE_TYPE_FIXED                   (0)
#define FHNT_TABLE_TYPE_HOPPING                 (1)

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

/*! Node Entry is invalid */
#define FHNT_NODE_INVALID                   (0x0000)
/*! Node Entry is just created */
#define FHNT_NODE_CREATED                   (0x0001)
/*! Node Entry has UTIE information */
#define FHNT_NODE_W_UTIE                    (0x0002)
/*! Node Entry has USIE information */
#define FHNT_NODE_W_USIE                    (0x0004)
/*! Node Entry infomation is expired */
#define FHNT_NODE_EXPIRED                   (0x0008)

/******************************************************************************
 Typedefs
 *****************************************************************************/

/*! Node Entry channel information */
//typedef struct chInfo
typedef union chInfo
{
  uint8_t fixedChannel;
  uint8_t bitMap[FHPIB_MAX_BIT_MAP_SIZE];
}chInfo_t;

/*! Node Entry USIE parameter */
typedef struct structUsieParams
{
   uint8_t dwellInterval;
   uint8_t clockDrift;
   uint8_t timingAccuracy;
   /*fixed or list */
   uint8_t channelFunc;
   chInfo_t chInfo;
   /* numChannels = max. chans - # of excl chans */
   uint8_t numChannels;
} usieParams_t;

/*! Node Entry BSIE parameter */
struct structBsieParams
{
   uint32_t bcInterval;
   uint16_t bcSchedId;
   usieParams_t UsieParams_s;
};

/*! Node Fixed Table Entry structure */
typedef struct __attribute__((__packed__)) node_fixed_entry
{
   uint16_t       ref_timeStamp;

   uint8_t        fixedChannel;

   uint8_t        valid;
   uint16_t        EUI_index;
} NODE_FIXED_ENTRY_s;

/*! Node Entry structure */
typedef struct __attribute__((__packed__)) node_entry
{
   uint32_t       ufsi;

   uint32_t       ref_timeStamp;

   //sAddrExt_t     dstAddr;

   usieParams_t   UsieParams_s;

   uint8_t        valid;
   uint16_t        EUI_index;
} NODE_ENTRY_s;

/*! Node Entry purge timer */
typedef struct nt_hnd_s
{
  uint32_t                  purgeCount;
  uint32_t                  purgeTime;
  FH_macFHTimer_t           purgeTimer;
  uint16_t                  maxNumNonSleepNodes;   /*<! Maximum number of nonSleep Node */
  uint16_t                  maxNumSleepNodes;      /*<! Maximum number of Sleep Node */
  uint16_t                  numNonSleepNode;       /*<! number of active non-sleepy node */
  uint16_t                  numSleepNode;          /*<! number of active sleepy node */
  NODE_ENTRY_s             *pNonSleepNtTable;     /*<! pointer to the non sleep node NT table */
  NODE_FIXED_ENTRY_s       *pSleepNtTable;        /*<! pointer to sleep node NT table  */
  uint8_t                  macSecurity;           /*<! FH module security setting  */
} FHNT_HND_s;

typedef struct FHNT_TEMP_table
{
    sAddrExt_t  *pAddr;
    uint16_t     maxNumNode;
    uint16_t     num_node;
} FHNT_TEMP_TABLE_s;

typedef struct nt_eui_debug
{
    uint16_t num_device_add;
    uint16_t num_device_del_temp;
    uint8_t  num_eui_not_found_in_temp;
    uint8_t  num_temp_index_not_found_in_nt;
}FHNT_DEBUG_s;

#define FH_OPT_LRM_NODES 10

typedef struct {
  sAddr_t nodeAddr;
  uint8   channel;
} NODE_OPT_ENTRY_s;

typedef struct {
    NODE_OPT_ENTRY_s node[FH_OPT_LRM_NODES];
    uint8    nodeIdx;
} FHNT_OPT_Table_s;

/******************************************************************************
 Global Externs
 *****************************************************************************/

/******************************************************************************
 Prototype
 *****************************************************************************/

/*!
 * @brief       This function is used to initialize node variables
 */
MAC_INTERNAL_API void FHNT_reset(void);

/*!
 * @brief       This function is used to initialize node variables and timer
 */
MAC_INTERNAL_API void FHNT_init(void);

/*!
 * @brief       This function is used to create the new entry in neighbor table
 *
 * @param pAddr - pointer of node address
 *
 * @return      pointer of node entry
 */
MAC_INTERNAL_API FHAPI_status FHNT_createEntry(sAddrExt_t *pAddr,NODE_ENTRY_s *pEntry);

/*!
 * @brief       This function is used to retrieve the node entry information
 *              from neighbor table
 *
 * @param pAddr - pointer of node address
 * @param pEntry - pointer of pointer of node entry
 *
 * @return      The status of the operation, as follows:<BR>
 *              [FHAPI_STATUS_SUCCESS]
 *              (@ref FHAPI_STATUS_SUCCESS)
 *               - Operation successful<BR>
 *              [FHAPI_STATUS_ERR_EXPIRED_NODE]
 *              (@ref FHAPI_STATUS_ERR_EXPIRED_NODE)
 *               - UTIE info is expired<BR>
 *              [FHAPI_STATUS_ERR_NO_ENTRY_IN_THE_NEIGHBOR]
 *              (@ref FHAPI_STATUS_ERR_NO_ENTRY_IN_THE_NEIGHBOR)
 *               - the destination address is not in the FH neighbor table<BR>
 *              [FHAPI_STATUS_ERR]
 *              (@ref FHAPI_STATUS_ERR)
 *               - parameter is wrong<BR>
 */
MAC_INTERNAL_API FHAPI_status FHNT_getEntry(sAddr_t *pDstAddr,
                                            NODE_ENTRY_s *pEntry);

MAC_INTERNAL_API FHAPI_status FHNT_putEntry(const NODE_ENTRY_s *pEntry);

/*!
 * @brief       This function is used to purge the node entry information
 *              from neighbor table
 *
 * @param ts - time stamp
 */
MAC_INTERNAL_API void FHNT_purgeEntry(uint32_t ts);

MAC_INTERNAL_API sAddrExt_t *FHNT_getEUI(uint16_t index);

MAC_INTERNAL_API uint16_t FHNT_AddDeviceCB(sAddrExt_t *pEUI, uint16_t devIndex);
MAC_INTERNAL_API uint16_t FHNT_DelDeviceCB(sAddrExt_t *pEUI, uint16_t devIndex);

#ifdef FHNT_SPLIT_TABLE
MAC_INTERNAL_API FHAPI_status FHNT_getFixedEntry(sAddrExt_t *pAddr,
                                            NODE_ENTRY_s *pEntry);
MAC_INTERNAL_API FHAPI_status FHNT_putFixedEntry(const NODE_ENTRY_s *pEntry);
MAC_INTERNAL_API FHAPI_status FHNT_createFixedEntry(sAddrExt_t *pAddr,NODE_ENTRY_s *pEntry);

#endif

void FHNT_initTempTable(void);

void FHNT_addOptEntry(sAddr_t *pSrcAddr, uint8_t channel);
MAC_INTERNAL_API FHAPI_status FHNT_getOptEntry(sAddr_t *pSrcAddr, NODE_ENTRY_s *pEntry);
#endif
