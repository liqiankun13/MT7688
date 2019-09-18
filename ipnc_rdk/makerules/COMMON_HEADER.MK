# (c) Texas Instruments

ifndef $(COMMON_HEADER_MK)
COMMON_HEADER_MK = 1


CC=$(BUILD_TOOL_PREFIX)gcc
AR=$(BUILD_TOOL_PREFIX)ar
LD=$(BUILD_TOOL_PREFIX)gcc 

LIB_BASE_DIR=$(IPNC_INSTALL_DIR)/build/lib
OBJ_BASE_DIR=$(IPNC_INSTALL_DIR)/build/obj
EXE_BASE_DIR=$(IPNC_INSTALL_DIR)/build/bin

ifeq ($(CONFIG),)
LIB_DIR=$(LIB_BASE_DIR)
else
LIB_DIR=$(LIB_BASE_DIR)
endif

CC_OPTS=-c -Wall

ifeq ($(TREAT_WARNINGS_AS_ERROR), yes)
CC_OPTS+= 
#-Werror
endif

ifeq ($(CONFIG), debug)

CC_OPTS+=-g
OPTI_OPTS=
DEFINE=-DDEBUG

else

CC_OPTS+=
OPTI_OPTS=-O3
DEFINE=

endif

AR_OPTS=-rc
LD_OPTS=-lpthread -lm -lstdc++

DEFINE += $(IPNC_RDK_CFLAGS)
FILES=$(subst ./, , $(foreach dir,.,$(wildcard $(dir)/*.c)) ) 

vpath %.a $(LIB_DIR)

include $(IPNC_INSTALL_DIR)/makerules/INCLUDES.MK

INCLUDE=
INCLUDE+=$(KERNEL_INC)
INCLUDE+=$(COMMON_INC)

endif # ifndef $(COMMON_HEADER_MK)

