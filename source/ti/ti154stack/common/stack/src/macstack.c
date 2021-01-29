
/******************************************************************************

 @file  macstack.c

 @brief Mac Stack interface function implementation

 Group: WCS LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2013-2021, Texas Instruments Incorporated
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

#include "macstack.h"
#include <icall.h>
#include "osal.h"
#include "mac_main.h"
#include "mac_data.h"
#include "mac_pib.h"
#include "macs.h"
#include "mac_mgmt.h"
#include "mac_security.h"
#include "mac_security_pib.h"
#include "hal_mcu.h"
#include "macwrapper.h"
#include "icall_lite_translation.h"
/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */
#define TRUE 1
#define FALSE 0

#include "rom_jt_154.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */
/* Size table for MAC structures */
const uint8 msacbackSizeTable [] =
{
  0,                                   /* unused */
  sizeof(macMlmeAssociateInd_t),       /* MAC_MLME_ASSOCIATE_IND */\
  sizeof(macMlmeAssociateCnf_t),       /* MAC_MLME_ASSOCIATE_CNF */
  sizeof(macMlmeDisassociateInd_t),    /* MAC_MLME_DISASSOCIATE_IND */
  sizeof(macMlmeDisassociateCnf_t),    /* MAC_MLME_DISASSOCIATE_CNF */
  sizeof(macMlmeBeaconNotifyInd_t),    /* MAC_MLME_BEACON_NOTIFY_IND */
  sizeof(macMlmeOrphanInd_t),          /* MAC_MLME_ORPHAN_IND */
  sizeof(macMlmeScanCnf_t),            /* MAC_MLME_SCAN_CNF */
  sizeof(macMlmeStartCnf_t),           /* MAC_MLME_START_CNF */
  sizeof(macMlmeSyncLossInd_t),        /* MAC_MLME_SYNC_LOSS_IND */
  sizeof(macMlmePollCnf_t),            /* MAC_MLME_POLL_CNF */
  sizeof(macMlmeCommStatusInd_t),      /* MAC_MLME_COMM_STATUS_IND */
  sizeof(macMcpsDataCnf_t),            /* MAC_MCPS_DATA_CNF */
  sizeof(macMcpsDataInd_t),            /* MAC_MCPS_DATA_IND */
  sizeof(macMcpsPurgeCnf_t),           /* MAC_MCPS_PURGE_CNF */
  sizeof(macEventHdr_t),               /* MAC_PWR_ON_CNF */
  sizeof(macMlmePollInd_t),            /* MAC_MLME_POLL_IND */
  sizeof(macMlmeWSAsyncCnf_t),         /* MAC_MLME_WS_ASYNC_CNF */
  sizeof(macMlmeWSAsyncInd_t)          /* MAC_MLME_WS_ASYNC_IND */
};

/* ------------------------------------------------------------------------------------------------
 *                                         Local Variables
 * ------------------------------------------------------------------------------------------------
 */
/* Save the AppTaskId, part of MAC_INIT message in this variable */
uint8 AppTaskId;

/* Holds the value for query transmit, passed by the application.
 * A value of 0 indicates no continuation of retry,
 * A value of 1 indicates continue retransmission.
 */
static uint8 queryRetransmit = 0;

/* Holds the value for pending indirect msgs, passed by the application.
 * A value of 0 indicates no pending indirect msg.
 */
static uint8 checkPending = 0;

/* The Application will send the address of the MAC_CbackQueryRetransmit()
 * as part of MAC_INIT message. Save to function pointer below
 */
uint8 (*pMacCbackQueryRetransmit)() = NULL;

/* The Application will send the address of the MAC_CbackCheckPending()
 * as part of MAC_INIT message. Save to function pointer below
 */
uint8 (*pMacCbackCheckPending)() = NULL;

/* ------------------------------------------------------------------------------------------------
 *                                           Functions
 * ------------------------------------------------------------------------------------------------
 */
static void macApp(macCmd_t *pMsg);
extern uint8 MAC_MlmeGetReqSize( uint8 pibAttribute );
extern uint8 MAC_MlmeGetSecurityReqSize( uint8 pibAttribute );
extern uint8 MAC_MlmeFHGetReqSize( uint16 pibAttribute );

/*********************************************************************
 * @fn      icall_slimMsgParser
 *
 * @brief   parsing of the mac specific icall direct API message.
 *
 * @param   *msg - pointer to message receive
 *
 * @return  None
*/
void icall_slimMsgParser(void * msg)
{
  osal_msg_hdr_t *hdr;

  hdr = (osal_msg_hdr_t *) msg - 1;

  if (hdr->format == ICALL_MSG_FORMAT_DIRECT_API_ID)
  {
      icall_directAPIMsg_t* appMsg = msg;
      icall_liteTranslation(appMsg);
  }
}

