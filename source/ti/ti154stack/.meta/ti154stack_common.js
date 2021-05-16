/*
 * Copyright (c) 2017-2019 Texas Instruments Incorporated - http://www.ti.com
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
 *  ======== ti154stack_common.js ========
 */

"use strict";

// Max values for C types
const cTypeMax = {
    u_int8: 255,
    u_int16: 65535,
    u_int32: 4294967295,
    int8_t: 127
};

// Settings for ti/devices/CCFG module
const ti154stackCCFGSettings = {
    CC1312R1_LAUNCHXL_CCFG_SETTINGS: {
        forceVddr: false
    },
    CC1352R1_LAUNCHXL_CCFG_SETTINGS: {
        forceVddr: false
    },
    CC1352P1_LAUNCHXL_CCFG_SETTINGS: {
        forceVddr: false
    },
    CC1352P_2_LAUNCHXL_CCFG_SETTINGS: {
        forceVddr: false
    },
    CC1352P_4_LAUNCHXL_CCFG_SETTINGS: {
        forceVddr: false
    },
    CC26X2R1_LAUNCHXL_CCFG_SETTINGS: {
        forceVddr: false
    },
    LP_CC2652RB_CCFG_SETTINGS: {
        forceVddr: false
    },
    LP_CC2652RSIP_CCFG_SETTINGS: {
        forceVddr: false
    },
    LP_CC2652PSIP_CCFG_SETTINGS: {
        forceVddr: false
    },
    LP_CC1312R7_CCFG_SETTINGS: {
        forceVddr: false
    },
    LP_CC1352P7_1_CCFG_SETTINGS: {
        forceVddr: false
    },
    LP_CC1352P7_4_CCFG_SETTINGS: {
        forceVddr: false
    },
    LP_CC2652R7_CCFG_SETTINGS: {
        forceVddr: false
    }
};

// Dictionary mapping a device name to default LaunchPad
const deviceToBoard = {
    CC1352P7: "LP_CC1352P7_1",
    CC1312R7: "LP_CC1312R7",
    CC1352R: "CC1352R1_LAUNCHXL",
    CC1352P: "CC1352P1_LAUNCHXL",
    CC1312R: "CC1312R1_LAUNCHXL",
    CC2652R7: "LP_CC2652R7",
    CC2652R1FSIP: "LP_CC2652RSIP",
    CC2652P1FSIP: "LP_CC2652PSIP",
    CC2652R1: "CC26X2R1_LAUNCHXL",
    CC2652RB: "LP_CC2652RB"
};

const currBoardName = getDeviceOrLaunchPadName(true);
const ccfgSettings = ti154stackCCFGSettings[currBoardName + "_CCFG_SETTINGS"];

// Dictionary mapping current device/board name regex to supported target
const supportedMigrations = {
    CC1312R1: {
        CC1312R1F3RGZ: {},
        CC1312R1_LAUNCHXL: {},
        CC1352R1F3RGZ: (target) => isMigrationValidFreqProject(target),
        CC1352R1_LAUNCHXL: (target) => isMigrationValidFreqProject(target)
    },
    CC1312R7: {
        CC1312R7RGZ: {},
        LP_CC1312R7: {}
    },
    CC1352R: {
        CC1352R1F3RGZ: {},
        CC1352R1_LAUNCHXL: {},
        CC1312R1F3RGZ: (target) => isMigrationValidFreqProject(target),
        CC1312R1_LAUNCHXL: (target) => isMigrationValidFreqProject(target),
        LPSTK_CC1352R: () => isMigrationValidLPSTK()
    },
    CC1352P1_LAUNCHXL: {
        CC1352P1F3RGZ: {},
        CC1352P1_LAUNCHXL: {}
    },
    CC1352P_2_LAUNCHXL: {
        CC1352P1F3RGZ: {},
        CC1352P_2_LAUNCHXL: {}
    },
    CC1352P_4_LAUNCHXL: {
        CC1352P1F3RGZ: {},
        CC1352P_4_LAUNCHXL: {}
    },
    CC1352P1F3RGZ: {
        CC1352P1F3RGZ: {},
        CC1352P1_LAUNCHXL: {},
        CC1352P_2_LAUNCHXL: {},
        CC1352P_4_LAUNCHXL: {}
    },
    LP_CC1352P7_1: {
        CC1352P7RGZ: {},
        LP_CC1352P7_1: {}
    },
    LP_CC1352P7_4: {
        CC1352P7RGZ: {},
        LP_CC1352P7_4: {}
    },
    CC1352P7RGZ: {
        CC1352P7RGZ: {},
        LP_CC1352P7_1: {},
        LP_CC1352P7_4: {}
    },
    /* Represents RSIP board and device */
    "CC26.2R.*SIP": {
        CC2652R1FSIP: {},
        LP_CC2652RSIP: {},
        CC2652R1FRGZ: () => isMigrationValidProject(),
        CC26X2R1_LAUNCHXL: () => isMigrationValidProject()
    },
    /* Represents 26X2R1 board and device */
    "CC26.2R1": {
        CC2652R1FRGZ: {},
        CC26X2R1_LAUNCHXL: {},
        CC2652R1FSIP: () => isMigrationValidProject(),
        LP_CC2652RSIP: () => isMigrationValidProject()
    },
    CC2652RB: {
        CC2652RB1FRGZ: {},
        LP_CC2652RB: {}
    },
    CC2652R7: {
        CC2652R7RGZ: {},
        LP_CC2652R7: {}
    }
};

