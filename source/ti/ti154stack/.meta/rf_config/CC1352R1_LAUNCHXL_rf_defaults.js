/*
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 *  ======== CC1352R1_LAUNCHXL_rf_defaults.js ========
 */

"use strict";

// Get common rf settings
const rfCommon = system.getScript("/ti/ti154stack/rf_config/"
    + "ti154stack_rf_config_common.js");

/*
 *  ======== Device Specific PHY Settings ========
 *
 * These Objects will be used to extend the common phy settings defined
 * in ti154stack_rf_config_common.js.
 *
 * These Objects must contain the following elements:
 *      - args: Phy setting arguments to be passed to the radio config module
 *          - freqBand: The frequency band containing the carrierFrequency.
 *                      Valid options: 433, 868, 2440(reserved for future use)
 *          - phyType<band>: Name of the phy found in the radio config module.
 *                           Valid options: phyType868, phyType433,
 *                           phyType2440(reserved for future use)
 *          - carrierFrequency: Frequency setting within the freqBand
 *          - whitening: Types of whitening
 *          - codeExportConfig: Exported code names to be passed to
 *                              radioConfig.codeExportConfig
 *              - cmdPropRadioDivSetup: Name of the generated
 *                      rfc_CMD_PROP_RADIO_DIV_SETUP_t command
 */

// Object containing SimpleLink Long Range, 5kbps settings for CC1352R1_LAUNCHXL
const devSpecificSlLr5KbpsSettings = {
    args: {
        freqBand: "868", // options: 868 or 433
        phyType868: "slr5kbps2gfsk", // phyType suffix must match freqBand
        carrierFrequency: 868.00,
        whitening: `No whitener, dynamically IEEE 802.15.4g compatible \
16/32-bit CRC`,
        codeExportConfig: {
            cmdList_prop: ["cmdPropRadioDivSetup"],
            cmdPropRadioDivSetup: "RF_cmdPropRadioDivSetup_slr5kbps2gfsk"
        }
    }
};

// Object containing 2GFSK, 50kbps settings for the CC1352R1_LAUNCHXL
const devSpecific2Gfsk50KbpsSettings = {
    args: {
        freqBand: "868", // options: 868 or 433
        phyType868: "2gfsk50kbps154g", // phyType suffix must match freqBand
        carrierFrequency: 868.00,
        whitening: `Dynamically IEEE 802.15.4g compatible whitener and \
16/32-bit CRC`,
        codeExportConfig: {
            cmdList_prop: ["cmdPropRadioDivSetup"],
            cmdPropRadioDivSetup: "RF_cmdPropRadioDivSetup_2gfsk50kbps154g"
        }
    }
};

// Object containing 2GFSK, 200kbps settings for the CC1352R1_LAUNCHXL
const devSpecific2Gfsk200KbpsSettings = {
    args: {
        freqBand: "868",
        phyType868: "2gfsk200kbps154g", // phyType sfx must match freqBand
        carrierFrequency: 915.00,
        whitening: `Dynamically IEEE 802.15.4g compatible whitener and \
16/32-bit CRC`,
        codeExportConfig: {
            cmdList_prop: ["cmdPropRadioDivSetup"],
            cmdPropRadioDivSetup: "RF_cmdPropRadioDivSetup_2gfsk200kbps154g"
        }
    }
};

/*
 *  ======== Device Specific IEEE PHY Settings ========
 *
 * These Objects will be used to extend the common IEEE phy settings defined in
 * ti154stack_rf_config_common.js.
 *
 * These Objects must contain the following elements:
 *      - args: Phy setting arguments to be passed to the radio config module
 *          - highPA: Boolean for enabling/disabling High PA
 *          - codeExportConfig: Exported code names to be passed to
 *                              radioConfig.codeExportConfig
 *              - cmdRadioSetup: Name of the generated rfc_CMD_RADIO_SETUP_t
 *                               command
 */

// Object containing IEEE settings for the CC1352R1_LAUNCHXL
const devSpecificIEEESettings = {
    args: {
        codeExportConfig: {
            cmdList_ieee_15_4: ["cmdRadioSetup"],
            cmdRadioSetup: "RF_cmdRadioSetup_ieee154"
        }
    }
};

/*
 *  ======== Arrays Containing all PHY Settings ========
 *
 * These arrays will pull the common phy settings defined in
 * ti154stack_rf_config_common.js and merge them with the device specific phy
 * settings defined in this file.
 */

// Array containing all the phy settings for the CC1352R1_LAUNCHXL
const defaultPropPhyList = [
    rfCommon.mergeRFSettings(devSpecificSlLr5KbpsSettings,
        rfCommon.commonSlLr5KbpsSettings),
    rfCommon.mergeRFSettings(devSpecific2Gfsk50KbpsSettings,
        rfCommon.common2Gfsk50KbpsSettings),
    rfCommon.mergeRFSettings(devSpecific2Gfsk200KbpsSettings,
        rfCommon.common2Gfsk200KbpsSettings)
];

// Array containing all the IEEE phy settings for the CC1352R1_LAUNCHXL
const defaultIEEEPhyList = [
    rfCommon.mergeRFSettings(devSpecificIEEESettings,
        rfCommon.commonIEEESettings)
];

exports = {
    defaultPropPhyList: defaultPropPhyList,
    defaultIEEEPhyList: defaultIEEEPhyList
};
