/******************************************************************************

 @file  osaltasks.c

 @brief Settings and other functions that the user could set and change.

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

/*********************************************************************
 * INCLUDES
 */
#include <icall.h>
#include <icall_cc26xx_defs.h>
#include "zcomdef.h"

#include "osal.h"
#include "osal_tasks.h"
#include "osal_snv.h"

#include "ti_drivers_config.h"

#include "mac_user_config.h"
#include "crypto_mac_api.h"
#include <chipinfo.h>

#include "macstack.h"
#include "mac_low_level.h"

#include "hal_mcu.h"
#include "hw_ints.h"
#include "hal_trng_wrapper.h"

#ifdef FEATURE_MAC_SECURITY
#include "hal_aes.h"
#endif

#if defined DEBUG_SW_TRACE || defined DBG_ENABLE
/* Header files required for tracer enabling */
#include <ioc.h>
#include <hw_ioc.h>
#include <hw_memmap.h>
#endif // defined DEBUG_SW_TRACE || defined DBG_ENABLE

#ifdef TIMAC_ROM_IMAGE_BUILD
extern void TIMAC_ROM_Init( void );
#endif

/*********************************************************************
 * GLOBAL VARIABLES
 */

// The order in this table must be identical to the task initialization calls below in osalInitTask.
const pTaskEventHandlerFn tasksArr[] = {
  macStackEventLoop
};

const uint8 tasksCnt = sizeof( tasksArr ) / sizeof( tasksArr[0] );
uint16 *macTasksEvents;

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/


/*********************************************************************
 * @fn      osalInitTasks
 *
 * @brief   This function invokes the initialization function for each task.
 *
 * @param   void
 *
 * @return  none
 */
void osalInitTasks( void )
{
  uint8 taskID = 0;

  macTasksEvents = (uint16 *)osal_mem_alloc( sizeof( uint16 ) * tasksCnt);
  if (macTasksEvents == NULL)
  {
    /* Abort if memory alloc fails */
    ICall_abort();
  }
  osal_memset( macTasksEvents, 0, (sizeof( uint16 ) * tasksCnt));

  /* Initialize OSAL tasks */
  macStackTaskInit(taskID);
}

/**
 * Main entry function for the stack image
 */
