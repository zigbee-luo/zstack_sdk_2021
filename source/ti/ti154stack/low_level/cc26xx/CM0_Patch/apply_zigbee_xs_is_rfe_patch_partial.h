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
// Generated Thu Dec 18 17:05:27 2014 using the following files:
// /vobs/cc26xxFwDev/radio_domain/rfe/src/rfe_rom_bank1.asm@@/main/13       Rule: CC26_RF_ROM_FW_RFE--DEV--1.0--N.A.00.18--2014.04.01
// /vobs/cc26xxFwDev/radio_domain/rfe/src/dbg.asm@@/main/1  Rule: CC26_RF_ROM_FW_RFE--DEV--1.0--N.A.00.18--2014.04.01
// /vobs/cc26xxFwDev/radio_domain/patch/pg2/topsm_zigbee/rfe/src/rfe_commonlib.asm@@/main/1                 Rule: /main/LATEST
// /vobs/cc26xxFwDev/radio_domain/rfe/doc/rfeadiconf.txt@@/main/10          Rule: CC26_RF_ROM_FW_RFE--DEV--1.0--N.A.00.18--2014.04.01
// /vobs/cc26xxIpDev/modules/cc26_rfcore/doc/RFEregs.txt@@/main/26          Rule: /main/LATEST
// /vobs/cc26xxIpDev/modules/cc26_rfcore/doc/RFEADIregs.txt@@/main/12       Rule: /main/LATEST
//

// This file implements patches for the RFE on CC26xx
// It should only be included from ONE source file to avoid duplicated constant arrays


#ifndef _APPLY_ZIGBEE_XS_IS_RFE_PATCH_H
#define _APPLY_ZIGBEE_XS_IS_RFE_PATCH_H

#include <stdint.h>

#ifndef RFE_PATCH_TYPE
#define RFE_PATCH_TYPE static const uint32_t
#endif

#ifndef RFE_ADDR_TYPE
#define RFE_ADDR_TYPE static const uint16_t
#endif

#ifndef PATCH_FUN_SPEC
#define PATCH_FUN_SPEC static inline
#endif

#ifndef RFC_RFERAM_BASE
#define RFC_RFERAM_BASE 0x2100C000
#endif
#define ZIGBEE_XS_IS_RFE_NUM_COPY 0x0260

RFE_ADDR_TYPE zigbeeXsIsRfePatchAdd[1] = {
   65
};

RFE_PATCH_TYPE zigbeeXsIsRfePatchDat[1] = {
   0x26514084
};

// Note that this function does NOT call the DUMP API function of the TOPsm,
// the ROM contents are assumed to be present in RAM
PATCH_FUN_SPEC void enterZigbeeXsIsRfePatch(void)
{
   uint32_t *pRfeRam = (uint32_t *) RFC_RFERAM_BASE;
   for (int k = 0; k < 1; k++) {
      pRfeRam[zigbeeXsIsRfePatchAdd[k]] = zigbeeXsIsRfePatchDat[k];
   }
}

#endif
