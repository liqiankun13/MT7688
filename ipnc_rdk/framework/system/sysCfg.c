
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sysCfg.h>
#include <includes.h>
#include <common.h>


#define PARA_SAFE_CHECK(ParaPTR,MIN,MAX) if(*(ParaPTR) < MIN)  \
											*(ParaPTR) = MIN;	\
										 if(*(ParaPTR) > MAX) 	\
										 	*(ParaPTR) = MAX;




#define Soft_VERSION "1.0.0"


volatile SYS_CFG_st gSYS_cfg_para = {0};

#define CFGDBG


int SYS_initALLCfgPara(SYS_CFG_st *CfgPara)
{
#if 0
	/************初始化系统参数***********************/
	CfgPara->sysPara.isUpdate = GetConfigFileIntValue("sys", "isUpdate", 0, MDVR_CFG_PATHNAME);
	CfgPara->sysPara.isBeep = GetConfigFileIntValue("sys", "isBeep", 1, MDVR_CFG_PATHNAME);
	CfgPara->sysPara.language = GetConfigFileIntValue("sys", "language", 1, MDVR_CFG_PATHNAME);
	CfgPara->sysPara.isLog  = GetConfigFileIntValue("sys", "isLog", 1, MDVR_CFG_PATHNAME);
	CfgPara->sysPara.isOSD= GetConfigFileIntValue("sys", "isOSD", 1, MDVR_CFG_PATHNAME);
	
	/************初始化视频参数***********************/
	CfgPara->videoPara.interface = GetConfigFileIntValue("video", "interface", DVR_INTERFACE_SDI, MDVR_CFG_PATHNAME);
	CfgPara->videoPara.fps = GetConfigFileIntValue("video", "fps", 0, MDVR_CFG_PATHNAME);
	CfgPara->videoPara.isAutoIdentify = GetConfigFileIntValue("video", "isAutoIdentify", 1, MDVR_CFG_PATHNAME);
	CfgPara->videoPara.inRes= GetConfigFileIntValue("video", "inRes", VIDEO_RES_HD_1080P, MDVR_CFG_PATHNAME);
	CfgPara->videoPara.outRes= GetConfigFileIntValue("video", "outRes", VIDEO_RES_HD_1080P, MDVR_CFG_PATHNAME);
	CfgPara->videoPara.chromat = GetConfigFileIntValue("video", "chromat", 0, MDVR_CFG_PATHNAME);
	
	/************初始化音频参数***********************/
	CfgPara->audioPara.inSrc = GetConfigFileIntValue("audio", "inSrc", 2, MDVR_CFG_PATHNAME);
	CfgPara->audioPara.gain = GetConfigFileIntValue("audio", "gain", 1, MDVR_CFG_PATHNAME);
	CfgPara->audioPara.quality= GetConfigFileIntValue("audio", "quality", 1, MDVR_CFG_PATHNAME);
	
	
	/************初始化网络参数***********************/
	CfgPara->netPara.isRTSP= GetConfigFileIntValue("network", "isRTSP", 1, MDVR_CFG_PATHNAME);
	CfgPara->netPara.isDHCP= GetConfigFileIntValue("network", "isDHCP", 1, MDVR_CFG_PATHNAME);
	//GetConfigFileStringValue("network", "devName", "MDVR",CfgPara->netPara.devName, sizeof(CfgPara->netPara.devName), MDVR_CFG_PATHNAME);
	GetConfigFileStringValue("network", "ipAddr", "192.168.1.168",CfgPara->netPara.ipAddr, sizeof(CfgPara->netPara.ipAddr), MDVR_CFG_PATHNAME);
	GetConfigFileStringValue("network", "netMask", "255.255.255.0",CfgPara->netPara.netMask, sizeof(CfgPara->netPara.netMask), MDVR_CFG_PATHNAME);
	GetConfigFileStringValue("network", "gateWayIp", "192.168.1.1",CfgPara->netPara.gateWayIp, sizeof(CfgPara->netPara.gateWayIp), MDVR_CFG_PATHNAME);
	GetConfigFileStringValue("network", "macAddr", "00:00:00:00:00:00",CfgPara->netPara.macAddr, sizeof(CfgPara->netPara.macAddr), MDVR_CFG_PATHNAME);
	/*****************nfs*************************/
	GetConfigFileStringValue("nfs", "serverIP", "192.168.1.8",CfgPara->netPara.NFServer.serverIP, sizeof(CfgPara->netPara.NFServer.serverIP), MDVR_CFG_PATHNAME);
	GetConfigFileStringValue("nfs", "username", "admin",CfgPara->netPara.NFServer.username, sizeof(CfgPara->netPara.NFServer.username), MDVR_CFG_PATHNAME);
	GetConfigFileStringValue("nfs", "passwd", "9999",CfgPara->netPara.NFServer.passwd, sizeof(CfgPara->netPara.NFServer.passwd), MDVR_CFG_PATHNAME);

	/***********初始化抓图参数***************************/
	CfgPara->pthotoPara.fFomat = GetConfigFileIntValue("photo", "fFomat", 1, MDVR_CFG_PATHNAME);
	
	/************初始化录像参数***********************/
	CfgPara->recordPara.quality = GetConfigFileIntValue("record", "quality", 1, MDVR_CFG_PATHNAME);
	CfgPara->recordPara.isMute = GetConfigFileIntValue("record", "isMute", 0, MDVR_CFG_PATHNAME);
	CfgPara->recordPara.fFomat = GetConfigFileIntValue("record", "fFomat", 1, MDVR_CFG_PATHNAME);
	CfgPara->recordPara.storage = GetConfigFileIntValue("record", "storage", 1, MDVR_CFG_PATHNAME);
	CfgPara->recordPara.timeLimit = GetConfigFileIntValue("record", "timeLimit", 1, MDVR_CFG_PATHNAME);
	CfgPara->recordPara.sizeLimit = GetConfigFileIntValue("record", "sizeLimit", 1, MDVR_CFG_PATHNAME);
	/************外围设备********************************************/
//	CfgPara->perpDev.isGpsON = GetConfigFileIntValue("Peripheral", "isGpsON", 0, MDVR_CFG_PATHNAME);
//	CfgPara->perpDev.nBaud = GetConfigFileIntValue("Peripheral", "nBaud", 0, MDVR_CFG_PATHNAME);
//	CfgPara->perpDev.isGpsChkTime = GetConfigFileIntValue("Peripheral", "isGpsChkTime", 21, MDVR_CFG_PATHNAME);
	gDVR_cfg_para.perpDev.nBaud = DVR_UART_4800B;

#endif 
	CfgPara->sysPara.is1thBoot = GetConfigFileIntValue("sys", "is1thBoot", False, SYS_CFG_PATHNAME);
	
	GetConfigFileStringValue("report", "date", "2019-01-01 00:00",CfgPara->report.date, sizeof(CfgPara->report.date), SYS_CFG_PATHNAME);
	CfgPara->report.result = GetConfigFileIntValue("report", "result", 0, SYS_CFG_PATHNAME);
	CfgPara->report.percent = GetConfigFileIntValue("report", "Percent", 0, SYS_CFG_PATHNAME);
	printf("\n\033[46;37m/***************************************************/\033[0m\n\n");	
	printf("soft version %s\n", Soft_VERSION);
	printf("soft date %s %s\n", __DATE__,	__TIME__);
	printf("\033[33mreport: %s   %d\033[0m\n",CfgPara->report.date,CfgPara->report.result);
	printf("\n\033[46;37m/***************************************************/\033[0m\n");

}






