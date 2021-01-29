/**************************************************************************************************
  Filename:       zcl_samplesw_data.c
  Revised:        $Date: 2014-07-30 12:57:37 -0700 (Wed, 30 Jul 2014) $
  Revision:       $Revision: 39591 $


  Description:    Zigbee Cluster Library - sample device application.


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

/*********************************************************************
 * INCLUDES
 */
#include "zcomdef.h"
#include "zcl.h"
#include "zcl_general.h"
#include "zcl_ha.h"
#include "zcl_ss.h"
#include "zcl_poll_control.h"
#include "zcl_electrical_measurement.h"
#include "zcl_diagnostic.h"
#include "zcl_meter_identification.h"
#include "zcl_appliance_identification.h"
#include "zcl_appliance_events_alerts.h"
#include "zcl_power_profile.h"
#include "zcl_appliance_control.h"
#include "zcl_appliance_statistics.h"
#include "zcl_hvac.h"
#include "zcl_port.h"
#include "zcl_samplewarningdevice.h"
#include <string.h>
/*********************************************************************
 * CONSTANTS
 */

#define SAMPLEWARNINGDEVICE_DEVICE_VERSION     1
#define SAMPLEWARNINGDEVICE_FLAGS              0

#define SAMPLEWARNINGDEVICE_HWVERSION          0
#define SAMPLEWARNINGDEVICE_ZCLVERSION         0

#define DEFAULT_IDENTIFY_TIME 0

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Global attributes
const uint16_t zclSampleWarningDevice_basic_clusterRevision = 0x0002;
const uint16_t zclSampleWarningDevice_identify_clusterRevision = 0x0001;
const uint16_t zclSampleWarningDevice_iasZone_clusterRevision = 0x0001;
const uint16_t zclSampleWarningDevice_iasWd_clusterRevision = 0x0001;

// Basic Cluster
const uint8_t zclSampleWarningDevice_HWRevision = SAMPLEWARNINGDEVICE_HWVERSION;
const uint8_t zclSampleWarningDevice_ZCLVersion = SAMPLEWARNINGDEVICE_ZCLVERSION;
const uint8_t zclSampleWarningDevice_ManufacturerName[] = { 16, 'T','e','x','a','s','I','n','s','t','r','u','m','e','n','t','s' };
const uint8_t zclSampleWarningDevice_PowerSource = POWER_SOURCE_MAINS_1_PHASE;
uint8_t zclSampleWarningDevice_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;

// Identify Cluster
uint16_t zclSampleWarningDevice_IdentifyTime = 0;

uint16_t zclSampleWarningDevice_MaxDuration = SAMPLEWARNINGDEVICE_ALARM_MAX_DURATION;

// IAS ZONE Cluster
//Zone information attributes
uint8_t  zclSampleWarningDevice_ZoneState = SS_IAS_ZONE_STATE_NOT_ENROLLED;
uint16_t zclSampleWarningDevice_ZoneType = SS_IAS_ZONE_TYPE_FIRE_SENSOR;
uint16_t zclSampleWarningDevice_ZoneStatus = 0;

