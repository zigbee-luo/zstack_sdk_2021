TI 15.4-Stack Collector Example
==========================

Example Summary
---------------

The collector example application demonstrates how to implement a Personal Area Network (PAN) Coordinator device using TI 15.4-Stack. A TI 15.4-Stack based star network consists of two types of logical devices: the PAN-Coordinator and the network devices, e.g. the Collector and Sensor applications, respectively.

The PAN-Coordinator is the device that starts the network and allows for other devices to join the network. The network devices join the network through and always communicate with the PAN-Coordinator.

The example applications in TI 15.4-Stack are developed for the CC13x2 Launchpad platform. In addition, the Linux example applications for the external host (AM335x + MAC Coprocessor) are located in the [**TI 15.4-Stack Gateway SDK**](http://www.ti.com/tool/ti-15.4-stack-gateway-linux-sdk).

> The project names for CC1352 and CC2652 platforms are referred to as CC13x2 or CC26x2. Replace x with either **1** or **5** depending on the specific wireless MCU being used.

> Note that this also includes the CC1352P-X boards, where the X represents which board subset is used, and the power amplification range.

Peripherals Exercised
---------------------

> To trigger various events, buttons can be used as well as the configurable user interface.
> The Example Usage section of this document explains how to use the user interface, although both the button presses and the UART perform the
> same actions.

* `CONFIG_GPIO_RLED` - Set when the collector application is initialized. Flashes when the network is open for joining.
* `CONFIG_GPIO_BTN1` - Press to initialize the collector application.
* `CONFIG_GPIO_BTN2` - Press to allow new devices to join the network. While the network is open for joining, `CONFIG_GPIO_RLED` will flash. Press again to disallow joining.

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
The collector application has three processing loops each handling a different set of
events. These are as follows:

* Collector_process: Collector application event handling
	* Collector event handling
		* Network start (COLLECTOR_START_EVT)
		* Permit join toggle (COLLECTOR_OPEN_NWK_EVT/COLLECTOR_CLOSE_NWK_EVT)
		* Generate tracking message (COLLECTOR_TRACKING_TIMEOUT_EVT)
		* Generate config message (COLLECTOR_CONFIG_EVT)
	* Triggers Cllc_process and Csf_processEvents
	* Triggers MAC callback handling via ApiMac_processIncoming
* Cllc_process: Collector logical link controller event handling
	* Trickle timer handling (PAN Advertisement/PAN Configuration message events)
	* State change processing
	* Join permit timeout handling
* Csf_processEvents: External input handling
	* CUI input handling
	* Button press input handling
	* Triggers events in collector/cllc processing loops based on input

All three processing loops handle specialized tasks to service collector functionality.
Additionally, ApiMac_processIncoming will trigger collector and cllc callbacks if
they are defined, acting as the trigger for several collector and cllc processing loop
events.

An overview of the collector cllc states and state transitions is as follows:

	          Cllc_states_initWaiting
	                     | COLLECTOR_START_EVT, initiated by CSF_KEY_EVENT,
	                     | COLLECTOR_UI_INPUT_EVT, or by defining AUTO_START
	                     |
	    Existing         |          New
	    Network          |          Network
		   +-------------+-------------+
		   |                           |
		   V                           V
	  Cllc_states_                Cllc_states_
	  initRestoringCoordinator    startingCoordinator
		   |                           |
		   V                           V
	  Cllc_states_                Cllc_states_
	  restored                    started
		   |                           |
		   +-------------+-------------+
	                     | CSF_KEY_EVENT/COLLECTOR_UI_INPUT_EVT triggers
	                     | COLLECTOR_OPEN_NWK_EVT or COLLECTOR_CLOSE_NWK_EVT,
	                     | triggering Cllc_setJoinPermit
	                     |
	    Permit join      |          Permit join
	    Off              |          On
		   +-------------+-------------+
		   |                           |
		   V                           V
	  Cllc_states_                Cllc_states_
	  joiningNotAllowed           joiningAllowed


Example Usage
-------------
This example project implements a collector device: the PAN-Coordinator for the network. This device creates a TI 15.4-Stack network, allows sensor devices to join the network, collects sensor information sent by devices running the sensor example application, and tracks if the devices are on the network or not by periodically sending tracking request messages.

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
 TI Collector
 Press Enter for Help
<      HELP      >

Security Status: --
Status: Waiting...
Device Status: --
Number of Joined Devices: --
```

The configurable user interface is provided to allow you to make changes at runtime, as follows:
```
    0xFFFF

<      SET PANID      >

Status: Waiting...
Device Status: --
Number of Joined Devices: --
```

```
 00 00 0F 00 00 00 00 00 00 00 00 00 00 00 00 00 00

<    SET CHAN MASK    >

Status: Waiting...
Device Status: --
Number of Joined Devices: --
```
```
 12 34 56 78 9A BC DE F0 00 00 00 00 00 00 00 00

<     SET NWK KEY     >

Status: Waiting...
Device Status: --
Number of Joined Devices: --
```


Note that these changes will only take effect if the collector is in a waiting state.
Keys 0-F can be used to change the value when in edit mode, and left/right keys can be used for navigating the digits.
Once the collector is started, the settings can only be changed if it is restarted.

> If the **AUTO_START** symbol is defined in your application, then the application will automatically configure itself on startup.
> This is not enabled by default with the project, but it can be configured as described below.

**AUTO_START** can be defined by removing the x in the .opts file under the defines folder.
-DxAUTO_START -> -DAUTO_START

> If **AUTO_START** is defined, the collector will display Starting... instead of Waiting...
After the collector has been started, permit join must be turned on, and this can be done either with the right button press on the collector, or through the user interface as shown below.

> If **AUTO_START** is not defined, then the collector will not open a network unless it is started with the left button press, and permit join is turned on with the right button press.
Alternatively, the configurable user interface will allow you to start the collector, and open the network for sensors to join using the following menu options under the NETWORK ACTIONS tab:

To FORM Network:
```
 TI Collector

<       FORM NWK      >

Status: Waiting...
Device Status: --
Number of Joined Devices: --
```

To OPEN Network:

```
 TI Collector

<       OPEN NWK      >

Status: Waiting...
Device Status: --
Number of Joined Devices: --
```

To CLOSE Network:

```
 TI Collector

<       CLOSE NWK     >

Status: Waiting...
Device Status: --
Number of Joined Devices: --
```

* Start the application by pressing `CONFIG_GPIO_BTN1` or selecting `FORM NWK` under the `NETWORK ACTIONS` tab.
```
 TI Collector
 Press Enter for Help
<      HELP      >

Status: Starting...
Device Status: --
Number of Joined Devices: 0
```

After starting, sensor specific application information will be displayed, such as the devices current state.
```
 TI Collector

<   NETWORK ACTIONS   >

Status: Started--Mode=NBCN, Addr=0xaabb, PanId=0x0001, Ch=0, PermitJoin=Off
Device Status: --
Number of Joined Devices: 0
```

* Now turn on permit join using `CONFIG_GPIO_BTN2` or by selecting OPEN NWK under the `NETWORK ACTIONS` tab.
Once the network is started, and sensors begin to join, each of the status lines will update accordingly.
```
 TI Collector

<   NETWORK ACTIONS   >

Status: Started--Mode=NBCN, Addr=0xaabb, PanId=0x0001, Ch=0, PermitJoin=On
Device Status: Joined - 0x0001
Number of Joined Devices: 1
```

> The coordinator device state variable information can be seen in the Collector Logical Link Controller's header file (`cllc.h`), within the `Cllc_states_t` structure.

On the sensor side:
* Wait for the sensor device to join a network, after which the output will be updated with the channel number and device ID of the sensor that was started. After joining the network `CONFIG_GPIO_RLED` will be set.
The settings that you selected above in the waiting state will then take effect.
```
     TI Sensor
     Press Enter for Help
    <      HELP      >

    Sensor Status: Joined - Mode=NBCN, Addr=0x0001, PanId=0x0001, Ch=0
```

* After the sensor node has successfully joined the network, it receives a configuration request message from the collector application.
 The node then configures the time interval on how often to report the sensor data to the collector application, and how often to poll
 for buffered messages in case of sleepy devices. After receiving the configuration request message, the green LED (`CONFIG_GPIO_GLED`)
 toggles whenever the device sends the message.

> In order for the network device to join, it must have either the generic PAN Id (0xFFFF, default configuration)
or the same PAN Id as the collector. These settings can be found in the application's SysConfig dashboard.


Analyzing Network Traffic
-------------------------

TI 15.4-Stack provides the means to analyze over-the-air traffic by including a packet sniffer firmware image. With an additional CC13x2 Launchpad, users can set up a packet sniffer with the software provided in the SDK. More information about this can be found in the TI 15.4-Stack documentation under **Packet Sniffer**.

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

By default, this project is configured to use four pages of NV. A maximum of five NV pages are supported. In order to modify the NV pages, update the following:
* `NVOCMP_NVPAGES=4` in the project-specific .opts file
* `NVOCMP_NVPAGES=4` in the linker options
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