/*
 * ======== isMigrationValidFreq ========
 * Determines whether a migration from one board/device to another board/device
 * is supported by the 15.4 module based on the frequency bands supported
 *
 * @param migTarget - Target board/device for migration
 * @returns One of the following Objects:
 *    - {} <--- Empty object if migration is valid
 *    - {warn: "Warning markdown text"} <--- Object with warn property
 *                                           if migration is valid but
 *                                           might require user action
 *    - {disable: "Disable markdown text"} <--- Object with disable property
 *                                              if migration is not valid
 */
function isMigrationValidFreq(migTarget)
{
    let migSupported = {};

    const inst = system.modules["/ti/ti154stack/ti154stack"].$static;

    if(inst.freqBand === "freqBandSub1" && !isSub1GHzDevice(migTarget))
    {
        migSupported = {
            disable: "Cannot migrate from a Sub-1 GHz device or project to a "
            + "2.4GHz device"
        };
    }
    else if(inst.freqBand === "freqBand24" && !is24GHzDevice(inst, migTarget))
    {
        migSupported = {
            disable: "Cannot migrate from a 2.4 GHz device or project to a "
            + "Sub-1 GHz device"
        };
    }

    return(migSupported);
}

/*
 * ======== isMigrationValidProject ========
 * Determines whether a migration from one board/device to another board/device
 * is supported by the 15.4 module based on the project
 *
 * @returns One of the following Objects:
 *    - {} <--- Empty object if migration is valid
 *    - {warn: "Warning markdown text"} <--- Object with warn property
 *                                           if migration is valid but
 *                                           might require user action
 *    - {disable: "Disable markdown text"} <--- Object with disable property
 *                                              if migration is not valid
 */
function isMigrationValidProject()
{
    let migSupported = {};

    const inst = system.modules["/ti/ti154stack/ti154stack"].$static;

    if(inst.project === "coprocessor")
    {
        migSupported = {
            disable: "Migrations are not supported on the coprocessor project. "
            + "Consider starting from an example in "
            + "<SDK_INSTALL_DIR>/examples/ that is closer to the desired "
            + "migration target"
        };
    }

    return(migSupported);
}

/*
 * ======== isMigrationValidLPSTK ========
 * Determines whether a migration to the LPSTK_CC1352R board is valid based on
 * whether the DMM module is included
 *
 * @returns One of the following Objects:
 *    - {} <--- Empty object if migration is valid
 *    - {warn: "Warning markdown text"} <--- Object with warn property
 *                                           if migration is valid but
 *                                           might require user action
 *    - {disable: "Disable markdown text"} <--- Object with disable property
 *                                              if migration is not valid
 */
function isMigrationValidLPSTK()
{
    let migSupported = {};

    if(system.modules["/ti/dmm/dmm"] === undefined)
    {
        migSupported = {
            warn: "This migration is not recommended and has not been tested. "
            + "The LPSTK_CC1352R is not supported on the standalone TI 15.4 "
            + "Stack."
        };
    }

    return(migSupported);
}