void GetCompletePath(const char *pszConfigFileName,  char *pszWholePath)
{
    char *pszHomePath      = NULL;
    char  szWholePath[256] = {0};

    // 先对输入参数进行异常判断
    if (pszConfigFileName == NULL || pszWholePath == NULL)
    {
        printf("GetCompletePath: input parameter(s) is NULL!\n");
        return;
    }

    pszHomePath = getenv("HOME");     // 获取当前用户所在的路径
    if (pszHomePath == NULL)
    {
        printf("GetCompletePath: Can't find home path!\n");
        return;
    }

    // 拼装配置文件路径
    snprintf(szWholePath, sizeof(szWholePath)-1, "%s/GetConfig/%s", pszHomePath, pszConfigFileName);

    strncpy(pszWholePath, szWholePath, strlen(szWholePath));
}

void GetStringContentValue(FILE *fp, const char *pszSectionName, const char *pszKeyName, char *pszOutput, uint32_t iOutputLen)
{
    char  szSectionName[100]    = {0};
    char  szKeyName[100]        = {0};
    char  szContentLine[256]    = {0};
    char  szContentLineBak[256] = {0};
    uint32_t iContentLineLen       = 0;
    uint32_t iPositionFlag         = 0;

    // 先对输入参数进行异常判断
    if (fp == NULL || pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL)
    {
        printf("GetStringContentValue: input parameter(s) is NULL!\n");
        return;
    }

    sprintf(szSectionName, "[%s]", pszSectionName);
    strcpy(szKeyName, pszKeyName);

    while (feof(fp) == 0)
    {
        memset(szContentLine, 0x00, sizeof(szContentLine));
        fgets(szContentLine, sizeof(szContentLine), fp);      // 获取段名

        // 判断是否是注释行(以;开头的行就是注释行)或以其他特殊字符开头的行
        if (szContentLine[0] == ';' || szContentLine[0] == '\r' || szContentLine[0] == '\n' || szContentLine[0] == '\0' /*|| szContentLine[0] == '#' || szContentLine[0] == 0x20*/)
        {
            continue;
        }

        // 匹配段名
        if (strncasecmp(szSectionName, szContentLine, strlen(szSectionName)) == 0)     
        {
            while (feof(fp) == 0)
            {
                memset(szContentLine,    0x00, sizeof(szContentLine));
                memset(szContentLineBak, 0x00, sizeof(szContentLineBak));
                fgets(szContentLine, sizeof(szContentLine), fp);     // 获取字段值

                // 判断是否是注释行(以;开头的行就是注释行)
                if (szContentLine[0] == ';')
                {
                    continue;
                }

                memcpy(szContentLineBak, szContentLine, strlen(szContentLine));

                // 匹配配置项名
                if (strncasecmp(szKeyName, szContentLineBak, strlen(szKeyName)) == 0)     
                {
                    iContentLineLen = strlen(szContentLine);
                    for (iPositionFlag = strlen(szKeyName); iPositionFlag <= iContentLineLen; iPositionFlag ++)
                    {
                        if (szContentLine[iPositionFlag] == ' ')
                        {
                            continue;
                        }
                        if (szContentLine[iPositionFlag] == '=')
                        {
                            break;
                        }

                        iPositionFlag = iContentLineLen + 1;
                        break;
                    }

                    iPositionFlag = iPositionFlag + 1;    // 跳过=的位置

                    if (iPositionFlag > iContentLineLen)
                    {
                        continue;
                    }

                    memset(szContentLine, 0x00, sizeof(szContentLine));
                    strcpy(szContentLine, szContentLineBak + iPositionFlag);
					int len = 0;
					len=strlen(szContentLine);
					int i = 0;
					int meetStartFlag = 0;//遇到有效字符开始保存

                    // 去掉内容中的无关字符
                    for (iPositionFlag = 0; iPositionFlag < len; iPositionFlag ++)
                    {
                    	if(meetStartFlag==0 && szContentLine[iPositionFlag] == ' ')
							continue;
						meetStartFlag = 1;
                        if (szContentLine[iPositionFlag] == '\r' || szContentLine[iPositionFlag] == '\n' || szContentLine[iPositionFlag] == '\0')
                        {
                            break;
                        }
						pszOutput[i] = szContentLine[iPositionFlag];
						i ++;
						if(i == iOutputLen)
						{
							pszOutput[iOutputLen-1] = '\0';
							break;
						}
                    }
						if(i > 0)
							pszOutput[i] = '\0';

                    // 将配置项内容拷贝到输出缓存中
                   // strncpy(pszOutput, szContentLine, iOutputLen-1);
                    break;
                }
                else if (szContentLine[0] == '[')
                {
                    break;
                }
            }
            break;
        }
    }
}


 
void GetConfigFileStringValue(const char *pszSectionName, const char *pszKeyName, const char *pDefaultVal, char *pszOutput, uint32_t iOutputLen, const char *pszConfigFileName)
{
    FILE  *fp                    = NULL;
    char  szWholePath[256]      = {0};

    // 先对输入参数进行异常判断
    if (pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL || pszConfigFileName == NULL)
    {
        printf("GetConfigFileStringValue: input parameter(s) is NULL!\n");
        return;
    }

    // 获取默认值
    if (pDefaultVal == NULL)
    {
        strcpy(pszOutput, "");
    }
    else
    {
        strcpy(pszOutput, pDefaultVal);
    }

    // 打开配置文件
   // GetCompletePath(pszConfigFileName, szWholePath);
    fp = fopen(pszConfigFileName, "r");
    if (fp == NULL)
    {
        printf("GetConfigFileStringValue: open %s failed!\n", pszConfigFileName);
        return;
    }

    // 调用函数用于获取具体配置项的值
    GetStringContentValue(fp, pszSectionName, pszKeyName, pszOutput, iOutputLen);

    // 关闭文件
    fclose(fp);
    fp = NULL;
}


