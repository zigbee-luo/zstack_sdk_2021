/**************************************************************************************************
  Filename:       per_test.c
  Revised:        $Date: 2018-05-2 11:51:49 -0700 (Wed, 2 May 2018) $
  Revision:       $Revision: - $

  Description:    This file contains the implementation of the Packet Error Rate
	                test interface.


  Copyright 2006-2015 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
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
**************************************************************************************************/

#ifdef PER_TEST
/*********************************************************************
 * INCLUDES
 */

#include "per_test.h"
#include "string.h"
#include "zstackmsg.h"
#include "rom_jt_154.h"
#include "util_timer.h"
#include "zstackapi.h"
#include <ti/sysbios/knl/Semaphore.h>


/*********************************************************************
 * MACROS
 */


#define PER_TEST_SEND1    1
#define PER_TEST_SEND2    2
#define PER_TEST_FWD1     3
#define PER_TEST_FWD2     4

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * CONSTANTS
 */

#define PER_TEST_CLUSTER         ZCL_CLUSTER_ID_GENERAL_ON_OFF
#define PER_TEST_ENDPOINT        8
#define PER_TEST_FC_BUFFER_SIZE  15
#define PER_TEST_INVALID_FC      0xFFFF

/*********************************************************************
 * GLOBAL VARIABLES
 */
#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
uint32_t  FramesQueued = 0;          //Frames queued
uint32_t  FramesFailedToSend = 0;  //Frames attempted to be send but failed   (invalid configuration of the frame send or no parent connected?)
uint32_t  FramesSuccess = 0;     //APS ACK Received
uint32_t  FramesNoAck = 0;        //Frames that did not receive APS ACK
#endif

/*********************************************************************
 * LOCAL VARIABLES
 */

static uint8_t perTest_fcIndex;
// initialize every entry to invalid FC value
static uint16_t perTest_fcBuffer[PER_TEST_FC_BUFFER_SIZE];

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING) || defined(PER_TEST_ENABLE_FWD_NOTIFICATION)
static uint8_t stAppID = 0xFF;
#endif

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
static Semaphore_Handle sem;
static uint16_t events = 0;

static Clock_Handle PERTest_sendCmdClkHandle;
static Clock_Struct PERTest_sendCmdClkStruct;

static zclOptionRec_t PERTestOptions[] =
{
  {
    PER_TEST_CLUSTER,
    ( AF_ACK_REQUEST ),
  },
};

static afAddrType_t perTest_DstAddr;
#endif

#ifdef  PER_TEST_ENABLE_FWD_NOTIFICATION
static uint32_t  CounterFramesQueued = 0;
static uint32_t  CounterFramesSuccess = 0;

static uint32_t  CounterFramesNoMacAck = 0;
static uint32_t  CounterFramesNoRoute = 0;
static uint32_t  CounterFramesTransExpired = 0;
#endif



#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
/* Only one screen */
//Frame send accepted by the nwk layer to be send
char FrameQueuedStr[] =    "QUEUED: ";
//Frame failed to Queue
char FrameFailedStr[] =    "FAILED: ";
//Frame for which we receive APS ACK
char FrameSuccessStr[] =   "SUCCESS: ";
//Frame for which we did not receive APS ACK
char FrameNoAckStr[] =    "NoAck: ";
#endif



#if defined(PER_TEST_ENABLE_FWD_NOTIFICATION)
/* First Screen */
//Frame received at nwk layer and queued to be send
char FrameQueuedStr[] =    "QUEUED: ";
//Frame MAC confirm success
char FrameSuccessStr[] =   "SUCCESS: ";
/* Second Screen */
//Nwk layer says no route to destination
char FrameNoRouteStr[] =   "NoRoute: ";
//No MAC ACK
char FrameNoAckStr[] =     "NoMAck: ";
//Frame expired at MAC layer
char FrameExpiredStr[] =   "Expired: ";
#endif

static CUI_clientHandle_t gPERCuiHandle;
static uint32_t gPerTestInfoLine;

/*********************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 */

/*********************************************************************
 * LOCAL FUNCTION DEFINITIONS
 */
static void PerTest_initializeClocks(void);

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)

static void PERTest_processPERTestSendCmdTimeoutCallback(UArg a0);
static void PERTest_sendCmd(void);
static void PERTest_dataConfirmAnalysis(zstack_afDataConfirmInd_t *req);
#endif

#ifdef PER_TEST_ENABLE_FWD_NOTIFICATION
static void PERTest_routingFramesAnalysis(zstack_nwkFrameFwdNotification_t *req);
#endif

static void PERTest_UpdateStatusLine(void);

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

