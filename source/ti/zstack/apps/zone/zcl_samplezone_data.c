/**************************************************************************************************
  Filename:       zcl_samplefiredetector_data.c
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
#include "zcl_samplezone.h"
#include <string.h>
/*********************************************************************
 * CONSTANTS
 */

#define SAMPLEFIREDETECTOR_DEVICE_VERSION     1
#define SAMPLEFIREDETECTOR_FLAGS              0

#define SAMPLEFIREDETECTOR_HWVERSION          0
#define SAMPLEFIREDETECTOR_ZCLVERSION         BASIC_ZCL_VERSION

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
const uint16_t zclSampleZone_basic_clusterRevision = 0x0002;
const uint16_t zclSampleZone_identify_clusterRevision = 0x0001;
const uint16_t zclSampleZone_Zone_clusterRevision = 0x0001;

// Basic Cluster
const uint8_t zclSampleZone_HWRevision = SAMPLEFIREDETECTOR_HWVERSION;
const uint8_t zclSampleZone_ZCLVersion = SAMPLEFIREDETECTOR_ZCLVERSION;
const uint8_t zclSampleZone_ManufacturerName[] = { 16, 'T','e','x','a','s','I','n','s','t','r','u','m','e','n','t','s' };
const uint8_t zclSampleZone_PowerSource = POWER_SOURCE_MAINS_1_PHASE;
uint8_t zclSampleZone_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;

// Identify Cluster
uint16_t zclSampleZone_IdentifyTime = 0;

// IAS ZONE Cluster
//Zone information attributes
uint8_t zclSampleZone_ZoneState = SS_IAS_ZONE_STATE_NOT_ENROLLED;
uint16_t zclSampleZone_ZoneType = SS_IAS_ZONE_TYPE_FIRE_SENSOR;
uint16_t zclSampleZone_ZoneStatus = 0;

//Zone settings attributes
uint8_t zclSampleZone_CIE_Address[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t zclSampleZone_ZoneId = 0xFF;


/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */

  // NOTE: The attributes listed in the AttrRec must be in ascending order
  // per cluster to allow right function of the Foundation discovery commands

CONST zclAttrRec_t zclSampleZone_Attrs[] =
{
  // *** General Basic Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_ZCL_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleZone_ZCLVersion
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,             // Cluster IDs - defined in the foundation (ie. zcl.h)
    {  // Attribute record
      ATTRID_BASIC_HW_VERSION,            // Attribute ID - Found in Cluster Library header (ie. zcl_general.h)
      ZCL_DATATYPE_UINT8,                 // Data Type - found in zcl.h
      ACCESS_CONTROL_READ,                // Variable access control - found in zcl.h
      (void *)&zclSampleZone_HWRevision  // Pointer to attribute variable
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_MANUFACTURER_NAME,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclSampleZone_ManufacturerName
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_POWER_SOURCE,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleZone_PowerSource
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_PHYSICAL_ENVIRONMENT,
      ZCL_DATATYPE_ENUM8,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclSampleZone_PhysicalEnvironment
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleZone_basic_clusterRevision
    }
  },
  // *** Identify Cluster Attribute ***
  {
    ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
    { // Attribute record
      ATTRID_IDENTIFY_IDENTIFY_TIME,
      ZCL_DATATYPE_UINT16,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclSampleZone_IdentifyTime
    }
  },
  {
   ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleZone_identify_clusterRevision
    }
  },
  // *** Zone Cluster Attributes ***
    {
      ZCL_CLUSTER_ID_SS_IAS_ZONE,
      { // Attribute record
        ATTRID_IAS_ZONE_ZONE_STATE,
        ZCL_DATATYPE_ENUM8,
        ACCESS_CONTROL_READ,
        (void *)&zclSampleZone_ZoneState
      }
    },

     {
      ZCL_CLUSTER_ID_SS_IAS_ZONE,
      { // Attribute record
        ATTRID_IAS_ZONE_ZONE_TYPE,
        ZCL_DATATYPE_ENUM16,
        ACCESS_CONTROL_READ,
        (void *)&zclSampleZone_ZoneType
      }
    },

    {
      ZCL_CLUSTER_ID_SS_IAS_ZONE,
      { // Attribute record
        ATTRID_IAS_ZONE_ZONE_STATUS,
        ZCL_DATATYPE_BITMAP16,
        ACCESS_CONTROL_READ,
        (void *)&zclSampleZone_ZoneStatus
      }
    },

    {
      ZCL_CLUSTER_ID_SS_IAS_ZONE,
      { // Attribute record
        ATTRID_SS_IAS_CIE_ADDRESS,
        ZCL_DATATYPE_IEEE_ADDR,
        (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE | ACCESS_CONTROL_AUTH_WRITE),
        (void *)zclSampleZone_CIE_Address
      }
    },
    {
      ZCL_CLUSTER_ID_SS_IAS_ZONE,
      { // Attribute record
        ATTRID_IAS_ZONE_ZONE_ID,
        ZCL_DATATYPE_UINT8,
        ACCESS_CONTROL_READ,
        (void *)&zclSampleZone_ZoneId
      }
    },
    {
     ZCL_CLUSTER_ID_SS_IAS_ZONE,
      {  // Attribute record
        ATTRID_CLUSTER_REVISION,
        ZCL_DATATYPE_UINT16,
        ACCESS_CONTROL_READ,
        (void *)&zclSampleZone_Zone_clusterRevision
      }
    },
};

