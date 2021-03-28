
COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR ?= $(abspath ../../../..)
include $(COM_TI_SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR)/imports.mak

# Find all subdirectories
TIRTOS-CCS-SUBDIRS   = $(wildcard */*/tirtos/ccs/.)
TIRTOS-IAR-SUBDIRS   = $(wildcard */*/tirtos/iar/.)

TIRTOS-SUBDIRS =
ifneq ($(CCS_ARMCOMPILER),)
	TIRTOS-SUBDIRS += $(TIRTOS-CCS-SUBDIRS)
endif
ifneq ($(IAR_ARMCOMPILER),)
	TIRTOS-SUBDIRS += $(TIRTOS-IAR-SUBDIRS)
endif

.PHONY: tirtos $(TIRTOS-SUBDIRS)

all: tirtos

tirtos: $(TIRTOS-SUBDIRS)

$(TIRTOS-SUBDIRS):
	@ $(MAKE) -C $@

CLEAN-TIRTOS-SUBDIRS = $(addprefix clean-,$(TIRTOS-SUBDIRS))

.PHONY: clean-tirtos $(CLEAN-TIRTOS-SUBDIRS)

clean: clean-tirtos

clean-tirtos: $(CLEAN-TIRTOS-SUBDIRS)

$(CLEAN-TIRTOS-SUBDIRS):
	@ $(MAKE) -C $(subst clean-,,$@) clean
