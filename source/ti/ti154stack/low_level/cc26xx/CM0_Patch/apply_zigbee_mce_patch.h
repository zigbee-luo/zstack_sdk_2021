//------------------------------------------------------------------------------
// TI Confidential - NDA Restrictions
//
// Copyright (c) 2013 Texas Instruments, Inc.
//
//    This is an unpublished work created in the year stated above.
//    Texas Instruments owns all rights in and to this work and
//    intends to maintain and protect it as an unpublished copyright.
//    In the event of either inadvertent or deliberate publication,
//    the above stated date shall be treated as the year of first
//    publication. In the event of such publication, Texas Instruments
//    intends to enforce its rights in the work under the copyright
//    laws as a published work.
//
//------------------------------------------------------------------------------
//
// This file is auto-generated and should not be edited 
// Generated Mon Feb 17 15:52:56 2014 using the following files:
// /vobs/cc26xxFwDev/radio_domain/patch/topsm_zigbee/mce/src/mce_ram_bank.asm@@/main/8      Rule: /main/LATEST
// /vobs/cc26xxFwDev/radio_domain/patch/topsm_zigbee/mce/src/dbg.asm@@/main/1               Rule: /main/LATEST
// /vobs/cc26xxFwDev/radio_domain/patch/topsm_zigbee/mce/src/mce_commonlib.asm@@/main/3     Rule: /main/LATEST
// /vobs/cc26xxFwDev/radio_domain/patch/topsm_zigbee/mce/doc/mdmconf_802_15_4.txt@@/main/2  Rule: /main/LATEST
// /vobs/cc26xxIpDev/modules/cc26_rfcore/doc/MCEregs.txt@@/main/66          Rule: CC26_RFCORE--SOFTMACRO--DEV--0.8--N.A.08.01--GS60--2013.05.02
//

// This file implements patches for the MCE on CC26xx
// It should only be included from ONE source file to avoid duplicated constant arrays


#ifndef _APPLY_MCE_ZIGBEE_PATCH_H
#define _APPLY_MCE_ZIGBEE_PATCH_H

#include <stdint.h>

#ifndef MCE_PATCH_TYPE
#define MCE_PATCH_TYPE static const uint32_t
#endif

#ifndef MCE_ADDR_TYPE
#define MCE_ADDR_TYPE static const uint16_t
#endif

#ifndef PATCH_FUN_SPEC
#define PATCH_FUN_SPEC static inline
#endif

#define MCERAM_ADD_BASE 0x21008000  
#define ZIGBEE_MCE_NUM_COPY 0x03F8

MCE_ADDR_TYPE zigbeeMcePatchAdd[9] = {
   0,
   16,
   28,
   31,
   32,
   34,
   35,
   47,
   109
};

MCE_PATCH_TYPE zigbeeMcePatchDat[9] = {
   0xf703605f,
   0x2fc0c64c,
   0x00011400,
   0x002b0670,
   0x0a1116aa,
   0x40100000,
   0x00000040,
   0x72200000,
   0x1000a0c3
};

// Note that this function does NOT call the DUMP API function of the TOPsm,
// the ROM contents are assumed to be present in RAM
PATCH_FUN_SPEC void enterZigbeeMcePatch(void)
{
   uint32_t *pMceRam = (uint32_t *) MCERAM_ADD_BASE;
   for (int k = 0; k < 9; k++) {
     pMceRam[zigbeeMcePatchAdd[k]] = zigbeeMcePatchDat[k];
   }
}

#endif
