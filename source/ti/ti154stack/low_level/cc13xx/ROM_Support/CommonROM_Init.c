/******************************************************************************

 @file  CommonROM_Init.c

 @brief This file contains the entry point for the Common ROM.

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2014-2021, Texas Instruments Incorporated
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

/*******************************************************************************
 * INCLUDES
 */

#include "hal_types.h"
#include "hw_types.h"
#include "CommonROM_Init.h"
//
#include "OnBoard.h"
#include "osal_bufmgr.h"
#include "hal_assert.h"

#ifdef OSAL_PORT2TIRTOS
#include <ICall.h>
#endif

/*******************************************************************************
 * EXTERNS
 */

// RAM address containing a pointer to the R2F flash jump table base address.
// Note: This linker imported symbol is treated as a variable by the compiler.

// ROM base address where the ROM's C runtime routine is expected.
extern uint32 COMMON_ROM_BASE_ADDR;

// RAM base address of a table a pointers that are used by ROM and which are
// initialized by ROM_Init.
extern uint32 COMMON_RAM_BASE_ADDR;

// Checksum start/end address and value for Common ROM.
extern const uint32 __checksum_begin_CommonROM;
extern const uint32 __checksum_end_CommonROM;

// Function pointers used by the Flash software (provided by RTOS).
extern ICall_Dispatcher ICall_dispatcher;
extern ICall_EnterCS    ICall_enterCriticalSection;
extern ICall_LeaveCS    ICall_leaveCriticalSection;

// calculate checksum routine
extern uint16 slow_crc16( uint16 crc, uint8 *pAddr, uint32 len );
extern uint8  validChecksum( const uint32 *beginAddr, const uint32 *endAddr );

/*******************************************************************************
 * PROTOTYPES
 */

/*******************************************************************************
 * MACROS
 */

// ICall function pointers and R2F/R2R flash JT pointers for Common ROM
#define icallCmnRomDispatchPtr (uint32 *)(&COMMON_RAM_BASE_ADDR+0)
#define icallCmnRomEnterCSPtr  (uint32 *)(&COMMON_RAM_BASE_ADDR+1)
#define icallCmnRomLeaveCSPtr  (uint32 *)(&COMMON_RAM_BASE_ADDR+2)
#define r2fCmnRomPtr           (uint32 *)(&COMMON_RAM_BASE_ADDR+3)
#define r2rCmnRomPtr           (uint32 *)(&COMMON_RAM_BASE_ADDR+4)

// Runtime Init code for ROM
#define RT_Init_CommonROM ((RT_Init_fp)&COMMON_ROM_BASE_ADDR)

/*******************************************************************************
 * CONSTANTS
 */

/*******************************************************************************
 * TYPEDEFS
 */

typedef void (*RT_Init_fp)(void);

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

// ROM-to-Flash Flash Jump Table
// Note: Any change here must accompany a change to R2F_CommonFlashJT.h defines!

#pragma data_alignment=4
const uint32 R2F_Flash_JT_CommonROM[] =
{
  // ROM-to-Flash Functions
  (uint32)osal_mem_alloc,                                 // R2F_JT_OFFSET[0]
  (uint32)osal_mem_free,                                  // R2F_JT_OFFSET[1]
  (uint32)osal_bm_free,                                   // R2F_JT_OFFSET[2]
  (uint32)halAssertHandler,                               // R2F_JT_OFFSET[3]
};

/*******************************************************************************
 * @fn          Common ROM Initialization
 *
 * @brief       This routine initializes the Common ROM software. First,
 *              the image's CRC is verified. Next, its C runtime is initialized.
 *              Then the ICall function pointers for dispatch, and enter/leave
 *              critical section are initialized. Finally, the location of the
 *              R2F and R2R flash jump tables are initialized.
 *
 * input parameters
 *
 * @param       None.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void CommonROM_Init( void )
{
  volatile uint8 i;

  // verify the Controller ROM image
  i = validChecksum(&__checksum_begin_CommonROM, &__checksum_end_CommonROM);

  // trap a checksum failure - what now?
  while( !i );

  // execute the Common ROM C runtime initializion
  // Note: This is the ROM's C Runtime initialization, not the flash's, which
  //       has already taken place.
  RT_Init_CommonROM();

  // initialize ICall function pointers for Common ROM
  // Note: The address of these functions is determined by the Application, and
  //       is passed to the Stack image via startup_entry.
  *icallCmnRomDispatchPtr = (uint32)ICall_dispatcher;
  *icallCmnRomEnterCSPtr  = (uint32)ICall_enterCriticalSection;
  *icallCmnRomLeaveCSPtr  = (uint32)ICall_leaveCriticalSection;

  // initialize RAM pointer to R2F Flash JT for Common ROM code
  *r2fCmnRomPtr = (uint32)R2F_Flash_JT_CommonROM;

  return;
}

/*******************************************************************************
 */

