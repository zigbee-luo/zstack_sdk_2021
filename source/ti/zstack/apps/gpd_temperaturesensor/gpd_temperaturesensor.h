/**************************************************************************************************
  Filename:       gpd_temperaturesensor.h
  Revised:        $Date: 2014-06-19 08:38:22 -0700 (Thu, 19 Jun 2014) $
  Revision:       $Revision: 39101 $

  Description:    This file contains the Green Power Library Home
                  Automation Temperature Sensor Application.


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

#ifndef GPD_SAMPLESWITCH_H
#define GPD_SAMPLESWITCH_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "gpd.h"
#include "nvintf.h"
#include "zcomdef.h"

/*********************************************************************
 * CONSTANTS
 */

#define SAMPLEAPP_REPORT_TEMP_EVT             0x0001
#define SAMPLEAPP_KEY_EVT                     0x0002
#define SAMPLEAPP_LONG_KEY_EVT                0x0004


//Time after which a report will be generated
#define GPD_REPORT_TEMP_DELAY                 10000
#define GPD_LONG_PRESS_KEY_TICK               100
#define GPD_NUMBER_OF_TICKS                    10
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * VARIABLES
 */
extern gpdfReq_t   commissioningReq;
extern gpdfReq_t   appDescriptionReq;
extern gpdfReq_t   attributeReportCmd;
extern gpReport_t  attributeReport;
extern uint16_t      gpd_attr_CurrentTemperature;
extern const uint8_t frameDuplicates;
extern const uint8_t gpdDeviceId;
extern const uint8_t gpdChannel;

/*********************************************************************
 * FUNCTIONS
 */

/*
 *  Application task entry point for the ZStack Green Power Device Temperature Sensor Application
 */
extern void gpdSampleTempSensor_task(NVINTF_nvFuncts_t *pfnNV);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* GPD_SAMPLESWITCH_H */
