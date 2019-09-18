
#ifndef _DRV_UART_H
#define _DRV_UART_H





int open_port(const char *dev);
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);











#endif