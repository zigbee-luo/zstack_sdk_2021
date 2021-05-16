PROJECT_LOC = $(abspath ../../)
PROJECT_ROOT = $(PROJECT_LOC)
COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR ?= $(abspath ../../../../../../..)

include $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/imports.mak

XDCPATH = $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source;$(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/kernel/tirtos/packages;
SYSCONFIG_PRODUCT := $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/.metadata/product.json

CC = "$(CCS_ARMCOMPILER)/bin/armcl"
LNK = "$(CCS_ARMCOMPILER)/bin/armcl"

XDCTARGET = ti.targets.arm.elf.M4F
PLATFORM = ti.platforms.simplelink:CC1352R1F3

OBJECTS = ti_drivers_config.obj ti_devices_config.obj ti_radio_config.obj CC1352R1_LAUNCHXL_fxns.obj mac_user_config.obj mac_settings.obj gpd_sw.obj gpd_sw_data.obj main.obj cui.obj crc.obj nvocmp.obj saddr.obj mac_util.obj util_timer.obj gpd.obj gpd_memory.obj macTask.obj api_mac.obj osal_port.obj osal_port_timers.obj dbg.obj mac_cfg.obj rom_init_154.obj fh_rom_init.obj hmac_rom_init.obj lmac_rom_init.obj icall_osal_rom_init.obj

CONFIGPKG = gpd_switch

NAME = gpd_switch

CFLAGS = -I../.. \
    -I. \
    -DxBATTERYLESS_DEVICE \
    --silicon_version=7M4 \
    --code_state=16 \
    --float_support=FPv4SPD16 \
    --opt_level=4 \
    --opt_for_speed=0 \
    --abi=eabi \
    --c99 \
    --diag_warning=225 \
    --diag_wrap=off \
    --gen_func_subsections=on \
    --fp_reassoc=off \
    --unaligned_access=on \
    --embedded_constants=on \
    --wchar_t=16 \
    --enum_type=packed \
    --common=on \
    --sat_reassoc=off \
    --plain_char=unsigned \
    -DTIMAC_ROM_IMAGE_BUILD \
    -DTIMAC_ROM_PATCH \
    -DxCUI_DISABLE \
    -DZSTACK_SECURITY \
    -DOSAL_PORT2TIRTOS \
    -DOSAL_PORT2TIRTOS_OSALMAP \
    -DMAX_DEVICE_TABLE_ENTRIES=1 \
    -DZSTACK_GPD \
    -DFREQ_2_4G \
    -DNV_RESTORE \
    -DNV_INIT \
    -DFEATURE_NON_BEACON_MODE \
    "-I${PROJECT_ROOT}/Application/StartUp" \
    "-I${PROJECT_ROOT}/Application/Services" \
    "-I${PROJECT_ROOT}/Application/util" \
    "-I${PROJECT_ROOT}/Common/gpd" \
    "-I${PROJECT_ROOT}/Application" \
    "-I${PROJECT_ROOT}/Stack/Config" \
    "-I${PROJECT_ROOT}/Stack/osal_port" \
    "-I${PROJECT_ROOT}/Stack/HAL/Crypto" \
    "-I${PROJECT_ROOT}/Stack/HAL/Platform" \
    "-I${PROJECT_ROOT}/Stack/HAL/RF" \
    "-I${PROJECT_ROOT}/Stack/MAC/HighLevel" \
    "-I${PROJECT_ROOT}/Stack/MAC/LowLevel" \
    "-I${PROJECT_ROOT}/Stack/MAC" \
    "-I${PROJECT_ROOT}/Stack/ROM" \
    "-I${PROJECT_ROOT}/Stack/sys" \
    "-I${PROJECT_ROOT}/Stack/sec" \
    "-I${CG_TOOL_ROOT}/include" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/mt" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/osal" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/services/saddr" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/services/sdata" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/hal/crypto" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/hal/platform" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/hal/rf" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/stack/nwk" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/zmac" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/zmac/f8w" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/stack/api" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/stack/af" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/stack/sec" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/stack/sys" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/stack/zdo" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/gpd" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/common/cui" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/fh" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/high_level" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/inc" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/inc/cc13xx" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/low_level/cc13xx" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/tracer" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/rom" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/common/heapmgr" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/common/osal_port" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/common/osal_port/osal_port_tirtos" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/common/nv" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/util" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/common/util" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/config" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/startup" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/common/api/inc" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/devices/cc13x2_cc26x2" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/devices/cc13x2_cc26x2/inc" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/devices/cc13x2_cc26x2/driverlib" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/common/time" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/zstack/apps/gpd_switch" \
    -DDEVICE_FAMILY=cc13x0 \
    -DDeviceFamily_CC13X2 \
    -DNO_CC1312R1_SUPPORT \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/common/boards" \
    "-I$(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source" \
    "-I${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/posix/ccs" \
    --little_endian \
    --display_error_number \
    --diag_warning=255 \
    --symdebug:dwarf \
    "-I$(CCS_ARMCOMPILER)/include"

LFLAGS = --diag_wrap=off \
    --unused_section_elimination=on \
    "--xml_link_info=${ProjName}_linkInfo.xml" \
    -x \
    "--library=${COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR}/source/ti/ti154stack/library/tirtos/ccs/timac_rom_PG2_0_rom_api_linker.cmd" \
    "-l${PROJECT_BUILD_DIR}/syscfg/ti_utils_build_linker.cmd.genlibs" \
    "-i$(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source" \
    "-i$(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/kernel/tirtos/packages" \
    "-l$(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/devices/cc13x2_cc26x2/driverlib/bin/ccs/driverlib.lib" \
    "$(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/boards/cc13x2_cc26x2/cc13x2_cc26x2.cmd" \
    "-m$(NAME).map" \
    --warn_sections \
    --display_error_number \
    --rom_model \
    "-i$(CCS_ARMCOMPILER)/lib" \
    -llibc.a

.PRECIOUS: $(CONFIGPKG)/compiler.opt $(CONFIGPKG)/linker.cmd

all: $(NAME).out

$(CONFIGPKG)/compiler.opt: $(CONFIGPKG)/linker.cmd

$(CONFIGPKG)/linker.cmd: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/rtos/app.cfg
	@ echo Running Configuro...
	@ $(XDC_INSTALL_DIR)/xs --xdcpath="$(XDCPATH)" xdc.tools.configuro -c "$(CCS_ARMCOMPILER)" -t $(XDCTARGET) -p $(PLATFORM) --compileOptions "$(CFLAGS)" -o "$(CONFIGPKG)" $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/rtos/app.cfg

.INTERMEDIATE: syscfg
ti_drivers_config.c ti_drivers_config.h ti_zstack_config.h ti_devices_config.c ti_radio_config.c ti_radio_config.h: syscfg
	@ echo generation complete

syscfg: ../../gpd_switch.syscfg
	@ echo Generating configuration files...
	@ $(SYSCONFIG_TOOL) --product $(SYSCONFIG_PRODUCT) --compiler ccs --output $(@D) $<

ti_drivers_config.obj: ti_drivers_config.c $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

CC1352R1_LAUNCHXL_fxns.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/boards/CC1352R1_LAUNCHXL/CC1352R1_LAUNCHXL_fxns.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

mac_user_config.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/common/boards/mac_user_config.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

mac_settings.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/low_level/cc13xx/mac_settings.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

gpd_sw.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/apps/gpd_switch/gpd_sw.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

gpd_sw_data.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/apps/gpd_switch/gpd_sw_data.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

main.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/startup/main.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

cui.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/common/cui/cui.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

crc.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/common/nv/crc.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

nvocmp.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/common/nv/nvocmp.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

saddr.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/services/saddr/saddr.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

mac_util.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/common/util/mac_util.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

util_timer.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/common/util/util_timer.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

gpd.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/gpd/gpd.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

gpd_memory.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/zstack/gpd/gpd_memory.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

macTask.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/common/osal_port/osal_port_tirtos/macTask.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

api_mac.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/common/osal_port/osal_port_tirtos/api_mac.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

osal_port.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/common/osal_port/osal_port_tirtos/osal_port.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

osal_port_timers.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/common/osal_port/osal_port_tirtos/osal_port_timers.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

dbg.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/tracer/dbg.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

mac_cfg.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/high_level/mac_cfg.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

rom_init_154.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/rom/rom_init_154.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

fh_rom_init.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/rom/fh_rom_init.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

hmac_rom_init.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/rom/hmac_rom_init.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

lmac_rom_init.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/rom/lmac_rom_init.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

icall_osal_rom_init.obj: $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/source/ti/ti154stack/rom/icall_osal_rom_init.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

ti_devices_config.obj: ti_devices_config.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

ti_radio_config.obj: ti_radio_config.c ti_drivers_config.h $(CONFIGPKG)/compiler.opt
	@ echo Building $@
	@ $(CC) $(CFLAGS) $< --cmd_file=$(CONFIGPKG)/compiler.opt --output_file=$@

$(NAME).out: $(OBJECTS) $(CONFIGPKG)/linker.cmd
	@ echo linking $@
	@ $(LNK) $(CFLAGS) -z $(OBJECTS) -l$(CONFIGPKG)/linker.cmd $(LFLAGS) -o $(NAME).out

clean:
	@ echo Cleaning...
	@ $(RM) $(OBJECTS) > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).out > $(DEVNULL) 2>&1
	@ $(RM) $(NAME).map > $(DEVNULL) 2>&1
	@ $(RMDIR) $(NAME) > $(DEVNULL) 2>&1
	@ $(RM) ti_drivers_config.c ti_drivers_config.h ti_zstack_config.h ti_devices_config.c ti_radio_config.c ti_radio_config.h > $(DEVNULL) 2>&1
