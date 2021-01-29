/***************************************************************************************************
  Filename:       MT_GP.c
  Revised:        $Date: 2016-06-21 01:06:52 -0700 (Thu, 21 July 2016) $
  Revision:       $Revision:  $

  Description:    MonitorTest functions GP interface.

  Copyright 2007-2013 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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

 ***************************************************************************************************/

/***************************************************************************************************
 * INCLUDES
 ***************************************************************************************************/


#include "zcomdef.h"
#include "mt.h"
#include "mt_gp.h"


/***************************************************************************************************
* CONST
***************************************************************************************************/
#define GP_DATA_REQ_PAYLOAD_LEN_POS   17
#define GP_DATA_REQ_APP_ID_POS         2

#define SEC_KEY_LEN                   16

/***************************************************************************************************
* LOCAL FUNCTIONs
***************************************************************************************************/

#if defined (MT_GP_CB_FUNC)
static void MT_GpDataReq(uint8_t* pBuf);
static void MT_GpSecRsp(uint8_t* pBuf);

void MT_GPDataCnf(gp_DataCnf_t* gp_DataCnf);
void MT_GPSecReq(gp_SecReq_t* gp_SecReq);
void MT_GPDataInd(gp_DataInd_t* gp_DataInd);



/***************************************************************************************************
* External variables
***************************************************************************************************/


uint8_t MT_GpCommandProcessing(uint8_t *pBuf)
{
  uint8_t status = MT_RPC_SUCCESS;

  switch (pBuf[MT_RPC_POS_CMD1])
  {
//GP_UPDATE
    case MT_GP_DATA_REQ:
      MT_GpDataReq(pBuf);
    break;
    case MT_GP_SEC_RSP:
      MT_GpSecRsp(pBuf);
    break;


  }
  return status;
}


 /***************************************************************************************************
 * @fn      MT_GpDataReq
 *
 * @brief   GP data request interface
 *
 * @param   pBuf - gp data request
 *
 * @return  void
 ***************************************************************************************************/
 static void MT_GpDataReq(uint8_t* pBuf)
{
  uint8_t retValue = ZSuccess;
  uint8_t cmdId;
  gp_DataReq_t *gp_DataReq;
  uint8_t  payloadLen;


  /* parse header */
  cmdId = pBuf[MT_RPC_POS_CMD1];
  pBuf += MT_RPC_FRAME_HDR_SZ;

  //Get the payload length
  payloadLen = pBuf[GP_DATA_REQ_PAYLOAD_LEN_POS];

  gp_DataReq = (gp_DataReq_t*)OsalPort_msgAllocate(sizeof(gp_DataReq_t) + payloadLen);

  //No memory
  if(gp_DataReq == NULL)
  {
    retValue = FAILURE;
  }
  //Invalid application ID
  if( (pBuf[GP_DATA_REQ_APP_ID_POS] != GP_APP_ID_GPID) && (pBuf[GP_DATA_REQ_APP_ID_POS] != GP_APP_ID_IEEE ) )
  {
    retValue = INVALIDPARAMETER;
  }
  //Return fail/InvalidParameter
  if(retValue)
  {
    /* Build and send back the response */
    MT_BuildAndSendZToolResponse(((uint8_t)MT_RPC_CMD_SRSP | (uint8_t)MT_RPC_SYS_GP), cmdId, 1, &retValue);
    return;
  }


  gp_DataReq->Action = *pBuf++;
  gp_DataReq->TxOptions = *pBuf++;
  gp_DataReq->gpdID.appID = *pBuf++;
  if(gp_DataReq->gpdID.appID == GP_APP_ID_GPID)
  {
    gp_DataReq->gpdID.id.srcID = OsalPort_buildUint32(pBuf,4);
    pBuf += sizeof(uint32_t) + Z_EXTADDR_LEN;
  }
  else
  {
    pBuf += sizeof(uint32_t);
    OsalPort_memcpy(gp_DataReq->gpdID.id.gpdExtAddr,pBuf,Z_EXTADDR_LEN);
    pBuf += Z_EXTADDR_LEN;
  }

  gp_DataReq->EndPoint = *pBuf++;
  gp_DataReq->GPDCmmdId = *pBuf++;
  gp_DataReq->GPDasduLength = *pBuf++;
  OsalPort_memcpy(gp_DataReq->GPDasdu,pBuf,payloadLen);
  pBuf += payloadLen;
  gp_DataReq->GPEPhandle = *pBuf++;
  gp_DataReq->gpTxQueueEntryLifeTime[2] = *pBuf++;
  gp_DataReq->gpTxQueueEntryLifeTime[1] = *pBuf++;
  gp_DataReq->gpTxQueueEntryLifeTime[0] = *pBuf++;

  gp_DataReq->hdr.event = GP_DATA_REQ;
  gp_DataReq->hdr.status = 0;

  OsalPort_msgSend(gp_TaskID,(uint8_t*)gp_DataReq);

  /* Build and send back the response */
  MT_BuildAndSendZToolResponse(((uint8_t)MT_RPC_CMD_SRSP | (uint8_t)MT_RPC_SYS_GP), cmdId, 1, &retValue);
}



