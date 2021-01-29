/*******************************************************************************
  Filename:       mac_isr.h
  Revised:        $Date: 2014-11-06 11:03:55 -0800 (Thu, 06 Nov 2014) $
  Revision:       $Revision: 41021 $

  Description:    This file contains the Interrupt Service Routines (ISR)
                  for the Bluetooth Low Energy CC26xx RF Core Firmware
                  Specification.

  Copyright 2009-2014 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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
*******************************************************************************/

#ifndef MAC_ISR_H
#define MAC_ISR_H

/*******************************************************************************
 * INCLUDES
 */
#include "hal_types.h"
#include "hal_mcu.h"

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

/* MAC Radio CPE Interrupts */
#define MB_FG_COMMAND_DONE_INT         BV(2)
#define MB_FG_LAST_COMMAND_DONE_INT    BV(3)
#define MB_RX_NOK_INT                  BV(17)
#define MB_RX_IGNORED_INT              BV(18)
#define MB_RX_OK_INT                   BV(16)
#if defined( CC26XX_PG1 )
#define MB_BG_COMMAND_SUSPENDED_INT    BV(4)
#define MB_TX_DONE_INT                 BV(5)
#define MB_TX_ACK_INT                  BV(6)
#define MB_RX_BUF_FULL_INT             BV(19)
#define MB_RX_ENTRY_DONE_INT           BV(20)
#elif defined( CC26XX )
#define MB_TX_DONE_INT                 BV(4)
#define MB_TX_ACK_INT                  BV(5)
#define MB_BG_COMMAND_SUSPENDED_INT    BV(12)
#define MB_RX_BUF_FULL_INT             BV(22)
#define MB_RX_ENTRY_DONE_INT           BV(23)
#define MB_MODULES_UNLOCKED_INT        BV(29)
#define MB_BOOT_DONE_INT               BV(30)
#else /* unknown device */
#error "ERROR: Unknown device!"
#endif /* CC26XX_PG1 */

/*******************************************************************************
 * TYPEDEFS
 */

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/*******************************************************************************
 * FUNCTIONS
 */
extern void cpe0IntCback( void );
extern void cpe1IntCback( void );
extern void hwIntCback( void );
extern void macRatChanCBack_A ( void );
extern void macRatChanCBack_B ( void );

/*******************************************************************************
 */

#endif /* BLE_ISR_H */