//Zone settings attributes
uint8_t zclSampleWarningDevice_CIE_Address[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t zclSampleWarningDevice_ZoneId = 0xFF;



/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */

  // NOTE: The attributes listed in the AttrRec must be in ascending order
  // per cluster to allow right function of the Foundation discovery commands

CONST zclAttrRec_t zclSampleWarningDevice_Attrs[] =
{
  // *** General Basic Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_ZCL_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_ZCLVersion
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,             // Cluster IDs - defined in the foundation (ie. zcl.h)
    {  // Attribute record
      ATTRID_BASIC_HW_VERSION,            // Attribute ID - Found in Cluster Library header (ie. zcl_general.h)
      ZCL_DATATYPE_UINT8,                 // Data Type - found in zcl.h
      ACCESS_CONTROL_READ,                // Variable access control - found in zcl.h
      (void *)&zclSampleWarningDevice_HWRevision  // Pointer to attribute variable
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_MANUFACTURER_NAME,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclSampleWarningDevice_ManufacturerName
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_POWER_SOURCE,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_PowerSource
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_PHYSICAL_ENVIRONMENT,
      ZCL_DATATYPE_ENUM8,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclSampleWarningDevice_PhysicalEnvironment
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_basic_clusterRevision
    }
  },

  // *** Identify Cluster Attribute ***
  {
    ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
    { // Attribute record
      ATTRID_IDENTIFY_IDENTIFY_TIME,
      ZCL_DATATYPE_UINT16,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclSampleWarningDevice_IdentifyTime
    }
  },
  {
   ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_identify_clusterRevision
    }
  },
  // *** WD Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_SS_IAS_WD,
    { // Attribute record
      ATTRID_IASWD_MAX_DURATION,
      ZCL_DATATYPE_UINT16,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclSampleWarningDevice_MaxDuration
    }
  },
  {
   ZCL_CLUSTER_ID_SS_IAS_WD,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_iasWd_clusterRevision
    }
  },
  // *** Zone Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_SS_IAS_ZONE,
    { // Attribute record
      ATTRID_IAS_ZONE_ZONE_STATE,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_ZoneState
    }
  },
  {
    ZCL_CLUSTER_ID_SS_IAS_ZONE,
    { // Attribute record
      ATTRID_IAS_ZONE_ZONE_TYPE,
      ZCL_DATATYPE_ENUM16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_ZoneType
    }
  },
  {
    ZCL_CLUSTER_ID_SS_IAS_ZONE,
    { // Attribute record
      ATTRID_IAS_ZONE_ZONE_STATUS,
      ZCL_DATATYPE_BITMAP16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_ZoneStatus
    }
  },
  {
    ZCL_CLUSTER_ID_SS_IAS_ZONE,
    { // Attribute record
      ATTRID_SS_IAS_CIE_ADDRESS,
      ZCL_DATATYPE_IEEE_ADDR,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE | ACCESS_CONTROL_AUTH_WRITE),
      (void *)zclSampleWarningDevice_CIE_Address
    }
  },
  {
    ZCL_CLUSTER_ID_SS_IAS_ZONE,
    { // Attribute record
      ATTRID_IAS_ZONE_ZONE_ID,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_ZoneId
    }
  },
  {
   ZCL_CLUSTER_ID_SS_IAS_ZONE,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleWarningDevice_iasZone_clusterRevision
    }
  },

};

uint8_t CONST zclSampleWarningDevice_NumAttributes = ( sizeof(zclSampleWarningDevice_Attrs) / sizeof(zclSampleWarningDevice_Attrs[0]) );

/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
const cId_t zclSampleWarningDevice_InClusterList[] =
{
  ZCL_CLUSTER_ID_GENERAL_BASIC,
  ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
  ZCL_CLUSTER_ID_SS_IAS_WD,
  ZCL_CLUSTER_ID_SS_IAS_ZONE
};

#define ZCLSAMPLEWARNINGDEVICE_MAX_INCLUSTERS    ( sizeof( zclSampleWarningDevice_InClusterList ) / sizeof( zclSampleWarningDevice_InClusterList[0] ))


SimpleDescriptionFormat_t zclSampleWarningDevice_SimpleDesc =
{
  SAMPLEWARNINGDEVICE_ENDPOINT,                  //  int Endpoint;
  ZCL_HA_PROFILE_ID,                  //  uint16_t AppProfId[2];
  ZCL_DEVICEID_IAS_WARNING,//  uint16_t AppDeviceId[2];
  SAMPLEWARNINGDEVICE_DEVICE_VERSION,            //  int   AppDevVer:4;
  SAMPLEWARNINGDEVICE_FLAGS,                     //  int   AppFlags:4;
  ZCLSAMPLEWARNINGDEVICE_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)zclSampleWarningDevice_InClusterList, //  byte *pAppInClusterList;
  0,                                             //  byte  AppNumInClusters;
  (cId_t *)NULL, //  byte *pAppInClusterList;
};

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      zclSampleLight_ResetAttributesToDefaultValues
 *
 * @brief   Reset all writable attributes to their default values.
 *
 * @param   none
 *
 * @return  none
 */
void zclSampleWarningDevice_ResetAttributesToDefaultValues(void)
{

    zclSampleWarningDevice_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;

    zclSampleWarningDevice_IdentifyTime = DEFAULT_IDENTIFY_TIME;

    zclSampleWarningDevice_MaxDuration = SAMPLEWARNINGDEVICE_ALARM_MAX_DURATION;

    zclSampleWarningDevice_ZoneState = SS_IAS_ZONE_STATE_NOT_ENROLLED;
    zclSampleWarningDevice_ZoneType = SS_IAS_ZONE_TYPE_FIRE_SENSOR;
    zclSampleWarningDevice_ZoneStatus = 0;

    memset(zclSampleWarningDevice_CIE_Address,0xFF,8);
    zclSampleWarningDevice_ZoneId = 0xFF;


}

/****************************************************************************
****************************************************************************/


