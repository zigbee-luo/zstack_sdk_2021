/***************************************************************************************************
  Filename:       MT.c
  Revised:        $Date: 2015-01-18 19:44:10 -0800 (Sun, 18 Jan 2015) $
  Revision:       $Revision: 41896 $

  Description:    MonitorTest Event Loop functions.
                  Everything in the MonitorTest Task (except the serial driver).


  Copyright 2007-2014 Texas Instruments Incorporated. All rights reserved.

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

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include "zcomdef.h"
#include "mt.h"
#include "mt_app.h"
#include "mt_debug.h"
#include "mt_util.h"
#include "mt_sys.h"

#include "rom_jt_154.h"
#include "osal_nv.h"

#include "debug_trace.h"
#include "zmac.h"

#if !defined ( NONWK )
  #include "nl_mede.h"
  #include "nwk_bufs.h"
  #include "zd_object.h"
  #include "ssp.h"
  #include "nwk_util.h"
  #include "af.h"

#endif

#if defined( MT_MAC_FUNC ) || defined( MT_MAC_CB_FUNC )
  #include "mt_mac.h"
#endif
#if defined( MT_NWK_FUNC ) || defined( MT_NWK_CB_FUNC )
  #include "mt_nwk.h"
  #include "nwk.h"
  #include "nwk_bufs.h"
#endif
#if defined( MT_AF_FUNC ) || defined( MT_AF_CB_FUNC )
  #include "mt_af.h"
#endif
#if defined( MT_USER_TEST_FUNC )
  #include "af.h"
#endif
#if defined( MT_ZDO_FUNC )
  #include "mt_zdo.h"
#endif
#if defined (MT_SAPI_FUNC)

#endif
#if defined (MT_OTA_FUNC)
  #include "mt_ota.h"
#endif

#if defined( APP_TP )
 #include "testprofile.h"
#endif
#if defined( APP_TP2 )
 #include "testprofile2.h"
#endif
#if defined(APP_DEBUG)
	#include "debugapp.h"
#endif
#if defined (MT_UBL_FUNC)
  extern uint8_t MT_UblCommandProcessing(uint8_t *pBuf);
#endif
#if defined (MT_ZNP_FUNC)
  #include "mt_znp.h"
#endif
#if defined (MT_MAC_PROTOBUF_FUNC)
  #include "mtmacpb.h"
#endif
#if defined (MT_GP_CB_FUNC)
  #include "mt_gp.h"
#endif

#if defined (MT_APP_CNF_FUNC)
  #include "mt_app_config.h"
#endif

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

mtProcessMsg_t mtProcessIncoming[] =
{
  NULL,                               // MT_RPC_SYS_RES0

#if defined (MT_SYS_FUNC)
  MT_SysCommandProcessing,            // MT_RPC_SYS_SYS
#else
  NULL,
#endif

#if defined (MT_MAC_FUNC)
  MT_MacCommandProcessing,            // MT_RPC_SYS_MAC
#else
  NULL,
#endif

#if defined (MT_NWK_FUNC)
  MT_NwkCommandProcessing,            // MT_RPC_SYS_NWK
#else
  NULL,
#endif

#if defined (MT_AF_FUNC)
  MT_AfCommandProcessing,             // MT_RPC_SYS_AF
#else
  NULL,
#endif

#if defined (MT_ZDO_FUNC)
  MT_ZdoCommandProcessing,            // MT_RPC_SYS_ZDO
#else
  NULL,
#endif

#if defined (MT_SAPI_FUNC)
  MT_SapiCommandProcessing,           // MT_RPC_SYS_SAPI
#else
  NULL,
#endif

#if defined (MT_UTIL_FUNC)
  MT_UtilCommandProcessing,           // MT_RPC_SYS_UTIL
#else
  NULL,
#endif

#if defined (MT_DEBUG_FUNC)
  MT_DebugCommandProcessing,          // MT_RPC_SYS_DBG
#else
  NULL,
#endif

#if defined (MT_APP_FUNC)
  MT_AppCommandProcessing,            // MT_RPC_SYS_APP
#else
  NULL,
#endif

#if defined (MT_OTA_FUNC)
  MT_OtaCommandProcessing,            // MT_RPC_SYS_OTA
#else
  NULL,
#endif

#if defined (MT_ZNP_FUNC)
  MT_ZnpCommandProcessing,
#else
  NULL,
#endif

  NULL,  // Spare sub-system 12.

#if defined (MT_UBL_FUNC)
  MT_UblCommandProcessing,
#else
  NULL,
#endif

  NULL,                               // MT_RPC_SYS_RESERVED14

#if defined (MT_APP_CNF_FUNC)
  MT_AppCnfCommandProcessing,        // MT_RPC_SYS_APP_CNF
#else
  NULL,
#endif

  NULL,                               // MT_RPC_SYS_RESERVED16
#if defined (MT_MAC_PROTOBUF_FUNC)
  MT_MacPBCmdProc,                    // MT_RPC_SYS_PROTOBUF
#else
  NULL,
#endif
  NULL,                               // MT_RPC_SYS_RES18
  NULL,                               // MT_RPC_SYS_RES19
  NULL,                               // MT_RPC_SYS_RES20
#if defined (MT_GP_CB_FUNC)
  MT_GpCommandProcessing,
#else
  NULL,
#endif
};

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

byte debugThreshold;
byte debugCompId;

/**************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/

void MT_MsgQueueInit( void );
void MT_ResetMsgQueue( void );
byte MT_QueueMsg( byte *msg , byte len );
void MT_ProcessQueue( void );

#if defined ( MT_USER_TEST_FUNC )
void MT_ProcessAppUserCmd( byte *pData );
#endif

/**************************************************************************************************
 * @fn         MT_Init
 *
 * @brief      This function is the secondary initialization that resolves conflicts during
 *             osalInitTasks(). For example, since MT is the highest priority task, and
 *             specifically because the MT task is initialized before the ZDApp task, if MT_Init()
 *             registers anything with ZDO_RegisterForZdoCB(), it is wiped out when ZDApp task
 *             initialization invokes ZDApp_InitZdoCBFunc().
 *             There may be other existing or future such races, so try to do all possible
 *             MT initialization here vice in MT_TaskInit().
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void MT_Init(void)
{
  debugThreshold = 0;
  debugCompId = 0;

#if defined (MT_ZDO_FUNC)
  MT_ZdoInit();
#endif
  MT_SysResetInd();
}

/***************************************************************************************************
 * @fn      MT_BuildAndSendZToolResponse
 *
 * @brief   Build and send a ZTOOL msg
 * @param   uint8_t cmdType - include type and subsystem
 *          uint8_t cmdId - command ID
 *          byte dataLen
 *          byte *pData
 *
 * @return  void
 ***************************************************************************************************/
