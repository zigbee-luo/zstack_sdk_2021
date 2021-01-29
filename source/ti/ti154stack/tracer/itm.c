/******************************************************************************

 @file  itm.c

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

/* Standard Includes */
#include "itm.h"
#include "itm_private.h"
#include <stdint.h>

#include <stdbool.h>
#include <string.h>

#include <ioc.h>

#define ITM_STIM_PORT_8(x)    (*(volatile uint8_t *) ITM_STIM_PORT((x)))
#define ITM_STIM_PORT_32(x)   (*(volatile uint32_t *) ITM_STIM_PORT((x)))

void ITM_initModule(const ITM_config itm_config)
{
    // Disable module
    SCS_DEMCR &= (~SCS_DEMCR_TRCEN);
    ITM_TCR   = 0x00000000;

    // Enable trace
    SCS_DEMCR |= SCS_DEMCR_TRCEN;

    // Unlock and Setup TPIU for SWO UART mode
    TPIU_LAR   = CS_LAR_UNLOCK;
    TPIU_SPPR  = TPIU_SPPR_SWO_UART;
    TPIU_CSPSR = TPIU_CSPSR_PIN_1;

    // Unlock and enable all ITM stimulus ports with default settings
    ITM_LAR    = CS_LAR_UNLOCK;
    ITM_TER    = ITM_TER_ENABLE_ALL;
    ITM_TPR    = ITM_TPR_ENABLE_USER_ALL;

    // Setup Baud rate
    if (itm_config.systemClock)
    {
        uint32_t prescalar = itm_config.systemClock / itm_config.baudRate;

        // Offset with current prescalar value
        uint32_t diff1 = itm_config.systemClock - (prescalar * itm_config.baudRate);
        // Offset with prescalar+1 value
        uint32_t diff2 = ((prescalar+1) * itm_config.baudRate) - itm_config.systemClock;

        if (diff2 < diff1)  prescalar++;
        // Program prescalar value as (prescalar factor - 1)
        TPIU_ACPR = (prescalar - 1);
    }

    // Disable formatter
    TPIU_FFCR  = 0;

    // Unlock DWT
    DWT_LAR    = CS_LAR_UNLOCK;

    // Configure SWO Traces
    IOCPortConfigureSet(IOID_18, IOC_PORT_MCU_SWV, IOC_STD_OUTPUT);

}

void ITM_enableModule()
{
    // Enable ITM module
    ITM_TCR |= ITM_TCR_ENABLE_ITM;
}

void ITM_disableModule()
{
    // Disable ITM module
    ITM_TCR &= ~ITM_TCR_ENABLE_ITM;
}

void ITM_putStringBlocking(const char* msg, const uint8_t port)
{
    // Write one character at a time.
    while('\0' != *msg)
    {
        while(0 == ITM_STIM_PORT_8(port));
        ITM_STIM_PORT_8(port) = *msg++;
    }
}

void ITM_putValueBlocking(uint32_t value, const uint8_t port)
{
    char strValue[8];
    int8_t digitIdx;
    bool putStarted = false;

    for(digitIdx = 7; digitIdx >= 0; digitIdx--)
    {
        strValue[digitIdx] = (value & (0xF << digitIdx * 4)) >> (digitIdx*4);
        if(strValue[digitIdx] < 10)
        {
            strValue[digitIdx] += '0';
        }
        else
        {
            strValue[digitIdx] += 'a'-10;
        }

        if( (strValue[digitIdx] != '0') || putStarted )
        {
            while(0 == ITM_STIM_PORT_8(port));
            ITM_STIM_PORT_8(port) = strValue[digitIdx];
            putStarted = true;
        }
    }
}

void ITM_putNewLineBlocking(const uint8_t port)
{
    while(0 == ITM_STIM_PORT_8(port));
    ITM_STIM_PORT_8(port) = '\n';
}

void ITM_putWordBlocking(const uint32_t data, const uint8_t port)
{
    // Write the entire32-bit value
    while(0 == ITM_STIM_PORT_32(port));
    ITM_STIM_PORT_32(port) = data;
}

