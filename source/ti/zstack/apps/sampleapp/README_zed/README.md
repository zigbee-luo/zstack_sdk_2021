# Zigbee End Device Sample App

* [Introduction](#intro)
* [Software Prerequisites](#software-prereqs)
* [Functional Description](#functional-desc)
    * [Software Overview](#software-overview)
        * [Application Files](#application)
* [Configuration With SysConfig](#sysconfig)
* [Example Usage](#usage)
    * [Buttons](#usage-buttons)

# <a name="intro"></a> Introduction

This document discusses briefly the template code provide for developers to create its own application with the minimum required overhead.

# <a name="hardware-prereqs"></a> Hardware Prerequisites

- The Launchpad which is used by this particular project.
  - [SimpleLink&trade; CC26x2R1 Launchpad](http://www.ti.com/tool/LAUNCHXL-CC26X2R1)
  - [SimpleLink&trade; CC1352P Launchpad](https://www.ti.com/tool/LAUNCHXL-CC1352P)
  - [SimpleLink&trade; CC1352R1 Launchpad](https://www.ti.com/tool/LAUNCHXL-CC1352R1)

# <a name="software-prereqs"></a> Software Prerequisites

- [Code Composer Studio&trade; (CCS)](http://processors.wiki.ti.com/index.php/Download_CCS#Download_the_latest_CCS) v10.1 or newer

- [SimpleLink&trade; CC13x2/CC26x2 SDK](http://www.ti.com/tool/simplelink-cc13x2-26x2-sdk)

# <a name="functional-desc"></a> Functional Description

## <a name="software-overview"></a> Software Overview

This section describes the software components and the corresponding source files.

### <a name="application"></a> Application Files

- **sampleapp.c/.h:** Contains the application's event loop, device initialization function calls (for timers, peripherals, Nvm, LEDs, Switches, Serial interface),  Zigbee stack notifications and all application logic is expected to be defined in this module.
- **main.c:** Defines the application task and the application stack size.
- **bdb_interface.h:** Defines commissioning parameters to be used and configured in the application upon calling commissioning methods (see **sampleApp_initParameters()**).

# <a name="sysconfig"></a> Configuration With SysConfig

SysConfig is a GUI configuration tool that allows for TI driver and stack configurations. With this Z-Stack Sample App project, the Zigbee Application Builder is supported in Sysconfig so the user can also configure the Zigbee application device type, clusters, attribute, commands, and command callback APIs required.

To configure using SysConfig, import the SysConfig-enabled project into CCS. Double click the `*.syscfg` file from the CCS project explorer, where `*` is the name of the example project. The SysConfig GUI window will appear, where Zigbee stack and TI driver configurations can be adjusted. These settings will be reflected in the generated files.

The example project comes with working default settings for SysConfig. For the purposes of this README, it is not recommended to change the default driver settings, as any changes may impact the functionality of the example. The Zigbee stack settings may be changed as required for your use case. For more information on how to use the Application Builder, please refer to the Z-Stack SDK Documentation and the SimpleLink Academy modules for Z-Stack.

Note that some Z-Stack settings are stored in non-volatile storage, and Z-Stack prioritizes stored settings over SysConfig settings. To guarantee SysConfig settings are applied, perform a factory reset of the device to  clear non-volatile storage.

# <a name="usage"></a> Example Usage

This application only provides two buttons as the most basic interface to create/join a network and reset the application to factory default state.

## <a name="usage-buttons"></a> Buttons

- `BTN-1`: Starts the commissioning process.
- `BTN-2`: If hold after reset, perform Factory New reset (erase network and application persistent data)
- `BTN-2`: Reset the application to factory new state.

















