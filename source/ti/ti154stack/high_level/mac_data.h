/******************************************************************************

 @file  mac_data.h

 @brief Internal interface file for the MAC data module.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2005-2021, Texas Instruments Incorporated
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

#ifndef MAC_DATA_H
#define MAC_DATA_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "mac_high_level.h"
#include "mac_main.h"

#if defined(TIMAC_ROM_IMAGE_BUILD)
#include "icall_osal_rom_jt.h"
#else
#include "icall_osal_map_direct.h"
#endif

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

/* Internal frame type values. */
#define MAC_INTERNAL_BEACON                  0x00
#define MAC_INTERNAL_DATA                    0x01
#define MAC_INTERNAL_ASSOC_REQ               0x02
#define MAC_INTERNAL_ASSOC_RSP               0x03
#define MAC_INTERNAL_DISASSOC_NOTIF          0x04
#define MAC_INTERNAL_DATA_REQ                0x05
#define MAC_INTERNAL_PAN_CONFLICT            0x06
#define MAC_INTERNAL_ORPHAN_NOTIF            0x07
#define MAC_INTERNAL_BEACON_REQ              0x08
#define MAC_INTERNAL_COORD_REALIGN           0x09
#define MAC_INTERNAL_GTS_REQ                 0x0A
#define MAC_INTERNAL_COEXIST_EB_REQ          0x0B
#define MAC_INTERNAL_ENHANCED_BEACON_REQ     0x0C
#define MAC_INTERNAL_ENH_ACK                 0x0D

#define MAC_INTERNAL_DATA_UNICAST            0x15
#define MAC_INTERNAL_DATA_BROADCAST          0x25
#define MAC_INTERNAL_ASYNC                   0x35
#define MAC_INTERNAL_DATA_EDFE_INIT          0x45
#define MAC_INTERNAL_DATA_EDFE_FIN           0x55

/* Internal zero length data frame */
#define MAC_INTERNAL_ZERO_DATA        MAC_INTERNAL_DATA

/* Additional byte used by low level in tx buffer */
#define MAC_TX_OFFSET_LEN             1

/* Enhanced Beacon filter IE */
#define MAC_ATTRIB_ID_INCLUDE_MPMIE          0x40  /* MPM IE Attribute ID bit */
#define MAC_ATTRIB_ID_LEN_1_BYTE_VAL         0x08  /* Attribute ID field length value for 1 byte */


/* length of an ack frame in bytes */
#define MAC_LEN_ACK_FRAME           (MAC_FCF_FIELD_LEN + MAC_SEQ_NUM_FIELD_LEN + MAC_FCS4_FIELD_LEN)
#define MAC_LEN_ACK_IEEE_FRAME      (MAC_FCF_FIELD_LEN + MAC_SEQ_NUM_FIELD_LEN + MAC_FCS2_FIELD_LEN)
#define MAC_LEN_ACK_FRAME_FH        49

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

/* Data info type */
typedef struct
{
  MAP_osal_msg_q_t        txQueue;          /* transmit data queue */
  MAP_osal_msg_q_t        rxQueue;          /* receive data queue */
  uint16              duration;         /* duration of current frame */
  uint8               indirectCount;    /* number of indirect frames in tx queue */
  uint8               directCount;      /* number of direct frames in tx queue */
  uint8               rxCount;          /* number of frames in rx queue */
} macData_t;

/* general purpose data handling function type */
typedef void (*macDataTxFcn_t)(macTx_t *pMsg);

/* critical beacon handling function */
typedef void (*macRxBeaconCritical_t)(macRx_t *pBuf);

/* beacon tx complete function */
typedef void (*macTxBeaconComplete_t)(uint8 status);

/* tx frame retransmit function */
typedef void (*macTxFrameRetransmit_t)(void);

/* tx frame check schedule function */
typedef uint8 (*macDataCheckSched_t)(void);

/* tx frame check tx time function */
typedef uint16 (*macDataCheckTxTime_t)(void);

/* ------------------------------------------------------------------------------------------------
 *                                           Global Variables
 * ------------------------------------------------------------------------------------------------
 */
#ifndef ENHACK_MALLOC
extern uint8 enhancedAckBuf[250];
#endif

/* ------------------------------------------------------------------------------------------------
 *                                           Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

MAC_INTERNAL_API void macDataReset(void);
MAC_INTERNAL_API macTx_t *macAllocTxBuffer(uint8 cmd, ApiMac_sec_t *sec);
MAC_INTERNAL_API uint16 macFrameDuration(uint16 len, uint16 txOptions);
MAC_INTERNAL_API void macDataRetransmit(void);
MAC_INTERNAL_API uint8 macBuildDataFrame(macEvent_t *pEvent);
MAC_INTERNAL_API uint8 macCheckSched(void);
MAC_INTERNAL_API void macSetSched(macTx_t *pMsg);
MAC_INTERNAL_API void macDataTxComplete(macTx_t *pMsg);
MAC_INTERNAL_API void macDataTxSend(void);
MAC_INTERNAL_API void macDataTxEnqueue(macTx_t *pMsg);
MAC_INTERNAL_API void macDataSend(macEvent_t *pEvent);
MAC_INTERNAL_API void macApiDataReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macDataRxInd(macEvent_t *pEvent);
MAC_INTERNAL_API uint8_t macCheckIndPacket(macRx_t  *pRxBuf);
#if defined(COMBO_MAC) || defined(FREQ_2_4G)
MAC_INTERNAL_API void macAutoPendMaintainSrcMatchTable(macTx_t *pCurr);
MAC_INTERNAL_API void macAutoPendAddSrcMatchTableEntry(macTx_t *pMsg);
#endif
/**************************************************************************************************
*/

#endif /* MAC_DATA_H */