#if !defined(NPI)
void MT_BuildAndSendZToolResponse(uint8_t cmdType, uint8_t cmdId, uint8_t dataLen, uint8_t *pData)
{
  uint8_t *msg_ptr;

  if ((msg_ptr = MT_TransportAlloc((mtRpcCmdType_t)(cmdType & 0xE0), dataLen)) != NULL)
  {
    msg_ptr[MT_RPC_POS_LEN] = dataLen;
    msg_ptr[MT_RPC_POS_CMD0] = cmdType;
    msg_ptr[MT_RPC_POS_CMD1] = cmdId;
    (void)OsalPort_memcpy(msg_ptr+MT_RPC_POS_DAT0, pData, dataLen);

    MT_TransportSend(msg_ptr);
  }
}
#endif /* NPI */
/***************************************************************************************************
 * @fn      MT_ProcessIncoming
 *
 * @brief  Process Incoming Message.
 *
 * @param   byte *pBuf - pointer to event message
 *
 * @return  void
 ***************************************************************************************************/
void MT_ProcessIncoming(uint8_t *pBuf)
{
  mtProcessMsg_t func;
  uint8_t rsp[MT_RPC_FRAME_HDR_SZ];

  /* pre-build response message:  | status | cmd0 | cmd1 | */
  rsp[1] = pBuf[MT_RPC_POS_CMD0];
  rsp[2] = pBuf[MT_RPC_POS_CMD1];

  /* check length */
  if (pBuf[MT_RPC_POS_LEN] > MT_RPC_DATA_MAX)
  {
    rsp[0] = MT_RPC_ERR_LENGTH;
  }
  /* check subsystem range */
  else if ((rsp[1] & MT_RPC_SUBSYSTEM_MASK) < MT_RPC_SYS_MAX)
  {
    /* look up processing function */
    func = mtProcessIncoming[rsp[1] & MT_RPC_SUBSYSTEM_MASK];
    if (func)
    {
      /* execute processing function */
      rsp[0] = (*func)(pBuf);
    }
    else
    {
      rsp[0] = MT_RPC_ERR_SUBSYSTEM;
    }
  }
  else
  {
    rsp[0] = MT_RPC_ERR_SUBSYSTEM;
  }

  /* if error and this was an SREQ, send error message */
  if ((rsp[0] != MT_RPC_SUCCESS) && ((rsp[1] & MT_RPC_CMD_TYPE_MASK) == MT_RPC_CMD_SREQ))
  {
    MT_BuildAndSendZToolResponse(((uint8_t)MT_RPC_CMD_SRSP | (uint8_t)MT_RPC_SYS_RES0), 0,
                                                                  MT_RPC_FRAME_HDR_SZ, rsp);
  }
}

/***************************************************************************************************
 * @fn      MTProcessAppRspMsg
 *
 * @brief   Process the User App Response Message
 *
 * @param   data - output serial buffer.  The first byte must be the
 *          endpoint that send this message.
 * @param   len - data length
 *
 * @return  none
 ***************************************************************************************************/
void MTProcessAppRspMsg( byte *pData, byte len )
{
  /* Send out Reset Response message */
  MT_BuildAndSendZToolResponse( ((uint8_t)MT_RPC_CMD_SRSP | (uint8_t)MT_RPC_SYS_APP), MT_APP_RSP, len, pData );
}


