#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <net/if.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>
#include <includes.h>
#include <common.h>
#include <pinghost.h>


#define PACKET_SIZE 4096
uint16_t cal_chksum(unsigned short *addr,int len)
{
    int sum=0;
    int nleft = len;
    uint16_t *w = addr;
    uint16_t answer = 0;
  
    while(nleft > 1){
        sum += *w++;
        nleft -= 2;
    }
   
    if(nleft == 1){
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;    
    }
    sum = (sum >> 16) + (sum & 0xffff);       
    sum += (sum >> 16);      
    answer = ~sum;           
    return answer;
}

Bool isLive(const char *ifname, const char* hostname) 
{

    char    sendpacket[64], recvpacket[256];//4096  
    int    datalen = 56, ret; 
	struct ifreq ifr;
	struct timeval timeout = {5,0};
	struct sockaddr_in from;
	int fromlen = sizeof(struct sockaddr_in);
	
	/*
	if(inet_addr(url) == INADDR_NONE)
	{
		hostname = gethostbyname(url);
		if(hostname== NULL)
		{
			LOG_ERR("gethostbyname error\r\n");
			return 0;
		}
	}
	*/
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_RAW,IPPROTO_ICMP)) < 0) {
         LOG_ERR("socket error\r\n");
		 return False;
    }
  	if(ifname){
		strncpy(ifr.ifr_name,ifname, IFNAMSIZ);
		int err = setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr));
		if(err) {
			LOG_ERR("[%s] %s\r\n",ifr.ifr_name, strerror(errno));
			goto	Err_Handler;
		}
	}
    fcntl(sockfd, F_SETFL, O_NONBLOCK);//ÉèÖÃ·Ç×èÈû
    struct sockaddr_in dest_addr;
    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(hostname);
    int packsize;
    struct icmp *icmp;
    icmp = (struct icmp*)sendpacket;
    icmp->icmp_type = ICMP_ECHO;    
    icmp->icmp_code = 0;           
    icmp->icmp_cksum = 0;         
    icmp->icmp_seq = 1;      
    icmp->icmp_id = 0;          
    packsize = 8 + datalen; 
    icmp->icmp_cksum = cal_chksum((unsigned short *)icmp, packsize);

    if(sendto(sockfd, sendpacket, packsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0){
        LOG_ERR("sendto error\r\n");
		goto Err_Handler;
    }
  
    fd_set set;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);
    //read , write;
    ret = select(sockfd+1, &set, NULL, NULL, &timeout);
    if(ret == -1) {
         LOG_ERR("select error %s\n", strerror(errno));
		 goto Err_Handler;
    }else if(ret == 0 ) {
    	
        goto Err_Handler;
    }else{
        if( FD_ISSET(sockfd, &set) ){
			recvfrom(sockfd, recvpacket,sizeof(recvpacket), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
			close(sockfd);
            return True;
        }
    }
	
    // n = recvfrom(sockfd, recvpacket,sizeof(recvpacket), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
    // if(n<0) {
    //     perror("recvfrom error");
    // }else{
    //     printf("%d\n",n);
    // }
Err_Handler:
    close(sockfd);
    return False;
}