uint32_t GetConfigFileIntValue(const char *pszSectionName, const char *pszKeyName, uint32_t iDefaultVal, const char *pszConfigFileName)
{
    char  szGetValue[512] = {0};
    uint32_t  iGetValue       = 0;

    // 先对输入参数进行异常判断
    if (pszSectionName == NULL || pszKeyName == NULL || pszConfigFileName == NULL)
    {
        printf("GetConfigFileIntValue: input parameter(s) is NULL!\n");
        return -1;
    }

    GetConfigFileStringValue(pszSectionName, pszKeyName, NULL, szGetValue, 512-1, pszConfigFileName);    // 先将获取的值存放在字符型缓存中

    if (szGetValue[0] == '\0' || szGetValue[0] == ';')    // 如果是结束符或分号, 则使用默认值
    {
        iGetValue = iDefaultVal;
    }
    else
    {
        iGetValue = atoi(szGetValue);
    }

    return iGetValue;
}


void SetStringContentValue(FILE *fp, const char *pszSectionName, const char *pszKeyName, const char *pszInput)
{
    char  szSectionName[100]    = {0};
    char  szKeyName[100]        = {0};
   //  char  szContentLine[256]    = {0};
    char  szContentLineBak[256] = {0};
    uint32_t iContentLineLen       = 0;
    uint32_t iPositionFlag         = 0;
	char lineArray[128][128] = {0};
	uint32_t line = 0;
	
    // 先对输入参数进行异常判断
    if (fp == NULL || pszSectionName == NULL || pszKeyName == NULL || pszInput == NULL)
    {
        printf("GetStringContentValue: input parameter(s) is NULL!\n");
        return;
    }

    sprintf(szSectionName, "[%s]", pszSectionName);
    strcpy(szKeyName, pszKeyName);

    while (feof(fp) == 0)
    {
		fgets(lineArray[line++],127,fp);
		if(line > 128)
			break;
    }
	//printf("line = %d\n", line);
	int i = 0, findSection = 0,addLocaltion = 0;
	while(i < line)
	{
		if(findSection == 0 && (strncasecmp(szSectionName, lineArray[i], strlen(szSectionName)) == 0))//匹配扇区
		{
			findSection = 1;
			//printf("find sec %s seek [%s]\n", lineArray[i], pszKeyName);
			i ++;
		}
		if(findSection == 1 && (strncasecmp(pszKeyName, lineArray[i], strlen(pszKeyName)) == 0))//匹配到扇区,从这个扇区找key
		{
			
			char *p = lineArray[i];
			char bak[64] = {0};
			for(;*p != '\0';p ++)
			{
				if(*p == ';' || *p == '#' || *p == '\r' || *p == 0x20 || *p=='\n')
					break;
			}
			strncpy(bak, p, 63);
			//printf("find key [%s] [%s]\n", p, pszInput);
			//sprintf("%s=%s%s",pszKeyName,pszInput, bak);
			sprintf(lineArray[i],"%s=%s\r\n",pszKeyName,pszInput, bak);
			//puts(lineArray[i]);
			findSection = 2;
			break;
		}
		else if(findSection == 1 && (lineArray[i][0]=='[' || !isalpha(lineArray[i][0])))//在扇区尾部追加新的key
		{
			addLocaltion = i;
			findSection = 2;
			//printf("=====================[%d]\n", addLocaltion);
			break;
		}
	//	printf("-------------------%d\n",i);
		i ++;
	}
	
	if(findSection == 2)
	{
		fseek(fp, 0, SEEK_SET);
		for(i = 0; i < line; i ++)
		{

			if(addLocaltion && addLocaltion == i)
			{
				char tmp[128];
				sprintf(tmp,"%s=%s\r\n",pszKeyName,pszInput);
				fputs(tmp,fp);
				//puts(tmp);
			}
			fputs(lineArray[i],fp);
		}
		fflush(fp);
	}
	else
	{
		//printf("no find str\n");
	}
}

