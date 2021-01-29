/**************************************************************************************************
  Filename:       gpd.h
  Revised:        $Date: 2016-05-23 11:51:49 -0700 (Mon, 23 May 2016) $
  Revision:       $Revision: - $

  Description:    This file contains the Green Power Device functions.


  Copyright 2006-2014 Texas Instruments Incorporated. All rights reserved.

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

#ifndef GPD_H_
#define GPD_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "gpd_common.h"
#include "api_mac.h"
#include "zcomdef.h"
#include "nvintf.h"
#include "ti_zstack_config.h"

#include <ti/drivers/AESCCM.h>
#include <ti/drivers/cryptoutils/cryptokey/CryptoKeyPlaintext.h>

/******************************************************************************
 * DEFINES
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */
extern ApiMac_sAddrExt_t ApiMac_extAddr;
extern AESCCM_Handle ZAESCCM_handle;

/******************************************************************************
 * TYPEDEFS
 */
typedef struct reportDescriptor
{
    uint8_t  reportIdentifier;
    uint8_t  reportOptions;
    uint16_t timeoutPeriod;
    uint8_t  remainingLengthReportDescriptor;
    uint8_t  dataPointOptions;
    uint16_t ClusterID;
    uint16_t manufacturerID;
    uint16_t attributeID;
    uint8_t  attributeDataType;
    uint8_t  attributeOptions;
    uint8_t  attributeOffsetWithinReport;
    uint8_t  *attributeValue;
    struct reportDescriptor *pNext;
}reportDescriptor_t;

/*              */
typedef struct
{
    uint8_t totalNumbersReports;
    uint8_t numberReport;
    uint8_t *reportDescriptor;
}applicationDescription_t;

/*              */
typedef struct
{
    uint8_t        AppID;
    union
    {
        uint32_t   SrcID;
        uint8_t   *GPDExtAddr;
    }GPDId;
}gpd_ID_t;

/*              */
typedef struct
{
    uint8_t  gpdDeviceID;
    uint8_t  options;
#if ( OPT_EXTENDED_OPTIONS == TRUE )
    uint8_t  extendedOptions;
#endif
#if (EXOPT_GPD_KEY_PRESENT == TRUE)
    uint8_t  GPDKey[ GP_KEY_LENGTH ];
#if (EXOPT_KEY_ENCRYPTION == TRUE)
    uint32_t GPDKeyMIC;
#endif  // EXOPT_KEY_ENCRYPTION
    uint32_t GPDOutgoingCounter;
#endif  // EXOPT_GPD_KEY_PRESENT
#if (OPT_APPLICATION_INFORMATION == TRUE)
    uint8_t  applicationInformation;
#endif
    uint16_t manufacturerID;
    uint16_t modelID;
    uint8_t  numberGPDCommands;
    uint8_t  GPDCommandIDList;
    uint8_t  clusterList;
    uint8_t  switchInformation;
}gpdCommissioningCommand_t;

/*              */
typedef struct
{
    uint8_t     NWKFrameControl;
    uint8_t     NWKExtFC;
    gpd_ID_t  gpdId;
#if (GPD_APP_ID == GPD_APP_TYPE_IEEE_ID)
    uint8_t     endPoint;
#endif
#if (GP_SECURITY_LEVEL == GP_SECURITY_LVL_4FC_4MIC) || (GP_SECURITY_LEVEL == GP_SECURITY_LVL_4FC_4MIC_ENCRYPT)
    uint32_t    securityFrameCounter;
#endif
    uint8_t     gpdCmdID;
    uint8_t     payloadSize;
    uint8_t     *payload;
}gpdfReq_t;


// Attribute Report
typedef struct
{
  uint16_t attrID;             // atrribute ID
  uint8_t  dataType;           // attribute data type
  uint8_t  *attrData;          // this structure is allocated, so the data is HERE
                             // - the size depends on the data type of attrID
} gpReport_t;



/******************************************************************************
 * PUBLIC FUNCTIONS
 */

/*
* @brief   Initialize AESCCM for green power device
*/
uint8_t gpdAESCCMInit(void);

/*
 * @brief   Send GPDF on requested channel
 */
extern uint8_t GreenPowerDataFrameSend( gpdfReq_t *pReq, uint8_t channel, bool secure);
#if defined ( GPD_COMMISSIONING_CMD_SUPPORT )
extern uint8_t GreenPowerCommissioningSend( gpdfReq_t *pCommissioningGpdf, uint8_t channel);
#endif
extern uint8_t GreenPowerAttributeReportingSend(uint16_t clusterID, uint8_t numAttr, gpdfReq_t *attrReportGpdf, uint8_t channel );
extern void gpdDuplicateFrameInit(ApiMac_mcpsDataReq_t *pDataReq);
#if (GP_SECURITY_LEVEL == GP_SECURITY_LVL_4FC_4MIC) || (GP_SECURITY_LEVEL == GP_SECURITY_LVL_4FC_4MIC_ENCRYPT)
extern void gpd_setSecurityFrameCounter(uint32_t frameCounter);
#endif
#ifdef __cplusplus
}
#endif

#endif /* GPD_H_ */