/***************************************************************************************************
 * @fn      MT_ReverseBytes
 *
 * @brief
 *
 *   Reverses bytes within an array
 *
 * @param   data - ptr to data buffer to reverse
 * @param    len - number of bytes in buffer
 *
 * @return  void
 ***************************************************************************************************/
void MT_ReverseBytes( byte *pData, byte len )
{
  byte i,j;
  byte temp;

  for ( i = 0, j = len-1; len > 1; len-=2 )
  {
    temp = pData[i];
    pData[i++] = pData[j];
    pData[j--] = temp;
  }
}


/***************************************************************************************************
 * @fn      MT_Word2Buf
 *
 * @brief   Copy a uint16_t array to a byte array, little endian.
 *
 * @param   pBuf - byte array
 * @param   pWord - uint16_t array
 * @param   len - length of uint16_t array
 *
 * @return  pointer to end of byte array
 ***************************************************************************************************/
uint8_t *MT_Word2Buf( uint8_t *pBuf, uint16_t *pWord, uint8_t len )
{
  while ( len-- > 0 )
  {
    *pBuf++ = LO_UINT16( *pWord );
    *pBuf++ = HI_UINT16( *pWord );
    pWord++;
  }

  return pBuf;
}

/***************************************************************************************************
 * @fn      MT_BuildEndpointDesc
 *
 * @brief   Build endpoint descriptor and simple descriptor structure from incoming buffer
 *
 * @param   pBuf - byte array
 *
 * @return  epDesc - pointer to the endpoint descriptor
 ***************************************************************************************************/
uint8_t MT_BuildEndpointDesc( uint8_t *pBuf, void *param )
{
  uint8_t i;
  uint8_t ret = ZSuccess;
  endPointDesc_t *epDesc;

  epDesc = (endPointDesc_t *)param;
  /* check if this endpoint is already registered */
  if ( afFindEndPointDesc( *pBuf ) != NULL )
  {
    ret = ZApsDuplicateEntry;
  }
  else if ( epDesc )
  {
    epDesc->endPoint = *pBuf;

    /* Ignore the latency reqs */
    epDesc->latencyReq = noLatencyReqs;

    /* allocate memory for the simple descriptor */
    epDesc->simpleDesc = (SimpleDescriptionFormat_t *) OsalPort_malloc(sizeof(SimpleDescriptionFormat_t));
    if (epDesc->simpleDesc)
    {
      /* Endpoint */
      epDesc->simpleDesc->EndPoint = *pBuf++;

      /* AppProfId */
      epDesc->simpleDesc->AppProfId = BUILD_UINT16(pBuf[0], pBuf[1]);
      pBuf += sizeof(uint16_t);

      /* AppDeviceId */
      epDesc->simpleDesc->AppDeviceId = BUILD_UINT16(pBuf[0],pBuf[1]);
      pBuf += sizeof(uint16_t);

      /* AppDevVer */
      epDesc->simpleDesc->AppDevVer = (*pBuf++) & AF_APP_DEV_VER_MASK ;

      /* LatencyReq */
      pBuf++;

      /* AppNumInClusters */
      epDesc->simpleDesc->AppNumInClusters = *pBuf++;
      if (epDesc->simpleDesc->AppNumInClusters)
      {
        epDesc->simpleDesc->pAppInClusterList = (uint16_t *)
                  OsalPort_malloc((epDesc->simpleDesc->AppNumInClusters)*sizeof(uint16_t));
        if ( epDesc->simpleDesc->pAppInClusterList )
        {
          for (i=0; i<(epDesc->simpleDesc->AppNumInClusters); i++)
          {
            epDesc->simpleDesc->pAppInClusterList[i] = BUILD_UINT16(*pBuf, *(pBuf+1));
            pBuf += 2;
          }
        }
        else
        {
          ret = ZMemError;
        }
      }

      /* AppNumOutClusters */
      epDesc->simpleDesc->AppNumOutClusters = *pBuf++;
      if (epDesc->simpleDesc->AppNumOutClusters)
      {
        epDesc->simpleDesc->pAppOutClusterList = (uint16_t *)
                          OsalPort_malloc((epDesc->simpleDesc->AppNumOutClusters)*sizeof(uint16_t));
        if (epDesc->simpleDesc->pAppOutClusterList)
        {
          for (i=0; i<(epDesc->simpleDesc->AppNumOutClusters); i++)
          {
            epDesc->simpleDesc->pAppOutClusterList[i] = BUILD_UINT16(*pBuf, *(pBuf+1));
            pBuf += 2;
          }
        }
        else
        {
          ret = ZMemError;
        }
      }

      /* if any list cannot be allocated...free all */
      if ( ret == ZMemError )
      {
        if (epDesc->simpleDesc->pAppInClusterList)
        {
          OsalPort_free(epDesc->simpleDesc->pAppInClusterList);
        }

        if (epDesc->simpleDesc->AppNumOutClusters)
        {
          OsalPort_free(epDesc->simpleDesc->pAppOutClusterList);
        }

        OsalPort_free(epDesc->simpleDesc);
      }
    }
    else
    {
      ret = ZMemError;
    }
  }

  return ret;
}

/***************************************************************************************************
***************************************************************************************************/