/**************************************************************************************************
 * @fn          macStackTaskInit
 *
 * @brief       This function is called when OSAL is initialized.
 *
 * input parameters
 *
 * @param       taskId - OSAL task ID for MAC
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void macStackTaskInit(uint8 taskId)
{
  ICall_EntityID entity;
  ICall_Semaphore sem;

  /* Enroll the service that this stack represents */
  ICall_enrollService(ICALL_SERVICE_CLASS_TIMAC, NULL, &entity, &sem);

  /* Enroll the obtained dispatcher entity and task ID to OSAL
   * so that OSAL can route the dispatcher message into
   * the appropriate OSAL task.
   */
  osal_enroll_dispatchid(taskId, entity);

  /* Enroll the obtained service entity. This is to ensure
   * that non-task (such as ISR) can route the dispatcher message
   * into the appropriate alien task (application).
   */
  osal_enroll_notasksender(entity);

  macTaskId = taskId;
}

/**************************************************************************************************
 * @fn          macStackEventLoop
 *
 * @brief       This function is the main event handling function of the MAC executing
 *              in task context.  This function is called by OSAL when an event or message
 *              is pending for the MAC.
 *
 * input parameters
 *
 * @param       taskId - OSAL task ID of this task.
 * @param       events - OSAL event mask.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
uint16 macStackEventLoop(uint8 taskId, uint16 events)
{
  macEvent_t          *pMsg;
  macEventHdr_t       hdr;
  halIntState_t       intState;
  uint8               status;

  (void)taskId;  // Intentionally unreferenced parameter

  /* handle events on rx queue */
  if (events & MAC_RX_QUEUE_TASK_EVT)
  {
    while ((pMsg = (macEvent_t *) osal_msg_dequeue(&macData.rxQueue)) != NULL)
    {
      HAL_ENTER_CRITICAL_SECTION(intState);
      macData.rxCount--;
      HAL_EXIT_CRITICAL_SECTION(intState);

      hdr.event = pMsg->hdr.event;

      macMain.pBuf = (uint8 *)pMsg;

      /* Check security processing result from the LMAC */
      status = pMsg->hdr.status;

      if (status == MAC_SUCCESS)
      {
        macExecute(pMsg);
      }
#ifdef FEATURE_MAC_SECURITY
      else if ((status  == MAC_IMPROPER_KEY_TYPE) ||
               (status  == MAC_IMPROPER_SECURITY_LEVEL) ||
               (status  == MAC_SECURITY_ERROR) ||
               (status  == MAC_UNAVAILABLE_KEY) ||
               (status  == MAC_UNSUPPORTED_LEGACY) ||
               (status  == MAC_UNSUPPORTED_SECURITY) ||
               (status  == MAC_INVALID_PARAMETER) ||
               (status  == MAC_COUNTER_ERROR))
      {
        macCommStatusInd(pMsg);

        /* discard invalid DATA_IND frame */
        if (hdr.event == MAC_RX_DATA_IND_EVT)
        {
          mac_msg_deallocate((uint8 **)&pMsg);
        }
      }
#else
      else
      {
        /* discard invalid DATA_IND frame */
        if (hdr.event == MAC_RX_DATA_IND_EVT)
        {
          mac_msg_deallocate((uint8 **)&pMsg);
        }
      }
#endif /* FEATURE_MAC_SECURITY */

      /* discard all frames except DATA_IND.
       * do not free data frames for app
       */
      if (hdr.event != MAC_RX_DATA_IND_EVT)
      {
        mac_msg_deallocate(&macMain.pBuf);
      }
    }
  }

  /* handle events on osal msg queue */
  if (events & SYS_EVENT_MSG)
  {
    while ((pMsg = (macEvent_t *) osal_msg_receive(macTaskId)) != NULL)
    {
      if(pMsg->hdr.event >= 0xD0)
      {
        /* 0xE0 to 0xFF is reserved for App to send messages to TIMAC */
        macApp((macCmd_t *)pMsg);
      }
      else
      {
        macMain.pBuf = (uint8 *)pMsg;
        /* execute state machine */
        macExecute(pMsg);
        mac_msg_deallocate(&macMain.pBuf);
      }
    }
  }

  /* handle tx complete */
  if (events & MAC_TX_COMPLETE_TASK_EVT)
  {
    hdr.event = MAC_INT_TX_COMPLETE_EVT;
    macExecute((macEvent_t *) &hdr);
  }

  /* handle response wait timer */
  if (events & MAC_RESPONSE_WAIT_TASK_EVT)
  {
    hdr.event = MAC_TIM_RESPONSE_WAIT_EVT;
    macExecute((macEvent_t *) &hdr);
  }

  /* handle frame response timer */
  if (events & MAC_FRAME_RESPONSE_TASK_EVT)
  {
    hdr.event = MAC_TIM_FRAME_RESPONSE_EVT;
    macExecute((macEvent_t *) &hdr);
  }

  /* handle scan timer */
  if (events & MAC_SCAN_TASK_EVT)
  {
    hdr.event = MAC_TIM_SCAN_EVT;
    macExecute((macEvent_t *) &hdr);
  }

  /* handle indirect timer */
  if (events & MAC_EXP_INDIRECT_TASK_EVT)
  {
    hdr.event = MAC_TIM_EXP_INDIRECT_EVT;
    macExecute((macEvent_t *) &hdr);
  }

  /* handle start complete */
  if (events & MAC_START_COMPLETE_TASK_EVT)
  {
    hdr.status = MAC_SUCCESS;
    hdr.event = MAC_INT_START_COMPLETE_EVT;
    macExecute((macEvent_t *) &hdr);
  }

  /* handle pending broadcast */
  if (events & MAC_BROADCAST_PEND_TASK_EVT)
  {
    hdr.status = MAC_SUCCESS;
    hdr.event = MAC_INT_BROADCAST_PEND_EVT;
    macExecute((macEvent_t *) &hdr);
  }

  if (events & MAC_CSMA_TIM_TASK_EVT)
  {
    hdr.status = MAC_SUCCESS;
    hdr.event = MAC_CSMA_TIM_EXP_EVT ;
    macExecute((macEvent_t *) &hdr);
  }
  if (events & MAC_TX_DATA_REQ_TASK_EVT )
  {
    hdr.status = MAC_SUCCESS;
    hdr.event = MAC_TX_DATA_REQ_PEND_EVT ;
    macExecute((macEvent_t *) &hdr);
  }
  if (events & MAC_BC_TIM_TASK_EVT)
  {
    hdr.status = MAC_SUCCESS;
    hdr.event = MAC_BC_TIM_EXP_EVT ;
    macExecute((macEvent_t *) &hdr);
  }

  /* handle pending message, if any */
  if (macMain.pPending != NULL)
  {
    /* Hold off interrupts */
    HAL_ENTER_CRITICAL_SECTION(intState);

    pMsg = macMain.pPending;
    macMain.pPending = NULL;

    /* Allow interrupts */
    HAL_EXIT_CRITICAL_SECTION(intState);

    macMain.pBuf = (uint8 *)pMsg;
    /* execute state machine */
    macExecute(pMsg);
    mac_msg_deallocate(&macMain.pBuf);
  }

  return 0;
}