/*
 * ======== isMigrationValidFreqProject ========
 * Determines whether a migration from one board/device to another board/device
 * is supported by the 15.4 module based on the project and frequency settings
 *
 * @param migTarget - Target board/device for migration
 * @returns One of the following Objects:
 *    - {} <--- Empty object if migration is valid
 *    - {warn: "Warning markdown text"} <--- Object with warn property
 *                                           if migration is valid but
 *                                           might require user action
 *    - {disable: "Disable markdown text"} <--- Object with disable property
 *                                              if migration is not valid
 */
function isMigrationValidFreqProject(migTarget)
{
    const migSupported = _.merge(isMigrationValidFreq(migTarget),
        isMigrationValidProject());

    if(migSupported.disable)
    {
        return({disable: migSupported.disable});
    }

    if(migSupported.warn)
    {
        return({warn: migSupported.warn});
    }

    return({});
}

/*
 * ======== isMigrationValid ========
 * Determines whether a migration from one board/device to another board/device
 * is supported by the 15.4 module.
 *
 * @param currentTarget - Current board/device
 * @param migrationTarget - Target board/device for migration
 * @returns One of the following Objects:
 *    - {} <--- Empty object if migration is valid
 *    - {warn: "Warning markdown text"} <--- Object with warn property
 *                                           if migration is valid but
 *                                           might require user action
 *    - {disable: "Disable markdown text"} <--- Object with disable property
 *                                              if migration is not valid
 */
function isMigrationValid(currentTarget, migrationTarget)
{
    let migRegex = null;

    const defaultDisableText = "Consider starting from an example in "
    + " <SDK_INSTALL_DIR>/examples/ that is closer to the desired migration "
    + "target";

    // Migrations are not supported on CoP project
    let migSupported = _.merge({disable: defaultDisableText},
        isMigrationValidProject());

    for(migRegex in supportedMigrations)
    {
        if(currentTarget.match(new RegExp(migRegex))
            && supportedMigrations[migRegex][migrationTarget])
        {
            migSupported = supportedMigrations[migRegex][migrationTarget];

            // If function exists then migration support is conditional
            if(_.isFunction(migSupported))
            {
                migSupported = migSupported(migrationTarget);
            }
            break;
        }
    }

    return(migSupported);
}

/*
* ======== getMigrationMarkdown ========
* Returns text in markdown format that customers can use to aid in migrating a
* project between device/boards. It is recommended to provide no more
* than 3 bullet points with up to 120 characters per line.
*
* @param currTarget - Board/device being migrated FROM
* @returns string - Markdown formatted string
*/
function getMigrationMarkdown(currTarget)
{
    const inst = system.modules["/ti/ti154stack/ti154stack"].$static;

    // May need to add guidelines when other boards are supported
    let migrationText = "";

    if(inst.project === "coprocessor")
    {
        migrationText = "* Migrations to different boards or devices are not "
        + "currently supported on the CoProcessor project";
    }

    return(migrationText);
}

/*
 * ======== migrate ========
 * Perform stack specific changes to the SysConfig env POST migration
 *
 * @param currTarget - Board/device being migrated FROM
 * @param migrationTarget - Board/device being migrated TO
 * @param env - SysConfig environment providing access to all configurables
 * @param projectName - Optional name of the project being migrated
 *
 * @returns boolean - true when migration is supported and succesful, false when
 *                    migration is not supported and/or unsuccesful
 */
