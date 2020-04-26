
#ifndef _SYS_CFG_H
#define _SYS_CFG_H

#include <sys/types.h>
#include <common.h>


typedef struct 
{
	int interface;
	int inRes;
	int isInterval;
	int fps;
	int isAutoIdentify;
	int outRes;
	int chromat;
	
} video_cfg_st;

typedef struct 
{
	int inSrc;
	int gain;
	int quality;
} audio_cfg_st;

typedef struct 
{
	int fFomat;
} photo_cfg_st;

typedef struct 
{
	int quality;
	int isMute;
	int fFomat;
	int storage;
	int timeLimit;
	int sizeLimit;

} record_cfg_st;

typedef struct 
{
	char serverIP[16];
	char username[16];
	char passwd[16];
} nfs_cfg_st;

typedef struct 
{
	char devName[16];
	char ipAddr[16];
	char netMask[16];
	char gateWayIp[16];
	char macAddr[24];
	int  isRTSP;
	int  isDHCP;
	nfs_cfg_st NFServer;
	
} network_cfg_st;

typedef struct 
{
	int language;
	int is1thBoot;
	int isUpdate;
	int isBeep;
	int isLog;
} sys_cfg_st;

typedef struct 
{
	int isGpsON;
	int nBaud;
	int isGpsChkTime;
} Peripheral_dev_st;

typedef struct 
{
	char date[20];
	int  percent;
	int result;
	int  isValid;
	int errCode;
} Check_report_st;

 
typedef struct 
{
	/*video_cfg_st videoPara;
	audio_cfg_st audioPara;
	record_cfg_st recordPara;
	photo_cfg_st  pthotoPara;
	network_cfg_st netPara;
	Peripheral_dev_st perpDev;*/
	sys_cfg_st sysPara;
	Check_report_st report;
} SYS_CFG_st;
extern volatile SYS_CFG_st gSYS_cfg_para;










//void SetConfigFileStringValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT8 *pszInput, UINT8 *pszConfigFileName);
//void SetConfigFileIntValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT32 Value, UINT8 *pszConfigFileName);
//INT32 GetConfigFileIntValue(UINT8 *pszSectionName, UINT8 *pszKeyName, UINT32 iDefaultVal, UINT8 *pszConfigFileName);


/******************system state******************************************************/

enum 
{
	MDVR_Sys_ERR = 0,
	MDVR_Sys_OK,
	MDVR_Sys_STANDY,
	MDVR_Sys_OFF,
	MDVR_Sys_EXE_TASK,
	MDVR_Sys_IDLE,
	MDVR_Sys_MENU,
	MDVR_Sys_REPLAY,
	MDVR_Sys_WARNING,
	MDVR_Sys_AIRKISS,
	
	MDVR_Disk_FULL = 20,
	MDVR_Disk_FREE,
	MDVR_Disk_NO,
	MDVR_Disk_ERR,
	MDVR_Disk_UNSUPPORT,
	
	MDVR_Gps_OFF = 30,
	MDVR_Gps_OK,
	MDVR_Gps_NO_SIGNAL,
	MDVR_Gps_DATA_INVILD,

	MDVR_Video_NO_SIGAL = 40,
	MDVR_Video_SIGAL_OK,


	/*无线*/
	WIRELESS_RSSI_DISCONMECT = 100,
	WIRELESS_RSSI_LEVEL_1,
	WIRELESS_RSSI_LEVEL_2,
	WIRELESS_RSSI_LEVEL_3,

	/*外设*/
	SYS_DEV_Camera_Disconnect,
	SYS_DEV_Camera_Err,
	SYS_DEV_Camera_Normal,
	
};

typedef struct _sys_storge
{
	int freeSpace;
	int totalSpace;
	int state;

}sys_storge_st;

typedef struct _sys_gps
{
	int state;

}sys_gps_st;
typedef struct _sys_video
{
	int inState;

}sys_video_st;

typedef struct _sys_net
{
	int linkState;
	int linkSpeed;
	int RSSI;
	
}sys_net_st;


typedef struct _sys_state
{
	int count;
	int runState;
	sys_storge_st disk;
	sys_gps_st gpsState; 
	sys_video_st video;
	sys_net_st wifiLink;
	sys_net_st ethLink;
	int  serverState;
	int  cameraState;
}sys_state_st;


enum {
	Sys_State_All,
	Sys_State_storage,
	Sys_State_Gps,
	Sys_State_Run,
	Sys_State_VideoInput,

	/*net */
	sys_net_ethnet = 100,
	sys_net_wireless_wifi,
	sys_net_wireless_4G,
	sys_net_server_link,

	
	/*外设*/
	sys_dev_Camera,
};

#define SYS_CFG_PATHNAME "/root/record.ini"


void setSysState(int opt, int value);
int getSysState(int opt, void *value);


void SetConfigFileIntValue(const char *pszSectionName, const char *pszKeyName, uint32_t Value, const char *pszConfigFileName);
void SetConfigFileStringValue(const char *pszSectionName, const char *pszKeyName, const char *pszInput, const char *pszConfigFileName);
void GetConfigFileStringValue(const char *pszSectionName, const char *pszKeyName, const char *pDefaultVal,  char *pszOutput, uint32_t iOutputLen, const char *pszConfigFileName);
uint32_t GetConfigFileIntValue(const char *pszSectionName, const char *pszKeyName, uint32_t iDefaultVal, const char *pszConfigFileName);

int SYS_initALLCfgPara();


#endif

