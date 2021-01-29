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
// Generated 2014-12-18 16:51:04
//

// This file implements patches for the radio CPU on CC26xx
// It should only be included from ONE source file to avoid duplicated constant arrays

// The following patches are included:

//
// /vobs/cc26xxFwDev/radio_domain/patch/cpe_pg2/patchsrc/ieee_state_patch.c@@/main/1        Rule: /main/LATEST
//
//   This patch fixes a bug in the IEEE 802.15.4 state machine:
//   - Workaround for modem done flag that may be lost, causing Rx to stall
//

//
// /vobs/cc26xxFwDev/radio_domain/patch/cpe_pg2/patchsrc/ieee_stop_tx.c@@/main/1            Rule: /main/LATEST
//
//   Workaround for CC26_RFCORE--BUG00040: Performs a double init of the modulator when stopping Tx to
//   ensure that a subsequent Tx will not get FIFO underflow due to a stuck read request.
//

//
// /vobs/cc26xxFwDev/radio_domain/patch/cpe_pg2/patchsrc/fscapatch.c@@/main/4               Rule: /main/LATEST
//
//   The patch modifies the FSCA interrupt handler to fix the following issues:
//   - Holds PLLCOEFFDYNCHANGE for at least 1 us when changing PLL bandwidth
//   - Fixes a bug when synth programming is finished at the same time as 
//     starting a foreground level IEEE 802.15.4 operation
//   - Ensures correct operation if synth reports loss of lock
//

#ifndef _APPLY_PATCH_H
#define _APPLY_PATCH_H

#include <stdint.h>
#include <string.h>

#ifndef CPE_PATCH_TYPE
#define CPE_PATCH_TYPE static const uint32_t
#endif

#ifndef SYS_PATCH_TYPE
#define SYS_PATCH_TYPE static const uint32_t
#endif

#ifndef PATCH_FUN_SPEC
#define PATCH_FUN_SPEC static inline
#endif

#ifndef _APPLY_PATCH_TAB
#define _APPLY_PATCH_TAB
typedef struct {
   uint8_t patchInd_antDecodeState;
   uint8_t patchInd_bleDecodeState;
   uint8_t patchInd_errorHandler;
   uint8_t patchInd_hsDecodeState;
   uint8_t patchInd_hsOutIrqHandler;
   uint8_t patchInd_ieeeDecodeState;
   uint8_t patchInd_srcMatchShort;
   uint8_t patchInd_srcMatchExt;
   uint8_t patchInd_propDecodeState;
   uint8_t patchInd_bleProgramFreq;
   uint8_t patchInd_blePrepareTx;
   uint8_t patchInd_blePrepareRx;
   uint8_t patchInd_bleRestartRx;
   uint8_t patchInd_RDBELL_IRQHandler;
   uint8_t patchInd_checkObservedTriggers;
   uint8_t patchInd_checkBgObservedTriggers;
   uint8_t patchInd_quickStopAbortCheck;
   uint8_t patchInd_setupTrigger;
   uint8_t patchInd_processCommandTrigger;
   uint8_t patchInd_cancelTrigger;
   uint8_t patchInd_submitCmd;
   uint8_t patchInd_endRadioOp;
   uint8_t patchInd_processStartTrigger;
   uint8_t patchInd_setDelayedRadioOpStart;
   uint8_t patchInd_setExternalRadioOpStart;
   uint8_t patchInd_setImmediateRadioOpStart;
   uint8_t patchInd_ratChAvail;
   uint8_t patchInd_updateReportedFwInfo;
   uint8_t patchInd_RFCTRC_IRQHandler;
   uint8_t patchInd_allocateTx;
   uint8_t patchInd_freeDataEntry;
   uint8_t patchInd_finishDataEntry;
   uint8_t patchInd_checkValidQueue;
   uint8_t patchInd_allocateRx;
   uint8_t patchInd_writeToPartialRx;
   uint8_t patchInd_finishRx;
   uint8_t patchInd_freeRxEntry;
   uint8_t patchInd_getDataBuffer;
   uint8_t patchInd_ieeeRxDoneHandler;
   uint8_t patchInd_ieeeSendPhyHeader;
   uint8_t patchInd_ieeeReceivePhyHeader;
   uint8_t patchInd_ieeeReceiveBytes;
   uint8_t patchInd_findCcaState;
   uint8_t patchInd_setupRadioFreq;
   uint8_t patchInd_radioSetupOverride;
   uint8_t patchInd_radioParOverride;
   uint8_t patchInd_enableForSetup;
   uint8_t patchInd_enableForTx;
   uint8_t patchInd_startTx;
   uint8_t patchInd_enableForRx;
   uint8_t patchInd_startRx;
   uint8_t patchInd_restartRx;
   uint8_t patchInd_stopRadio;
   uint8_t patchInd_stopFs;
   uint8_t patchInd_setupPreamSw;
   uint8_t patchInd_phaSetup;
   uint8_t patchInd_phaTxWhSetup;
   uint8_t patchInd_phaSeedDefault;
   uint8_t patchInd_phaCheckCrcErr;
   uint8_t patchInd_phaPseudoRand16;
   uint8_t patchInd_getRssi;
   uint8_t patchInd_resetMaxRssi;
   uint8_t patchInd_enaMdmDoneIrqHiPri;
   uint8_t patchInd_enaMdmDoneIrqStdPri;
   uint8_t patchInd_disMdmDoneIrq;
   uint8_t patchInd_enaMdmOutIrqHiPri;
   uint8_t patchInd_disMdmOutIrq;
   uint8_t patchInd_writeAdi0Reg;
   uint8_t patchInd_readAdi0Reg;
   uint8_t patchInd_MDM_Soft_IRQHandler;
   uint8_t patchInd_RFE_Soft2_IRQHandler;
   uint8_t patchInd_updateAdiShadow;
   uint8_t patchInd_writeAdiShadow;
   uint8_t patchInd_updateAdi0Shadow;
   uint8_t patchInd_readFcfg1;
   uint8_t patchInd_findMdmBaudSetting;
   uint8_t patchInd_startRat;
   uint8_t patchInd_stopRat;
   uint8_t patchInd_setupTiming;
   uint8_t patchInd_setupExtTxStart;
   uint8_t patchInd_abortTiming;
   uint8_t patchInd_timingDone;
   uint8_t patchInd_setupCaptureSyncFound;
   uint8_t patchInd_setupCaptureCorrSync;
   uint8_t patchInd_modifyCaptureCorrSync;
   uint8_t patchInd_readCapturedSyncFound;
   uint8_t patchInd_readCapturedCorrTops;
   uint8_t patchInd_abortCaptureSyncFound;
   uint8_t patchInd_setupSyncStopRadioTimer;
   uint8_t patchInd_doSyncStopRadioTimer;
   uint8_t patchInd_setupSyncStartRadioTimer;
   uint8_t patchInd_doSyncStartRadioTimer;
   uint8_t patchInd_abortRatCh;
   uint8_t patchInd_timeIsLater;
   uint8_t patchInd_findRatComp;
   uint8_t patchInd_RAT_Ch5_IRQHandler;
   uint8_t patchInd_synthPowerUpInitialize;
   uint8_t patchInd_turnOffSynth;
   uint8_t patchInd_powerDownSynth;
   uint8_t patchInd_findSynthFreqInfo;
   uint8_t patchInd_writeSynthFreqCalInfo;
   uint8_t patchInd_setSynthMode;
   uint8_t patchInd_updateSynthFreq;
   uint8_t patchInd_setSynthOutput;
   uint8_t patchInd_setSynthPrecal;
   uint8_t patchInd_softwareSynthStartTuneSequence;
   uint8_t patchInd_writeAdi1Reg;
   uint8_t patchInd_readAdi1Reg;
   uint8_t patchInd_programModIsfShape;
   uint8_t patchInd_setCwMode;
   uint8_t patchInd_findDefaultPreDivRatio;
} patchTab_t;
#endif