uint8_t CONST zclSampleZone_NumAttributes = ( sizeof(zclSampleZone_Attrs) / sizeof(zclSampleZone_Attrs[0]) );

/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
const cId_t zclSampleZone_InClusterList[] =
{
  ZCL_CLUSTER_ID_GENERAL_BASIC,
  ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
  ZCL_CLUSTER_ID_SS_IAS_ZONE
};

#define zclSampleZone_MAX_INCLUSTERS    ( sizeof( zclSampleZone_InClusterList ) / sizeof( zclSampleZone_InClusterList[0] ))

const cId_t zclSampleZone_OutClusterList[] =
{
  ZCL_CLUSTER_ID_GENERAL_IDENTIFY,    //Not mandated by device definition, but needed as this cluster IAS Zone device is F&B initiator
};

#define zclSampleZone_MAX_OUTCLUSTERS   ( sizeof( zclSampleZone_OutClusterList ) / sizeof( zclSampleZone_OutClusterList[0] ))

SimpleDescriptionFormat_t zclSampleZone_SimpleDesc =
{
  SAMPLEFIREDETECTOR_ENDPOINT,        //  int Endpoint;
  ZCL_HA_PROFILE_ID,                  //  uint16_t AppProfId[2];
  ZCL_DEVICEID_IAS_ZONE,//  uint16_t AppDeviceId[2];
  SAMPLEFIREDETECTOR_DEVICE_VERSION,            //  int   AppDevVer:4;
  SAMPLEFIREDETECTOR_FLAGS,                     //  int   AppFlags:4;
  zclSampleZone_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)zclSampleZone_InClusterList, //  byte *pAppInClusterList;
  zclSampleZone_MAX_OUTCLUSTERS,        //  byte  AppNumInClusters;
  (cId_t *)zclSampleZone_OutClusterList //  byte *pAppInClusterList;
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
void zclSampleZone_ResetAttributesToDefaultValues(void)
{
  zclSampleZone_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;

  zclSampleZone_IdentifyTime = DEFAULT_IDENTIFY_TIME;

  zclSampleZone_ZoneState = SS_IAS_ZONE_STATE_NOT_ENROLLED;
  zclSampleZone_ZoneType = SS_IAS_ZONE_TYPE_FIRE_SENSOR;
  zclSampleZone_ZoneStatus = 0;

  memset(zclSampleZone_CIE_Address,0xFF,8);
  zclSampleZone_ZoneId = 0xFF;


}

/****************************************************************************
****************************************************************************/