void PERTest_init(Semaphore_Handle appSem, uint8_t stEnt, CUI_clientHandle_t gCuiHandle)
{
#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
  sem = appSem;
#endif

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING) || defined(PER_TEST_ENABLE_FWD_NOTIFICATION)
  stAppID = stEnt;
#endif
  PerTest_initializeClocks();

  uint8_t i;
  for ( i = 0; i < PER_TEST_FC_BUFFER_SIZE; i++)
  {
    perTest_fcBuffer[i] = PER_TEST_INVALID_FC;
  }

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
    zstack_sysConfigWriteReq_t zstack_sysConfigWriteReq;

    memset(&zstack_sysConfigWriteReq,0,sizeof(zstack_sysConfigWriteReq_t));

    //Set poll rate
    zstack_sysConfigWriteReq.has_pollRate = TRUE;
    zstack_sysConfigWriteReq.pollRate = PER_TEST_POLL_RATE;
    zstack_sysConfigWriteReq.pollRateType = POLL_RATE_TYPE_DEFAULT;

    //Set the APS ACK wait time
    zstack_sysConfigWriteReq.has_apsAckWaitDuration = TRUE;
    zstack_sysConfigWriteReq.apsAckWaitDuration = PER_TEST_APS_ACKS_WAIT_TIME/2;   //(for some reason this is multiplied by 2, so divide it by 2)

    //Set APS retries retries
    zstack_sysConfigWriteReq.has_apsFrameRetries = TRUE;
    zstack_sysConfigWriteReq.apsFrameRetries = PER_TEST_APS_ACKS;

    Zstackapi_sysConfigWriteReq(stAppID, &zstack_sysConfigWriteReq);

    //Enable APS ACKs
    zcl_registerClusterOptionList(PER_TEST_ENDPOINT,1,PERTestOptions);
#endif


#ifdef PER_TEST_ENABLE_FWD_NOTIFICATION
  zstack_setNwkFrameFwdNotificationReq_t zstack_setNwkFrameFwdNotificationReq;
  zstack_setNwkFrameFwdNotificationReq.Enabled = TRUE;

  Zstackapi_SetNwkFrameFwdNotificationReq(stAppID,&zstack_setNwkFrameFwdNotificationReq);
#endif

  gPERCuiHandle = gCuiHandle;
  CUI_statusLineResourceRequest(gCuiHandle, "PER Test", false, &gPerTestInfoLine);
}

void PERTest_process(void)
{
#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
  if ( events & SAMPLEAPP_PER_TEST_SEND_CMD_EVT )
  {
      PERTest_sendCmd();
      events &= ~SAMPLEAPP_PER_TEST_SEND_CMD_EVT;
  }
#endif
}

