

#ifndef __HTTP_THREAD_H
#define __HTTP_THREAD_H


#include <curl/curl.h>













int sendPostFile(const char *fileName,const char *hostname);
void uploadPicture(const char *fileName);



#endif


