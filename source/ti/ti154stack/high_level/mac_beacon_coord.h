/******************************************************************************

 @file  mac_beacon_coord.h

 @brief Interface to coordinator only procedures for beacon enabled networks.

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

#ifndef MAC_BEACON_COORD_H
#define MAC_BEACON_COORD_H

/* ------------------------------------------------------------------------------------------------
 *                                              Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "mac_high_level.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Constants
 * ------------------------------------------------------------------------------------------------
 */

/* Time before beacon transmission to prepare beacon, in symbols */
#define MAC_BEACON_PREPARE_OFFSET     64

/* The timer alignment is initialized using this offset when a network is started, in symbols */
#define MAC_BEACON_START_OFFSET       32

/* Extra time to allow for battery life extension to work, in symbols */
#define MAC_BEACON_BATT_LIFE_OFFSET   32

/* Time before enhanced beacon transmission to prepare beacon, in symbols */
#define MAC_EBEACON_PREPARE_OFFSET    160

#define MAC_EBEACON_NOT_STARTED       0x01  /* The enhanced beacon is prepared */
#define MAC_EBEACON_PREPARED          0x02  /* The enhanced beacon is prepared */
#define MAC_EBEACON_QUEUED            0x04  /* The cmd to tx beacon has been queued */
#define MAC_EBEACON_TX                0x08  /* The enhanced beacon has been tx */

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

typedef struct
{
  macTimer_t      txTimer;            /* timer for beacon transmission */
  macTimer_t      prepareTimer;       /* timer for beacon preparation */
  macTimer_t      battLifeTimer;      /* timer for battery life extension */
  uint32          prepareTime;        /* beacon preparation time in superframe */
  macTx_t         *pBeacon;           /* pointer to beacon to be transmitted */
#ifdef FEATURE_ENHANCED_BEACON
  macTimer_t      eBeaconTxTimer;     /* timer for enhanced beacon transmission */
  macTimer_t      eBeaconPrepareTimer;/* timer for enhanced beacon transmission */
  uint32          eBeaconPrepareTime; /* ebeacon preparation time */
#endif /* FEATURE_ENHANCED_BEACON */
  uint8           origPhyId;          /* original phy Id */
  uint8           origSched;          /* original superframe schedule */
  uint8           eBeaconStatus;      /* Flags indicating enhanced beacon status */

} macBeaconCoord_t;


/* ------------------------------------------------------------------------------------------------
 *                                           Function Prototypes
 * ------------------------------------------------------------------------------------------------
 */

MAC_INTERNAL_API void macApiBeaconStartReq(macEvent_t *pEvent);
MAC_INTERNAL_API void macBeaconStartContinue(macEvent_t *pEvent);
MAC_INTERNAL_API void macBeaconSetupBroadcast(void);
MAC_INTERNAL_API void macOutgoingNonSlottedTx(void);
MAC_INTERNAL_API void macBeaconSchedRequested(void);
MAC_INTERNAL_API void macBeaconClearIndirect(void);
MAC_INTERNAL_API void macTxBeaconCompleteCallback(uint8 status);
MAC_INTERNAL_API void macBeaconPrepareCallback(uint8 param);

#endif /* MAC_BEACON_COORD_H */
