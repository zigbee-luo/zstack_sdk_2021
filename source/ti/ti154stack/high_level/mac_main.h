/******************************************************************************

 @file  mac_main.h

 @brief Internal interface file for the MAC.

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

#ifndef MAC_MAIN_H
#define MAC_MAIN_H

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

/* State machine states */
enum
{
  MAC_IDLE_ST,          /* Idle state */
  MAC_ASSOCIATING_ST,   /* MAC device is performing the association procedure */
  MAC_POLLING_ST,       /* MAC device is polling coordinator for data */
  MAC_SCANNING_ST,      /* MAC device or coordinator is performing a scan procedure */
  MAC_STARTING_ST       /* MAC coordinator is starting a network */
};

/* State machine events */
enum
{
  MAC_API_DATA_REQ_EVT, /* MAP_macApiDataReq */
  MAC_API_PURGE_REQ_EVT,
  MAC_API_ASSOCIATE_REQ_EVT,
  MAC_API_ASSOCIATE_RSP_EVT,
  MAC_API_DISASSOCIATE_REQ_EVT,
  MAC_API_ORPHAN_RSP_EVT,
  MAC_API_POLL_REQ_EVT,
  MAC_API_SCAN_REQ_EVT,
  MAC_API_START_REQ_EVT,
  MAC_API_SYNC_REQ_EVT,
  MAC_API_WS_ASYNC_REQ_EVT,
  MAC_API_PWR_ON_REQ_EVT,
  MAC_RX_ASSOC_REQ_EVT,
  MAC_RX_ASSOC_RSP_EVT,
  MAC_RX_DISASSOC_EVT,
  MAC_RX_DATA_REQ_EVT,
  MAC_RX_PAN_CONFLICT_EVT,
  MAC_RX_ORPHAN_EVT,
  MAC_RX_BEACON_REQ_EVT,
  MAC_RX_REALIGN_EVT,
  MAC_RX_GTS_REQ_EVT,
  MAC_RX_BEACON_EVT,
  MAC_RX_DATA_IND_EVT,
  MAC_TX_ASSOC_REQ_OK_EVT,
  MAC_TX_ASSOC_REQ_FAIL_EVT,
  MAC_TX_ASSOC_RSP_EVT,
  MAC_TX_DISASSOC_EVT,
  MAC_TX_ORPHAN_OK_EVT,
  MAC_TX_ORPHAN_FAIL_EVT,
  MAC_TX_BEACON_REQ_OK_EVT,
  MAC_TX_BEACON_REQ_FAIL_EVT,
  MAC_TX_REALIGN_OK_EVT,
  MAC_TX_REALIGN_FAIL_EVT,
  MAC_TX_DATA_REQ_OK_EVT,
  MAC_TX_DATA_REQ_FAIL_EVT,
  MAC_TX_DATA_REQ_PEND_EVT,
  MAC_TX_CONFLICT_OK_EVT,
  MAC_TIM_RESPONSE_WAIT_EVT,
  MAC_TIM_FRAME_RESPONSE_EVT,
  MAC_TIM_SCAN_EVT,
  MAC_TIM_SYNC_EVT,
  MAC_TIM_EXP_INDIRECT_EVT,
  MAC_INT_SCAN_COMPLETE_EVT,
  MAC_INT_ASSOC_FAILED_EVT,
  MAC_INT_AUTO_POLL_EVT,
  MAC_INT_START_COMPLETE_EVT,
  MAC_INT_TX_COMPLETE_EVT,
  MAC_INT_BEACON_RESP_TIM_EVT,
  MAC_INT_BROADCAST_PEND_EVT,
  MAC_NO_ACTION_EVT,
  MAC_RX_ENHANCED_BEACON_REQ_EVT,
  MAC_TX_ENHANCED_BEACON_REQ_OK_EVT,
  MAC_TX_ENHANCED_BEACON_REQ_FAIL_EVT,
  /* not needed MAC_RX_WS_ASYNC_IND_EVT, */
  MAC_CSMA_TIM_EXP_EVT,
  MAC_BC_TIM_EXP_EVT,
  TX_BACKOFF_TIM_EXP_EVT,
  RX_BACKOFF_TIM_EXP_EVT
};

/* This must be set to the last API event in the enumeration */
#define MAC_API_EVT_MAX   MAC_API_PWR_ON_REQ_EVT

