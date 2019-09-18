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
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>

#include <includes.h>
#include <common.h>
#include <pinghost.h>


#define PACKET_SIZE 4096
/* 计算校验和的算法 */
uint16_t cal_chksum(unsigned short *addr,int len)
{
    int sum=0;
    int nleft = len;
    uint16_t *w = addr;
    uint16_t answer = 0;
    /* 把ICMP报头二进制数据以2字节为单位累加起来 */
    while(nleft > 1){
        sum += *w++;
        nleft -= 2;
    }
    /*
     * 若ICMP报头为奇数个字节，会剩下最后一字节。
     * 把最后一个字节视为一个2字节数据的高字节，
     * 这2字节数据的低字节为0，继续累加
     */
    if(nleft == 1){
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;    /* 这里将 answer 转换成 int 整数 */
    }
    sum = (sum >> 16) + (sum & 0xffff);        /* 高位低位相加 */
    sum += (sum >> 16);        /* 上一步溢出时，将溢出位也加到sum中 */
    answer = ~sum;             /* 注意类型转换，现在的校验和为16位 */
    return answer;
}

Bool isLive(const char* hostname) 
{

    char    sendpacket[PACKET_SIZE];    /* 发送的数据包 */
    char    recvpacket[PACKET_SIZE];    /* 接收的数据包 */
    pid_t    pid;
    int    datalen = 56;    /* icmp数据包中数据的长度 */
    struct protoent *protocol;
	in_addr_t inaddr;
	struct hostent *host;
	
    protocol = getprotobyname("icmp");
    int sockfd;
    int size = 50*1024;
    if((sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto)) < 0) {
         LOG_ERR("socket error");
		 return False;
    }
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size) );
    
    struct sockaddr_in dest_addr;
    bzero(&dest_addr, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(hostname);
/*	if( inaddr=inet_addr(hostname)==INADDR_NONE)
    {   if((host=gethostbyname(hostname) )==NULL)
            {       
				LOG_ERR("gethostbyname error");
				 return False;
            }
            memcpy( (char *)&dest_addr.sin_addr,host->h_addr,host->h_length);
    }
    else    
        memcpy( (char *)&dest_addr,(char *)&inaddr,host->h_length);
	*/
    //send packet;
    int packsize;
    struct icmp *icmp;
    struct timeval *tval;
    icmp = (struct icmp*)sendpacket;
    icmp->icmp_type = ICMP_ECHO;    /* icmp的类型 */
    icmp->icmp_code = 0;            /* icmp的编码 */
    icmp->icmp_cksum = 0;           /* icmp的校验和 */
    icmp->icmp_seq = 1;       /* icmp的顺序号 */
    icmp->icmp_id = pid;            /* icmp的标志符 */
    packsize = 8 + datalen;   /* icmp8字节的头 加上数据的长度(datalen=56), packsize = 64 */
    tval = (struct timeval *)icmp->icmp_data;    /* 获得icmp结构中最后的数据部分的指针 */
    gettimeofday(tval, NULL); /* 将发送的时间填入icmp结构中最后的数据部分 */
    icmp->icmp_cksum = cal_chksum((unsigned short *)icmp, packsize);/*填充发送方的校验和*/

    if(sendto(sockfd, sendpacket, packsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0){
        LOG_ERR("sendto error");
		  goto Err_Handler;
    }
   // printf("send %d, send done\n",1 );
    int n;
    struct sockaddr_in from;
    int fromlen = sizeof(from);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    struct timeval timeo = {1,0};
    fd_set set;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);
    //read , write;
    int retval = select(sockfd+1, &set, NULL, NULL, &timeo);
    if(retval == -1) {
       // printf("select error\n");
		 goto Err_Handler;
    }else if(retval == 0 ) {
    	
       // printf("timeout\n");
        goto Err_Handler;
    }else{
        if( FD_ISSET(sockfd, &set) ){
			close(sockfd);
        //    printf("host is live\n");
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
