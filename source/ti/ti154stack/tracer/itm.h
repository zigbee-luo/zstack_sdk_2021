/******************************************************************************

 @file  itm.h

 @brief Provides ITM module functions

 Group: WCS, LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2011-2021, Texas Instruments Incorporated
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

#ifndef ITM_H_
#define ITM_H_


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum
{
    ITM_9600 = 9600,
    ITM_57600 = 57600,
    ITM_115200 = 115200,
    ITM_230400 = 230400,
    ITM_460800 = 460800,
    ITM_500000 = 500000,
    ITM_576000 = 576000,
    ITM_921600 = 921600,
    ITM_1000000 = 1000000,
    ITM_1152000 = 1152000,
    ITM_1500000 = 1500000,
    ITM_2000000 = 2000000,
    ITM_2500000 = 2500000,
    ITM_3000000 = 3000000,
    ITM_3500000 = 3500000,
    ITM_4000000 = 4000000,
    ITM_6000000 = 6000000
} ITM_baudrate;


typedef struct _ITM_config_
{
    uint_fast32_t systemClock;
    ITM_baudrate  baudRate;
} ITM_config;


typedef enum
{
    ITM_TS_DIV_NONE = 0,
    ITM_TS_DIV_4 = 1,
    ITM_TS_DIV_16 = 2,
    ITM_TS_DIV_64 = 3
} ITM_tsPrescale;

typedef enum
{
    ITM_SYNC_NONE = 0,
    ITM_SYNC_16M_CYCLES = 1,
    ITM_SYNC_64M_CYCLES = 2,
    ITM_SYNC_256M_CYCLES = 3
} ITM_syncPacketRate;

//*****************************************************************************
//
//! Initialize the ITM module.
//!
//! \param itm_config provides configuration parameters.
//!
//! This function will initialize the ITM/TPIU module for UART SWO mode.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_initModule(const ITM_config itm_config);

//*****************************************************************************
//
//! Enable ITM module.
//!
//! \param None.
//!
//! This function will enable ITM module for trace generation.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_enableModule();

//*****************************************************************************
//
//! Disable ITM module.
//!
//! \param None.
//!
//! This function will disable trace generation by ITM module.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_disableModule();

//*****************************************************************************
//
//! Generate string message.
//!
//! \param msg Null terminated string to transmit.
//! \param port Stimulus port number to use for transmission.
//!
//! This function will write out the characters to the stimulus port specified.
//! This function is blocking which means the function will not return until
//! all characters are transferred.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_putStringBlocking(const char* msg, const uint8_t port);

//*****************************************************************************
//
//! Generate value string message.
//!
//! \param value integer to be preonted in hex
//! \param port Stimulus port number to use for transmission.
//!
//! This function will write out the characters to the stimulus port specified.
//! This function is blocking which means the function will not return until
//! all characters are transferred.
//!
//! \return None.
//
//*****************************************************************************
void ITM_putValueBlocking(uint32_t value, const uint8_t port);

//*****************************************************************************
//
//! Generate new line.
//!
//! \param port Stimulus port number to use for transmission.
//!
//! This function will write out the characters to the stimulus port specified.
//! This function is blocking which means the function will not return until
//! all characters are transferred.
//!
//! \return None.
//
//*****************************************************************************
void ITM_putNewLineBlocking(const uint8_t port);

//*****************************************************************************
//
//! Generate 32-bit word message.
//!
//! \param msg 32-bit word.
//! \param port Stimulus port number to use for transmission.
//!
//! This function will write out the 32-bit word to the stimulus port
//! specified. This function is blocking which means the function will not
//! return until all data is transferred.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_putWordBlocking(const uint32_t msg, const uint8_t port);

//*****************************************************************************
//
//! Enable exception trace.
//!
//! \param None.
//!
//! This function will generate hardware events for exception entry, exit and
//! return.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_enableExceptionTrace();

//*****************************************************************************
//
//! Enable Program Counter sampling.
//!
//! \param None.
//!
//! This function will generate hardware events for periodically sampled PC.
//! The sampling period has been set to 1024 system clock cycles.
//! return.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_enablePCSampling();

//*****************************************************************************
//
//! Enable data access event generation.
//!
//! \param variable Address of the variable to profile.
//!
//! This function will generate a hardware event for data address reads and
//! writes. This function can be used for profiling variables.
//!
//! \return true if watchpoint resource is available to program, else is false.
//
//*****************************************************************************
extern bool ITM_enableDataTrace(const uint32_t *variable);

//*****************************************************************************
//
//! Enable cycle count event generation.
//!
//! \param None
//!
//! This function will generate a hardware event every time 1024 system clock
//! cycles have elapsed.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_enableCycleCounter();

//*****************************************************************************
//
//! Enable timestamp generation.
//!
//! \param tsPrescale Timestamp scale factor.
//!
//! This function will generate local timestamp packets for time correlation.
//! The timestamp interval is set to the input value which means the local
//! timestamp counter is running at system clock/tsPrescale frequency. Lowering
//! the timestamp counter frequency (i.e using higher prescale factor) results
//! in lower timing resolution, however it helps reduce the volume of trace data
//! generated making it bandwidth efficient.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_enableTiming(ITM_tsPrescale tsPrescale);

//*****************************************************************************
//
//! Enable sync packet generation.
//!
//! \param syncPacketRate Rate at which sync packets are generated.
//!
//! This function will generate synchronization packets at specified intervals.
//! This function is necessary if the host side decoder is expected to connect
//! and decode from the middle of a live stream. The synchronization packet
//! helps host decoder identify the starting point to begin decoding given that
//! SWO trace is a byte based protocol.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_enableSyncPackets(ITM_syncPacketRate syncPacketRate);


//*****************************************************************************
//
//! Enable sync packet generation.
//!
//! \param majorVersion Major version.
//! \param majorVersion Minor version.
//!
//! This function returns the library version.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_getLibraryVersion(uint32_t *majorVersion, uint32_t *minorVersion);


//*****************************************************************************
//
//! Flush ITM module.
//!
//! \param None.
//!
//! This function will wait till data is flushed from the ITM. This API must be
//! called before core enters sleep.
//!
//! \return None.
//
//*****************************************************************************
extern void ITM_flushModule();
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* ITM_H_ */
