/*******************************************************************************
  Filename:       R2F_FlashJT.h
  Revised:        $Date: 2014-07-28 16:14:51 -0700 (Mon, 28 Jul 2014) $
  Revision:       $Revision: 39541 $

  Description:    This file contains the defines for every flash function call
                  using the ROM-to-Flash Flash Jump Table.

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

#ifndef R2F_FLASH_JT_H
#define R2F_FLASH_JT_H

#if defined( ROM_BUILD )

/*******************************************************************************
 * EXTERNS
 */

// ROM's RAM table for pointers to ICall functions and flash jump tables.
// Note: This linker imported symbol is treated as a variable by the compiler.
// 0: iCall Dispatch Function Pointer
// 1: iCall Enter Critical Section Function Pointer
// 2: iCall Leave Crtiical Section Function Pointer
// 3: R2F Flash Jump Table Pointer
// 4: R2R Flash Jump Table Pointer
extern uint32 RAM_BASE_ADDR[];

/*******************************************************************************
 * INCLUDES
 */

#include "hal_types.h"

/*******************************************************************************
 * CONSTANTS
 */

// ROM's RAM table offset to R2F flash jump table pointer.
#define ROM_RAM_TABLE_R2F          3

// Defines used for the flash jump table routines that are not part of build.
// Note: Any change to this table must accompany a change to R2F_Flash_JT[]!
#define R2F_JT_LOCATION            (&RAM_BASE_ADDR[ROM_RAM_TABLE_R2F])

#define R2F_JT_BASE                (*((uint32 **)R2F_JT_LOCATION))
#define R2F_JT_OFFSET(index)       (*(R2F_JT_BASE+(index)))

