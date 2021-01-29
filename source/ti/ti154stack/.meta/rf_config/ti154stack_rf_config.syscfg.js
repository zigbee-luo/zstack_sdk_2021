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
 *  ======== ti154stack_rf_config.syscfg.js ========
 */

"use strict";

// Get common utility functions
const Common = system.getScript("/ti/ti154stack/ti154stack_common.js");

// Get common rf settings
const rfCommon = system.getScript("/ti/ti154stack/rf_config/"
    + "ti154stack_rf_config_common.js");

// Get transmit power settings script
const powerScript = system.getScript("/ti/ti154stack/power_config/"
    + "ti154stack_power_config");

// Get network settings script
const networkScript = system.getScript("/ti/ti154stack/network_config/"
    + "ti154stack_network_config");

// Get network settings script
const oadScript = system.getScript("/ti/ti154stack/oad_config/"
    + "ti154stack_oad_config");

// Get RF setting descriptions
const Docs = system.getScript("/ti/ti154stack/rf_config/"
    + "ti154stack_rf_config_docs.js");

// Configurables for the RF Configuration module
const config = {
    name: "radioSettings",
    displayName: "Radio",
    description: "Configure PHY settings for radio operations",
    config: [
        {
            name: "rfDesign",
            displayName: "Based On RF Design",
            options: getRfDesignOptions(),
            default: getRfDesignOptions()[0].name,
            description: Docs.rfDesign.description,
            longDescription: Docs.rfDesign.longDescription,
            onChange: onRadioConfigChange
        },
        {
            name: "freqBand",
            displayName: "Frequency Band",
            options: rfCommon.getFreqBandOptions,
            default: rfCommon.getDefaultFreqBand(Common.isSub1GHzDevice()),
            description: Docs.freqBand.description,
            longDescription: Docs.freqBand.longDescription,
            onChange: onFreqBandChange
        },
        {
            name: "freqSub1",
            displayName: "Sub-1 GHz Frequency",
            options: getFreqSub1Options,
            default: getDefaultFreqSub1(null),
            hidden: !Common.isSub1GHzDevice(),
            description: Docs.freqSub1.description,
            longDescription: Docs.freqSub1.longDescription,
            onChange: onFreqSub1orPhyTypeChange
        },
        {
            name: "phyType",
            displayName: "Phy Type",
            options: rfCommon.getPhyTypeOptions,
            default: rfCommon.getDefaultPhyType(Common.isSub1GHzDevice()),
            description: Docs.phyType.description,
            longDescription: Docs.phyType.longDescription,
            onChange: onFreqSub1orPhyTypeChange
        },
        {
            name: "phyID",
            displayName: "Phy ID",
            default: getDefaultPhy154Settings().ID,
            readOnly: Docs.phyID.readOnly,
            description: Docs.phyID.description,
            longDescription: Docs.phyID.longDescription
        },
        {
            name: "channelPage",
            displayName: "Channel Page",
            default: getDefaultPhy154Settings().channelPage,
            readOnly: Docs.channelPage.readOnly,
            description: Docs.channelPage.description,
            longDescription: Docs.channelPage.longDescription
        }
    ]
};

/*
 *******************************************************************************
 Radio Group-Specific Functions
 *******************************************************************************
 */

/*
 * ======== getRfDesignOptions ========
 * Generates an array of SRFStudio compatible rfDesign options based on device
 *
 * @param deviceId - device being used
 * @returns Array - Array of rfDesign options, if the device isn't supported,
 *                  returns null
 */
