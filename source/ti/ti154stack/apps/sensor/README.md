TI 15.4-Stack Sensor Example
=======================

Example Summary
---------------

The sensor example application demonstrates how to implement a sensor network device using TI 15.4-Stack. TI 15.4-Stack based star network consists of two types of logical devices: the PAN-Coordinator and the network devices, e.g. the Collector and Sensor applications, respectively.

The PAN-Coordinator is the device that starts the network and allows other devices to join the network. The network devices join the network through and always communicate with the PAN-Coordinator.

The example applications in TI 15.4-Stack are developed for the CC13x2 Launchpad platform. In addition, the Linux example applications for the external host (AM335x + MAC Coprocessor) are located in the [**TI 15.4-Stack Gateway SDK**](http://www.ti.com/tool/ti-15.4-stack-gateway-linux-sdk).

> The project names for CC1352 and CC2652 platforms are referred to as CC13x2 or CC26x2. Replace x with either **1** or **5** depending on the specific wireless MCU being used.

> Note that this also includes the CC1352P-X boards, where the X represents which board subset is used, and the power amplification range.


Peripherals Exercised
---------------------

> To trigger various events, buttons can be used as well as the configurable user interface.
> The Example Usage section of this document explains how to use the user interface, although both the button presses and the UART perform the
> same actions.

* `CONFIG_GPIO_RLED` - Turns on after the sensor connects to the collector.
* `CONFIG_GPIO_BTN1` - Press to initialize the sensor application.
* `CONFIG_GPIO_BTN2` - Press to disassociate the device from the network.

> If `CONFIG_GPIO_BTN2` is held while power is applied to the Launchpad, NV Flash will be erased.


Resources & Jumper Settings
---------------------------
The following hardware is required to run TI 15.4-Stack Out of Box (OOB) example applications:
* Two [**CC13x2 Launchpad development kits**](http://www.ti.com/tool/launchxl-cc1352r1) or two [**CC26x2 Launchpad development kits**](http://www.ti.com/tool/launchxl-cc26x2r1)

> If you're using an IDE (such as CCS or IAR), please refer to `Board.html` in
your project directory for resources used and board-specific jumper settings.
Otherwise, you can find `Board.html` in the directory
&lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Please refer to the following link for helpful SimpleLink Academy guides for ramping up
on TI 15.4-Stack: [TI 15.4-Stack SimpleLink Academy](https://dev.ti.com/tirex/explore/node?node=ABRXrYdFS1e-0P3PY6NmNg__pTTHBmu__LATEST).

For an in-depth overview of the TI 15.4-Stack application, please refer to the TI 15.4-Stack User Guide at
`<SDK_ROOT>/docs/ti154stack/html/ti154stack/application-overview.html#application-overview`).


Example Application Dataflow
---------------------------
The sensor application has three processing loops each handling a different set of
events. These are as follows:

* Sensor_process: Sensor application event handling
	* Sensor event handling:
		* Start sensor scan for network (SENSOR_START_EVT)
		* Read sensor value and report to collector (SENSOR_READING_TIMEOUT_EVT)
		* Diassociate sensor (SENSOR_DISASSOC_EVT)
	* Triggers Jdllc_process and Ssf_processEvents
	* Triggers MAC callback handling via ApiMac_processIncoming
* Jdllc_process: Sensor logical link controller event handling
	* Trickle timer handling (PAN Advertisement/PAN Configuration message events)
	* Poll event handling (JDLLC_POLL_EVT)
	* Association request handling (JDLLC_ASSOCIATE_REQ_EVT)
	* Coordinator realignment handling (JDLLC_COORD_REALIGN)
	* Scan backoff handling (JDLLC_SCAN_BACKOFF)
	* State change handling
* Ssf_processEvents: External input handling
	* CUI input handling
	* Button press input handling
	* Triggers events in sensor/jdllc processing loops based on input

All three processing loops handle specialized tasks to service sensor functionality.
Additionally, ApiMac_processIncoming will trigger sensor and jdllc callbacks if
they are defined, acting as the trigger for several sensor and jdllc processing loop
events.

An overview of the sensor jdllc states and state transitions is as follows:

	               Jdllc_states_initWaiting
	                          | SENSOR_START_EVT, initiated by KEY_EVENT,
	                          | SENSOR_UI_INPUT_EVT, or by defining AUTO_START
	                          |
	         Existing         |          New
	         Network          |          Network
	            +-------------+-------------+
	            |                           |
	            V                           V
	  +--> Jdllc_states_                Jdllc_states_
	  |    initRestoring                joining
	  |         |                           |
	  |         V                           V
	  |    Jdllc_states_                Jdllc_states_
	  |    rejoined                     joined
	  |         |                           |
	  |         +-------------+-------------+
	  |                       | MAC reports sync loss (BCN mode) or
	  |                       | CONFIG_MAX_DATA_FAILURES consecutive data frames
	  |                       | fail to be ACK'ed by collector
	  | Orphan scan +         |
	  | Coord realign         V
	  +----------------- Jdllc_states_
	                     orphan


Example Usage
-------------
This example project implements a sensor end device: one of potentially many network devices in a PAN. This end device reads sensor information and sends it to the PAN-coordinator at a configured interval. This example assumes a second Launchpad is running the default collector application code to act as the PAN-coordinator.

The example output can be viewed through the UART terminal.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's
Homepage"), etc.) to the appropriate COM port with the following settings.

* Note that if you are using Tera Term, by default, the Backspace key will be replaced with the delete key.
If you go to Setup->Keyboard There is a section called `Transmitting DEL by:`
Make sure that the backspace character is checked as well.

> The COM port can be determined via Device Manager in Windows or via
`ls /dev/tty*` in Linux.

* Upon example start, the connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```
and initially display the following text on the UART terminal.

Main Menu:
```
 TI Sensor
 Press Enter for Help
<      HELP      >

Status: Waiting...
```

The configurable user interface is provided to allow you to make changes at runtime, as follows:
```
    0xFFFF

<      SET PANID      >

Status: Waiting...
```

```
 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 00 00

<    SET CHAN MASK    >

Status: Waiting...
```
```
 12 34 56 78 9A BC DE F0 00 00 00 00 00 00 00 00

<     SET NWK KEY     >

Status: Waiting...
```

Note that these changes will only take effect if the sensor is in a waiting state.
Keys 0-F can be used to change the value when in edit mode, and left/right keys can be used for navigating the digits.
Once the sensor is started, the settings can only be changed if it is restarted.


> If the **AUTO_START** symbol is defined in your application, then the application will automatically configure itself on startup.
> This is not enabled by default with the project, but it can be configured as described below.

AUTO_START can be defined by removing the x in the .opts file under the defines folder.
-DxAUTO_START -> -DAUTO_START

> If the **AUTO_START** symbol is defined in your application, then the application will automatically configure itself on startup,
and the sensor will display Starting... instead of Waiting...

> If the **AUTO_START** symbol is not defined pressing `CONFIG_GPIO_BTN1` will initialize the sensor application until the sensor has connected to a network.  Alternatively, the sensor can also be started through the user interface, as shown below.
Note that the sensor will not join a network unless it is started.

To ASSOCIATE to a network:
```
 TI Sensor

<      ASSOCIATE      >

Status: Starting...
```

To DISASSOCIATE from a network:

```
 TI Sensor

<     DISASSOCIATE    >

Status: Starting...
```

* Start the application by pressing `CONFIG_GPIO_BTN1` or selecting ASSOCIATE under the NETWORK ACTIONS tab.
After starting, sensor specific application information will be displayed, such as the device's current state.
```
 TI Sensor
 Press Enter for Help
<      HELP      >

Status: Starting...
```

Once the network is started, and sensors begin to join, each of the status lines will update accordingly.
```
 TI Sensor

<      HELP      >

Status: Joined--Mode=NBCN, Addr=0x0001, PanId=0x0001, Ch=0
```

> The joining device state variable information can be seen in the Joining Device Logical Link Controller's header file (`jdllc.h`), within the `Jdllc_states_t` structure.

* Wait for the sensor device to join a network, after which the output will be updated with the channel number and device ID of the sensor that was started. After joining the network `CONFIG_GPIO_RLED` will be set.
The settings that you selected above in the waiting state will then take effect.
```
  TI Sensor
  Press Enter for Help
<      HELP      >

Status: Joined--Mode=NBCN, Addr=0x0001, PanId=0x0001, Ch=0
```

* After the sensor node has successfully joined the network, it receives a configuration request message from the collector application. The node then configures the time interval on how often to report the sensor data to the collector application, and how often to poll for buffered messages in case of sleepy devices. After receiving the configuration request message, the green LED (`CONFIG_GPIO_GLED`) toggles whenever the device sends the message.

> In order for the network device to join, it must have either the generic PAN Id (0xFFFF, default configuration) or the same PAN Id as the collector. These settings can be found in the application's SysConfig dashboard.

Analyzing Network Traffic
-------------------------

TI 15.4-Stack provides the means to analyze over-the-air traffic by including a packet sniffer firmware image. With an additional CC13x2 Launchpad, users can set up a packet sniffer with the software provided in the SDK. More information about this can be found in the TI 15.4-Stack documentation under **Packet Sniffer**.


Viewing Device PER (Packet Error Rate)
--------------------------------------

PER is a simple value with the following equation
* PER = 100 * (Number of Failed Packets / (Number of Successful Packets + Number of Failed Packets))

This value can be used to judge how well the network doing.

If you would like to see the stats on the Number of Failed and Successful Packets then simply define **DISPLAY_PER_STATS**. This will add the code necessary to update and display the stats to the UART Display.

Project Configuration
--------------------------

The System Configuration (SysConfig) tool is a graphical interface for configuring your TI 15.4-Stack project. Based on the parameters configured in the SysConfig dashboard, C source files and header files are generated. Further advanced parameters can be located in `advanced_config.h`.

Some important settings in the TI 15.4-Stack module include:

| **Parameter**         | **SysConfig Location**            | **Description**                                               |
|-----------------------|-----------------------------------|---------------------------------------------------------------|
| Mode                  | Top of TI-15.4 Stack module       | Configures the mode of network operation                      |
| MAC Beacon Order      | MAC group within Network category | Configures how often the coordinator transmits a beacon       |
| MAC Super Frame Order | MAC group within Network category | Configures the length of the active portion of the superframe |
| Channel Mask          | Network category                  | Configures channels to be scanned                             |
| Security Level        | Security category                 | Configures network security level                             |

> SysConfig generated files are dynamically generated upon build, and any manual changes to them will be overwritten.

More information about the configuration and feature options can be found in the TI 15.4-Stack documentation under **Example Applications > Configuration Parameters**.

### Disabling Common User Interface

The common user interface (CUI) is a UART based interface that allows users to control and receive updates regarding the application. For various reasons, including reducing the memory footprint, the user is able to disable the common user interface (CUI). To disable the CUI, the following variable must be defined in the project-specific .opts file:

```
-DCUI_DISABLE
```

> Please Note: particular features that are dependednt on the CUI wil be unavailable when this feature is enabled.

### Multi-Page NV Configuration

By default, this project is configured to use two pages of NV. In order to modify this value, update the following:
* `NVOCMP_NVPAGES=2` in the project-specific .opts file
* `NVOCMP_NVPAGES=2` in the linker options
* SysConfig NVS module:
   * Set Region Size based on the formula `NVOCMP_NVPAGES * 0x2000`
   * Set Region Base based on the formula `0x56000 - (NVOCMP_NVPAGES * 0x2000)`

A detailed description of the application architecture can be found in your installation within the
TI-15.4 Stack Getting Started Guide's Application Overview section.

&lt;SDK_INSTALL_DIR&gt;/docs/ti154stack/ti154stack-getting-started-guide.html.

> For IAR users:
When using the CC13x2SDK, the TI XDS110v3 USB Emulator must
be selected. For the CC13x2_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.

In order to build from flash, within the IAR Project options-> Build Actions
Update the "Pre-build command line" and change the "NO_ROM=0" to "NO_ROM=1".