void SetConfigFileStringValue(const char *pszSectionName, const char *pszKeyName, const char *pszInput, const char *pszConfigFileName)
{
    FILE  *fp                    = NULL;
    char  szWholePath[256]      = {0};

    // 先对输入参数进行异常判断
    if (pszSectionName == NULL || pszKeyName == NULL || pszInput == NULL || pszConfigFileName == NULL)
    {
        printf("GetConfigFileStringValue: input parameter(s) is NULL!\n");
        return;
    }


    // 打开配置文件
   // GetCompletePath(pszConfigFileName, szWholePath);
    fp = fopen(pszConfigFileName, "r+");
    if (fp == NULL)
    {
        printf("GetConfigFileStringValue: open %s failed!\n", pszConfigFileName);
        return;
    }

    // 调用函数用于获取具体配置项的值
    SetStringContentValue(fp, pszSectionName, pszKeyName, pszInput);

    // 关闭文件
    fclose(fp);
    fp = NULL;
}

void SetConfigFileIntValue(const char *pszSectionName, const char *pszKeyName, uint32_t Value, const char *pszConfigFileName)
{
	char pszInput[4];
	sprintf(pszInput, "%d", Value);
	SetConfigFileStringValue(pszSectionName, pszKeyName,(const char *)pszInput, pszConfigFileName);

}




