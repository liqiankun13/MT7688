# ============================================================================
# Copyright (c) Texas Instruments Inc 2013
#
# Use of this software is controlled by the terms and conditions found in the
# license agreement under which this software has been supplied or provided.
# ============================================================================

# Default build environment, windows or linux
ifeq ($(OS), )
  OS := Linux
endif

#####################################################################################
#CONFIGURATION
#####################################################################################

SYSTEM_PLATFORM  := IPNC
#SYSTEM_PLATFORM  := EVM


# Values are "LOW_POWER" and "FULL_FEATURE"
IPNC_CONFIG := FULL_FEATURE
#IPNC_CONFIG := LOW_POWER

# Memory foot print
MEMORY_CONFIG := 256MB
#MEMORY_CONFIG := 512MB



# Hardware Revision: REV1 or REV2 that the Software to be built for
# REV1 is the older design which has higher power design (initial design)
# REV2 is the new low power design which has low power numbers compared to older design
# Please note: DM8127 runs with REV1 design with 3.3V I/O
# And DM385, DMVA3 has support for both REV1 and REV2 and has 1.8V I/O
# And DM388 supports REV2 only and runs with 1.8V I/O
#HARDWARE_REVISION := REV1
HARDWARE_REVISION := REV2




################################################################################
# All build tools paths are set in this section
################################################################################

BASE_INSTALL_DIR    := $(shell pwd)
EXTERN_OPENLIB      :=/home/lqk/workspace/openlib/lib

# Defining the install base directory for IPNC RDK
IPNC_INSTALL_DIR    := $(BASE_INSTALL_DIR)/ipnc_rdk
TOOLS_INSTALL_DIR   := $(BASE_INSTALL_DIR)/ti_tools

# Defining the build tools directory for IPNC RDK
BUILD_TOOL_DIR      := $(TOOLS_INSTALL_DIR)/linux_devkit
BUILD_TOOL_PREFIX   := /home/lqk/workspace/MT7688/openwrt_widora/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-uclibc-


# The directory that points to the Linux Support Package
lsp_PATH	  := $(TOOLS_INSTALL_DIR)/ipnc_psp_arago
KERNELDIR         := $(lsp_PATH)/kernel
UBOOTDIR          := $(lsp_PATH)/u-boot

# The directory that points to where filesystem is mounted
FILESYS_INSTALL_DIR  := $(IPNC_INSTALL_DIR)/target
TARGET_FS            := $(FILESYS_INSTALL_DIR)/filesys


# The directory that points IPNC RDK source code
IPNC_DIR          := $(IPNC_INSTALL_DIR)/ipnc_app
TFTP_HOME         := /home/lqk/workspace/tftpboot
FRAMEWORK_DIR	  := $(IPNC_INSTALL_DIR)/framework
BUILD_INSTALL_DIR := $(IPNC_INSTALL_DIR)/build






################################################################################
# All EXPORT symbols are defined here
################################################################################
export OS
export PLATFORM
export CORE
export PROFILE_m3vpss
export PROFILE_m3video
export PROFILE_c6xdsp
export CODEGEN_PATH_M3
export CODEGEN_PATH_A8
export CODEGEN_PATH_DSP
export ROOTDIR
export bios_PATH
export xdc_PATH
export iss_PATH
export hdvpss_PATH
export ipc_PATH
export fc_PATH
export xdais_PATH
export h264dec_PATH
export h264enc_PATH
export jpegenc_PATH
export jpegdec_PATH
export mctnf_PATH
export hdvicplib_PATH
export linuxdevkit_PATH
export edma3lld_PATH
export XDCPATH
export KW_BUILD
export syslink_PATH
export KERNELDIR
export TARGET_FS
export TARGET_MCFW_DIR
export ROOT_FILE_SYS
export MCFW_ROOT_PATH
export UBOOTDIR
export DVR_RDK_BOARD_TYPE
export DEST_ROOT
export IPNC_INSTALL_DIR
export TFTP_HOME
export LINUX_MEM
export BUILD_TOOL_DIR
export BUILD_TOOL_PREFIX
export IPNC_DIR
export PUBLIC_INCLUDE_DIR
export APP_LIB_DIR
export IPNC_INTERFACE2_DIR
export PUBLIC_INCLUDE2_DIR
export APP_LIB2_DIR
export EXEC_DIR
export syslink_INCLUDE
export syslink_LIB_DIR
export ipc_INCLUDE
export linuxutils_PATH
export IPNC_CONFIG
export POWER_OPT_DSP_OFF 
export POWER_OPT_DSS_OFF 
export IMGS_ID
export OSD_MODE
export TREAT_WARNINGS_AS_ERROR
export SC_SCRIPTS_BASE_DIR
export CMEM_LIB_DIR
export CMEM_INC_DIR
export mpeg4enc_PATH
export mpeg4dec_PATH
export CAPTURE_DISPLAY_MODE_ON
export CAPTURE_MCTNF_DISP_MODE_ON
export FRAMES_TO_A8
export MEMORY_256MB
export IPNC_TILER_ENABLE
export CAMERA_TILER_ENABLE
export BTE_ENABLE
export IPNC_DEVICE
export TARGET_FS_DIR
export AES_MODULE_MODE
export USE_A8_FOR_TC_CALC
export USE_ISS_BOXCAR
export TEARDOWN_LOAD_UNLOAD
export SENSOR_DATA_OP
export ENC_A8_DEC_USE_CASE
export LOW_POWER_OPP100_MODE
export QUAD_STREAM_NF_USE_CASE
export HARDWARE_REVISION
export CAPTURE_MODE
export WDR_ON
export MJPEG_RC_MODE
export PLATFORM_NAME
export RVM_DEMO_MODE
export DMVA3_VNF_MODE
export GUI_ENGINE_MODE
export SENSOR_BOARD_VENDOR
export EXTERN_OPENLIB