function getRfDesignOptions()
{
    const deviceId = system.deviceData.deviceId;
    let newRfDesignOptions = null;

    if(deviceId === "CC1352P1F3RGZ")
    {
        newRfDesignOptions = [
            {name: "LAUNCHXL-CC1352P1"},
            {name: "LAUNCHXL-CC1352P-2"},
            {name: "LAUNCHXL-CC1352P-4"}
        ];
    }
    else if(deviceId === "CC1352R1F3RGZ")
    {
        newRfDesignOptions = [{name: "LAUNCHXL-CC1352R1"}];
    }
    else if(deviceId === "CC1312R1F3RGZ")
    {
        newRfDesignOptions = [{name: "LAUNCHXL-CC1312R1"}];
    }
    else if(deviceId === "CC2642R1FRGZ")
    {
        newRfDesignOptions = [{name: "LAUNCHXL-CC26X2R1"}];
    }
    else if(deviceId === "CC2652R1FRGZ")
    {
        newRfDesignOptions = [{name: "LAUNCHXL-CC26X2R1"}];
    }
    else if(deviceId === "CC2652RB1FRGZ")
    {
        newRfDesignOptions = [{name: "LAUNCHXL-CC2652RB"}];
    }
    else if(deviceId === "CC2652R1FSIP")
    {
        newRfDesignOptions = [{name: "LP_CC2652RSIP"}];
    }
    else if(deviceId === "CC2652P1FSIP")
    {
        newRfDesignOptions = [{name: "LP_CC2652PSIP"}];
    }
    else
    {
        throw new Error("Unknown deviceId " + deviceId + ".");
    }

    return(newRfDesignOptions);
}

/*
 * ======== onRadioConfigChange ========
 * On change function for rfDesign config
 * Updates visibility and values of RF device-dependent configs
 *
 * @param inst - 15.4 instance
 * @param ui   - user interface object
 */
function onRadioConfigChange(inst, ui)
{
    // Update dependencies
    const isSubGSelected = (inst.freqBand === "freqBandSub1");

    inst.freqBand = rfCommon.getDefaultFreqBand(isSubGSelected);
    inst.freqSub1 = getDefaultFreqSub1(inst);
    onFreqBandChange(inst, ui);
}

/*
 * ======== onFreqBandChange ========
 * On change function for freqBand config
 * Updates visibility and values of frequency band-dependent configs
 *
 * @param inst - 15.4 instance
 * @param ui   - user interface object
 */
function onFreqBandChange(inst, ui)
{
    // Set visibility of dependent configs
    setRFConfigHiddenState(inst, ui, "freqSub1");

    // Phy type must be updated before phy ID and channel page
    setPhyType(inst);
    onFreqSub1orPhyTypeChange(inst);

    // Update values of frequency dependent configs
    oadScript.setDefaultOADBlockSize(inst, inst.freqBand);
}

/*
 * ======== onFreqSub1orPhyTypeChange ========
 * On change function for freqBandSub1 and phy type configs
 * Updates values of phy ID and channel page based on frequency and phy type
 *
 * @param inst - 15.4 instance
 */
function onFreqSub1orPhyTypeChange(inst)
{
    setPhyIDChannelPage(inst);

    // Update values of frequency dependent configs
    networkScript.setDefaultChannelMasks(inst);
    oadScript.setDefaultOadBlockReqRate(inst);
    oadScript.setDefaultOadBlockReqPollDelay(inst);
}

/*
 * ======== getFreqSub1Options ========
 * Generates a list of sub-1 GHz frequency bands supported for drop down config
 *
 * @param inst - 15.4 instance (null during initialization)
 * @returns Array - array of options representing sub-1 GHz bands supported
 */
function getFreqSub1Options(inst)
{
    let freqSub1Options;
    if(Common.is433MHzDevice(inst))
    {
        freqSub1Options = [
            {
                name: "freq433",
                displayName: "433 MHz"
            }
        ];
    }
    else
    {
        freqSub1Options = [
            {
                name: "freq863",
                displayName: "868 MHz"
            },
            {
                name: "freq915",
                displayName: "915 MHz"
            }
        ];
    }

    return(freqSub1Options);
}