/************************************************************************/
volatile  sys_state_st gSysState = {

						.video.inState = MDVR_Video_NO_SIGAL,

								};
static pthread_mutex_t stateMutex;
void initSysState()
{
	pthread_mutex_init(&stateMutex,NULL);
	memset(&gSysState, 0, sizeof(sys_state_st));
}
void exitSysState()
{
	pthread_mutex_destroy(&stateMutex);  
}
void setSysState(int opt, int value)
{
//	if(value == NULL)
//		return ;
	void *ptr = (void *)value; 	
	sys_state_st *pSysState = &gSysState;
	//枷锁
	pthread_mutex_lock(&stateMutex);
	switch(opt)
	{
		
			
		case Sys_State_Run:
			pSysState->runState = (int)value;
			break;
		case Sys_State_Gps:
			pSysState->gpsState.state= (int)value;
			break;
		case Sys_State_storage:
			//memcpy(&pSysState->disk,ptr, sizeof(sys_state_st));
			pSysState->disk.state= value;
			break;
		case Sys_State_VideoInput:
			pSysState->video.inState= value;
			break;
		case sys_net_wireless_wifi:
			pSysState->wifiLink.linkState = value;
			break;
		case sys_net_ethnet:
			pSysState->ethLink.linkState = value;
			break;
		case sys_net_server_link:
			pSysState->serverState = value;
			break;
		case sys_dev_Camera:
			pSysState->cameraState= value;
			break;
		default:
			printf("setSysState:unkown req <%d>\n", opt);
			break;
	
	}
	pthread_mutex_unlock(&stateMutex);
}
int getSysState(int opt, void *value)
{
	void *ptr = value; 
	sys_state_st *pSysState = &gSysState;
	int ret ;
	pthread_mutex_lock(&stateMutex);
	switch(opt)
	{
		case Sys_State_All:
			memcpy(ptr, pSysState, sizeof(sys_state_st));
		case Sys_State_Run:
			ret = pSysState->runState;
			break;
		case Sys_State_Gps:
			ret = pSysState->gpsState.state;
			break;
		case Sys_State_storage:
			//memcpy(ptr, &pSysState->disk, sizeof(sys_state_st));
			ret = pSysState->disk.state;
			break;
		case Sys_State_VideoInput:
			ret = pSysState->video.inState;
			break;
		case sys_net_wireless_wifi:
			ret = pSysState->wifiLink.linkState;
			break;
		case sys_net_ethnet:
			ret = pSysState->ethLink.linkState;
			break;
		case sys_net_server_link:
			ret = pSysState->serverState;
			break;
		case sys_dev_Camera:
			ret = pSysState->cameraState;
			break;
		default:
			LOG_ERR("getSysState:unkown req <%d>\n", opt);
			break;

	}
	pthread_mutex_unlock(&stateMutex);
	if(ptr == NULL)
		return ret;
	return 0;

}

























































