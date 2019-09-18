ifneq ($(KERNELRELEASE),)

MOD_NAME = lcd
#MOD_SRC = gsenser.o 
obj-m := $(MOD_NAME).o 
#$(MOD_NAME)-objs :=  $(MOD_SRC)

else
KERNELDIR = /home/lqk/workspace/MT7688/openwrt_widora/build_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/linux-ramips_mt7688/linux-3.18.29
PWD := $(shell pwd)
ARCH:=arm
CROSS_COMPILE = mipsel-openwrt-linux-
CC = $(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld

#gpioi2c-y += gpio_i2c.o

modules:
		#@$(CC) -g -Wall -o i2c_read i2c_read.c 
		$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
		#make clean
		#make install
clean:

		rm -rf *.o *~ core .depend .*.cmd  *.mod.c .tmp_versions *.symvers *.order
APP:
		@$(CC) -g -Wall -o test test.c 
install:
		cp *.ko  /home/lqk/tftpboot 
		#cp  test /home/lqk/HD_DVR/ipnc/rootfs/opt/ipnc/modules/ 
.PHONY: modules clean install APP

endif
