# (c) Texas Instruments

ifndef $(INCLUDES_MK)
INCLUDES_MK = 1


##############
IPNC_RDK_DIR=/home/lqk/MT7688/openwrt_app/ipnc_rdk

GLOBAL_INC=$(IPNC_RDK_DIR)/makerules/


OSA_INC = -I$(IPNC_RDK_DIR)/framework/osa/inc



endif # ifndef $(INCLUDES_MK)

