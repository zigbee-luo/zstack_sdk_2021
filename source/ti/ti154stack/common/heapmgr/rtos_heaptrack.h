/******************************************************************************

 @file  rtos_heaptrack.h

 @brief Heap memory template based on teh TI-RTOS Heap MEm module.

    This file should be included in .c file to generate a heap memory
    management module with its own function names, heap size and, platform
    specific mutex implementation, etc. Note that although the idea of this
    file is the analogy of C++ template or ADA generic, this file can be
    included only once in a C file due to use of fixed type names and macro
    names.

 Group: WCS, LPC, BTS
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
#include <string.h>
#include <stdint.h>
#include <xdc/std.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/heaps/HeapTrack.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>
// This include file will bring HEAPMGR_SIZE and stackHeap is properly setup in the ti-rtos
// config file.
#include <xdc/cfg/global.h>

/* macros to override the function names for efficient linking and multiple instantiations */
#ifndef HEAPMGR_INIT
#define HEAPMGR_INIT   heapmgrInit
#endif

#ifndef HEAPMGR_MALLOC
#define HEAPMGR_MALLOC heapmgrMalloc
#endif

#ifndef HEAPMGR_FREE
#define HEAPMGR_FREE heapmgrFree
#endif

/* macro for debug assert */
#ifndef HEAPMGR_ASSERT
#define HEAPMGR_ASSERT(_exp)
#endif

#define FORCED_ALIGNEMENT 4

/* This limit is used when the API HEAPMGR_MALLOC_LIMITED is used.
 * It is the minimum number of free bytes that must be available in
 * the heap after the allocation is done (not taking the heap header into account).
 */
#ifndef HEAPMGR_FREE_SAFE_LIMIT
#ifdef HCI_TL_FULL
// Bigger to account for the maximum size of the TL Rx buffer which does not fully
// manage heap exhaustion yet.
#define HEAPMGR_FREE_SAFE_LIMIT  550
#else
#define HEAPMGR_FREE_SAFE_LIMIT  300
#endif
#endif

#ifndef HEAPMGR_PREFIXED
#define HEAPMGR_PREFIXED(_name) heapmgr ## _name
#endif

#ifdef HEAPMGR_METRICS
#define HEAPMGR_MEMALO HEAPMGR_PREFIXED(MemAlo)
#define HEAPMGR_MEMMAX HEAPMGR_PREFIXED(MemMax)
#define HEAPMGR_MEMFAIL HEAPMGR_PREFIXED(MemFail)
#endif
#define HEAPMGR_TOTALFREESIZE HEAPMGR_PREFIXED(MemFreeTotal)

/*********************************************************************
 * TYPEDEFS
 */
typedef struct Header_Custom {
   uint32_t size;
} Header_Custom;

/*********************************************************************
 * GLOBAL VARIABLES
 */
#ifdef HEAPMGR_METRICS
uint32_t HEAPMGR_MEMALO  = 0; // Current total memory allocated.
uint32_t HEAPMGR_MEMMAX  = 0; // Max total memory ever allocated at once.
uint16_t HEAPMGR_MEMFAIL = 0; // Memory allocation failure count
uint8_t  *HEAPMGR_HEAP;
#endif // HEAPMGR_METRICS
uint32_t HEAPMGR_TOTALFREESIZE = 0;

/*********************************************************************
 * EXTERNAL VARIABLES
 */
/* This variable is define in TI-RTOS */
extern const xdc_runtime_IHeap_Handle Memory_defaultHeapInstance;

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static uint8_t heapInitialize = 0;

#define HEAPMGR_OVERHEAD (24 + /*sizeof(HeapTrack_Header)*/ + FORCED_ALIGNEMENT + sizeof(Header_Custom))
/**
 * @brief   Initialize the heap memory management system.
 */
void HEAPMGR_INIT(void)
{
  uint8_t * temp;
  Memory_Stats memStats;

  if (heapInitialize == 1)
  {
    // Initialization already done. Only once initialization allowed.
    return;
  }
  heapInitialize = 1;
  // Since dynamic heap may be used, the RAM may not be initialize and contains
  // random data. for precausion, initialize the full buffer to 0xDD.
  HeapTrack_getStats((HeapTrack_Handle)stackHeap, &memStats);
  if (memStats.largestFreeSize == 0)
  {
    // This mean the Heap has not been properly created , is something else
    // than HeapTrack has been used?
    HEAPMGR_ASSERT();
  }
  // sizeof(HeapTrack_Header) need to be taken into account since it is not
  // taken into account by heapTrack (issue has been reported)
  temp = HEAPMGR_MALLOC(memStats.largestFreeSize - HEAPMGR_OVERHEAD);
  memset(temp,0x00, memStats.largestFreeSize - HEAPMGR_OVERHEAD);
  HEAPMGR_FREE(temp);

#ifdef HEAPMGR_METRICS
  {
    HEAPMGR_HEAP = NULL;
    HEAPMGR_MEMFAIL = 0;
    HEAPMGR_MEMMAX = 0;
  }
#endif // HEAPMGR_METRICS
}

/**
 * @brief   Implementation of the allocator functionality.
 * @param   size - number of bytes to allocate from the heap.
 * @return  void * - pointer to the heap allocation; NULL if error or failure.
 */
