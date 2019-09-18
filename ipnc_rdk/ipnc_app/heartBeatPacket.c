#include <stdio.h>
#include <osa_thr.h>
#include <includes.h>
#include <sysCfg.h>
#include <common.h>
#include <sys_ctrl.h>
#include "LCD12864.h"
#include <ipnc_gio_util.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
	和服务器发送心跳包，或者通过icmp包检测服务器主机存在

*/

