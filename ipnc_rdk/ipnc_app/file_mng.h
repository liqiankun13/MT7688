
#ifndef __FILE_MNG_H
#define __FILE_MNG_H



#define SYS_DEBUG_LEVEL_0 1<0
#define SYS_DEBUG_LEVEL_1 1<1
#define SYS_DEBUG_LEVEL_2 1<2
#define SYS_DEBUG_LEVEL_3 1<3
#define SYS_DEBUG_ALL (SYS_DEBUG_LEVEL_0|SYS_DEBUG_LEVEL_1|SYS_DEBUG_LEVEL_2|SYS_DEBUG_LEVEL_3)










#define FILE_SYS_ENV_UPDATE_MASK 1<<0
#define FILE_SYS_CFG_UPDATE_MASK 1<<2
#define FILE_SYS_LOG_UPDATE_MASK 1<<2
#define FILE_SYS_ALL_UPDATE (FILE_SYS_ENV_UPDATE_MASK|FILE_SYS_CFG_UPDATE_MASK|FILE_SYS_LOG_UPDATE_MASK)

int tFileMngInit();
void tFileMngExit();

void initLogMng();
int tFileMngInit();
int printToLog(int logType, const char* format, ...);

#endif   /* __FILE_MNG_H__ */


