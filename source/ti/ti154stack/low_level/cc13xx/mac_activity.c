
/******************************************************************************

 @file  mac_activity.c

 @brief This file contains the data structures and API definitions for the MAC
        Activity tracking module.
        Note: Only utilized in DMM applications.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2019-2021, Texas Instruments Incorporated
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

/*******************************************************************************
 * INCLUDES
 */

/* High Level MAC */
#include "mac_timer.h"
#include "mac_hl_patch.h"

/* Low Level MAC */
#include "mac_activity.h"

/* debug */
#include "mac_assert.h"
#include "dbg.h"

/* CM0 related */
#include "mac_settings.h"

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/* Activity tracking variables, updated within ISR and MAC Task context. */
static volatile macTxIntActivityData_t txActivityData = {0};
static volatile macRxIntActivityData_t rxActivityData = {0};


/*******************************************************************************
 * GLOBAL VARIABLES
 */

/* ------------------------------------------------------------------------------------------------
 *                                        Local Functions
 * ------------------------------------------------------------------------------------------------
 */
static void setActivityTrackingTx(macTxIntData_t *txData, uint16_t cmdStatus, RF_EventMask rfEvent);
static void setActivityTrackingRx(macRx_t *pRxBuf, bool resetCount);
static uint32_t getActivityPriorityTx(void);
static uint32_t getActivityPriorityRx(void);
static void setActivityTx(macTxIntData_t *txData);
static void setActivityRx(uint8_t rxEnableFlags);
static macActivity_t getActivityTx(void);
static macActivity_t getActivityRx(void);
#ifdef MAC_ACTIVITY_PROFILING
static void activityPreemptCb(void *arg);
static void startActivityProfilingTimer(RF_Handle handle);
static void printActivityInfo(uint32_t activity, uint32_t priority, uint8_t frameType, sAddr_t destAddr);
#endif /* MAC_ACTIVITY_PROFILING */

const activityObject_t activityObject = {
    .pSetActivityTrackingTxFn = setActivityTrackingTx,
    .pSetActivityTrackingRxFn = setActivityTrackingRx,
    .pGetActivityPriorityTxFn = getActivityPriorityTx,
    .pGetActivityPriorityRxFn = getActivityPriorityRx,
    .pSetActivityTxFn = setActivityTx,
    .pSetActivityRxFn = setActivityRx,
    .pGetActivityTxFn = getActivityTx,
    .pGetActivityRxFn = getActivityRx,
#ifdef MAC_ACTIVITY_PROFILING
    .pStartActivityProfilingTimerFn = startActivityProfilingTimer,
    .pPrintActivityInfoFn = printActivityInfo
#endif
};