// ROM-to-Flash Functions
#define osal_memcpy             ((void  * (*) (void *, const void *, uint32))       R2F_JT_OFFSET(0))
#define IntMasterEnable         ((uint8   (*) (void))                               R2F_JT_OFFSET(1))
#define IntMasterDisable        ((uint8   (*) (void))                               R2F_JT_OFFSET(2))
#define osal_bm_free            ((void    (*) (void *))                             R2F_JT_OFFSET(3))
#define osal_mem_alloc          ((void  * (*) (uint16))                             R2F_JT_OFFSET(4))
#define osal_mem_free           ((void    (*) (void *))                             R2F_JT_OFFSET(5))
#define IntDisable              ((void    (*) (uint32))                             R2F_JT_OFFSET(6))
#define IntEnable               ((void    (*) (uint32))                             R2F_JT_OFFSET(7))
#define macBackoffTimerCount         ((uint32  (*) (void))                          R2F_JT_OFFSET(8))
#define macBackoffTimerSetTrigger    ((void    (*) (uint32))                        R2F_JT_OFFSET(9))
#define macBackoffTimerCancelTrigger ((void    (*) (void))                          R2F_JT_OFFSET(10))
#define halAssertHandler             ((void    (*) (void))                          R2F_JT_OFFSET(11))
#define macBackoffTimerSetRollover   ((void    (*) (uint32))                        R2F_JT_OFFSET(12))
#define macBackoffTimerSetCount      ((void    (*) (uint32))                        R2F_JT_OFFSET(13))
#define macBackoffTimerRealign       ((int32   (*) (macRx_t *))                     R2F_JT_OFFSET(14))
#define macBackoffTimerGetTrigger    ((uint32  (*) (void))                          R2F_JT_OFFSET(15))
#define osal_memcmp             ((uint8   (*) (const void *, const void *, uint32)) R2F_JT_OFFSET(16)) 
#define zaesccmDecryptAuth      ((int8    (*) (uint8, uint8, uint8 *, uint8 *, uint16, uint8 *, uint16, uint8 *, uint8 *, uint8)) R2F_JT_OFFSET(17)) 
#define zaesccmAuthEncrypt      ((int8    (*) (uint8, uint8, uint8 *, uint8 *, uint16, uint8 *, uint16, uint8 *, uint8 *, uint8)) R2F_JT_OFFSET(18)) 
#define macRadioStartScan            ((void    (*) (uint8))                         R2F_JT_OFFSET(19))
#define macRxEnable                  ((void    (*) (uint8))                         R2F_JT_OFFSET(20))
#define macRadioEnergyDetectStop     ((uint8   (*) (void))                          R2F_JT_OFFSET(21))
#define macRadioEnergyDetectStart    ((void    (*) (void))                          R2F_JT_OFFSET(22))
#define MAC_CbackEvent               ((void    (*) (macCbackEvent_t *))             R2F_JT_OFFSET(23))
#define macRadioStopScan             ((void    (*) (void))                          R2F_JT_OFFSET(24))
#define macRxDisable                 ((void    (*) (uint8))                         R2F_JT_OFFSET(25))
#define macSleep                     ((uint8   (*) (uint8))                         R2F_JT_OFFSET(26))
#define macSleepWakeUp               ((void    (*) (void))                          R2F_JT_OFFSET(27))
#define macRadioRandomByte           ((uint8   (*) (void))                          R2F_JT_OFFSET(28))
#define macRadioSetPanID             ((void    (*) (uint16))                        R2F_JT_OFFSET(29))
#define macRadioSetShortAddr         ((void    (*) (uint16))                        R2F_JT_OFFSET(30))
#define macRadioSetChannel           ((void    (*) (uint8))                         R2F_JT_OFFSET(31))
#define macRadioSetIEEEAddr          ((void    (*) (uint8 *))                       R2F_JT_OFFSET(32))
#define macRadioSetTxPower           ((uint8   (*) (uint8))                         R2F_JT_OFFSET(33))
#define macLowLevelReset             ((void    (*) (void))                          R2F_JT_OFFSET(34))
#define macRadioSetPanCoordinator    ((void    (*) (uint8))                         R2F_JT_OFFSET(35))
#define macLowLevelResume            ((void    (*) (void))                          R2F_JT_OFFSET(36))
#define macLowLevelYield             ((bool    (*) (void))                          R2F_JT_OFFSET(37))
#define macLowLevelInit              ((void    (*) (void))                          R2F_JT_OFFSET(38))
#define osal_msg_receive        ((uint8 * (*) (uint8))                              R2F_JT_OFFSET(39))
#define osal_set_event          ((uint8   (*) (uint8, uint16))                      R2F_JT_OFFSET(40))
#define osal_msg_allocate       ((uint8 * (*) (uint16))                             R2F_JT_OFFSET(41))
#define osal_msg_send           ((uint8   (*) (uint8, uint8 *))                     R2F_JT_OFFSET(42))
#define osal_msg_dequeue        ((void  * (*) (void *))                             R2F_JT_OFFSET(43))
#define macTasksEvents             ((uint16 *)                                         R2F_JT_OFFSET(44))
#define macRxSoftEnable              ((void    (*) (uint8))                         R2F_JT_OFFSET(45))
#define MAC_CbackCheckPending        ((uint8   (*) (void))                          R2F_JT_OFFSET(46))
#define osal_msg_enqueue        ((void    (*) (void *, void *))                     R2F_JT_OFFSET(47))
#define MAC_CbackQueryRetransmit     ((uint8   (*) (void))                          R2F_JT_OFFSET(48))
#define macTxFrameRetransmit         ((void    (*) (void))                          R2F_JT_OFFSET(49))
#define osal_msg_extract        ((void    (*) (void *, void *, void*))              R2F_JT_OFFSET(50))
#define macTxFrame                   ((void    (*) (uint8))                         R2F_JT_OFFSET(51))
#define osal_msg_enqueue_max    ((uint8   (*) (void *, void *, uint8))              R2F_JT_OFFSET(52))
#define macCfg                       (*(macCfg_t *)                                 R2F_JT_OFFSET(53))
#define osal_memset             ((void  * (*) (void *, uint8, int))                 R2F_JT_OFFSET(54))
#define osal_msg_deallocate     ((uint8   (*) (uint8 *))                            R2F_JT_OFFSET(55))
#define osal_msg_push           ((void    (*) (void *, void *))                     R2F_JT_OFFSET(56))
#define macRxHardDisable             ((void    (*) (void))                          R2F_JT_OFFSET(57))
#define macTxSlottedDelay            (*(uint8 const *)                              R2F_JT_OFFSET(58))
#define macMcuRandomByte        ((uint8   (*) (void))                               R2F_JT_OFFSET(59))
#define macCanSleep             ((uint8   (*) (void))                               R2F_JT_OFFSET(60))
#define osal_pwrmgr_task_state  ((uint8   (*) (uint8, uint8))                       R2F_JT_OFFSET(61))
#define macLowLevelDiags        ((void    (*) (uint8))                              R2F_JT_OFFSET(62))
#define MAC_SrcMatchDeleteEntry ((uint8   (*) (sAddr_t *, uint16))                  R2F_JT_OFFSET(63))
#define MAC_SrcMatchAddEntry    ((uint8   (*) (sAddr_t *, uint16))                  R2F_JT_OFFSET(64))
#define MAC_SrcMatchAckAllPending ((void  (*) (uint8))                              R2F_JT_OFFSET(65))
#define MAC_SrcMatchCheckAllPending ((uint8 (*) (void))                             R2F_JT_OFFSET(66))

#endif // ROM_BUILD

#endif /* R2F_FLASH_JT_H */



