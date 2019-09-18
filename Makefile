# ============================================================================
# Copyright (c) Texas Instruments Inc 2013
#
# Use of this software is controlled by the terms and conditions found in the
# license agreement under which this software has been supplied or provided.
# ============================================================================

DIR=default
FILE=default

include Rules.make


ipncapp: app appinstall 

clean: appclean 

all:  framework_clean framework clean ipncapp

##### Code Checkers #####

framework:
	@$(MAKE) -fMAKEFILE.MK -C $(IPNC_INSTALL_DIR)/framework 
framework_clean:
	@$(MAKE) -fMAKEFILE.MK -C $(IPNC_INSTALL_DIR)/framework clean
framework_install:
	cp $(BUILD_INSTALL_DIR)/lib/*.so  $(TFTP_HOME)
	
##### IPNCAPP BUILD #####

applibs:
ifneq ($(MAKE_TARGET), depend)
	@$(MAKE) -C$(IPNC_DIR) ARCH=arm CROSS_COMPILE=$(BUILD_TOOL_PREFIX) $(MAKE_TARGET) TOP_DIR=$(BASE_INSTALL_DIR)
endif

appclean:
	$(MAKE) applibs MAKE_TARGET=clean

appdepend:
	$(MAKE) applibs MAKE_TARGET=depend  

appinstall:
	$(MAKE) applibs MAKE_TARGET=install

app: appdepend applibs 

appall: appclean app

	
##### U-BOOT BUILD #####


##### LSP BUILD #####
lspbuild:
	make -C$(KERNELDIR) ARCH=arm CROSS_COMPILE=$(BUILD_TOOL_PREFIX) $(MAKE_TARGET)
	
lspmenu:
	make lspbuild MAKE_TARGET=menuconfig

lspcfg:
	make lspbuild MAKE_TARGET=$(PLATFORM_CFG)_$(SYSTEM_CFG)_$(FS_CFG)_defconfig
	
lsp:
	cp $(linuxutils_PATH)/packages/ti/sdo/linuxutils/cmem/src/module/cmemk.o $(KERNELDIR)/drivers/char/
	#make lspcfg
	make lspbuild MAKE_TARGET=uImage
	make lspbuild MAKE_TARGET=modules
	cp $(KERNELDIR)/arch/arm/boot/uImage $(TFTP_HOME)/uImage

##### SYSLINK BUILD #####



##### CMEM MAKE #####


##### FILESYS MAKE #####

ubifs:
	mkdir -p $(IPNC_INSTALL_DIR)/tmp
	sudo rm $(TARGET_FS)/var/volatile/lock/* -rf
	$(MTD_UTILS)/mkfs.ubifs -r $(TARGET_FS) -F -o ./tmp/ubifs.img -m 2048 -e 126976 -c 827
	$(MTD_UTILS)/ubinize -o $(TFTP_HOME)/ubifs_$(SYSTEM_CFG).bin -m 2048 -p 128KiB -O 2048 $(IPNC_INSTALL_DIR)/ubinize.cfg
	rm -rf $(IPNC_INSTALL_DIR)/tmp
	

