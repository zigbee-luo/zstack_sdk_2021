/******************************************************************************

 @file  mac_beacon.h

 @brief Interface to procedures for beacon enabled networks common to both
        device and coordinator.

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

#ifndef MAC_BEACON_H
#define MAC_BEACON_H

/* ------------------------------------------------------------------------------------------------
 *                                              Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                               Macros
 * ------------------------------------------------------------------------------------------------
 */

/* beacon sync and tracking states */
#define MAC_BEACON_NONE_ST            0   /* no tracking or sync in progress */
#define MAC_BEACON_SYNC_ST            1   /* trying to sync */
#define MAC_BEACON_TRACKING_ST        2   /* tracking successful */

/* special trackBeacon value for internally generated sync request for tx */
#define MAC_TRACK_BEACON_TX_SYNC      0xFF

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

typedef struct
{
  macTimer_t      periodTimer;      /* timer set to expire at the end of the CAP */
  uint32          capStart;         /* start of CAP in backoffs units */
  uint32          capEnd;           /* end of CAP in backoff units */
  uint8           sched;            /* current superframe schedule */
  uint8           finalCapSlot;     /* final CAP slot of current superframe */
  bool            battLifeExt;      /* battery life extension setting of current superframe */
  uint8           state;            /* beacon track/sync state */
  bool            txSync;           /* TRUE if beacon sync was internally generated */
  bool            rxWindow;         /* TRUE if ready to receive tracked beacon */
  uint8           coordBeaconOrder; /* Beacon order received in coordinator's beacon */
} macBeacon_t;

/* ------------------------------------------------------------------------------------------------
 *                                           Global Variables
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                          Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

MAC_INTERNAL_API void macBeaconInit(void);
MAC_INTERNAL_API void macBeaconReset(void);
MAC_INTERNAL_API void macBeaconSetSched(macTx_t *pMsg);
MAC_INTERNAL_API uint8 macBeaconCheckSched(void);
MAC_INTERNAL_API void macBeaconRequeue(macTx_t *pMsg);
MAC_INTERNAL_API uint16 macBeaconCheckTxTime(void);
MAC_INTERNAL_API void macBeaconSetupCap(uint8 sched, uint8 superframeOrder, uint16 beaconLen);
MAC_INTERNAL_API void macBeaconRetransmit(void);

#endif /* MAC_BEACON_H */