/***************************************************************************************************
* @fn      MT_GpSecRsp
*
* @brief   GP Sec Response interface
*
* @param   pBuf - gp sec rsp
*
* @return  void
***************************************************************************************************/

void MT_GpSecRsp(uint8_t *pBuf)
{

  uint8_t retValue = ZSuccess;
  uint8_t cmdId;
  gp_SecRsp_t *gp_SecRsp;

  /* parse header */
  cmdId = pBuf[MT_RPC_POS_CMD1];
  pBuf += MT_RPC_FRAME_HDR_SZ;

  gp_SecRsp = (gp_SecRsp_t*)OsalPort_msgAllocate(sizeof(gp_SecRsp_t));

    //No memory
  if(gp_SecRsp == NULL)
  {
    retValue = FAILURE;
  }
  //Invalid application ID
  if( (pBuf[GP_DATA_REQ_APP_ID_POS] != GP_APP_ID_GPID) && (pBuf[GP_DATA_REQ_APP_ID_POS] != GP_APP_ID_IEEE ) )
  {
    retValue = INVALIDPARAMETER;
  }
  //Return fail/InvalidParameter
  if(retValue)
  {
    /* Build and send back the response */
    MT_BuildAndSendZToolResponse(((uint8_t)MT_RPC_CMD_SRSP | (uint8_t)MT_RPC_SYS_GP), cmdId, 1, &retValue);
    return;
  }

  gp_SecRsp->Status = *pBuf++;
  gp_SecRsp->dGPStubHandle = *pBuf++;
  gp_SecRsp->gpdID.appID = *pBuf++;
  if(gp_SecRsp->gpdID.appID == GP_APP_ID_GPID)
  {
    gp_SecRsp->gpdID.id.srcID = OsalPort_buildUint32(pBuf,4);
    pBuf += sizeof(uint32_t) + Z_EXTADDR_LEN;
  }
  else
  {
    pBuf += sizeof(uint32_t);
    OsalPort_memcpy(gp_SecRsp->gpdID.id.gpdExtAddr,pBuf,Z_EXTADDR_LEN);
    pBuf += Z_EXTADDR_LEN;
  }
  gp_SecRsp->EndPoint = *pBuf++;
  gp_SecRsp->gp_SecData.GPDFSecLvl = *pBuf++;
  gp_SecRsp->gp_SecData.GPDFKeyType = *pBuf++;
  OsalPort_memcpy(gp_SecRsp->GPDKey,pBuf,SEC_KEY_LEN);
  pBuf += SEC_KEY_LEN;
  gp_SecRsp->gp_SecData.GPDSecFrameCounter = OsalPort_buildUint32(pBuf,4);

  gp_SecRsp->hdr.event = GP_SEC_RSP;
  gp_SecRsp->hdr.status = 0;

  OsalPort_msgSend(gp_TaskID,(uint8_t*)gp_SecRsp);

  /* Build and send back the response */
  MT_BuildAndSendZToolResponse(((uint8_t)MT_RPC_CMD_SRSP | (uint8_t)MT_RPC_SYS_GP), cmdId, 1, &retValue);
}