/*******************************************************************************
 * @fn          setActivityTrackingTx
 *
 * @brief       This call is used by the LMAC to update the Transmit activity
 *              tracking metrics.
 *
 * input parameters
 *
 * @param       txData - pointer to transmit packet.
 *
 * @param       resetCount - Whether to reset activity tracking count.
 *                           True to reset, False to increment count.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void setActivityTrackingTx(macTxIntData_t *txData, uint16_t cmdStatus, RF_EventMask rfEvent)
{
    bool resetCount = false;
    uint8 rfFreq = pMacPib->rfFreq;

    /* Replicate logic within Subg, IEEE & TX Ack callback functions */
    if (rfFreq == MAC_RF_FREQ_SUBG)
    {
        if (rfEvent & RF_EventLastCmdDone)
        {
          /* Normal cases */
          if (cmdStatus == PROP_DONE_OK)
          {
              /* Successful transmission, reset tracking data */
              resetCount = true;
          }
          else if (cmdStatus & 0x0800)
          {
              /* Increment failure status for current activity */
              resetCount = false;
          }
          else {
              /* Successful transmission of Tx Ack, reset tracking data */
              resetCount = true;
          }
        }
        else if (rfEvent & (RF_EventCmdStopped | RF_EventCmdAborted | RF_EventCmdCancelled | RF_EventCmdPreempted))
        {
          /* Increment failure status for current activity as TX was aborted */
          resetCount = false;
        }
        /* Ignore Rx ack chain events */
        else if (!(rfEvent & (RF_EventRxOk | RF_EventRxBufFull | RF_EventRxNOk | RF_EventMdmSoft)))
        {
            /* Increment failure status for current activity as TX was aborted */
            resetCount = false;
        }
    }
    else
    {
        if (rfEvent & (RF_EventCmdStopped | RF_EventCmdAborted | RF_EventCmdCancelled | RF_EventCmdPreempted))
        {
            /* TX is aborted */
            resetCount = false;
        }
        else
        {
            if ((rfEvent & RF_EventLastFGCmdDone) == RF_EventLastFGCmdDone)
            {
                /* Normal cases */
                if (cmdStatus == IEEE_DONE_OK)
                {
                    /* Successful transmission, reset tracking data */
                    resetCount = true;
                }
                else
                {
                    /* Command error */
                    resetCount = false;
                }
            }
            else
            {
                /* Command error */
                resetCount = false;
            }
        }
    }

    /* Update previous frame type for successfully transmitted packet */
    if ((txData) && (resetCount)) {
        txActivityData.prevTxFrameType = txData->frameType;
    }

    /* Update tracking information for activity */
    switch(txActivityData.txActivity)
    {
        case MAC_ACTIVITY_LINK_EST:
            txActivityData.numMissedAssocTxFrames = resetCount ? 0 : (txActivityData.numMissedAssocTxFrames + MAC_ACTIVITY_DEFAULT_INC);
            break;
        case MAC_ACTIVITY_TX_BEACON:
            txActivityData.numMissedBeaconTxFrames = resetCount ? 0 : (txActivityData.numMissedBeaconTxFrames + MAC_ACTIVITY_DEFAULT_INC);
            break;
        case MAC_ACTIVITY_FH:
            txActivityData.numMissedFHTxFrames = resetCount ? 0 : (txActivityData.numMissedFHTxFrames + MAC_ACTIVITY_DEFAULT_INC);
            break;
        case MAC_ACTIVITY_DATA:
            txActivityData.numMissedDataTxFrames = resetCount ? 0 : (txActivityData.numMissedDataTxFrames + MAC_ACTIVITY_DEFAULT_INC);
            break;
        default:
            break;
    }
}
/*******************************************************************************
 * @fn          setActivityTrackingRx
 *
 * @brief       This call is used by the LMAC to update the Receive activity
 *              tracking metrics.
 *
 * input parameters
 *
 * @param       pRxBuf - Pointer to MAC Receive buffer.
 *
 * @param       resetCount - Whether to reset activity tracking count.
 *                           True to reset, False to increment count.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static void setActivityTrackingRx(macRx_t *pRxBuf, bool resetCount)
{
    uint8_t cmdActivityFrameId = 0;

    if (pRxBuf) {
        /* Pre-calculate command ID for Link Establishment activity */
        if (((pRxBuf->internal.fcf & MAC_FCF_FRAME_TYPE_MASK) == MAC_FRAME_TYPE_COMMAND) && (pRxBuf->msdu.len > 0))
        {
          cmdActivityFrameId = *pRxBuf->msdu.p;
        }
    }

    /* Update previous frame type for successfully transmitted packet */
    if (cmdActivityFrameId && resetCount) {
        rxActivityData.prevRxFrameType = cmdActivityFrameId;
    }

    switch(rxActivityData.rxActivity)
    {
        case MAC_ACTIVITY_LINK_EST:
            rxActivityData.numRxAssocAbort = resetCount ? 0 : (rxActivityData.numRxAssocAbort + MAC_ACTIVITY_DEFAULT_INC);
            break;
        case MAC_ACTIVITY_FH:
            rxActivityData.numRxBCAbort = resetCount ? 0 : (rxActivityData.numRxBCAbort + MAC_ACTIVITY_DEFAULT_INC);
            break;
        case MAC_ACTIVITY_SCAN:
            rxActivityData.numRxScanAbort = resetCount ? 0 : (rxActivityData.numRxScanAbort + MAC_ACTIVITY_DEFAULT_INC);
            break;
        case MAC_ACTIVITY_DATA:
            rxActivityData.numRxDataAbort = resetCount ? 0 : (rxActivityData.numRxDataAbort + MAC_ACTIVITY_DEFAULT_INC);
            break;
        default:
            break;
    }
}