/*
 *  ======== getPhy154Settings ========
 *  Retrieves array of 15.4 phy ID and channel page settings corresponding
 *  to selected data rate from the <board_name>_rf_defaults.js
 *
 *  @param inst - 15.4 instance (null during initialization)
 *  @param freqBand - Frequency band (Sub-1 or 2.4 GHz)
 *  @param freqSub1 - Sub-1 GHz frequency (Has no effect for 2.4 GHz)
 *  @param phyType - Name of phy type (phy5kbps, phy50kbps, phy200kbps, phyIEEE)
 *  @returns Array - an array containing dictionary with channel page and phy ID
 */
function getPhy154Settings(inst, freqBand, freqSub1, phyType)
{
    const rfPhySettings = rfCommon.getBoardPhySettings(inst);
    let phy154Setting = null;

    if(freqBand === "freqBandSub1")
    {
        // Get proprietary Sub-1 GHz RF defaults for the device being used
        const propPhySettings = rfPhySettings.defaultPropPhyList;

        // Find phy object associated with phy type
        const phyObj = _.find(propPhySettings,
            (settings) => (settings.phyDropDownOption.name === phyType));

        // Get phy ID and channel page of given sub-1 frequency and rate
        if(phyObj)
        {
            phy154Setting = phyObj.phy154Settings[freqSub1];
        }
    }

    else if(freqBand === "freqBand24")
    {
        // Get IEEE 2.4 GHz RF defaults for the device being used
        const ieeePhySettings = rfPhySettings.defaultIEEEPhyList;

        // Only one phy type for 2.4GHz
        phy154Setting = ieeePhySettings[0].phy154Settings.phyIEEE;
    }

    return(phy154Setting);
}

/*
 *  ======== setPhyType ========
 *  Update phy type based on frequency band
 *
 *  @param inst - Instance of this module
 */
function setPhyType(inst)
{
    const isSubGSelected = (inst.freqBand === "freqBandSub1");
    inst.phyType = rfCommon.getDefaultPhyType(isSubGSelected);
}

/*
 *  ======== setPhyIDChannelPage ========
 *  Update phy ID and channel page based on phy type and frequency band
 *
 *  @param inst - Instance of this module
 */
function setPhyIDChannelPage(inst)
{
    const phyType = rfCommon.getSafePhyType(inst);
    const freqBand = rfCommon.getSafeFreqBand(inst);
    const freqSub1 = getSafeFreqSub1(inst);

    const newSettings = getPhy154Settings(inst, freqBand, freqSub1,
        phyType);

    // Check needed to ensure combination of safe values is valid
    if(newSettings)
    {
        inst.phyID = newSettings.ID;
        inst.channelPage = newSettings.channelPage;
    }
}

/*
 * ======== getDefaultFreqSub1 ========
 * Retrieves the default sub-1 GHz frequency value from the drop down options
 *     * 433MHz for CC1352P4
 *     * 915MHz for others
 *
 * @param inst - 15.4 instance (null during initialization)
 * @returns - name of default frequency
 */
function getDefaultFreqSub1(inst)
{
    const sub1FreqOpts = getFreqSub1Options(inst);
    return _.last(sub1FreqOpts).name;
}

/*
 *  ======== getDefaultPhy154Settings ========
 *  Gets default channel page and phy ID array
 *
 *  @returns Array - an array with default channel page and phy ID
 */
function getDefaultPhy154Settings()
{
    const isSubGSelected = Common.isSub1GHzDevice();
    const defaultFreqBand = rfCommon.getDefaultFreqBand(isSubGSelected);
    const defaultPhyType = rfCommon.getDefaultPhyType(isSubGSelected);
    const defaultFreqSub1 = getDefaultFreqSub1(null);

    return(getPhy154Settings(null, defaultFreqBand, defaultFreqSub1,
        defaultPhyType));
}

/*
 * ======== getRFConfigHiddenState ========
 * Get the expected visibility of the selected config
 *
 * @param inst    - module instance containing the config that changed
 * @param cfgName - name of config
 * @returns bool  - true if hidden, false if visible
 */
