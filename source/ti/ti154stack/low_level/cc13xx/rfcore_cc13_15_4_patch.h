/******************************************************************************

 @file  rfcore_cc13_15_4_patch.h

 @brief This file contains all patches that need to be applied for the
        RF core firmware (i.e. CM0, MCE and RFE)

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2013-2021, Texas Instruments Incorporated
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

#ifndef RFCORE_15_4_PATCH
#define RFCORE_15_4_PATCH

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * INCLUDES
 */

#include "hw_types.h"
#include "hw_rfc_pwr.h"
#include "rfHal.h"


#include "apply_patch.h" 
    
#ifndef NO_SINGLE_ENDED_MODE_SUPPORT
// Patch for MCE
#include "apply_zigbee_xs_is_mce_patch_partial.h"

#endif    

/*******************************************************************************
 * MACROS
 */

/*******************************************************************************
 * CONSTANTS
 */

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
 * @fn          patchRfCore API
 *
 * @brief       This subroutine is used to consolidate and execute 
 *              patching for the RF FW.
 *
 * input parameters
 *
 * @param       singleEndMode - TURE if single ended mode.
 *
 * output parameters
 *
 * @param       None.
 *
 * @return      None.
 */
void patchRfCore( bool singleEndedMode )
{
  RF_Stat stat;
  
#ifndef NO_SINGLE_ENDED_MODE_SUPPORT
  rfOpCmd_TopsmCopy_t topsmCopyCmd;
  rfOpImmedCmd_RW_RfReg_t writeRfRegCmd;
  uint32_t cpeIrqClrFlags;
#endif  

#ifndef NO_SINGLE_ENDED_MODE_SUPPORT
  if (singleEndedMode) {
    // PG 2 single ended mode
    memset(&topsmCopyCmd, 0, sizeof(topsmCopyCmd));  
    topsmCopyCmd.rfOpCmd.cmdNum    = CMD_TOPSM_COPY;
    topsmCopyCmd.rfOpCmd.status    = RFSTAT_IDLE;
    topsmCopyCmd.rfOpCmd.pNextRfOp = NULL;
    CLR_RFOP_ALT_TRIG_CMD( topsmCopyCmd.rfOpCmd.startTrig );
    SET_RFOP_TRIG_TYPE( topsmCopyCmd.rfOpCmd.startTrig, TRIGTYPE_NOW );
    SET_RFOP_COND_RULE( topsmCopyCmd.rfOpCmd.condition, COND_NEVER );
    topsmCopyCmd.mceBank           = 1;          // Bank containing 15.4 code
    topsmCopyCmd.rfeBank           = -1;         // No RFE patch
    topsmCopyCmd.mceStopAddr       = ZIGBEE_XS_IS_MCE_NUM_COPY;  // Number of instructions to copy
    topsmCopyCmd.rfeStopAddr       = 0;
  
    // Clear interrupt flags
    cpeIrqClrFlags = MB_RFCPEIFG_REG & (MB_COMMAND_DONE_INT | MB_LAST_COMMAND_DONE_INT);
    if (cpeIrqClrFlags) {
      do {
        MB_RFCPEIFG_REG = ~cpeIrqClrFlags;
      } while (MB_RFCPEIFG_REG & cpeIrqClrFlags);
   }
  
   stat = RF_runImmediateCmd(RF_handle, (uint32_t*) &topsmCopyCmd);
  }
#endif // NO_SINGLE_ENDED_MODE_SUPPORT
  
  applyPatch();

#ifndef NO_SINGLE_ENDED_MODE_SUPPORT
  if (singleEndedMode) {
    // Force enable MCE RAM
    stat = RF_runDirectCmd(RF_handle,
      (uint32_t) BUILD_DIRECT_PARAM_EXT_CMD(CMD_FORCE_CLK_ENA,
                                            RFC_PWR_PWMCLKEN_RFERAM));

    // Wait for CMD_TOPSM_COPY to finish
    while( !(MB_RFCPEIFG_REG & (MB_COMMAND_DONE_INT | MB_LAST_COMMAND_DONE_INT)) );
  
    enterZigbeeXsIsMcePatch();

    // Clear interrupt flags
    cpeIrqClrFlags = MB_RFCPEIFG_REG & (MB_COMMAND_DONE_INT | MB_LAST_COMMAND_DONE_INT);
    if (cpeIrqClrFlags) {
      do {
        MB_RFCPEIFG_REG = ~cpeIrqClrFlags;
      } while (MB_RFCPEIFG_REG & cpeIrqClrFlags);
    }
    // Stop force enabling RAMs
    stat = RF_runDirectCmd(RF_handle,
      (uint32_t) BUILD_DIRECT_PARAM_EXT_CMD(CMD_FORCE_CLK_ENA, 0));
  }
#endif // NO_SINGLE_ENDED_MODE_SUPPORT

  return;
}


#ifdef __cplusplus
}
#endif

#endif /* RFCORE_15_4_PATCH */