void PERTest_processZStackMsg(zstackmsg_genericReq_t *pMsg)
{

  static uint8_t isPERinitialized = 0;

  switch(pMsg->hdr.event)
  {

#ifdef PER_TEST_ENABLE_FWD_NOTIFICATION
    case zstackmsg_CmdIDs_SYS_NWK_FRAME_FWD_NOTIFICATION_IND:
    {
        zstackmsg_nwkFrameFwdNotification_t *pInd;
        pInd = (zstackmsg_nwkFrameFwdNotification_t*)pMsg;

        PERTest_routingFramesAnalysis(&(pInd->req));
        PERTest_UpdateStatusLine();
    }
    break;
#endif

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
    case zstackmsg_CmdIDs_AF_DATA_CONFIRM_IND:
    {
        // This message is received as a confirmation of a data packet sent.
        // The status is of ZStatus_t type [defined in nl_mede.h]
        // The message fields are defined in af.h
        zstackmsg_afDataConfirmInd_t *pInd;
        pInd = (zstackmsg_afDataConfirmInd_t*)pMsg;

        PERTest_dataConfirmAnalysis(&(pInd->req));
        PERTest_UpdateStatusLine();
    }
    break;

#ifdef PER_TEST_SEND_USE_FINDING_BINDING
    case zstackmsg_CmdIDs_BDB_BIND_NOTIFICATION_CB:
    {
      //If we have created a bind with the light, then start configuration for PER test on zed
      UtilTimer_setTimeout( PERTest_sendCmdClkHandle, SAMPLEAPP_PER_TEST_SEND_CMD_DELAY );
      UtilTimer_start(&PERTest_sendCmdClkStruct);
      isPERinitialized = TRUE;
    }
    break;
#endif

    case zstackmsg_CmdIDs_BDB_NOTIFICATION:
    {
      if(!isPERinitialized)
      {
        zstackmsg_bdbNotificationInd_t *pInd;
        pInd = (zstackmsg_bdbNotificationInd_t*)pMsg;

        switch(pInd->Req.bdbCommissioningMode)
        {
#ifndef PER_TEST_SEND_USE_FINDING_BINDING
          case BDB_COMMISSIONING_NWK_STEERING:
            if(pInd->Req.bdbCommissioningStatus == BDB_COMMISSIONING_SUCCESS)
            {
                //If we are on a network, then start configuration for PER test on zed
                UtilTimer_setTimeout( PERTest_sendCmdClkHandle, SAMPLEAPP_PER_TEST_SEND_CMD_DELAY );
                UtilTimer_start(&PERTest_sendCmdClkStruct);
                isPERinitialized = TRUE;
            }
          break;
#endif
          case BDB_COMMISSIONING_INITIALIZATION:
            if(pInd->Req.bdbCommissioningStatus == BDB_COMMISSIONING_NETWORK_RESTORED)
            {
              //If we are on a network, then start configuration for PER test on zed
              UtilTimer_setTimeout( PERTest_sendCmdClkHandle, SAMPLEAPP_PER_TEST_SEND_CMD_DELAY );
              UtilTimer_start(&PERTest_sendCmdClkStruct);
              isPERinitialized = TRUE;
            }
          break;
          case BDB_COMMISSIONING_PARENT_LOST:
            if(pInd->Req.bdbCommissioningStatus == BDB_COMMISSIONING_NETWORK_RESTORED)
            {
              //If we are on a network, then start configuration for PER test on zed
              UtilTimer_setTimeout( PERTest_sendCmdClkHandle, SAMPLEAPP_PER_TEST_SEND_CMD_DELAY );
              UtilTimer_start(&PERTest_sendCmdClkStruct);
              isPERinitialized = TRUE;
            }
          break;
        }
      }
    break;
    }
#endif  //defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
  }
}


/*********************************************************************
 * LOCAL FUNCTIONS
 */

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
static void PERTest_dataConfirmAnalysis(zstack_afDataConfirmInd_t *req)
{
  uint8_t i = 0;
  // find the matching ZCL Transaction Sequence Number
  for( i = (perTest_fcIndex - 1); i != perTest_fcIndex; i = ((i - 1) % PER_TEST_FC_BUFFER_SIZE) )
  {
    if(perTest_fcBuffer[i] == req->transID)
    {
      perTest_fcBuffer[i] = PER_TEST_INVALID_FC;
      switch(req->status)
      {
          case ZSuccess:
            FramesSuccess++;
          break;
          case ZApsNoAck:
            FramesNoAck++;
          break;
          default:
          break;
      }
      break;
    }
  }
}
#endif

#ifdef PER_TEST_ENABLE_FWD_NOTIFICATION
static void PERTest_routingFramesAnalysis(zstack_nwkFrameFwdNotification_t *req)
{
  // do not track broadcasts in statistics
  if( req->nwkFrameFwdNotification.dstAddr == 0xFFFF ||
      req->nwkFrameFwdNotification.dstAddr == 0xFFFD ||
      req->nwkFrameFwdNotification.dstAddr == 0xFFFC )
  {
    return;
  }

  switch(req->nwkFrameFwdNotification.frameState)
  {
      case NWK_FRAME_FWD_MSG_QUEUED:
          //Message has been received and it has been queued
          CounterFramesQueued++;
      break;

      case NWK_FRAME_FWD_MSG_SENT:
          switch (req->nwkFrameFwdNotification.status)
          {
              case ZMacSuccess:
                  //Frame being forwarded correctly
                  CounterFramesSuccess++;
              break;

              case ZMacNoACK:
                  CounterFramesNoMacAck++;
              break;

              case ZNwkNoRoute:
                  CounterFramesNoRoute++;
              break;

              case ZMacTransactionExpired:
                  CounterFramesTransExpired++;
              break;

              default:
                  //does not seems to be any other valid value of interest, if needed it can be added...
              break;
          }
      break;


      default:
          //there is no other possible value
      break;
  }
}
#endif



/*******************************************************************************
 * @fn      PerTest_initializeClocks
 *
 * @brief   Initialize Clocks
 *
 * @param   none
 *
 * @return  none
 */
static void PerTest_initializeClocks(void)
{

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
    // Initialize the timers needed for this application
    PERTest_sendCmdClkHandle = UtilTimer_construct(
    &PERTest_sendCmdClkStruct,
    PERTest_processPERTestSendCmdTimeoutCallback,
    SAMPLEAPP_PER_TEST_SEND_CMD_DELAY,
    0, false, 0);
#endif

}

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
/*******************************************************************************
 * @fn      PERTest_processPERTestSendCmdTimeoutCallback
 *
 * @brief   Timeout handler function
 *
 * @param   a0 - ignored
 *
 * @return  none
 */