function getRFConfigHiddenState(inst, cfgName)
{
    let isVisible = true;
    switch(cfgName)
    {
        case "freqSub1":
        {
            isVisible = (inst.freqBand === "freqBandSub1");
            break;
        }
        case "rfDesign":
        case "freqBand":
        case "phyType":
        case "phyID":
        case "channelPage":
        default:
        {
            isVisible = true;
            break;
        }
    }

    // Hide all configs for coprocessor project
    isVisible = isVisible && (inst.project !== "coprocessor");

    // Return whether config is hidden
    return(!isVisible);
}

/*
 * ======== setRFConfigHiddenState ========
 * Sets the visibility of the selected config
 *
 * @param inst    - module instance containing the config that changed
 * @param ui      - user interface object
 * @param cfgName - name of config
 */
function setRFConfigHiddenState(inst, ui, cfgName)
{
    Common.setConfigHiddenState(inst, ui, cfgName, config.config,
        getRFConfigHiddenState);

    // Separate case required for freqSub1 since default value depends
    // on board which can change at runtime via rfDesign
    if(cfgName === "freqSub1" && ui[cfgName].hidden)
    {
        inst.freqSub1 = getDefaultFreqSub1(inst);
    }
}

/*
 * ======== setAllRFConfigsHiddenState ========
 * Sets the visibility of all RF configs
 *
 * @param inst    - module instance
 * @param ui      - user interface object
 */
function setAllRFConfigsHiddenState(inst, ui)
{
    Common.setAllConfigsHiddenState(inst, ui, config.config,
        getRFConfigHiddenState, setRFConfigHiddenState);
}

/*
 * ======== setFreqBandReadOnlyState ========
 * Sets the read only status of freqBand config. If config is read only, a
 * read only reason is displayed
 *
 * @param ui       - user interface object
 * @param readOnly - true if freqBand config must be set to read only
 */
function setFreqBandReadOnlyState(ui, readOnly)
{
    // Set read only state of config
    ui.freqBand.readOnly = (readOnly) ? Docs.freqBand.readOnly : false;
}

/*
 * ======== getSafeFreqSub1 ========
 * Safely retrieve the value of the config by returning the instance value it's
 * valid, otherwise returns the default value.
 *
 * Due to their nature, dynamic enum configurables may be incorrectly modified
 * through the .syscfg file. While all dynamic configs have validation functions
 * to detect such errors, the dependency of the radio config module requires
 * safe access to some configs to avoid SysCOnfig breaks.
 *
 * @param inst - 15.4 module instance (null during initialization)
 * @returns - config value in instance (if valid), otherwise config default
 *            value
 */
function getSafeFreqSub1(inst)
{
    const validOptions = getFreqSub1Options(inst);
    const defaultFreqBand = getDefaultFreqSub1(inst);

    const freqBand = Common.getSafeDynamicConfig(inst, "freqSub1",
        defaultFreqBand, validOptions);

    return(freqBand);
}

/*
 * ======== validate ========
 * Validate this inst's configuration
 *
 * @param inst       - RF Settings instance to be validated
 * @param validation - object to hold detected validation issues
 */
function validate(inst, validation)
{
    // Validate dynamic configs
    let validOptions = rfCommon.getFreqBandOptions(inst);
    Common.validateDynamicEnum(inst, validation, "freqBand", validOptions);

    validOptions = getFreqSub1Options(inst);
    Common.validateDynamicEnum(inst, validation, "freqSub1", validOptions);

    validOptions = rfCommon.getPhyTypeOptions(inst);
    Common.validateDynamicEnum(inst, validation, "phyType", validOptions);

    // Get the RF Design module to verify that RF Design configs match
    const rfDesign = system.modules["/ti/devices/radioconfig/rfdesign"].$static;
    if(inst.rfDesign !== rfDesign.rfDesign)
    {
        validation.logError(`Must match ${system.getReference(rfDesign,
            "rfDesign")} in the RF Design Module`, inst, "rfDesign");
    }
}