function migrate(currTarget, migrationTarget, env, projectName = null)
{
    const migrationInfo = isMigrationValid(currTarget, migrationTarget);
    let migrationValid = true;
    if(migrationInfo.disable || migrationInfo.warn)
    {
        migrationValid = false;
    }

    if(migrationValid)
    {
        /* ======== RF Design Settings ======== */
        const rfDesign = env.system.modules[
            "/ti/devices/radioconfig/rfdesign"].$static;
        const rfDesignSettings = env.system.getScript(
            "/ti/common/lprf_rf_design_settings.js"
        ).rfDesignSettings;

        if(rfDesignSettings.rfDesign !== undefined)
        {
            let setting = null;
            for(setting in rfDesignSettings)
            {
                if(Object.prototype.hasOwnProperty.call(rfDesignSettings,
                    setting))
                {
                    rfDesign[setting] = rfDesignSettings[setting];
                }
            }

            if(env.system.modules["/ti/ti154stack/ti154stack"])
            {
                const TI154Stack = env.system.modules[
                    "/ti/ti154stack/ti154stack"].$static;
                TI154Stack.rfDesign = rfDesignSettings.rfDesign;
            }
        }

        /* ======== CCFG Settings ======== */
        const device = env.system.modules["/ti/devices/CCFG"].$static;
        const ccfgSettingObj = env.system.getScript(
            "/ti/common/lprf_ccfg_settings.js"
        ).ccfgSettings;

        let setting = null;
        for(setting in ccfgSettingObj)
        {
            if(Object.prototype.hasOwnProperty.call(ccfgSettingObj, setting))
            {
                device[setting] = ccfgSettingObj[setting];
            }
        }
    }

    return(migrationValid);
}

/*!
 *  ======== getDeviceOrLaunchPadName ========
 *  Get the name of the board (or device)
 *
 *  @param convertToBoard - Boolean. When true, return the associated LaunchPad
 *                          name if a device is being used without a LaunchPad
 *  @param name - Optional name of board or device. If null, will use current
 *                device set
 *  @param inst - Optional 15.4 instance. If not null, will use board set in RF
 *                design config
 *  @returns String - Name of the board with prefix /ti/boards and suffix
 *                    .syscfg.json stripped off.  If no board was specified,
 *                    the device name is returned.
 */
