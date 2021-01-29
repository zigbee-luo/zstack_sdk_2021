/**************************************************************************************************
  Filename:       gpd_config.h
  Revised:        $Date: 2011-05-05 13:52:19 -0700 (Thu, 05 May 2011) $
  Revision:       $Revision: 25878 $

  Description:    This file contains the configuration parameters for a Green Power Device.


  Copyright 2005-2011 Texas Instruments Incorporated. All rights reserved.

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

#ifndef GPD_CONFIG_H
#define GPD_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 * INCLUDES
 */
#include "gpd.h"
#include "ti_zstack_config.h"

/*
 * GPD CONFIGURATION
 */
#define TC_LINK_KEY          {0x5A, 0x69, 0x67, 0x42, 0x65, 0x65, 0x41, 0x6C, 0x6C, 0x69, 0x61, 0x6E, 0x63, 0x65, 0x30, 0x39}

/*
 * GPDF SETTINGS
 */
#if ( (GP_SECURITY_LEVEL == GP_SECURITY_LVL_4FC_4MIC) || (GP_SECURITY_LEVEL == GP_SECURITY_LVL_4FC_4MIC_ENCRYPT) || (GPD_APP_ID == GPD_APP_TYPE_IEEE_ID) )
#define FRAME_CTRL_EXT             TRUE //Default = TRUE
#elif (GP_SECURITY_LEVEL == GP_SECURITY_LVL_NO_SEC)
#define FRAME_CTRL_EXT             FALSE
#endif

/* GPDF OPTIONS */
#define GPD_FRAME_CONTROL        (uint8_t)(((GP_DATA_FRAME)&0x03) | (((GP_NWK_PROTOCOL_VERSION)&0x0F)<<2) | (((AUTO_COMMISSIONING)&0x01)<<6)   | (((FRAME_CTRL_EXT)&0x01)<<7))

/* GPDF EXTENDED OPTIONS */
#if ( (EXT_OPT_KEY_TYPE == KEY_TYPE_NO_KEY ) || (EXT_OPT_KEY_TYPE == KEY_TYPE_ZIGBEE_NWK_KEY ) || (EXT_OPT_KEY_TYPE == KEY_TYPE_GPD_GROUP_KEY ) || (EXT_OPT_KEY_TYPE == KEY_TYPE_NWK_KEY_DERIVED_GPD_GROUP_KEY ) )
  #define EXT_OPT_SECURITY_KEY  0
#elif( (EXT_OPT_KEY_TYPE == KEY_TYPE_OUT_BOX_GPD_KEY ) || (EXT_OPT_KEY_TYPE == KEY_TYPE_DERIVED_INDIVIDUAL_GPD_KEY ) )
  #define EXT_OPT_SECURITY_KEY  1
#endif

#define GPD_EXT_FRAME_CONTROL                 (uint8_t)(((GPD_APP_ID)&0x07) | (((GP_SECURITY_LEVEL)&0x03)<<3) | (((EXT_OPT_SECURITY_KEY)&0x01)<<5) | (((RX_AFTER_TX)&0x01)<<6))
#define GPD_UNSECURED_EXT_FRAME_CONTROL       (uint8_t)(((GPD_APP_ID)&0x07) | (((GP_SECURITY_LVL_NO_SEC)&0x03)<<3) | (((EXT_OPT_SECURITY_KEY)&0x01)<<5) | (((RX_AFTER_TX)&0x01)<<6))

#ifdef __cplusplus
}
#endif

#endif /* GPD_CONFIG_H */
