# 
# application need lib
# default is static lib 
# 
#

EXE_TARGET:
	make -fMAKEFILE.MK libs TARGET=depend
	make -fMAKEFILE.MK libs TARGET=so
	#make -fMAKEFILE.MK utils TARGET=exe
	#make -C$(MCFW_ROOT_PATH)/mcfw/src_linux/osa_kermod/build install
	
ALL:
	make -fMAKEFILE.MK -C$(IPNC_INSTALL_DIR)/framework/gpio MODULE=gpio $(TARGET)
	make -fMAKEFILE.MK -C$(IPNC_INSTALL_DIR)/framework/osa/src MODULE=osa $(TARGET)
	make -fMAKEFILE.MK -C$(IPNC_INSTALL_DIR)/framework/system MODULE=system $(TARGET)
	#make -fMAKEFILE.MK -C$(IPNC_INSTALL_DIR)/framework/ADV7842 MODULE=ADV7842 $(TARGET)
	
libs:ALL 

clean:
	make -fMAKEFILE.MK libs TARGET=clean

all: clean EXE_TARGET

.PHONY: libs  clean all 
