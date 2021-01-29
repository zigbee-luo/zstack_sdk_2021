/**************************************************************************************************
  Filename:       zcl_samplethermostat_data.c
  Revised:        $Date: 2014-09-25 13:20:41 -0700 (Thu, 25 Sep 2014) $
  Revision:       $Revision: 40295 $


  Description:    Zigbee Cluster Library - sample device application.


  Copyright 2013-2014 Texas Instruments Incorporated. All rights reserved.

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

#include "zcl_samplethermostat.h"

/*********************************************************************
 * CONSTANTS
 */

#define SAMPLETHERMOSTAT_DEVICE_VERSION     0
#define SAMPLETHERMOSTAT_FLAGS              0

#define SAMPLETHERMOSTAT_HWVERSION          1
#define SAMPLETHERMOSTAT_ZCLVERSION         BASIC_ZCL_VERSION

#define SAMPLETHERMOSTAT_ABS_MIN_HEAT_SETPOINT_LIMIT     1700  // 17.00C
#define SAMPLETHERMOSTAT_ABS_MAX_HEAT_SETPOINT_LIMIT     2700  // 27.00C
#define SAMPLETHERMOSTAT_ABS_MIN_COOL_SETPOINT_LIMIT     1700  // 17.00C
#define SAMPLETHERMOSTAT_ABS_MAX_COOL_SETPOINT_LIMIT     2700  // 27.00C
#define SAMPLETHERMOSTAT_OCCUPIED_HEATING_SETPOINT   2000  // 20.00C
#define SAMPLETHERMOSTAT_OCCUPIED_COOLING_SETPOINT   2400  // 24.00C

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
const uint16_t zclSampleThermostat_basic_clusterRevision = 0x0002;
const uint16_t zclSampleThermostat_identify_clusterRevision = 0x0001;
const uint16_t zclSampleThermostat_temperaturems_clusterRevision = 0x0001;
const uint16_t zclSampleThermostat_thermostat_clusterRevision = 0x0001;

// Basic Cluster
const uint8_t zclSampleThermostat_HWRevision = SAMPLETHERMOSTAT_HWVERSION;
const uint8_t zclSampleThermostat_ZCLVersion = SAMPLETHERMOSTAT_ZCLVERSION;
const uint8_t zclSampleThermostat_ManufacturerName[] = { 16, 'T','e','x','a','s','I','n','s','t','r','u','m','e','n','t','s' };
const uint8_t zclSampleThermostat_PowerSource = POWER_SOURCE_MAINS_1_PHASE;
uint8_t zclSampleThermostat_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;

// Identify Cluster
uint16_t zclSampleThermostat_IdentifyTime = 0;

// HVAC Thermostat Cluster
int16_t zclSampleThermostat_LocalTemperature = ATTR_INVALID_MEASUREMENT_HVAC_THERMOSTAT_LOCAL_TEMPERATURE;
int16_t zclSampleThermostat_AbsMinHeatSetpointLimit;
int16_t zclSampleThermostat_AbsMaxHeatSetpointLimit;
int16_t zclSampleThermostat_AbsMinCoolSetpointLimit;
int16_t zclSampleThermostat_AbsMaxCoolSetpointLimit;
int16_t zclSampleThermostat_OccupiedHeatingSetpoint;
int16_t zclSampleThermostat_OccupiedCoolingSetpoint;



uint8_t zclSampleThermostat_ControlSequenceOfOperation;
uint8_t zclSampleThermostat_SystemMode;

/*********************************************************************
 * ATTRIBUTE DEFINITIONS - Uses REAL cluster IDs
 */

// NOTE: The attributes listed in the AttrRec must be in ascending order
// per cluster to allow right function of the Foundation discovery commands

CONST zclAttrRec_t zclSampleThermostat_Attrs[] =
{
  // *** General Basic Cluster Attributes ***
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_ZCL_VERSION,
      ZCL_DATATYPE_UINT8,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleThermostat_ZCLVersion
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,             // Cluster IDs - defined in the foundation (ie. zcl.h)
    {  // Attribute record
      ATTRID_BASIC_HW_VERSION,            // Attribute ID - Found in Cluster Library header (ie. zcl_general.h)
      ZCL_DATATYPE_UINT8,                 // Data Type - found in zcl.h
      ACCESS_CONTROL_READ,                // Variable access control - found in zcl.h
      (void *)&zclSampleThermostat_HWRevision  // Pointer to attribute variable
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_MANUFACTURER_NAME,
      ZCL_DATATYPE_CHAR_STR,
      ACCESS_CONTROL_READ,
      (void *)zclSampleThermostat_ManufacturerName
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_POWER_SOURCE,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleThermostat_PowerSource
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    { // Attribute record
      ATTRID_BASIC_PHYSICAL_ENVIRONMENT,
      ZCL_DATATYPE_ENUM8,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclSampleThermostat_PhysicalEnvironment
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_BASIC,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleThermostat_basic_clusterRevision
    }
  },
  // *** Identify Cluster Attribute ***
  {
    ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
    { // Attribute record
      ATTRID_IDENTIFY_IDENTIFY_TIME,
      ZCL_DATATYPE_UINT16,
      (ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE),
      (void *)&zclSampleThermostat_IdentifyTime
    }
  },
  {
    ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ | ACCESS_GLOBAL,
      (void *)&zclSampleThermostat_identify_clusterRevision
    }
  },
  // *** Temperature measurement Attributes *** //
  {
    ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ | ACCESS_CLIENT,
      (void *)&zclSampleThermostat_temperaturems_clusterRevision
    }
  },
  // *** HVAC Thermostat Cluster Attributes *** //

  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_LOCAL_TEMPERATURE,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ | ACCESS_REPORTABLE,
      (void *)&zclSampleThermostat_LocalTemperature
    }
  },
  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_OCCUPIED_COOLING_SETPOINT,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,
      (void *)&zclSampleThermostat_OccupiedCoolingSetpoint
    }
  },
  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_OCCUPIED_HEATING_SETPOINT,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,
      (void *)&zclSampleThermostat_OccupiedHeatingSetpoint
    }
  },
  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_ABS_MIN_HEAT_SETPOINT_LIMIT,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ ,
      (void *)&zclSampleThermostat_AbsMinHeatSetpointLimit
    }
  },
  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_ABS_MAX_HEAT_SETPOINT_LIMIT,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ ,
      (void *)&zclSampleThermostat_AbsMaxHeatSetpointLimit
    }
  },
  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_ABS_MIN_COOL_SETPOINT_LIMIT,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ ,
      (void *)&zclSampleThermostat_AbsMinCoolSetpointLimit
    }
  },
  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_ABS_MAX_COOL_SETPOINT_LIMIT,
      ZCL_DATATYPE_INT16,
      ACCESS_CONTROL_READ ,
      (void *)&zclSampleThermostat_AbsMaxCoolSetpointLimit
    }
  },

  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_CONTROL_SEQUENCE_OF_OPERATION,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,
      (void *)&zclSampleThermostat_ControlSequenceOfOperation
    }
  },
  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    { // Attribute record
      ATTRID_THERMOSTAT_SYSTEM_MODE,
      ZCL_DATATYPE_ENUM8,
      ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,
      (void *)&zclSampleThermostat_SystemMode
    }
  },
  {
    ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
    {  // Attribute record
      ATTRID_CLUSTER_REVISION,
      ZCL_DATATYPE_UINT16,
      ACCESS_CONTROL_READ,
      (void *)&zclSampleThermostat_thermostat_clusterRevision
    }
  },
};