function getDeviceOrLaunchPadName(convertToBoard, boardName = null, inst = null)
{
    let name = boardName;

    if(_.isNil(name))
    {
        name = system.deviceData.deviceId;
    }

    // Use current board if none is passed
    if(!_.isNil(system.deviceData.board) && _.isNil(boardName))
    {
        name = system.deviceData.board.source;

        /* Strip off everything up to and including the last '/' */
        name = name.replace(/.*\//, "");

        /* Strip off everything after and including the first '.' */
        name = name.replace(/\..*/, "");
    }

    // Check if this is a standalone device without a LaunchPad
    if(convertToBoard && !name.includes("LAUNCHXL") && !name.includes("LP_"))
    {
        if(!_.isNil(inst))
        {
            /* Use board from RF Design */
            name = inst.rfDesign;
        }
        else
        {
            name = getLaunchPadFromDevice(name);
        }
    }

    return(name);
}

/*!
 * ======== isSub1GHzDevice ========
 * Returns whether device supports Sub-1 GHz frequencies
 *
 * @param name - Optional name of board or device. If null will use current
 *               board/device
 * @returns - Boolean
 */
function isSub1GHzDevice(boardName = null)
{
    const board = getLaunchPadFromDevice(boardName);
    return(board.includes("CC13"));
}

/*!
 * ======== is24GHzDevice ========
 * Returns whether device supports 2.4 GHz frequency band
 *
 * @param inst - 15.4 instance (null during initialization - uses device)
 * @param name - Optional name of board or device. If null will use current
 *               board/device
 * @returns - Boolean
 */
function is24GHzDevice(inst, boardName = null)
{
    const board = getDeviceOrLaunchPadName(true, boardName, inst);

    return(board.includes("CC26") || board.includes("CC1352R1")
        || board.includes("CC1352P-2") || board.includes("CC1352P_2")
        || board.includes("CC1352P-4") || board.includes("CC1352P_4")
        || board.includes("CC1352P7-4") || board.includes("CC1352P7_4"));
}

/*!
 * ======== is433MHzDevice ========
 * Returns whether device supports 433 MHz frequency band
 *
 * @param inst - 15.4 instance (null during initialization - uses device)
 * @param name - Optional name of board or device. If null will use current
 *               board/device
 * @returns - Boolean
 */
function is433MHzDevice(inst, boardName = null)
{
    let board = getLaunchPadFromDevice(boardName);
    if(inst !== null)
    {
        board = inst.rfDesign;
    }

    return(board.includes("P-4") || board.includes("P_4")
        || board.includes("P7-4") || board.includes("P7_4"));
}

/*!
 *  ======== isHighPADevice ========
 *  Returns whether device supports high PA
 *
 * @param name - Optional name of board or device. If null will use current
 *               board/device
 *  @returns - Boolean
 */
function isHighPADevice(boardName = null)
{
    const board = getLaunchPadFromDevice(boardName);
    return(board.includes("CC1352P") || board.includes("CC2652PSIP"));
}

/*!
 * ======== getLaunchPadFromDevice ========
 * Get the launchpad mapped to the device. With the exception of P-boards, all
 * devices have a 1 to 1 mapping with a launchpad. Note that P-devices default
 * to a P1 launchpad
 *
 * @param name - Optional name of board or device. If null will use current
 *               board/device
 *  @returns String - board that corresponds to device
 */
function getLaunchPadFromDevice(boardName = null)
{
    let name = boardName;
    if(_.isNil(name))
    {
        name = system.deviceData.deviceId;
    }

    // Find the LaunchPad name in deviceToBoard dictionary
    let key = null;
    for(key in deviceToBoard)
    {
        if(name.includes(key))
        {
            name = deviceToBoard[key];
            break;
        }
    }

    return(name);
}

/*!
 *  ======== toHexString ========
 *  Convert decimal number to hexadecimal string
 *
 *  @param number - decimal number to be converted
 *  @param padding - length of padding to be added
 *  @returns String - hexadecimal string representation of decimal value
 */
function toHexString(number, padding)
{
    let str = _.toUpper(number.toString(16));
    if(padding)
    {
        str = _.padStart(str, padding, "0");
    }
    return("0x" + str);
}

/*!
 *  ======== isPositiveInteger ========
 *  Returns whether number passed is a positive whole number
 *
 *  @param value - number to be verified
 *  @returns Boolean - true if positive whole number, false otherwise
 */
function isPositiveInteger(value)
{
    return(Number.isInteger(value) && value >= 0);
}

/*!
 *  ======== validateRange ========
 *  Validates value of config is within range passed
 *
 *  @param inst       - module instance containing the config to be validated
 *  @param validation - object to hold detected validation issues
 *  @param cfgName    - name of config to be validated
 *  @param min        - minimum value allowed
 *  @param max        - maximum value allowed
 *  @param transform  - function to transform value to hex/decimal
 *  @returns Boolean  - true if value does not fall within range
 */
function validateRange(inst, validation, cfgName, min, max, transForm)
{
    const value = inst[cfgName];

    if(!isPositiveInteger(value))
    {
        validation.logError("Must be a whole number", inst, cfgName);
    }

    if(value < min || value > max)
    { // not in range
        validation.logError(`Must be between ${transForm(min)} and `
            + `${transForm(max)}`, inst, cfgName);
    }

    return((value < min) || (value > max));
}

/*!
 *  ======== validateRangeHex ========
 *  Validates the value of config is within range passed and prints error
 *  message, if any, with hexadecimal values
 *
 *  @param inst       - module instance containing the config to be validated
 *  @param validation - object to hold detected validation issues
 *  @param cfgName    - name of config to be validated
 *  @param min        - minimum value allowed
 *  @param max        - maximum value allowed
 */
function validateRangeHex(inst, validation, cfgName, min, max)
{
    validateRange(inst, validation, cfgName, min, max, toHexString);
}

/*!
 *  ======== validateRangeInt ========
 *  Validates the value of config is within range passed and prints error
 *  message, if any, with integer values
 *
 *  @param inst       - module instance containing the config to be validated
 *  @param validation - object to hold detected validation issues
 *  @param cfgName    - name of config to be validated
 *  @param min        - minimum value allowed
 *  @param max        - maximum value allowed
 */
function validateRangeInt(inst, validation, cfgName, min, max)
{
    validateRange(inst, validation, cfgName, min, max, Number);
}

/*!
 * ======== validateDynamicEnum ========
 * Verifies that the selected option in the dynamic drop down config is valid
 * If non-null validation and instance are passed will generate error
 *
 * @param inst       - module instance containing the config to be validated
 * @param validation - object to hold detected validation issues
 * @param validOpts - current options displayed in the config
 * @param cfgName - name of config to be validated
 */
function validateDynamicEnum(inst, validation, cfgName, validOpts)
{
    const selectedOpt = inst[cfgName];
    const found = _.find(validOpts, (o) => o.name === selectedOpt);

    if(inst !== null && validation !== null && !found)
    {
        validation.logError("Selected option is invalid", inst, cfgName);
    }

    return(found);
}

/*!
 * ======== validateDynamicMultiEnum ========
 * Verifies that the selected option in the multi-selection dynamic drop down
 * config are valid. If non-null validation and instance are passed will
 * generate error
 *
 * @param inst       - module instance containing the config to be validated
 * @param validation - object to hold detected validation issues
 * @param cfgName - name of config to be validated
 * @param selectedOpt - value of config to be validated
 * @param validOpts - current options displayed in the config
 */
function validateDynamicMultiEnum(inst, validation, cfgName, selectedOpt,
    validOpts)
{
    const arraySupported = _.map(validOpts, "name");
    const validArrayOptsSelected = _.intersection(arraySupported,
        selectedOpt);

    const valid = (_.isEqual(_.sortBy(selectedOpt),
        _.sortBy(validArrayOptsSelected)));

    if(inst !== null && validation !== null && !valid)
    {
        validation.logError("Selected option is invalid", inst, cfgName);
    }

    return{
        validOptsSelected: validArrayOptsSelected,
        valid: valid
    };
}

/*!
 * ======== getSafeDynamicConfig ========
 * Safely retrieve the value of the config by returning the instance value it's
 * valid, otherwise returns the default value.
 *
 * Due to their nature, dynamic enum configurables may be incorrectly modified
 * through the .syscfg file. While all dynamic configs have validation functions
 * to detect such errors, the dependency of the radio config module requires
 * safe access to certain dynamic configs to avoid SysConfig breaks.
 *
 * @param inst - 15.4 module instance (null during initialization)
 * @returns - config value in instance (if valid), otherwise config default
 */
function getSafeDynamicConfig(inst, cfgName, defaultValue, validOptions)
{
    // Access instance value
    let safeConfig = inst[cfgName];

    // Verify config value without raising GUI error (handled in validate())
    const valid = validateDynamicEnum(inst, null, cfgName, validOptions);
    if(!valid)
    {
        safeConfig = defaultValue;
    }

    return(safeConfig);
}

/*!
 *  ======== convertToCByteArray ========
 *  Turns an array specifying bits (starting from bit 0) to a byte array with
 *  the specified bits set. LSB is the 0th byte of the array.
 *  Example:
 *  bitsArray = [1,8]
 *  corresponding c byte array
 *  cByteArray = [2,1,0,0,0,0,0,0,0,0,0,.. up to num bytes]
 *
 *  @param bitsArray - array of bits to be set, e.g. [1,8]
 *  @param numBytes  - total number of bytes
 *  @returns Array   - byte array with specified bits set
 */
function convertToCByteArray(bitsArray, numBytes)
{
    const arr = _.fill(Array(numBytes), 0); // array filled with number of 0's
    _.each(bitsArray, (bit) =>
    {
        const arrIndex = Math.floor(bit / 8);
        const arrBit = bit - (arrIndex * 8);
        arr[arrIndex] |= (1 << arrBit);
    });
    return(arr);
}

/*!
 *  ======== channelMaskCHexStrArr ========
 *  Returns array of bytes where each byte is presented as a hexadecimal string
 *  Example: ["0x02","0x00",...]
 *
 *  @param channelMask - channels selected in config
 *  @returns String    - array of hexadecimal strings representing channel mask
 */
function channelMaskCHexStrArr(channelMask)
{
    const channelMaskCByteArr = convertToCByteArray(channelMask, 17);

    return(_.map(channelMaskCByteArr, (byte) => toHexString(byte, 2)));
}

/*
 * ======== findConfig ========
 * Finds and returns the configurable with the matching provided name
 *
 * @param configArray  - A module's configurable array
 * @param configName - The name of the configurable to search for
 *
 * @returns - undefined if the configurable is not found, otherwise the entire
 *            configurable object
 */
function findConfig(configArray, configName)
{
    let enumDef;

    let element = null;
    for(element of configArray)
    {
        // If the element contains a group, need to search it's configurables
        if("config" in element)
        {
            // Recursively search the sub-groups config array
            enumDef = findConfig(element.config, configName);

            // Stop searching if the configurable was found in the sub-group
            if(enumDef !== undefined)
            {
                break;
            }
        }
        else if(element.name === configName)
        {
            // Stop searching if the current element is the correct configurable
            enumDef = element;
            break;
        }
    }

    return(enumDef);
}

/*
 * ======== findAllConfigs ========
 * Finds and returns a list of all configurables within array
 *
 * @param configArray - A module's configurable arrays
 * @returns - list of names of all configurable objects within array
 */
function findAllConfigs(configArray)
{
    let element = null;
    let allConfigs = [];

    for(element of configArray)
    {
        // If the element contains a group, need to search it's configurables
        if("config" in element)
        {
            // Recursively search the sub-groups config array
            allConfigs = allConfigs.concat(findAllConfigs(element.config));
        }
        else if(element.name !== undefined)
        {
            // Add to list if the current element is a configurable
            allConfigs.push(element.name);
        }
    }

    return(allConfigs);
}

/*
 * ======== setConfigHiddenState ========
 * Sets the visibility of the selected config
 *
 * @param inst    - module instance containing the config that changed
 * @param ui      - user interface object
 * @param cfgName - name of config
 * @param configArrary - A module's configurable array
 * @param getHidden - function to set configurable visibility
 */
function setConfigHiddenState(inst, ui, cfgName, configArray, getHidden)
{
    // Set visibility of config
    ui[cfgName].hidden = getHidden(inst, cfgName);

    if(ui[cfgName].hidden)
    {
        // Get a list of all nested and unnested configs
        const configToReset = findConfig(configArray, cfgName);
        // Restore the default value for the hidden parameter.
        restoreDefaultValue(inst, configToReset, cfgName);
    }
}

/*
 * ======== setAllConfigsHiddenState ========
 * Sets the visibility of all configs within the config array
 *
 * @param inst    - module instance
 * @param ui      - user interface object
 * @param configArray - A module's configurable array
 * @param getHidden - function to get configurable visibility
 * @param setHidden - Optional function to set configurable visibility. Uses
 *                    default setConfigHiddenState if none specified
 */
function setAllConfigsHiddenState(inst, ui, configArray, getHidden,
    setHidden = setConfigHiddenState)
{
    let cfgName;

    // Set visibility of all configs
    const allConfigs = findAllConfigs(configArray);
    for(cfgName of allConfigs)
    {
        setHidden(inst, ui, cfgName, configArray, getHidden);
    }
}

/*
 * ======== restoreDefaultValue ========
 * Restore the configurable's value to the default.
 *
 * @param inst  - The module instance
 * @param _cfg - the config that will be restored to default
 * @param cfgName - The name of the configurable to change back to default
 *
 * @returns - void
 */
function restoreDefaultValue(inst, _cfg, cfgName)
{
    /* if you find a match, set the default and return */
    if(_cfg.name === cfgName)
    {
        /* Make sure a default exists */
        if(_cfg.default !== null && _cfg.default !== undefined)
        {
            /* Set the instance based on the default. */
            inst[cfgName] = _cfg.default;
        }
    }
}

exports = {
    getMigrationMarkdown: getMigrationMarkdown,
    isMigrationValid: isMigrationValid,
    migrate: migrate,
    isSub1GHzDevice: isSub1GHzDevice,
    is24GHzDevice: is24GHzDevice,
    is433MHzDevice: is433MHzDevice,
    ccfgSettings: ccfgSettings,
    isHighPADevice: isHighPADevice,
    getLaunchPadFromDevice: getLaunchPadFromDevice,
    getDeviceOrLaunchPadName: getDeviceOrLaunchPadName,
    cTypeMax: cTypeMax,
    toHexString: toHexString,
    validateRangeHex: validateRangeHex,
    validateRangeInt: validateRangeInt,
    validateDynamicEnum: validateDynamicEnum,
    validateDynamicMultiEnum: validateDynamicMultiEnum,
    channelMaskCHexStrArr: channelMaskCHexStrArr,
    findConfig: findConfig,
    findAllConfigs: findAllConfigs,
    setConfigHiddenState: setConfigHiddenState,
    setAllConfigsHiddenState: setAllConfigsHiddenState,
    restoreDefaultValue: restoreDefaultValue,
    getSafeDynamicConfig: getSafeDynamicConfig
};
