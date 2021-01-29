//------------------------------------------------------------------------------
// TI Confidential - NDA Restrictions
//
// Copyright (c) 2014 Texas Instruments, Inc.
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
// Generated Fri Dec 19 16:19:08 2014 using the following files:
// /vobs/cc26xxFwDev/radio_domain/patch/pg2/topsm_zigbee/mce/src/mce_ram_bank.asm@@/main/1  Rule: /main/LATEST
// /vobs/cc26xxFwDev/radio_domain/mce/src/dbg.asm@@/main/1  Rule: CC26_RF_ROM_FW_MCE--DEV--1.0--N.A.00.18--2014.04.01
// /vobs/cc26xxFwDev/radio_domain/patch/pg2/topsm_zigbee/mce/src/mce_commonlib.asm@@/main/1                 Rule: /main/LATEST
// /vobs/cc26xxFwDev/radio_domain/patch/pg2/topsm_zigbee/mce/doc/mdmconf_802_15_4.txt@@/main/2              Rule: /main/LATEST
// /vobs/cc26xxIpDev/modules/cc26_rfcore/doc/MCEregs.txt@@/main/79          Rule: /main/LATEST
//

// This file implements patches for the MCE on CC26xx
// It should only be included from ONE source file to avoid duplicated constant arrays


#ifndef _APPLY_ZIGBEE_XS_IS_MCE_PATCH_H
#define _APPLY_ZIGBEE_XS_IS_MCE_PATCH_H

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

#ifndef RFC_MCERAM_BASE
#define RFC_MCERAM_BASE 0x21008000
#endif
#define ZIGBEE_XS_IS_MCE_NUM_COPY 0x0200

MCE_ADDR_TYPE zigbeeXsIsMcePatchAdd[4] = {
   16,
   28,
   31,
   32
};

MCE_PATCH_TYPE zigbeeXsIsMcePatchDat[4] = {
   0x0fcfc64c,
   0x000114c4,
   0x002b0670,
   0x0a11121d
};

// Note that this function does NOT call the DUMP API function of the TOPsm,
// the ROM contents are assumed to be present in RAM
PATCH_FUN_SPEC void enterZigbeeXsIsMcePatch(void)
{
   uint32_t *pMceRam = (uint32_t *) RFC_MCERAM_BASE;
   for (int k = 0; k < 4; k++) {
      pMceRam[zigbeeXsIsMcePatchAdd[k]] = zigbeeXsIsMcePatchDat[k];
   }
}

#endif
