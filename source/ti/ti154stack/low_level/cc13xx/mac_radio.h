/******************************************************************************

 @file  mac_radio.h

 @brief Describe the purpose and contents of the file.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2006-2021, Texas Instruments Incorporated
 All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License"). You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product. Other than for
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

 ******************************************************************************
 
 
 *****************************************************************************/

#ifndef MAC_RADIO_H
#define MAC_RADIO_H

/* ------------------------------------------------------------------------------------------------
 *                                         Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_types.h"
#include "rf_mac_api.h"
#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                            Defines
 * ------------------------------------------------------------------------------------------------
 */

/* RF registers(hidden) */
#define RFREG_RSSIMAXVAL      0x605C

/* Prop radio operation finished normally */
#define PROP_DONE_OK          0x3400   ///< Operation ended normally
#define PROP_DONE_RXTIMEOUT   0x3401   ///< Operation stopped after end trigger while waiting for sync
#define PROP_DONE_STOPPED     0x3404   ///< Operation stopped after stop command
#define PROP_DONE_RXERR       0x3406   ///< Operation ended after receiving packet with CRC error

#define PROP_DONE_IDLE        0x3407   ///< Carrier sense operation ended because of idle channel
#define PROP_DONE_BUSY        0x3408   ///< Carrier sense operation ended because of busy channel
#define PROP_DONE_BUSYTIMEOUT 0x340A   ///< Carrier sense operation ended because of timeout

#define PROP_DONE_SYNTH_ERROR 0x3804   ///< Operation ended due to synth error

/* TODO: Prop radio operation finished with error */
#define PROP_ERROR_RXBUF      0x3801   ///< No available RX buffer at the start of a packet

/* Modulation type */
#define MAC_MODULATION_TYPE_FSK             0
#define MAC_MODULATION_TYPE_GFSK            1

/* Symbol rate prescales */
#define MAC_SYMBOL_RATE_PRESCALE_4          4
#define MAC_SYMBOL_RATE_PRESCALE_5          5
#define MAC_SYMBOL_RATE_PRESCALE_6          6
#define MAC_SYMBOL_RATE_PRESCALE_7          7
#define MAC_SYMBOL_RATE_PRESCALE_8          8
#define MAC_SYMBOL_RATE_PRESCALE_9          9
#define MAC_SYMBOL_RATE_PRESCALE_10         10
#define MAC_SYMBOL_RATE_PRESCALE_11         11
#define MAC_SYMBOL_RATE_PRESCALE_12         12
#define MAC_SYMBOL_RATE_PRESCALE_13         13
#define MAC_SYMBOL_RATE_PRESCALE_14         14
#define MAC_SYMBOL_RATE_PRESCALE_15         15

/* Modulation Index */
#define MAC_MODULATION_INDEX_2FSK_50K       15
#define MAC_MODULATION_INDEX_2FSK_150K      5
#define MAC_MODULATION_INDEX_2FSK_5K        5
#define MAC_MODULATION_INDEX_2FSK_200K      5
#define MAC_MODULATION_INDEX_2FSK_500K      9

/* Receiver bandwidth */
#define MAC_RX_BANDWIDTH_55KHZ              0x21
#define MAC_RX_BANDWIDTH_88KHZ              0x23
#define MAC_RX_BANDWIDTH_110KHZ             0x24
#define MAC_RX_BANDWIDTH_180KHZ             0x26
#define MAC_RX_BANDWIDTH_350KHZ             0x29
#define MAC_RX_BANDWIDTH_1410KHZ            0x2F
#define MAC_RX_BANDWIDTH_310KHZ             0x59

/* Preamble mode */
#define MAC_FIRST_PREAMBLE_BIT_ZERO         0
#define MAC_FIRST_PREAMBLE_BIT_ONE          1

/* Sync word bit length */
#define MAC_SYNCWORD_BITS_16                16
#define MAC_SYNCWORD_BITS_24                24
#define MAC_SYNCWORD_BITS_32                32

#ifndef PREAMBLE_COMPATIBILITY
/* SFD value for uncoded PHR+PUSD */
#define MAC_2FSK_UNCODED_SFD_0              0x5555904E
#define MAC_2FSK_UNCODED_SFD_1              0x55557A0E
#define MAC_4FSK_UNCODED_SFD_0              0xD75575FD
#define MAC_4FSK_UNCODED_SFD_1              0x7FDD55FD

/* SFD value for coded PHR+PUSD */
#define MAC_2FSK_CODED_SFD_0                0x55556F4E
#define MAC_2FSK_CODED_SFD_1                0x5555632D
#define MAC_4FSK_CODED_SFD_0                0x7DFF75FD
#define MAC_4FSK_CODED_SFD_1                0x7D5F5DF7
#else
/* SFD value for uncoded PHR+PUSD */
#define MAC_2FSK_UNCODED_SFD_0              0x0000904E
#define MAC_2FSK_UNCODED_SFD_1              0x00007A0E
#define MAC_4FSK_UNCODED_SFD_0              0xD75575FD
#define MAC_4FSK_UNCODED_SFD_1              0x7FDD55FD

/* SFD value for coded PHR+PUSD */
#define MAC_2FSK_CODED_SFD_0                0x00006F4E
#define MAC_2FSK_CODED_SFD_1                0x0000632D
#define MAC_4FSK_CODED_SFD_0                0x7DFF75FD
#define MAC_4FSK_CODED_SFD_1                0x7D5F5DF7
#endif

/* MAC radio yield status, also used as a boolean
 * non-zero means radio not active.
 */
#define MAC_RADIO_ACTIVE                    0
#define MAC_RADIO_YIELDED                   1
#define MAC_RADIO_POWERING_UP               2

/* ------------------------------------------------------------------------------------------------
 *                                      Global Variables
 * ------------------------------------------------------------------------------------------------
 */
extern uint8 macPhyChannel;
extern uint32 macPhyTxPower;
extern volatile uint8 macRadioYielded;

/* Radio handle for TIMAC */
extern RF_Handle RF_handle;

/* Radio Parameters for TIMAC */
extern RF_Params RF_params;

/* Are we in radio swi context? */
extern volatile bool bInSwi;

/* ------------------------------------------------------------------------------------------------
 *                                        Prototypes
 * ------------------------------------------------------------------------------------------------
 */
MAC_INTERNAL_API void macRadioInit(void);
MAC_INTERNAL_API void macRadioSwInit(void);
MAC_INTERNAL_API void macRadioReset(void);
MAC_INTERNAL_API void macRadioSetRE(uint32 hgm);
MAC_INTERNAL_API void macRadioUpdateTxPower(void);
MAC_INTERNAL_API bool macRadioUpdateChannel(void);
MAC_INTERNAL_API bool macRadioUpdateChannelRx(void);
MAC_INTERNAL_API uint8 macRadioComputeLQI(int8 rssiDbm, uint8 correlation);
MAC_INTERNAL_API void macRadioPowerDown(bool bypassRAT);
MAC_INTERNAL_API void macRadioPowerUp(bool bypassRAT);
MAC_INTERNAL_API void scheduleNopWakeupCmd(void);
MAC_INTERNAL_API void macRadioPowerUpWait(void);
MAC_INTERNAL_API void macRadioWakeupCb(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);
MAC_INTERNAL_API void macRadioPowerUpBeaconMode(bool bypassRAT);
MAC_INTERNAL_API bool macGetRadioState(void);
MAC_INTERNAL_API uint8 radioComputeED(int8 rssiDbm);

/**************************************************************************************************
 */
#endif
