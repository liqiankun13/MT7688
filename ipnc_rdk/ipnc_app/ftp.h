
#ifndef _FTP_H
#define _FTP_H

#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>


int download(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries);


#endif
