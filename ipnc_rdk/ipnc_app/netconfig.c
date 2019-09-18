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
 #include <netconfig.h>


 in_addr_t net_get_ifaddr(char *ifname)
 { 
 	struct ifreq ifr;	 int skfd;	 
	struct sockaddr_in *saddr; 
	if ( (skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {		 
		printf("socket error");	 
		return -1; 
	}	 
	//int flags = fcntl(skfd, F_GETFL, 0); 			//获取文件的flags值。  
	// fcntl(skfd, F_SETFL, flags | O_NONBLOCK);	 //设置成非阻塞模式；	
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);	 
	if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) 
	{		
		 
		close(skfd);		//126 或获取不了ip
		return 0; 	 
 	}
	saddr = (struct sockaddr_in *) &ifr.ifr_addr;	 
	close(skfd);	
	
	return saddr->sin_addr.s_addr;

}