/**************************************************************************************************
 * @fn          macApp
 *
 * @brief       This function is called when Application sends MAC Stack a message.
 *
 * input parameters
 *
 * @param       pMsg - pointer to macCmd_t structure
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void macApp(macCmd_t *pMsg)
{
  uint8 dealloc = TRUE;
  /*
   * Set this flag to TRUE to send the message back to the app at the end
   * of the function.
   */
  uint8 sendMsg = FALSE;

  switch (pMsg->hdr.event)
  {
  case MAC_STACK_INIT_PARAMS:
   AppTaskId = pMsg->macInit.srctaskid;
   queryRetransmit = pMsg->macInit.retransmit;
   checkPending = pMsg->macInit.pendingMsg;
   pMacCbackQueryRetransmit = pMsg->macInit.pMacCbackQueryRetransmit;
   pMacCbackCheckPending = pMsg->macInit.pMacCbackCheckPending;
   dealloc = TRUE;
   break;
  }

  if(sendMsg == TRUE)
  {
    /* send message to App */
    osal_msg_send(AppTaskId, (uint8 *)pMsg);
  }

  if(dealloc)
  {
    osal_msg_deallocate((uint8 *)pMsg);
  }
}

/**************************************************************************************************
 *
 * @fn          MAC_CbackEvent
 *
 * @brief       This callback function sends MAC events to the application.
 *              The application must implement this function.  A typical
 *              implementation of this function would allocate an OSAL message,
 *              copy the event parameters to the message, and send the message
 *              to the application's OSAL event handler.  This function may be
 *              executed from task or interrupt context and therefore must
 *              be reentrant.
 *
 * @param       pData - Pointer to parameters structure.
 *
 * @return      None.
 *
 **************************************************************************************************
*/
void MAC_CbackEvent(macCbackEvent_t *pData)
{
  macCbackEvent_t *pMsg = NULL;

  uint8 len = msacbackSizeTable[pData->hdr.event];

  switch (pData->hdr.event)
  {
    case MAC_MLME_BEACON_NOTIFY_IND:
      if ( MAC_BEACON_STANDARD == pData->beaconNotifyInd.beaconType )
      {
        len += sizeof(macPanDesc_t) + pData->beaconNotifyInd.info.beaconData.sduLength +
             MAC_PEND_FIELDS_LEN(pData->beaconNotifyInd.info.beaconData.pendAddrSpec);
        if ((pMsg = (macCbackEvent_t *) osal_msg_allocate(len)) != NULL)
        {
          /* Copy data over and pass them up */
          osal_memcpy(pMsg, pData, sizeof(macMlmeBeaconNotifyInd_t));
          pMsg->beaconNotifyInd.info.beaconData.pPanDesc = (macPanDesc_t *) ((uint8 *) pMsg + sizeof(macMlmeBeaconNotifyInd_t));
          osal_memcpy(pMsg->beaconNotifyInd.info.beaconData.pPanDesc, pData->beaconNotifyInd.info.beaconData.pPanDesc, sizeof(macPanDesc_t));
          pMsg->beaconNotifyInd.info.beaconData.pSdu = (uint8 *) (pMsg->beaconNotifyInd.info.beaconData.pPanDesc + 1);
          osal_memcpy(pMsg->beaconNotifyInd.info.beaconData.pSdu, pData->beaconNotifyInd.info.beaconData.pSdu, pData->beaconNotifyInd.info.beaconData.sduLength);
        }
      }
      else if ( MAC_BEACON_ENHANCED == pData->beaconNotifyInd.beaconType )
      {
        len += sizeof(macPanDesc_t);

        if ((pMsg = (macCbackEvent_t *) osal_msg_allocate(len)) != NULL)
        {
          /* Copy data over and pass them up */
          osal_memcpy(pMsg, pData, sizeof(macMlmeBeaconNotifyInd_t));
          pMsg->beaconNotifyInd.info.eBeaconData.pPanDesc = (macPanDesc_t *) ((uint8 *) pMsg + sizeof(macMlmeBeaconNotifyInd_t));
          osal_memcpy(pMsg->beaconNotifyInd.info.eBeaconData.pPanDesc, pData->beaconNotifyInd.info.eBeaconData.pPanDesc, sizeof(macPanDesc_t));
        }
      }
      break;

    case MAC_MCPS_DATA_IND:
    case MAC_MLME_WS_ASYNC_IND:
      pMsg = pData;
      break;


    default:
      pMsg = (macCbackEvent_t *)osal_msg_allocate(len);
      if ((pMsg == NULL) && (pData->hdr.event == MAC_MCPS_DATA_CNF)
                      && (pData->dataCnf.pDataReq != NULL))
      {
        /*
         * The allocation failed for the data confirm, so we will
         * try to deallocate the memory used by the data request.
         * [mac_msg_deallocate() will NULL pDataReq]
         */
        mac_msg_deallocate((uint8**)&(pData->dataCnf.pDataReq));

        /* Then, try the allocation again */
        pMsg = (macCbackEvent_t *) osal_msg_allocate(len);
      }

      if (pMsg != NULL)
      {
        osal_memcpy(pMsg, pData, len);
      }
      break;
  }

  if (pMsg != NULL)
  {
    osal_msg_send(AppTaskId, (uint8 *) pMsg);
  }
}

