/******************************************************************************

 @file rom_test.c

 @brief TIMAC 2.0 FH API

 Group: WCS LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2016-2021, Texas Instruments Incorporated
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

/******************************************************************************
 Includes
 *****************************************************************************/


#include "hal_types.h"
#include "mac_main.h"
#include "mac_data.h"
#include "fh_api.h"
#include "fh_pib.h"
#include "fh_nt.h"
#include "fh_data.h"
#include "fh_util.h"
#include "fh_mgr.h"
#include "fh_sm.h"
#include "fh_ie.h"
#include "string.h"
#include "mac_high_level.h"
#include "fh_dh1cf.h"

/* LMAC function prototypes */
#include "mac.h"
#include "mac_rx.h"
#include "mac_radio.h"
#include "mac_tx.h"
#include "mac_mcu.h"
#include "mac_mgmt.h"
#include "mac_spec.h"
#include "osal_memory.h"

// SW Tracer
#include "dbg.h"
#ifdef DEBUG_SW_TRACE
#define DBG_ENABLE
#ifndef _DBGFILE
#define _DBGFILE fh_api_c
#endif
#include "dbgid_sys_mst.h"
#endif // DEBUG_SW_TRACE

//#pragma LOCATION(RAM_BASE_ADDR,0x20000200)
//uint32_t RAM_BASE_ADDR[4];


/*!
 This module is the FH module interface for the HMAC and LMAC. All function
 APIS in this file can be used by HMAC and LMAC.
 */
uint32_t x = 30;
//uint32_t y = *(uint32 *) (0x4000);

/******************************************************************************
 Constants and definitions
 *****************************************************************************/

/******************************************************************************
 Local variables
 *****************************************************************************/

/******************************************************************************
 Local Function Prototypes
 *****************************************************************************/

/******************************************************************************
 Glocal variables
 *****************************************************************************/
#include "rom_jt_154.h"

#if 0
#define MAP_test_func2      ( (void)(*)(int) (uint32 *)(0x50000))
void test_fh_func2(int x1)
{
    static int x=0;
    x++;
}

#define MAP_test_func2      ( (void (*)(int) ) test_fh_func2 )
void test_fh_func(void)
{
    static int x=0;
    x++;
    test_fh_func2(x);
    MAP_test_func2(10);
    // test_fh_func(FH_HND *pFhHnd, ...)
    //              HMAC_HND *pHmacHnd
    //              MAC_HND *pHnd
    // 1. function call first para ==> handler
    // 2. FH, HMAC,LMAC replacement

    //pFhHnd->test_func2();
    //pHmacHnd->test_func2();

    //RAM_BASE_ADDR[0] = 0x2001;
}
#endif