int stack_main( void *arg )
{
  halIntState_t state;
#ifdef FEATURE_MAC_SECURITY
#if !defined(DeviceFamily_CC13X2) && !defined(DeviceFamily_CC26X2)
  CryptoCC26XX_Params CryptoCC26XXParams;
#else
  AESCCM_Params AESCCMParams;
#endif
#endif /* FEATURE_MAC_SECURITY */

#ifdef TIMAC_ROM_IMAGE_BUILD
  TIMAC_ROM_Init();
#endif

#if defined( DEBUG_SW_TRACE )
  #if defined(HAL_UART) && (HAL_UART==TRUE)
    // Enable tracer output on DIO24/ATEST1.
    // Set max drive strength and reduced slew rate.
    // PortID = 46 = RFcore tracer.
    HWREG(IOC_BASE + IOC_O_IOCFG24) = (3 << IOC_IOCFG24_IOSTR_S) | (1 << IOC_IOCFG24_SLEW_RED_S) | 46;

  #else // no UART so allow trace on Tx pin - Debug Package only

    // Enable tracer output on DIO23/ATEST0 or DIO24/ATEST1:
    // For 7x7 EM on SmartRF06EB:
    // Port ID for RF Tracer is 46
    // [see ioc.h: #define IOC_PORT_RFC_TRC  0x0000002E // RF Core Tracer]

    // DIO 23 maps to IOID 8
    // [see Board.h: #define Board_SPI0_MISO IOID_8     /* RF1.20 */]
    HWREG(IOC_BASE + IOC_O_IOCFG23) = (3 << IOC_IOCFG23_IOSTR_S) | 46;

    // OR

    // DIO 24 maps to IOID 24
    // [see Board.h: #define Board_SPI1_MISO IOID_24   /* RF2.10 */]
    //HWREG(IOC_BASE + IOC_O_IOCFG24) = (3 << IOC_IOCFG24_IOSTR_S) | 46;

  #endif // HAL_UART
#endif // DEBUG_SW_TRACE

  /* User App Reconfiguration of TIMAC */
  macSetUserConfig( arg );

  /* Register an entity to use timer service to implement OSAL timer */
  if (ICall_registerApp(&osal_entity, &osal_semaphore) !=
      ICALL_ERRNO_SUCCESS)
  {
    /* abort */
    ICall_abort();
  }

  /* Check device revision */
  if (((HwRevision_t (*)(void)) ((macUserCfg_t*) arg)->getHwRevision)() > HWREV_2_0)
  {
    /* PG2.0 and PG2.1 devices are supported by this version of software.
     * This stub is reserved for future HW version check if it becomes necessary.
     */
  }

  HAL_ENTER_CRITICAL_SECTION(state);

  // Have to turn on the TRNG power before HalTRNG_InitTRNG
  // but must not repeat it every time the device wakes up
  // hence the call cannot be added to HalTRNG_InitTRNG();

  ICall_pwrRequire(ICALL_PWR_D_PERIPH_TRNG);

  HalTRNG_InitTRNG();

#ifdef FEATURE_MAC_SECURITY
#if !defined(DeviceFamily_CC13X2) && !defined(DeviceFamily_CC26X2)
  extern CryptoCC26XX_Handle Crypto_handle;

  CryptoCC26XX_init();
  CryptoCC26XX_Params_init(&CryptoCC26XXParams);
  Crypto_handle = CryptoCC26XX_open(Board_CRYPTO0, false, &CryptoCC26XXParams);
  if (!Crypto_handle)
  {
    /* abort */
    ICall_abort();
  }

  HalAesInit();
#else
  extern AESCCM_Handle AESCCM_handle;

  AESCCM_init();
  AESCCM_Params_init(&AESCCMParams);
  AESCCMParams.returnBehavior = AESCCM_RETURN_BEHAVIOR_POLLING;
  AESCCM_handle = AESCCM_open(CONFIG_AESCCM_0, &AESCCMParams);
  if (!AESCCM_handle)
  {
    /* abort */
    ICall_abort();
  }
#endif
#endif /* FEATURE_MAC_SECURITY */

#ifdef ICALL_LITE
{
  extern void icall_slimMsgParser(void * msg);
  //osal_set_icall_hook((osal_icallMsg_hook_t*)icall_slimMsgParser);
  
  // use the common component from RTM branch
  osal_set_icall_hook((osal_icallMsg_hook_t)icall_slimMsgParser);
}
#endif /* ICALL_SLIM */
  /* Initialize MAC */
  MAC_Init();

#ifdef COPROCESSOR
  /* Initialize the MAC function tables and features */
  MAC_InitDevice();
#ifdef FEATURE_FULL_FUNCTION_DEVICE
  MAC_InitCoord();
#endif /* FEATURE_FULL_FUNCTION_DEVICE */
#else
  /* Initialize the MAC function tables and features */

#ifdef FEATURE_FULL_FUNCTION_DEVICE
  MAC_InitCoord();
#else /* FEATURE_FULL_FUNCTION_DEVICE */
  MAC_InitDevice();
#endif
#endif

#ifdef FEATURE_BEACON_MODE
  MAC_InitBeaconDevice();
#ifdef FEATURE_FULL_FUNCTION_DEVICE
  MAC_InitBeaconCoord();
#endif /* FEATURE_FULL_FUNCTION_DEVICE */
#endif /* FEATURE_BEACON_MODE */

#ifdef FEATURE_FREQ_HOP_MODE
  MAC_InitFH();
#endif /* FEATURE_FREQ_HOP_MODE */
  // Initialize the operating system
  osal_init_system();

   /* Initialize MAC buffer */
  macLowLevelBufferInit();

  /* Enable interrupts */
  HAL_EXIT_CRITICAL_SECTION(state);

  // Must be done last
  macLowLevelInit();

  osal_start_system(); // No Return from here

  return 0;  // Shouldn't get here.
}

/*********************************************************************
*********************************************************************/
