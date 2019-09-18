#include <sys_proc.h>
#include <osa_thr.h>
#include <sysCfg.h>
#include <includes.h>
#include <common.h>
#include <sys/types.h>
 #include <sys/socket.h>
 #include <sys/ioctl.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <linux/wireless.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <netinet/in.h>
 #include <unistd.h>


 /****************************
 无线设备的这些信息通过iw_statistics结构进行保存，通过get_wireless_stats函数获取 ****************************/
/* 
 struct iw_statistics
  {
	  __u16 	   status;		  //* Status * - device dependent for now

	  struct iw_quality    qual;		//* Quality of the link * (instant/mean/max)
	  struct iw_discarded	 discard;	 //* Packet discarded counts
	  struct iw_missed	  miss; 	   //* Packet missed counts
  };
*/
int8_t getWifiRSSI()
{
	int sockfd;
	struct iw_statistics stats;
	struct iwreq req;
	memset(&stats, 0, sizeof(struct iw_statistics));
	memset(&req, 0, sizeof(struct iwreq));
	sprintf(req.ifr_name, "apcli0");
	req.u.data.pointer = &stats;
	req.u.data.length = sizeof(struct iw_statistics);
#ifdef CLEAR_UPDATED
	req.u.data.flags = 1;
#endif
 
  /* Any old socket will do, and a datagram socket is pretty cheap */
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) //#define AF_INET   2 	 // internetwork: UDP, TCP, etc.
	{ 						   //数据报套接字类型为SOCK_DGRAM	 0:应使用的传输层协议
		LOG_ERR("Could not create simple datagram socket <%s>",strerror(errno));
		return 0;
	}
	struct ifreq ifr;
	strcpy(ifr.ifr_name, "apcli0");/*获得链接状态*/
	if(ioctl(sockfd, SIOCGIFFLAGS, &ifr) <0 ) 
	{
           close(sockfd);
           return 0;
    }
	if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) 
	{		
		close(sockfd);		
		return 0; 	 
		
 	}
  	/* Perform the ioctl */
	if(ioctl(sockfd, SIOCGIWSTATS, &req) == -1)
	{
		LOG_ERR("Error performing SIOCGIWSTATS <%s>", strerror(errno));
		close(sockfd);
		return 0;
	}
 
	close(sockfd);
	//LOG_DBG("Signal[%d] level%s is %d%s.\n",ifr.ifr_flags&IFF_RUNNING,(stats.qual.updated & IW_QUAL_DBM ? " (in dBm)" :""),(signed char)stats.qual.level,(stats.qual.updated & IW_QUAL_LEVEL_UPDATED ? " (updated)" :""));
	return (int8_t)stats.qual.level;
}