/*******************************************************************************
 * @fn          getActivityPriorityTx
 *
 * @brief       This call is used by the LMAC to calculate the activity priority
 *              index for the current activity.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static uint32_t getActivityPriorityTx(void)
{
    uint32_t currPri;

    switch(txActivityData.txActivity)
    {
        case MAC_ACTIVITY_LINK_EST:
        {
            /* Default Priority for association is HIGH */
            if(txActivityData.numMissedAssocTxFrames < MAC_ACTIVITY_THRESHOLD_PRI_URGENT)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_LINK_EST, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_LINK_EST, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
        }
        break;

        case MAC_ACTIVITY_TX_BEACON:
        {
            /* Default Priority for Beacon Tx is HIGH, increase after single missed/rejected beacon */
            if(txActivityData.numMissedBeaconTxFrames == 0)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_BEACON, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_TX_BEACON, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
        }
        break;

        case MAC_ACTIVITY_FH:
        {
           /* Default Priority is Normal */
           if(txActivityData.numMissedFHTxFrames < MAC_ACTIVITY_THRESHOLD_PRI_NORMAL)
           {
               currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_FH, MAC_ACTIVITY_PRI_NORMAL_INDEX);
           }
           else if(txActivityData.numMissedFHTxFrames < MAC_ACTIVITY_THRESHOLD_PRI_HIGH)
           {
               currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_FH, MAC_ACTIVITY_PRI_HIGH_INDEX);
           }
           else
           {
               currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_FH, MAC_ACTIVITY_PRI_URGENT_INDEX);
           }
        }
        break;

        case MAC_ACTIVITY_DATA:
        {
            /* Default Priority is Normal */
            if (txActivityData.numMissedDataTxFrames < MAC_ACTIVITY_THRESHOLD_PRI_NORMAL)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_DATA, MAC_ACTIVITY_PRI_NORMAL_INDEX);
            }
            else if (txActivityData.numMissedDataTxFrames < MAC_ACTIVITY_THRESHOLD_PRI_HIGH)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_DATA, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_DATA, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
        }
        break;
        default:
            currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_DATA, MAC_ACTIVITY_PRI_NORMAL_INDEX);
            break;
    }
    return (currPri);
}

/*******************************************************************************
 * @fn          getActivityPriorityRx
 *
 * @brief       This call is used by the LMAC to calculate the activity priority
 *              index for the current activity.
 *              Note: The priority calculation will is based on a number of missed
 *              events. For most activities the priority will increase incrementally
 *              until the max priority is reached. This is synonymous to the amount
 *              of missed events.
 *              Ex) Every missed abort shall increase the priority, up to the maximum.
 *
 * input parameters
 *
 * @param       None
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
static uint32_t getActivityPriorityRx(void)
{
    uint8_t beaconLost;
    uint32_t currPri;

    switch(rxActivityData.rxActivity)
    {
        case MAC_ACTIVITY_LINK_EST:
        {
            /* Default Priority for association is HIGH */
            if(rxActivityData.numRxAssocAbort < MAC_ACTIVITY_THRESHOLD_PRI_URGENT)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_LINK_EST, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_LINK_EST, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
        }
        break;

        case MAC_ACTIVITY_RX_BEACON:
        {
            /* Obtain number of missed beacons */
            beaconLost = getLostBeaconCount();

            /* Default Priority is High */
            if(beaconLost == 0)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_BEACON, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_BEACON, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
        }
        break;

        case MAC_ACTIVITY_FH:
        {
           /* Default Priority is High */
           if(rxActivityData.numRxBCAbort < MAC_ACTIVITY_THRESHOLD_PRI_URGENT)
           {
               currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_FH, MAC_ACTIVITY_PRI_HIGH_INDEX);
           }
           else
           {
               currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_FH, MAC_ACTIVITY_PRI_URGENT_INDEX);
           }
        }
        break;
        case MAC_ACTIVITY_SCAN:
        {
            /* All scans high priority by default */
            if(rxActivityData.numRxScanAbort < MAC_ACTIVITY_THRESHOLD_PRI_URGENT)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_SCAN, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_SCAN, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
        }
        break;
        case MAC_ACTIVITY_DATA:
        {
            /* Default Priority is Normal */
            if(rxActivityData.numRxDataAbort < MAC_ACTIVITY_THRESHOLD_PRI_NORMAL)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_DATA, MAC_ACTIVITY_PRI_NORMAL_INDEX);
            }
            else if(rxActivityData.numRxDataAbort < MAC_ACTIVITY_THRESHOLD_PRI_HIGH)
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_DATA, MAC_ACTIVITY_PRI_HIGH_INDEX);
            }
            else
            {
                currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_DATA, MAC_ACTIVITY_PRI_URGENT_INDEX);
            }
        }
        break;
        case MAC_ACTIVITY_RX_ALWAYS_ON:
        {
            /* Default Priority is Normal */
            currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_RX_ALWAYS_ON, MAC_ACTIVITY_PRI_NORMAL_INDEX);
        }
        break;
        default:
            currPri = CALC_ACTIVITY_PRIORITY(MAC_ACTIVITY_DATA, MAC_ACTIVITY_PRI_NORMAL_INDEX);
        break;
    }
    return (currPri);
}

/*******************************************************************************
 * @fn          setActivityTx
 *
 * @brief       This call is used by the LMAC to set the transmit activity.
 *
 * input parameters
 *
 * @param       txData - pointer to transmit packet.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      macActivity_t Current activity.
 */