static void PERTest_processPERTestSendCmdTimeoutCallback(UArg a0)
{
    (void)a0; // Parameter is not used

    events |= SAMPLEAPP_PER_TEST_SEND_CMD_EVT;

    // Wake up the application thread when it waits for clock event
    Semaphore_post(sem);
}



/*******************************************************************************
 * @fn      PERTest_sendCmd
 *
 * @brief   Send the command to the device intended, parent or using F&B. See PER_TEST_SEND_TO_PARENT and PER_TEST_SEND_USE_FINDING_BINDING in per_test.h
 *
 * @param   none
 *
 * @return  none
 */
static void PERTest_sendCmd(void)
{

#if defined(PER_TEST_SEND_TO_PARENT)
    perTest_DstAddr.addrMode = afAddr16Bit;
    perTest_DstAddr.addr.shortAddr = _NIB.nwkCoordAddress;
    perTest_DstAddr.endPoint = PER_TEST_ENDPOINT;
#endif
#if defined(PER_TEST_SEND_USE_FINDING_BINDING)
    // Set destination address to indirect
    perTest_DstAddr.addrMode = (afAddrMode_t)AddrNotPresent;
    perTest_DstAddr.endPoint = 0;
    perTest_DstAddr.addr.shortAddr = 0;
#endif
    zstack_getZCLFrameCounterRsp_t Rsp;

    Zstackapi_getZCLFrameCounterReq(stAppID, &Rsp);

    // save the ZCL Transaction Sequence Number for comparison to
    // AF Data Confirm Indication from stack
    perTest_fcBuffer[perTest_fcIndex] = Rsp.zclFrameCounter;
    perTest_fcIndex = ((perTest_fcIndex + 1) % PER_TEST_FC_BUFFER_SIZE);

#ifdef ZCL_ON_OFF
    if(zclGeneral_SendOnOff_CmdToggle( PER_TEST_ENDPOINT, &perTest_DstAddr, TRUE, Rsp.zclFrameCounter ) == ZSuccess)
    {
        FramesQueued++;
    }
    else
    {
        FramesFailedToSend++;
    }
#endif
    //Keep sending the commands
    UtilTimer_setTimeout( PERTest_sendCmdClkHandle, SAMPLEAPP_PER_TEST_SEND_CMD_DELAY );
    UtilTimer_start(&PERTest_sendCmdClkStruct);
}
#endif




/*******************************************************************************
 * @fn      PERTest_UpdateStatusLine
 *
 * @brief   Update the PER status lines in the UI
 *
 * @param
 *
 * @return  none
 */
static void PERTest_UpdateStatusLine(void)
{

    char lineFormat[MAX_STATUS_LINE_VALUE_LEN] = {'\0'};

#if defined(PER_TEST_SEND_TO_PARENT) || defined(PER_TEST_SEND_USE_FINDING_BINDING)
    strcat(lineFormat, FrameQueuedStr);
    strcat(lineFormat, "%d || ");
    strcat(lineFormat, FrameFailedStr);
    strcat(lineFormat, "%d || ");
    strcat(lineFormat, FrameSuccessStr);
    strcat(lineFormat, "%d || ");
    strcat(lineFormat, FrameNoAckStr);
    strcat(lineFormat, "%d || ");

    CUI_statusLinePrintf(gPERCuiHandle, gPerTestInfoLine, lineFormat, FramesQueued, FramesFailedToSend, FramesSuccess, FramesNoAck);
#endif
#if defined(PER_TEST_ENABLE_FWD_NOTIFICATION)
    strcat(lineFormat, FrameQueuedStr);
    strcat(lineFormat, "%d || ");
    strcat(lineFormat, FrameSuccessStr);
    strcat(lineFormat, "%d || ");
    strcat(lineFormat, FrameNoRouteStr);
    strcat(lineFormat, "%d || ");
    strcat(lineFormat, FrameNoAckStr);
    strcat(lineFormat, "%d || ");
    strcat(lineFormat, FrameExpiredStr);
    strcat(lineFormat, "%d || ");

    CUI_statusLinePrintf(gPERCuiHandle, gPerTestInfoLine, lineFormat, CounterFramesQueued, CounterFramesSuccess, CounterFramesNoMacAck, CounterFramesNoRoute, CounterFramesTransExpired);
#endif
}

#endif /* PER_TEST */