void ITM_enableExceptionTrace()
{
    DWT_CTRL |= DWT_CTRL_ENABLE_EXC_TRC;
    ITM_TCR  |= ITM_TCR_ENABLE_DWT_TX;
}

void ITM_enablePCSampling()
{
    // Clear the PC Sampling and Cycle Event bits
    DWT_CTRL &= ~(DWT_CTRL_ENABLE_PC_SAMP | DWT_CTRL_ENABLE_CYC_EVT);
    // Setup Sampling interval
    DWT_CTRL |= DWT_CTRL_CYC_CNT_1024;
    ITM_TCR  |= ITM_TCR_ENABLE_DWT_TX;
    DWT_CTRL |= DWT_CTRL_ENABLE_CYC_CNT;
    // Enable PC sampling event
    DWT_CTRL |= DWT_CTRL_ENABLE_PC_SAMP;
}

bool ITM_enableDataTrace(const uint32_t *variable)
{
    uint_least8_t numDwtComp = (DWT_CTRL & DWT_CTRL_MASK_NUM_COMP) >> DWT_CTRL_SHIFT_NUM_COMP;
    uint_least8_t dwtIndex = 0;
    bool dwtAvailable = false;
    for (dwtIndex = 0; dwtIndex < numDwtComp ; dwtIndex++)
    {
        if (0 == DWT_FUNC(dwtIndex))
        {
            DWT_COMP(dwtIndex) = (uint32_t)variable;
            DWT_MASK(dwtIndex) = 0x0;
            DWT_FUNC(dwtIndex) = (DWT_FUNC_DATA_SIZE_32 | DWT_FUNC_ENABLE_ADDR_OFFSET | DWT_FUNC_ENABLE_COMP_RW);
            dwtAvailable = true;
        }
    }
    return dwtAvailable;
}

void ITM_enableCycleCounter()
{
    // Clear the PC Sampling and Cycle Event bits
    DWT_CTRL &= ~(DWT_CTRL_ENABLE_PC_SAMP | DWT_CTRL_ENABLE_CYC_EVT);
    // Setup Sampling interval
    DWT_CTRL |= DWT_CTRL_CYC_CNT_1024;
    ITM_TCR  |= ITM_TCR_ENABLE_DWT_TX;
    DWT_CTRL |= DWT_CTRL_ENABLE_CYC_CNT;
    // Enable Cycle Count event
    DWT_CTRL |= DWT_CTRL_ENABLE_CYC_EVT;
}

void ITM_enableTiming(ITM_tsPrescale tsPrescale)
{
    // Set timestamp prescalar enable timestamp generation
    ITM_TCR   |= ((tsPrescale << ITM_TCR_TS_PRESCALE_SHIFT) & ITM_TCR_TS_PRESCALE_MASK);
    ITM_TCR   |= (ITM_TCR_ENABLE_TS);
}

void ITM_enableSyncPackets(ITM_syncPacketRate syncPacketRate)
{
    // Clear sync packet rate
    DWT_CTRL &= ~(DWT_CTRL_MASK_SYNCTAP);
    // Set sync packet rate
    DWT_CTRL |= ((syncPacketRate << DWT_CTRL_SHIFT_SYNCTAP) & DWT_CTRL_MASK_SYNCTAP);
    // Enable sync packet generation
    DWT_CTRL |= DWT_CTRL_ENABLE_CYC_CNT;
    ITM_TCR  |= (ITM_TCR_ENABLE_SYNC);
}

void ITM_getLibraryVersion(uint32_t *majorVersion, uint32_t *minorVersion)
{
    if (majorVersion && minorVersion)
    {
        *majorVersion = ITM_LIB_MAJOR_VER;
        *minorVersion = ITM_LIB_MINOR_VER;
    }
}

void ITM_flushModule()
{
    // Wait till the ITM Busy status has been cleared
    while (ITM_TCR & ITM_TCR_BUSY)
    {
        asm (" NOP");
    }
}