static void setActivityTx(macTxIntData_t *txData)
{
    macActivity_t currActivity = MAC_ACTIVITY_DATA;

    /* Treat as ACK frame */
    if (!txData)
    {
        currActivity = MAC_ACTIVITY_DATA;

        if ((rxActivityData.prevRxFrameType == MAC_ASSOC_REQ_FRAME) ||
            (rxActivityData.prevRxFrameType == MAC_ASSOC_RSP_FRAME) ||
            (rxActivityData.prevRxFrameType == MAC_COORD_REALIGN_FRAME))
        {
            currActivity = MAC_ACTIVITY_LINK_EST;
        }
    }
    else
    {
        switch(txData->frameType)
        {
            /* MAC_ACTIVITY_LINK_EST */
            case MAC_INTERNAL_ASSOC_REQ:
            case MAC_INTERNAL_ASSOC_RSP:
            case MAC_INTERNAL_DISASSOC_NOTIF:
            {
                currActivity = MAC_ACTIVITY_LINK_EST;
            }
            break;

            /* MAC_ACTIVITY_TX_BEACON */
            case MAC_INTERNAL_BEACON:
            case MAC_INTERNAL_BEACON_REQ:
            case MAC_INTERNAL_COEXIST_EB_REQ:
            case MAC_INTERNAL_ENHANCED_BEACON_REQ:
            {
                currActivity = MAC_ACTIVITY_TX_BEACON;
            }
            break;

            /* MAC_ACTIVITY_FH */
            case MAC_INTERNAL_DATA_UNICAST:
            case MAC_INTERNAL_DATA_BROADCAST:
            {
                currActivity = MAC_ACTIVITY_FH;
            }
            break;

            /* MAC_ACTIVITY_DATA */
            case MAC_INTERNAL_ASYNC:
            case MAC_INTERNAL_DATA:
            case MAC_INTERNAL_DATA_REQ:
            {
                /* For Association poll requests, stay in link establishment activity */
                if (txActivityData.prevTxFrameType == MAC_INTERNAL_ASSOC_REQ)
                {
                    currActivity = MAC_ACTIVITY_LINK_EST;
                }
                else
                {
                    currActivity = MAC_ACTIVITY_DATA;
                }
            }
            break;
        }
    }
    txActivityData.txActivity = currActivity;
}

/*******************************************************************************
 * @fn          setActivityRx
 *
 * @brief       This call is used by the LMAC to set the receive activity.
 *
 * input parameters
 *
 * @param       rxEnableFlags - Bitmask containing the receiver enable flags.
 *
 * output parameters
 *
 * @param       None.
 *
 */
static void setActivityRx(uint8_t rxEnableFlags)
{
    macActivity_t currActivity = MAC_ACTIVITY_DATA;

    /* MAC_ACTIVITY_RX_BEACON */
    if ((!MAC_FH_ENABLED) && (rxEnableFlags & MAC_RX_BEACON_DEVICE))
    {
        currActivity = MAC_ACTIVITY_RX_BEACON;
    }
    /* MAC_ACTIVITY_FH */
    else if ((MAC_FH_ENABLED) && (rxEnableFlags & MAC_RX_FH_BC_SLOT))
    {
        currActivity = MAC_ACTIVITY_FH;
    }

    /* MAC_ACTIVITY_SCAN */
    else if (rxEnableFlags & MAC_RX_SCAN)
    {
        currActivity = MAC_ACTIVITY_SCAN;
    }

    /* MAC_ACTIVITY_DATA */
    else if (rxEnableFlags & MAC_RX_BROADCAST_PEND)
    {
        currActivity = MAC_ACTIVITY_DATA;
    }
    else if (rxEnableFlags & MAC_RX_WHEN_IDLE)
    {
        currActivity = MAC_ACTIVITY_RX_ALWAYS_ON;
    }
    /* For Rx poll (MAC_RX_POLL) requests or pre-tx explicit RxOn, accommodate Association procedure based on tx activity */
    else
    {
        /* Either switch to Link establishment or data activity */
        currActivity = (txActivityData.txActivity == MAC_ACTIVITY_LINK_EST) ? MAC_ACTIVITY_LINK_EST : MAC_ACTIVITY_DATA;
    }
    rxActivityData.rxActivity = currActivity;
}


/*******************************************************************************
 * @fn          getActivityTx
 *
 * @brief       This call is used by the LMAC to obtain the current transmit activity.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      macActivity_t Current activity.
 */
static macActivity_t getActivityTx(void)
{
    return (txActivityData.txActivity);
}