/***************************************************************************************************
* @fn      MT_GPDataInd
*
* @brief   Send GP Data Ind to Host Processor
*
* @param   gp_DataInd
*
* @return  void
***************************************************************************************************/
void MT_GPDataInd(gp_DataInd_t* gp_DataInd)
{
  uint8_t *pBuf = NULL;
  uint8_t *tempBuf = NULL;
  uint8_t bufLen = sizeof(gp_DataInd_t) + gp_DataInd->GPDasduLength - 1;

  pBuf = OsalPort_malloc(bufLen);

  if(pBuf == NULL)
  {
    return;
  }

  tempBuf = pBuf;

  *pBuf++ = gp_DataInd->status;
  *pBuf++ = gp_DataInd->Rssi;
  *pBuf++ = gp_DataInd->LinkQuality;
  *pBuf++ = gp_DataInd->SeqNumber;
  *pBuf++ = gp_DataInd->srcAddr.addrMode;
  *pBuf++ = LO_UINT16(gp_DataInd->srcPanID);
  *pBuf++ = HI_UINT16(gp_DataInd->srcPanID);

   pBuf   = osal_cpyExtAddr( pBuf,gp_DataInd->srcAddr.addr.extAddr);
  *pBuf++ = gp_DataInd->frameType;
  *pBuf++ = gp_DataInd->appID;
  *pBuf++ = gp_DataInd->GPDFSecLvl;
  *pBuf++ = gp_DataInd->GPDFKeyType;
  *pBuf++ = gp_DataInd->AutoCommissioning;
  *pBuf++ = gp_DataInd->RxAfterTx;

  UINT32_TO_BUF_LITTLE_ENDIAN(pBuf,gp_DataInd->SrcId);
  *pBuf++ = gp_DataInd->EndPoint;
  UINT32_TO_BUF_LITTLE_ENDIAN(pBuf,gp_DataInd->GPDSecFrameCounter);
  *pBuf++ = gp_DataInd->GPDCmmdID;
  *pBuf++ = gp_DataInd->GPDasduLength;
  OsalPort_memcpy(pBuf,gp_DataInd->GPDasdu,gp_DataInd->GPDasduLength);
  pBuf += gp_DataInd->GPDasduLength;
  UINT32_TO_BUF_LITTLE_ENDIAN(pBuf,gp_DataInd->MIC);

  MT_BuildAndSendZToolResponse(((uint8_t)MT_RPC_CMD_AREQ | (uint8_t)MT_RPC_SYS_GP), MT_GP_DATA_IND, 37 + gp_DataInd->GPDasduLength - 1, tempBuf);

  OsalPort_free(tempBuf);

}




/***************************************************************************************************
* @fn      MT_GPDataCnf
*
* @brief   Send GP Data Cnf to Host Processor
*
* @param   gp_DataCnf
*
* @return  void
***************************************************************************************************/
void MT_GPDataCnf(gp_DataCnf_t* gp_DataCnf)
{
  uint8_t buf[2];
  uint8_t *pBuf = buf;

  *pBuf++ = gp_DataCnf->status;
  *pBuf++ = gp_DataCnf->GPEPhandle;

  MT_BuildAndSendZToolResponse(((uint8_t)MT_RPC_CMD_AREQ | (uint8_t)MT_RPC_SYS_GP), MT_GP_DATA_CNF, 2, buf);
}

/***************************************************************************************************
* @fn      MT_GPSecReq
*
* @brief   Send GP Sec Req to Host Processor
*
* @param   gp_SecReq
*
* @return  void
***************************************************************************************************/
void MT_GPSecReq(gp_SecReq_t* gp_SecReq)
{
  uint8_t buf[21];
  uint8_t *pBuf = buf;


  *pBuf++ = gp_SecReq->gpdID.appID;

  UINT32_TO_BUF_LITTLE_ENDIAN(pBuf,gp_SecReq->gpdID.id.srcID);
  pBuf   = osal_cpyExtAddr( pBuf,gp_SecReq->gpdID.id.gpdExtAddr);
  *pBuf++ = gp_SecReq->EndPoint;
  *pBuf++ = gp_SecReq->gp_SecData.GPDFSecLvl;
  *pBuf++ = gp_SecReq->gp_SecData.GPDFKeyType;

  UINT32_TO_BUF_LITTLE_ENDIAN(pBuf,gp_SecReq->gp_SecData.GPDSecFrameCounter);

  *pBuf++ = gp_SecReq->dGPStubHandle;

  MT_BuildAndSendZToolResponse(((uint8_t)MT_RPC_CMD_AREQ | (uint8_t)MT_RPC_SYS_GP), MT_GP_SEC_REQ, 21, buf);
}



#endif

