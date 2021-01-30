//Your JOB:
//Remove the clusters that your application do not use. This will save some flash and processing
//when looking into matching clusters during the finding & binding procedure
const cId_t bdb_ZclType1Clusters[] =
{
  ZCL_CLUSTER_ID_GENERAL_SCENES,
  ZCL_CLUSTER_ID_GENERAL_ON_OFF,
  ZCL_CLUSTER_ID_GENERAL_LEVEL_CONTROL,
  ZCL_CLUSTER_ID_GENERAL_ALARMS,
  ZCL_CLUSTER_ID_GENERAL_PARTITION,
  ZCL_CLUSTER_ID_CLOSURES_WINDOW_COVERING,
  ZCL_CLUSTER_ID_HVAC_FAN_CONTROL,
  ZCL_CLUSTER_ID_HVAC_DIHUMIDIFICATION_CONTROL,
  ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL,
  ZCL_CLUSTER_ID_LIGHTING_BALLAST_CONFIG,
  ZCL_CLUSTER_ID_SS_IAS_ACE,
  ZCL_CLUSTER_ID_SS_IAS_WD,
  ZCL_CLUSTER_ID_PI_GENERIC_TUNNEL,
  ZCL_CLUSTER_ID_PI_BACNET_PROTOCOL_TUNNEL,
  ZCL_CLUSTER_ID_MS_ELECTRICAL_MEASUREMENT,
  ZCL_CLUSTER_ID_PI_11073_PROTOCOL_TUNNEL,
  ZCL_CLUSTER_ID_PI_ISO7818_PROTOCOL_TUNNEL,
  ZCL_CLUSTER_ID_PI_RETAIL_TUNNEL,
  ZCL_CLUSTER_ID_SE_PRICE,
  ZCL_CLUSTER_ID_SE_DRLC,
  ZCL_CLUSTER_ID_SE_METERING,
  ZCL_CLUSTER_ID_SE_MESSAGING,
  ZCL_CLUSTER_ID_TELECOMMUNICATIONS_INFORMATION,
  ZCL_CLUSTER_ID_TELECOMMUNICATIONS_VOICE_OVER_ZIGBEE,
};

//Your JOB:
//Remove the clusters that your application do not use. This will save some flash and processing
//when looking into matching clusters during the finding & binding procedure
const cId_t bdb_ZclType2Clusters[] =
{
  ZCL_CLUSTER_ID_GENERAL_ON_OFF_SWITCH_CONFIGURATION,
  ZCL_CLUSTER_ID_GENERAL_TIME,
  ZCL_CLUSTER_ID_GENERAL_ANALOG_INPUT_BASIC,
  ZCL_CLUSTER_ID_GENERAL_ANALOG_OUTPUT_BASIC,
  ZCL_CLUSTER_ID_GENERAL_ANALOG_VALUE_BASIC,
  ZCL_CLUSTER_ID_GENERAL_BINARY_INPUT_BASIC,
  ZCL_CLUSTER_ID_GENERAL_BINARY_OUTPUT_BASIC,
  ZCL_CLUSTER_ID_GENERAL_BINARY_VALUE_BASIC,
  ZCL_CLUSTER_ID_GENERAL_MULTISTATE_INPUT_BASIC,
  ZCL_CLUSTER_ID_GENERAL_MULTISTATE_OUTPUT_BASIC,
  ZCL_CLUSTER_ID_GENERAL_MULTISTATE_VALUE_BASIC,
  ZCL_CLUSTER_ID_GENERAL_APPLIANCE_CONTROL,
  ZCL_CLUSTER_ID_CLOSURES_SHADE_CONFIG,
  ZCL_CLUSTER_ID_CLOSURES_DOOR_LOCK,
  ZCL_CLUSTER_ID_HVAC_PUMP_CONFIG_CONTROL,
  ZCL_CLUSTER_ID_HVAC_THERMOSTAT,
  ZCL_CLUSTER_ID_MS_TEMPERATURE_MEASUREMENT,
  ZCL_CLUSTER_ID_MS_ILLUMINANCE_MEASUREMENT,
  ZCL_CLUSTER_ID_MS_ILLUMINANCE_LEVEL_SENSING_CONFIG,
  ZCL_CLUSTER_ID_MS_PRESSURE_MEASUREMENT,
  ZCL_CLUSTER_ID_MS_FLOW_MEASUREMENT,
  ZCL_CLUSTER_ID_MS_RELATIVE_HUMIDITY,
  ZCL_CLUSTER_ID_MS_OCCUPANCY_SENSING,
  ZCL_CLUSTER_ID_SS_IAS_ZONE,
  ZCL_CLUSTER_ID_PI_ANALOG_INPUT_BACNET_REG,
  ZCL_CLUSTER_ID_PI_ANALOG_INPUT_BACNET_EXT,
  ZCL_CLUSTER_ID_PI_ANALOG_OUTPUT_BACNET_REG,
  ZCL_CLUSTER_ID_PI_ANALOG_OUTPUT_BACNET_EXT,
  ZCL_CLUSTER_ID_PI_ANALOG_VALUE_BACNET_REG,
  ZCL_CLUSTER_ID_PI_ANALOG_VALUE_BACNET_EXT,
  ZCL_CLUSTER_ID_PI_BINARY_INPUT_BACNET_REG,
  ZCL_CLUSTER_ID_PI_BINARY_INPUT_BACNET_EXT,
  ZCL_CLUSTER_ID_PI_BINARY_OUTPUT_BACNET_REG,
  ZCL_CLUSTER_ID_PI_BINARY_OUTPUT_BACNET_EXT,
  ZCL_CLUSTER_ID_PI_BINARY_VALUE_BACNET_REG,
  ZCL_CLUSTER_ID_PI_BINARY_VALUE_BACNET_EXT,
  ZCL_CLUSTER_ID_PI_MULTISTATE_INPUT_BACNET_REG,
  ZCL_CLUSTER_ID_PI_MULTISTATE_INPUT_BACNET_EXT,
  ZCL_CLUSTER_ID_PI_MULTISTATE_OUTPUT_BACNET_REG,
  ZCL_CLUSTER_ID_PI_MULTISTATE_OUTPUT_BACNET_EXT,
  ZCL_CLUSTER_ID_PI_MULTISTATE_VALUE_BACNET_REG,
  ZCL_CLUSTER_ID_PI_MULTISTATE_VALUE_BACNET_EXT,
  ZCL_CLUSTER_ID_SE_TUNNELING,
  ZCL_CLUSTER_ID_TELECOMMUNICATIONS_INFORMATION,
  ZCL_CLUSTER_ID_HA_APPLIANCE_IDENTIFICATION,
  ZCL_CLUSTER_ID_HA_METER_IDENTIFICATION,
  ZCL_CLUSTER_ID_HA_APPLIANCE_EVENTS_ALERTS,
  ZCL_CLUSTER_ID_HA_APPLIANCE_STATISTICS,
};