/*******************************************************************************
 * @fn          getActivityRx
 *
 * @brief       This call is used by the LMAC to obtain the current receive activity.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 */
static macActivity_t getActivityRx(void)
{
    return (rxActivityData.rxActivity);
}

#ifdef MAC_ACTIVITY_PROFILING

/* Interrupt period based on 10us clock tick */
#define ACTIVITY_PROFILING_PREEMPT_TIMEOUT (1000)

/* Number of internal command flushes occurred */
uint32_t numFlush = 0;

static void activityPreemptCb(void *arg)
{
    /* Only preempt Tx commands until Rx resume is implemented. */
    if ((arg) && (macTxCmdActive == TRUE))
    {
        /* Flush 15.4 command queue to force priority increase */
        (void)RF_flushCmd((RF_Handle) arg, RF_CMDHANDLE_FLUSH_ALL, RF_ABORT_PREEMPTION);
        numFlush++;
    }
}

static void startActivityProfilingTimer(RF_Handle handle)
{
    static Clock_Handle macActivityClock = NULL;

    Clock_Params params;
    Clock_Params_init(&params);
    params.startFlag = FALSE;
    params.period = ACTIVITY_PROFILING_PREEMPT_TIMEOUT;
    params.arg = (xdc_UArg)handle;
    macActivityClock = Clock_create((Clock_FuncPtr) activityPreemptCb,
                                    ACTIVITY_PROFILING_PREEMPT_TIMEOUT, &params, NULL);
    MAC_ASSERT(macActivityClock);
    //Clock_start(macActivityClock);
}

static void printActivityInfo(uint32_t activity, uint32_t priority, uint8_t frameType, sAddr_t destAddr)
{
    /* Concatenated (last 2 bytes of extended address) or short address */
    uint16_t concatAddr;

    /* Parse address type */
    if (destAddr.addrMode == ApiMac_addrType_short) {
        concatAddr = destAddr.addr.shortAddr;
    }
    else
    {
        concatAddr = (uint16_t)(((destAddr.addr.extAddr[0]) & 0x00FF) + (((destAddr.addr.extAddr[1]) & 0x00FF) << 8));
    }

    switch(activity)
    {
        case MAC_ACTIVITY_LINK_EST:
        {
            DBG_PRINT3(DBGSYS, "ACTIVITY PROFILE: Activity | MAC_ACTIVITY_LINK_EST | Priority (%x) | TxFrameType(%x) | DST Address(%x)",  priority,  frameType,  concatAddr);
        }
        break;
        case MAC_ACTIVITY_TX_BEACON:
        {
            DBG_PRINT3(DBGSYS, "ACTIVITY PROFILE: Activity | MAC_ACTIVITY_TX_BEACON | Priority (%x) | TxFrameType(%x) | DST Address(%x)",  priority,  frameType,  concatAddr);
        }
        break;
        case MAC_ACTIVITY_RX_BEACON:
        {
            DBG_PRINT3(DBGSYS, "ACTIVITY PROFILE: Activity | MAC_ACTIVITY_RX_BEACON | Priority (%x) | TxFrameType(%x) | DST Address(%x)",  priority,  frameType,  concatAddr);
        }
        break;

        case MAC_ACTIVITY_FH:
        {
            DBG_PRINT3(DBGSYS, "ACTIVITY PROFILE: Activity | MAC_ACTIVITY_FH | Priority (%x) | TxFrameType(%x) | DST Address(%x)",  priority,  frameType,  concatAddr);
        }
        break;
        case MAC_ACTIVITY_SCAN:
        {
            DBG_PRINT3(DBGSYS, "ACTIVITY PROFILE: Activity | MAC_ACTIVITY_SCAN | Priority (%x) | TxFrameType(%x) | DST Address(%x)",  priority,  frameType,  concatAddr);
        }
        break;
        case MAC_ACTIVITY_DATA:
        {
            DBG_PRINT3(DBGSYS, "ACTIVITY PROFILE: Activity | MAC_ACTIVITY_DATA | Priority (%x) | TxFrameType(%x) | DST Address(%x)",  priority,  frameType,  concatAddr);
        }
        break;
        case MAC_ACTIVITY_RX_ALWAYS_ON:
        {
            DBG_PRINT3(DBGSYS, "ACTIVITY PROFILE: Activity | MAC_ACTIVITY_RX_ALWAYS_ON | Priority (%x) | TxFrameType(%x) | DST Address(%x)",  priority,  frameType,  concatAddr);
        }
        break;
    }
}
#endif /* MAC_ACTIVITY_PROFILING */

/********************************************************************************/
