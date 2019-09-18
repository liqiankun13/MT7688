#ifndef _WIFI_H
#define _WIFI_H
#include <arpa/inet.h> 
#include <net/if_arp.h>





int8_t getWifiRSSI();
in_addr_t net_get_ifaddr(char *ifname);




#endif