uint8_t CONST zclSampleThermostat_NumAttributes = ( sizeof(zclSampleThermostat_Attrs) / sizeof(zclSampleThermostat_Attrs[0]) );

/*********************************************************************
 * SIMPLE DESCRIPTOR
 */
// This is the Cluster ID List and should be filled with Application
// specific cluster IDs.
#define ZCLSAMPLETHERMOSTAT_MAX_INCLUSTERS       3
const cId_t zclSampleThermostat_InClusterList[ZCLSAMPLETHERMOSTAT_MAX_INCLUSTERS] =
{
  ZCL_CLUSTER_ID_GENERAL_BASIC,
  ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
  ZCL_CLUSTER_ID_HVAC_THERMOSTAT
};

#define ZCLSAMPLETHERMOSTAT_MAX_OUTCLUSTERS       2
const cId_t zclSampleThermostat_OutClusterList[ZCLSAMPLETHERMOSTAT_MAX_OUTCLUSTERS] =
{
  ZCL_CLUSTER_ID_GENERAL_IDENTIFY,
  ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT
};

SimpleDescriptionFormat_t zclSampleThermostat_SimpleDesc =
{
  SAMPLETHERMOSTAT_ENDPOINT,                  //  int Endpoint;
  ZCL_HA_PROFILE_ID,                          //  uint16_t AppProfId[2];
  ZCL_DEVICEID_THERMOSTAT,                 //  uint16_t AppDeviceId[2];
  SAMPLETHERMOSTAT_DEVICE_VERSION,            //  int   AppDevVer:4;
  SAMPLETHERMOSTAT_FLAGS,                     //  int   AppFlags:4;
  ZCLSAMPLETHERMOSTAT_MAX_INCLUSTERS,         //  byte  AppNumInClusters;
  (cId_t *)zclSampleThermostat_InClusterList, //  byte *pAppInClusterList;
  ZCLSAMPLETHERMOSTAT_MAX_OUTCLUSTERS,        //  byte  AppNumInClusters;
  (cId_t *)zclSampleThermostat_OutClusterList //  byte *pAppInClusterList;
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
void zclSampleThermostat_ResetAttributesToDefaultValues(void)
{
  zclSampleThermostat_PhysicalEnvironment = PHY_UNSPECIFIED_ENV;

#ifdef ZCL_IDENTIFY
  zclSampleThermostat_IdentifyTime = 0;
#endif

  zclSampleThermostat_LocalTemperature = ATTR_INVALID_MEASUREMENT_HVAC_THERMOSTAT_LOCAL_TEMPERATURE;
  zclSampleThermostat_AbsMinHeatSetpointLimit = SAMPLETHERMOSTAT_ABS_MIN_HEAT_SETPOINT_LIMIT;
  zclSampleThermostat_AbsMaxHeatSetpointLimit = SAMPLETHERMOSTAT_ABS_MAX_HEAT_SETPOINT_LIMIT;
  zclSampleThermostat_AbsMinCoolSetpointLimit = SAMPLETHERMOSTAT_ABS_MIN_COOL_SETPOINT_LIMIT;
  zclSampleThermostat_AbsMaxCoolSetpointLimit = SAMPLETHERMOSTAT_ABS_MAX_COOL_SETPOINT_LIMIT;
  zclSampleThermostat_OccupiedHeatingSetpoint = SAMPLETHERMOSTAT_OCCUPIED_HEATING_SETPOINT;
  zclSampleThermostat_OccupiedCoolingSetpoint = SAMPLETHERMOSTAT_OCCUPIED_COOLING_SETPOINT;
  zclSampleThermostat_ControlSequenceOfOperation = HVAC_THERMOSTAT_CTRL_SEQ_OF_OPER_COOLING_HEATING;
  zclSampleThermostat_SystemMode = HVAC_THERMOSTAT_SYSTEM_MODE_OFF;

}

/****************************************************************************
****************************************************************************/