/* Enumeration of action function table sets; maximum of 16 */
enum
{
  MAC_MGMT_ACTION_1,
  MAC_MGMT_ACTION_2,
  MAC_SCAN_ACTION,
  MAC_DEVICE_ACTION_1,
  MAC_DEVICE_ACTION_2,
  MAC_COORD_ACTION_1,
  MAC_COORD_ACTION_2,
  MAC_COORD_ACTION_3,
  MAC_BEACON_DEVICE_ACTION,
  MAC_ACTION_SET_MAX
};

/* these macros process the action enumeration value */
#define MAC_ACTION_INIT(n)      ((n) << 4)
#define MAC_ACTION_SET(n)       ((n) >> 4)
#define MAC_ACTION_ID(n)        ((n) & 0x0F)

/* Enumeration of action functions; maximum of 16 per set */
enum
{
  /* Mgmt set 1 */
  MAC_NO_ACTION = MAC_ACTION_INIT(MAC_MGMT_ACTION_1),
  MAC_API_BAD_STATE,
  MAC_API_PENDING,
  MAC_API_DATA_REQ,
  MAC_COMM_STATUS_IND,
  MAC_DATA_SEND,
  MAC_DATA_RX_IND,
  MAC_API_WS_ASYNC_REQ,
  MAC_CSMA_TIM_EXPIRY,
  MAC_BC_TIM_EXPIRY,
  TX_BACKOFF_TIM_EXPIRY,
  RX_BACKOFF_TIM_EXPIRY,
  NOP_BACKOFF_TIM_EXPIRY,

  /* Mgmt set 2 */
  MAC_API_PWR_ON_REQ = MAC_ACTION_INIT(MAC_MGMT_ACTION_2),
  MAC_API_DISASSOCIATE_REQ,
  MAC_DISASSOC_COMPLETE,
  MAC_RX_BEACON,
  MAC_RX_DISASSOC,

  /* Scan */
  MAC_API_SCAN_REQ = MAC_ACTION_INIT(MAC_SCAN_ACTION),
  MAC_SCAN_COMPLETE,
  MAC_SCAN_FAILED_IN_PROGRESS,
  MAC_SCAN_NEXT_CHAN,
  MAC_SCAN_RX_BEACON,
  MAC_SCAN_START_TIMER,

  /* Device set 1 */
  MAC_API_ASSOCIATE_REQ = MAC_ACTION_INIT(MAC_DEVICE_ACTION_1),
  MAC_API_POLL_REQ,
  MAC_ASSOC_DATA_REQ,
  MAC_ASSOC_DATA_REQ_COMPLETE,
  MAC_ASSOC_DATA_RX_IND,
  MAC_ASSOC_FAILED,
  MAC_ASSOC_FRAME_RESPONSE_TIMEOUT,
  MAC_POLL_DATA_REQ_COMPLETE,
  MAC_POLL_DATA_RX_IND,
  MAC_POLL_FRAME_RESPONSE_TIMEOUT,
  MAC_POLL_RX_ASSOC_RSP,
  MAC_RX_ASSOC_RSP,
  MAC_START_FRAME_RESPONSE_TIMER,
  MAC_START_RESPONSE_TIMER,

  /* Device set 2 */
  MAC_ASSOC_RX_DISASSOC = MAC_ACTION_INIT(MAC_DEVICE_ACTION_2),
  MAC_POLL_RX_DISASSOC,
  MAC_SCAN_RX_COORD_REALIGN,
  MAC_RX_COORD_REALIGN,
  MAC_PAN_CONFLICT_COMPLETE,

  /* Coordinator set 1 */
  MAC_API_START_REQ = MAC_ACTION_INIT(MAC_COORD_ACTION_1),
  MAC_START_CONTINUE,

  /* Coordinator set 2 */
  MAC_API_ASSOCIATE_RSP = MAC_ACTION_INIT(MAC_COORD_ACTION_2),
  MAC_RX_ASSOC_REQ,
  MAC_RX_BEACON_REQ,
  MAC_RX_DATA_REQ,
  MAC_START_COMPLETE,
  MAC_INDIRECT_EXPIRE,
  MAC_RX_ENHANCED_BEACON_REQ,

  /* Coordinator set 3 */
  MAC_API_ORPHAN_RSP = MAC_ACTION_INIT(MAC_COORD_ACTION_3),
  MAC_API_PURGE_REQ,
  MAC_RX_ORPHAN,
  MAC_RX_PAN_CONFLICT,

  /* Beacon device */
  MAC_API_SYNC_REQ = MAC_ACTION_INIT(MAC_BEACON_DEVICE_ACTION),
  MAC_AUTO_POLL,
  MAC_BEACON_START_FRAME_RSP_TIMER,
  MAC_START_BROADCAST_PEND_TIMER
};