/*
 *******************************************************************************
 Module Dependencies and Exports
 *******************************************************************************
 */

/*
 *  ======== addRFSettingDependency ========
 *  Creates an RF setting dependency module
 *
 * @param inst  - Module instance containing the config that changed
 * @returns dictionary - containing a single RF setting dependency module
 */
function addRFSettingDependency(inst)
{
    const boardPhySettings = rfCommon.getBoardPhySettings(inst);

    // Get proprietary Sub-1 GHz RF defaults for the device being used
    const propPhySettings = boardPhySettings.defaultPropPhyList;

    // Get IEEE 2.4 GHz RF defaults for the device being used
    const ieeePhySettings = boardPhySettings.defaultIEEEPhyList;

    const phyType = rfCommon.getSafePhyType(inst);
    const freqBand = rfCommon.getSafeFreqBand(inst);

    // Find PHY object
    const phyList = propPhySettings.concat(ieeePhySettings);
    const selectedPhy = _.find(phyList,
        (setting) => (setting.phyDropDownOption.name === phyType));

    // Get settings from selected phy
    const radioConfigArgs = _.cloneDeep(selectedPhy.args);

    // Retrieve phy and phy group from rf_defaults files to get tx power
    // configuration that needs to be set in the radio config module
    const rfPhySettings = rfCommon.getPhyTypeGroupFromRFConfig(inst);
    const rfPhyType = rfPhySettings.phyType;
    const rfPhyGroup = rfPhySettings.phyGroup;
    const txPower154Obj = powerScript.getRFTxPowerFrom154TxPower(inst, freqBand,
        rfPhyType, rfPhyGroup);

    // Set radio config tx power based on 15.4 tx power setting
    radioConfigArgs[txPower154Obj.cfgName] = txPower154Obj.txPower;

    // Set high PA in radio config if supported by board
    if(Common.isHighPADevice())
    {
        radioConfigArgs.highPA = txPower154Obj.highPA;
    }

    // Add 15.4 specific overrides for sub-G projects
    if(inst.freqBand === "freqBandSub1")
    {
        const overridesMacro = "TI_154_STACK_OVERRIDES";
        radioConfigArgs.codeExportConfig.stackOverride = "ti/ti154stack/"
            + "common/boards/ti_154stack_overrides.h";
        radioConfigArgs.codeExportConfig.stackOverrideMacro = overridesMacro;
    }

    return({
        name: "radioConfig",
        displayName: selectedPhy.phyDropDownOption.displayName,
        moduleName: selectedPhy.moduleName,
        description: "Radio configuration",
        readOnly: true,
        hidden: true,
        collapsed: true,
        group: "radioSettings",
        args: radioConfigArgs
    });
}

/*
 *  ======== moduleInstances ========
 *  Determines what modules are added as non-static sub-modules
 *
 *  @param inst  - Module instance containing the config that changed
 *  @returns     - Dependency module of selected phyType
 */
function moduleInstances(inst)
{
    const dependencyModule = [];

    if(inst.project !== "coprocessor")
    {
        // Add radio config module associated with phy selected
        dependencyModule.push(addRFSettingDependency(inst));
    }

    return(dependencyModule);
}

// Exports to the top level 15.4 module
exports = {
    config: config,
    validate: validate,
    moduleInstances: moduleInstances,
    getFreqSub1Options: getFreqSub1Options,
    getPhy154Settings: getPhy154Settings,
    setFreqBandReadOnlyState: setFreqBandReadOnlyState,
    setRFConfigHiddenState: setRFConfigHiddenState,
    setAllRFConfigsHiddenState: setAllRFConfigsHiddenState,
    getRFConfigHiddenState: getRFConfigHiddenState
};