/**************************************************************************************************
 * @fn          MAC_CbackQueryRetransmit
 *
 * @brief       This function callback function returns whether or not to continue MAC
 *              retransmission.
 *              A return value '0x00' will indicate no continuation of retry and a return value
 *              '0x01' will indicate to continue retransmission. This callback function shall be
 *              used to stop continuing retransmission for RF4CE.
 *              MAC shall call this callback function whenever it finishes transmitting a packet
 *              for macMaxFrameRetries times.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      0x00 to stop retransmission, 0x01 to continue retransmission.
 **************************************************************************************************
*/
uint8 MAC_CbackQueryRetransmit(void)
{
  uint8 reTransmit = queryRetransmit;

  if ( pMacCbackQueryRetransmit )
  {
    reTransmit = (*pMacCbackQueryRetransmit)();
  }

  return reTransmit;
}

/**************************************************************************************************
 * @fn          MAC_CbackCheckPending
 *
 * @brief       This callback function returns the number of pending indirect messages queued in
 *              the application. Most applications do not queue indirect data and can simply
 *              always return zero. The number of pending indirect messages only needs to be
 *              returned if macCfg.appPendingQueue to TRUE.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      The number of indirect messages queued in the application or zero.
 **************************************************************************************************
*/
uint8 MAC_CbackCheckPending(void)
{
  uint8 pend = checkPending;

  if ( pMacCbackCheckPending )
  {
    pend = (*pMacCbackCheckPending)();
  }

  return pend;
}