/* Special hdr.event mask value used to mark buffer as reserved (do not deallocate) */
#define MAC_BUF_RESERVED                0x80

/* OSAL task events (as uint8) */
#define MAC_RESPONSE_WAIT_TASK_EVT      0x01
#define MAC_FRAME_RESPONSE_TASK_EVT     0x02
#define MAC_SCAN_TASK_EVT               0x04
#define MAC_EXP_INDIRECT_TASK_EVT       0x08
#define MAC_TX_COMPLETE_TASK_EVT        0x10
#define MAC_RX_QUEUE_TASK_EVT           0x20
#define MAC_START_COMPLETE_TASK_EVT     0x40
#define MAC_BROADCAST_PEND_TASK_EVT     0x80
#define MAC_CSMA_TIM_TASK_EVT           0x100
#define MAC_BC_TIM_TASK_EVT             0x200
#define MAC_TX_DATA_REQ_TASK_EVT        0x400

#define MAC_TX_BACKOFF_TIM_TASK_EVT     0x800
#define MAC_RX_BACKOFF_TIM_TASK_EVT     0x1000

/* State table format */
#define MAC_ACTION_POS                  0       /* action position */
#define MAC_NEXT_STATE_POS              1       /* next state position */
#define MAC_NUM_COLS                    2       /* number of columns in state tables */

/* Configurable features */
enum
{
  MAC_FEAT_DEVICE,
  MAC_FEAT_COORD,
  MAC_FEAT_SECURITY,
  MAC_FEAT_BEACON_COORD,
  MAC_FEAT_BEACON_DEVICE,
  MAC_FEAT_GTS_CLIENT,
  MAC_FEAT_GTS_SERVER,
  MAC_FEAT_MAX
};

/* Configurable feature masks */
#define MAC_FEAT_DEVICE_MASK            ((uint16) 1 << MAC_FEAT_DEVICE)
#define MAC_FEAT_COORD_MASK             ((uint16) 1 << MAC_FEAT_COORD)
#define MAC_FEAT_SECURITY_MASK          ((uint16) 1 << MAC_FEAT_SECURITY)
#define MAC_FEAT_BEACON_COORD_MASK      ((uint16) 1 << MAC_FEAT_BEACON_COORD)
#define MAC_FEAT_BEACON_DEVICE_MASK     ((uint16) 1 << MAC_FEAT_BEACON_DEVICE)
#define MAC_FEAT_GTS_CLIENT_MASK        ((uint16) 1 << MAC_FEAT_GTS_CLIENT)
#define MAC_FEAT_GTS_SERVER_MASK        ((uint16) 1 << MAC_FEAT_GTS_SERVER)

#define MAC_FEAT_GTS_MASK               (MAC_FEAT_GTS_CLIENT_MASK | MAC_FEAT_GTS_SERVER_MASK)

/* RX enable/disable masks */
#define MAC_RX_POLL                     0x01    /* polling/associating */
#define MAC_RX_WHEN_IDLE                0x02    /* PIB rxOnWhenIdle */
#define MAC_RX_SCAN                     0x04    /* scanning */
#define MAC_RX_ACK_WAITING              0x08    /* waiting for an acknowledgment */
#define MAC_RX_BROADCAST_PEND           0x10    /* pending broadcast */
#define MAC_RX_BEACON_DEVICE            0x20    /* beacon tracking */
#define MAC_RX_BEACON_NETWORK           0x40    /* beacon network */
#define MAC_RX_BEACON_SYNC              0x80    /* beacon sync */

#define MAC_RX_FH_BC_SLOT               0x20  /* enable RX during BC slot for FH */

/* TX mode masks */
#define MAC_TX_MODE_NON_CSMA            0x01    /* frame tx without CSMA */
#define MAC_TX_MODE_SLOTTED             0x02    /* frame tx in slotted mode */
#define MAC_TX_MODE_INTERNAL            0x08    /* frame is for internal purposes */
#define MAC_TX_MODE_SCAN_RELATED        0x10    /* frame is scan related */
#define MAC_TX_MODE_BROADCAST           0x20    /* broadcast frame in beacon network */
#define MAC_TX_MODE_BEACON              0x40    /* beacon tx in beacon network */
#define MAC_TX_MODE_REQUESTED           0x80    /* indirect frame was requested in beacon network */

