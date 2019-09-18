
 #####################################################################
## file        : test makefile for build current dir .c            ##
## author      : jernymy                                           ##
## date-time   : 05/06/2010                                        ##
#####################################################################

include $(TOP_DIR)/Rules.make

COMPILE = $(CROSS_COMPILE)gcc -w
RM      = rm -rf

## debug flag
DBG_ENABLE   = 1

## source file path
SRC_PATH   := .

## target exec file name
TARGET     := main_app

## get all source files
SRCS         += $(wildcard $(SRC_PATH)/*.c)

## all .o based on all .c
OBJS        := $(SRCS:.c=.o)


## need libs, add at here
LIBS := pthread gpio osa system curl m airkiss

## used headers  file path
INCLUDE_PATH := . ./include $(TOP_DIR)/ipnc_rdk/build/include $(TOP_DIR)/ipnc_rdk/makerules $(TOP_DIR)/ipnc_rdk/framework/osa/inc \
				$(TOP_DIR)/ipnc_rdk/framework/system/include $(TOP_DIR)/ipnc_rdk/framework/gpio/include 

## used include librarys file path
LIBRARY_PATH := /lib $(TOP_DIR)/ipnc_rdk/build/lib/

## debug for debug info, when use gdb to debug
ifeq (1, ${DBG_ENABLE}) 
	CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1
endif

## get all include path
CFLAGS  += $(foreach dir, $(INCLUDE_PATH), -I$(dir))

## get all library path
LDFLAGS += $(foreach lib, $(LIBRARY_PATH), -L$(lib))

## get all librarys
LDFLAGS += $(foreach lib, $(LIBS), -l$(lib))



build:
	@echo [CC] $(TARGET)
	$(COMPILE) -c $(CFLAGS) $(SRCS)
	$(COMPILE) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

all: clean build
	
install:
	cp $(TARGET) $(TFTP_HOME)

clean:
	$(RM) $(OBJS) $(TARGET)