CPE_PATCH_TYPE patchImage[] = {
   0x210004bb,
   0x2100040d,
   0x22024823,
   0x421a7dc3,
   0xd0034472,
   0x1dc04678,
   0xb5f84686,
   0x4c1f4710,
   0x200834ae,
   0x490347a0,
   0x60082008,
   0x3cec6008,
   0xbdf847a0,
   0x40045004,
   0x4c17b5f0,
   0x18612140,
   0x280278c8,
   0x4809d005,
   0x60012100,
   0x47884908,
   0x6e25bdf0,
   0x60354e07,
   0x43280760,
   0x68276620,
   0x480e6024,
   0x60274780,
   0xbdf06035,
   0x4004112c,
   0x000065a5,
   0x40044028,
   0x4c07b510,
   0x29007da1,
   0x2101d105,
   0x024875a1,
   0x393e4904,
   0x68204788,
   0xd0002800,
   0xbd104780,
   0x21000254,
   0x0000398b,
   0x6a00480b,
   0xd00407c0,
   0x2201480a,
   0x43117801,
   0x48097001,
   0x72c84700,
   0xd006280d,
   0x00802285,
   0x18800252,
   0x60486840,
   0x48044770,
   0x0000e7fb,
   0x40045040,
   0x21000268,
   0x0000ff39,
   0x210004a5,
};
#define _NWORD_PATCHIMAGE 56

#define _NWORD_PATCHSYS 0

#define IRQ_PATCH_0 0x2100043d
#define IRQ_PATCH_1 0x2100047d


#define _SYS_PATCH_FIXED_ADDR 0x20000000

PATCH_FUN_SPEC void enterCpePatch(void)
{
   uint32_t *pPatchVec = (uint32_t *) 0x21000404;

#if (_NWORD_PATCHIMAGE > 0)
   memcpy(pPatchVec, patchImage, sizeof(patchImage));
#endif
}

PATCH_FUN_SPEC void enterSysPatch(void)
{
}

PATCH_FUN_SPEC void configurePatch(void)
{
   patchTab_t *pPatchTab = (patchTab_t *) 0x2100033C;
   uint32_t *pIrqPatch = (uint32_t *) 0x210003AC;


   pPatchTab->patchInd_ieeeDecodeState = 0;
   pPatchTab->patchInd_stopRadio = 1;

   pIrqPatch[1] = IRQ_PATCH_0;
   pIrqPatch[9] = IRQ_PATCH_1;
}

PATCH_FUN_SPEC void applyPatch(void)
{
   enterSysPatch();
   enterCpePatch();
   configurePatch();
}

PATCH_FUN_SPEC void refreshPatch(void)
{
   enterCpePatch();
   configurePatch();
}

#undef IRQ_PATCH_0
#undef IRQ_PATCH_1

#endif