/* TX schedule masks */
#define MAC_TX_SCHED_OUTGOING_CAP       0x01    /* outgoing CAP (default) */
#define MAC_TX_SCHED_INCOMING_CAP       0x02    /* incoming CAP */
#define MAC_TX_SCHED_INDIRECT_NO_ACK    0x04    /* frame is indirect and not acked */
#define MAC_TX_SCHED_READY              0x10    /* frame scheduled as ready to tx */
#define MAC_TX_SCHED_SCAN_RELATED       0x20    /* frame is scan related */
#define MAC_TX_SCHED_MARKED             0x40    /* frame is marked for pending address list */
#define MAC_TX_SCHED_INDIRECT           0x80    /* frame is indirect */
#define MAC_TX_SCHED_INACTIVE           0xFF    /* inactive portion of the superframe */



/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

/* API parameter checking macro */
#if defined(MAC_NO_PARAM_CHECK) && MAC_NO_PARAM_CHECK == TRUE
#define MAC_PARAM_STATUS(expr, status)
#else
#define MAC_PARAM_STATUS(expr, status)  if ((expr) == FALSE) status = MAC_INVALID_PARAMETER
#endif

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

/* State table type */
typedef const uint8 (CODE * macStateTbl_t)[MAC_NUM_COLS];

/* Event structure for API events with buffer for command frame */
typedef struct
{
  macEventHdr_t               hdr;
  sData_t                     mpdu;
  macTxIntData_t              internal;
  union
  {
    ApiMac_mlmeAssociateReq_t     associateReq;
    ApiMac_mlmeAssociateRsp_t     associateRsp;
    ApiMac_mlmeDisassociateReq_t  disassociateReq;
    ApiMac_mlmeOrphanRsp_t        orphanRsp;
    ApiMac_mlmeWSAsyncReq_t       asyncReq;
  } mac;
} macApiDataEvent_t;

/* MLME purge request type */
typedef struct
{
  uint8                       msduHandle;
} macMcpsPurgeReq_t;


/* Event structure for API events */
typedef struct
{
  macEventHdr_t               hdr;
  union
  {
    ApiMac_mlmePollReq_t      pollReq;
    macMlmeScanReq_t          scanReq;
    ApiMac_mlmeStartReq_t     startReq;
    ApiMac_mlmeSyncReq_t      syncReq;
    ApiMac_mlmeWSAsyncReq_t   wsAsyncReq;
    macMcpsPurgeReq_t         purgeReq;
  } mac;
} macApiEvent_t;

typedef struct
{
  macEventHdr_t               hdr;
  ApiMac_mcpsDataReq_t        dataReq;
} macMcpsDataReqApiMac_t;

/* Union of event structures */
typedef union
{
  macEventHdr_t               hdr;
  macApiEvent_t               api;
  macApiDataEvent_t           apiData;
  macMcpsDataReqApiMac_t      apiDataReq;
  macMcpsDataReq_t            dataReq;
  macMcpsDataInd_t            dataInd;
  macTx_t                     tx;
  macRx_t                     rx;
  macMlmeBeaconNotifyInd_t    beaconNotify;
} macEvent_t;

/* Action function type */
typedef void (*macAction_t)(macEvent_t *pEvent);

/* Action function set type */
typedef const macAction_t * macActionSet_t;

/* Type for mac_main data */
typedef struct
{
  macEvent_t    *pPending;      /* pointer to pending buffer */
  uint8         *pBuf;
  uint16        featureMask;    /* mask of initialized features */
  uint8         state;          /* state machine state */
} macMain_t;

/* Type for module reset functions */
typedef void (*macReset_t)(void);

/* ------------------------------------------------------------------------------------------------
 *                                           Global Variables
 * ------------------------------------------------------------------------------------------------
 */


/* ------------------------------------------------------------------------------------------------
 *                                           Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

MAC_INTERNAL_API void macMainReset(void);
MAC_INTERNAL_API void macSetEvent(uint8 event);
MAC_INTERNAL_API void macSetEvent16(uint16 event);
MAC_INTERNAL_API void macCbackForEvent(macEvent_t *pEvent, uint8 status);
MAC_INTERNAL_API bool macStateIdle(void);
MAC_INTERNAL_API bool macStateIdleOrPolling(void);
MAC_INTERNAL_API bool macStateScanning(void);
MAC_INTERNAL_API void macExecute(macEvent_t *pEvent);
MAC_INTERNAL_API uint8 macSendMsg(uint8 event, void *pData);
MAC_INTERNAL_API uint8 macSendDataMsg(uint8 event, void *pData, ApiMac_sec_t *sec);
void macMainSetTaskId(uint8 taskId);
MAC_INTERNAL_API void macMainReserve(uint8 *ptr);
MAC_INTERNAL_API void mac_msg_deallocate(uint8 **msg_ptr);

/**************************************************************************************************
*/

#endif /* MAC_MAIN_H */