void *HEAPMGR_MALLOC(uint32_t size)
{
  Header_Custom *tmp;
  Error_Block eb;

  // Return NULL if size is 0
  if (size == 0)
  {
    return(NULL);
  }

  Error_init(&eb);
  /* Add room for the "malloc" like header */
  size += sizeof(Header_Custom);

  /* Using the default system heap API */
  tmp = Memory_alloc(HeapTrack_Handle_upCast(stackHeap), size, FORCED_ALIGNEMENT, &eb);

  if (tmp == NULL)
  {
#ifdef HEAPMGR_METRICS
    HEAPMGR_MEMFAIL++;
#endif // HEAPMGR_METRICS
#ifdef MEM_ALLOC_ASSERT
  // If allocation failed, assert.
  {
    // When EXT_HAL_ASSERT is defined, set the assert cause.  Otherwise, trap
    // the assert with argument FALSE.
#ifdef EXT_HAL_ASSERT
    HAL_ASSERT( HAL_ASSERT_CAUSE_OUT_OF_MEMORY );
#else /* !EXT_HAL_ASSERT */
    HAL_ASSERT( FALSE );
#endif /* EXT_HAL_ASSERT */
  }
#endif /* MEM_ALLOC_ASSERT */
    return(NULL);
  }
#ifdef HEAPMGR_METRICS
  else
  {
    HEAPMGR_MEMALO += size;
    if (HEAPMGR_MEMMAX < HEAPMGR_MEMALO)
    {
      HEAPMGR_MEMMAX = HEAPMGR_MEMALO;
    }
  }
#endif // HEAPMGR_METRICS
  HEAPMGR_TOTALFREESIZE -= size;

  /* Store the size to be used in the custom_free */
  tmp->size = size;

  /* Return the buffer, but skipping over the internal header */
  return((uint8_t *)tmp + sizeof(Header_Custom));

}

/**
 * @brief   Implementation of the de-allocator functionality.
 * @param   ptr - pointer to the memory to free.
 */
void HEAPMGR_FREE(void *ptr)
{
  void  *tmp;
  uint_least16_t  hwikey;

  if (ptr != NULL)
  {
    /* Get the internal header */
    tmp = (Header_Custom *)((uint8_t *)ptr - sizeof(Header_Custom));

    /* Protect since HeapMem_freeNoGate does not */
    hwikey = (uint_least16_t) Hwi_disable();

#ifdef HEAPMGR_METRICS
    HEAPMGR_MEMALO -= ((Header_Custom*)tmp)->size;
#endif // HEAPMGR_METRICS
    HEAPMGR_TOTALFREESIZE += ((Header_Custom*)tmp)->size;

    /* Using the default system heap API */
    Memory_free(HeapTrack_Handle_upCast(stackHeap), (Ptr)tmp, ((Header_Custom*)tmp)->size);

    /* Restore the Key */
    Hwi_restore(hwikey);
  }
}

/**
 * @brief   return statistic on the Heap:
 *           - heap size
 *           - total free size
 *           - biggest free buffer allocatable
 * @param   stats - pointer to memory to write the information into.
 */
void HEAPMGR_GETSTATS(ICall_heapStats_t *stats)
{
  if (stats == NULL)
  {
    return;
  }

  HEAPMGR_LOCK();
  HeapTrack_getStats(stackHeap, (Memory_Stats *) stats);
  if (stats->largestFreeSize > HEAPMGR_OVERHEAD)
  {
    stats->largestFreeSize -= HEAPMGR_OVERHEAD;
  }
  else
  {
    stats->largestFreeSize = 0;
  }
  HEAPMGR_UNLOCK();
}

/**
 * @brief   Implementation of the limited allocator functionality.
 * The allocation is done only if at least HEAPMGR_FREE_SAFE_LIMIT bytes
 * remain available after the allocation.
 * @param   size - number of bytes to allocate from the heap.
 * @return  void * - pointer to the heap allocation; NULL if error or failure.
 */
void *HEAPMGR_MALLOC_LIMITED(uint32_t size)
{
    if((HEAPMGR_TOTALFREESIZE - size) > HEAPMGR_FREE_SAFE_LIMIT)
    {
      return(HEAPMGR_MALLOC(size));
    }
    else
    {
      return(NULL);
    }
}

/**
 * @brief   HeapCallback function pointer:
 *          used in conjonction with the HeapCallback Module in TI-RTOS
 */
UArg myHeapTrackInitFxn(UArg arg)
{
    HEAPMGR_INIT();
    return(0xBABE);
}

/**
 * @brief   HeapCallback function pointer:
 *          used in conjonction with the HeapCallback Module in TI-RTOS
 */
Ptr myHeapTrackAllocFxn(UArg arg,SizeT size ,SizeT alignement)
{
   return(HEAPMGR_MALLOC(size));
}

/**
 * @brief   HeapCallback function pointer:
 *          used in conjonction with the HeapCallback Module in TI-RTOS
 */
void myHeapTrackFreeFxn(UArg arg, Ptr myPtr, SizeT size)
{
  HEAPMGR_FREE(myPtr);
}

/**
 * @brief   HeapCallback function pointer:
 *          used in conjonction with the HeapCallback Module in TI-RTOS
 */
void myHeapTrackGetStatsFxn(UArg arg, Memory_Stats * stats)
{
  HEAPMGR_GETSTATS((ICall_heapStats_t *)stats);
}

/**
 * @brief   HeapCallback function pointer:
 *          used in conjonction with the HeapCallback Module in TI-RTOS
 */
Bool myHeapTrackIsBlockingFxn(UArg arg)
{
    return(TRUE);
}

/*********************************************************************
*********************************************************************/
