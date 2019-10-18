

#include <httpThread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h> 
#include <curl/curl.h>
#include <sys/stat.h> 
#include <dirent.h>
#include <sys/inotify.h>
#include <includes.h>
#include <cJSON.h>
#include <sysCfg.h>



#define POST_SERVER_URL "http://115.231.64.178:10088/api/predict" 
#define GET_SERVER_URL "http://115.231.64.178:10088/api/query" 


/***********************************************
*** 	 get_file_size
***		 功能：获取文件的大小
***		 描述：通过该函数获取需要上传文件的大小
***		 参数：
***		 返回：-1  --  获取失败
***			   else  --  返回文件的大小
***********************************************/

unsigned long get_file_size(const char *fileName)  

{  
	unsigned long filesize = -1;      
    struct stat statbuff;  

    if(stat(fileName, &statbuff) < 0)
	{  
        return filesize;  
    }
	else
	{  
        filesize = statbuff.st_size;  
    }  
	
    return filesize;  
}  

 

char imgId[64];
static size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)  
{  
	size_t len = size*nmemb;
    
	cJSON *root = cJSON_Parse(ptr); 
	if(!root)
	{
		printf("%s\r\n", (char *)ptr);
		return len;
	}
	printf("%s\r\n", cJSON_Print(root));
	int stateCode = cJSON_GetObjectItem(root,"code")->valueint;  
	if(stateCode == 0) 
	{
		cJSON *format = cJSON_GetObjectItem(root,"data"); 
		if(format)
		{
			cJSON *pJsonNode;
			pJsonNode = cJSON_GetObjectItem(format,"predict_percent");
			if(pJsonNode)
				gSYS_cfg_para.report.percent = (int)(pJsonNode->valuedouble*100)%100;
			pJsonNode = cJSON_GetObjectItem(format,"predict_result");
			if(pJsonNode)
				gSYS_cfg_para.report.result = pJsonNode->valueint;
			pJsonNode = cJSON_GetObjectItem(format,"image_id");
			if(pJsonNode)
				snprintf(imgId,sizeof(imgId)-1,"%s",pJsonNode->valuestring);
			
			LOG_INFO("predict_result =%d %d %s\r\n", gSYS_cfg_para.report.result,gSYS_cfg_para.report.percent,imgId);
		}
	}
	else
	{
		LOG_ERR("%s\r\n", cJSON_GetObjectItem(root,"msg")->valuestring); 
	}
	
	cJSON_Delete(root);
	return len;
}  

/***********************************************
*** 	 sendPostFile
***		 功能：发送http请求
***		 描述：通过读取文件的方式发送消息请求
***		 参数：fileName  --  待读取文件的名字
***			   hostname  --  需要请求的域名
***            header  --  请求头
***		 返回：-1  --  请求失败
***			   0  --  请求成功
***********************************************/
//https://blog.csdn.net/baidu_19348579/article/details/56479977
int sendPostFile(const char *fileName,const char *hostname)
{

	char recv[100]={0};
	CURL *curl;
	CURLM *multi_handle;
  	CURLcode res;
	char filesize[15]={0};
	struct curl_httppost *formpost=NULL;
  	struct curl_httppost *lastptr=NULL;
  	struct curl_slist *headerlist=NULL;
	int still_running = 0; /* keep number of running handles */

  	static const char buf[] = "Expect:";

  	curl_global_init(CURL_GLOBAL_ALL);

	/* Fill in the file upload field. This makes libcurl load data from
	   the given file name when curl_easy_perform() is called. */
	curl_formadd(&formpost,
				 &lastptr,
				 CURLFORM_COPYNAME, "file",
				 CURLFORM_FILE, fileName,
				 CURLFORM_END);

	curl = curl_easy_init();
	multi_handle = curl_multi_init();
	headerlist = curl_slist_append(headerlist, buf);
	if (curl)
	{
		/* what URL that receives this POST */ 
    	curl_easy_setopt(curl, CURLOPT_URL, hostname);
    	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //启用时会汇报所有的信息
    	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);// 设置回调函数
		curl_multi_add_handle(multi_handle, curl);
		
		/* we start some action by calling perform right away */  
		curl_multi_perform(multi_handle, &still_running);
		do {
		      struct timeval timeout;
		      int rc; /* select() return code */
		      CURLMcode mc; /* curl_multi_fdset() return code */

		      fd_set fdread;
		      fd_set fdwrite;
		      fd_set fdexcep;
		      int maxfd = -1;

		      long curl_timeo = -1;

		      FD_ZERO(&fdread);
		      FD_ZERO(&fdwrite);
		      FD_ZERO(&fdexcep);

		      /* set a suitable timeout to play around with */
		      timeout.tv_sec = 1;
		      timeout.tv_usec = 0;

		      curl_multi_timeout(multi_handle, &curl_timeo);
		      if(curl_timeo >= 0) {
		        timeout.tv_sec = curl_timeo / 1000;
		        if(timeout.tv_sec > 1)
		          timeout.tv_sec = 1;
		        else
		          timeout.tv_usec = (curl_timeo % 1000) * 1000;
		      }

		      /* get file descriptors from the transfers */
		      mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

		      if(mc != CURLM_OK)
		      {
		        fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
		        break;
		      }

		      /* On success the value of maxfd is guaranteed to be >= -1. We call
		         select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
		         no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
		         to sleep 100ms, which is the minimum suggested value in the
		         curl_multi_fdset() doc. */

		      if(maxfd == -1) {
		        /* Portable sleep for platforms other than Windows. */
		        struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
		        rc = select(0, NULL, NULL, NULL, &wait);
		      }
		      else {
		        /* Note that on some platforms 'timeout' may be modified by select().
		           If you need access to the original value save a copy beforehand. */
		        rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
		      }

		      switch(rc) {
			      case -1:
			        /* select error */
			        break;
			      case 0:
			      default:
			        /* timeout or readable/writable sockets */
			       // printf("perform!\n");
			        curl_multi_perform(multi_handle, &still_running);
			       // printf("running: %d!\n", still_running);
			        break;
		      }
		} while(still_running);
		
		curl_multi_cleanup(multi_handle);
		/* always cleanup */ 
		curl_easy_cleanup(curl); 
		/* then cleanup the formpost chain */ 
		curl_formfree(formpost);
		/* free slist */ 
		curl_slist_free_all (headerlist);
	}

	return 0;

}

/***********************************************

*** 	 sendPostDat
***		 功能：发送http请求
***		 描述：通过直接传字符串的方式发送消息请求
***		 参数：fileName  --  待读取文件的名字
***			   hostname  --  需要请求的域名
***		 返回：-1  --  请求失败
***			   0  --  请求成功

***********************************************/

int sendPostData(char *data,char *dataName,char *hostname)
{

	char recv[100]={0};
	CURL *curl;
  	CURLcode res;
	long namelength = strlen(dataName);
	struct curl_httppost *formpost=NULL;
  	struct curl_httppost *lastptr=NULL;
  	struct curl_slist *headerlist=NULL;
  	static const char buf[] = "Expect:";

  	curl_global_init(CURL_GLOBAL_ALL);
	curl_formadd(&formpost, 
				&lastptr, 
				CURLFORM_PTRNAME, dataName,
				CURLFORM_PTRCONTENTS, data, 
				CURLFORM_NAMELENGTH, namelength, 
				CURLFORM_END);

	curl = curl_easy_init();
	headerlist = curl_slist_append(headerlist, buf);
	if (curl)
	{

		// 设置超时时间为1秒  
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2); 
		/* what URL that receives this POST */ 

    	curl_easy_setopt(curl, CURLOPT_URL, hostname);
    	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)recv);	

		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
		{
			printf("curl url_easy_perform() failed:%s \n",curl_easy_strerror(res));
			return -1;
		}
		else
		{
			printf("recv:%s\n",recv);
		}
		/* always cleanup */ 
		curl_easy_cleanup(curl); 
		/* then cleanup the formpost chain */ 
		curl_formfree(formpost);
		/* free slist */ 
		curl_slist_free_all (headerlist);
	}
	return 0;
}

int sendGetRequest(const char *httpUrl)
{

	CURL *curl;
  	CURLcode res;

  	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl)
	{

	    curl_easy_setopt(curl, CURLOPT_URL, httpUrl); // url  
	    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time  
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3); 
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
 	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);    
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //启用时会汇报所有的信息
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);  
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)recv);    

		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
		{
			printf("curl url_easy_perform() failed:%s \n",curl_easy_strerror(res));
			return -1;
		}
		else
		{
			
		}
		/* always cleanup */ 
		curl_easy_cleanup(curl); 
	}
	return 0;
}


void uploadPicture(const char *fileName)
{
	//getResult();

	LOG_DBG("%s  upload ...........\r\n", fileName);
	int ret = sendPostFile(fileName,POST_SERVER_URL);
	if (ret == -1)
	{
		LOG_ERR("uploadPicture failed\n");
		return -1;
	
	}
	else
	{
		LOG_INFO("uploadPicture success\n");

	}
	return 0;
}
int  getResult()//查询结果
{
	sendGetRequest(GET_SERVER_URL);
}


#if 0

 

int main(int argc,char *argv[])

{

	char *fileName = "log.txt";

	char *hostname = "127.0.0.1:8088";

	char *header =  "Content-Type:application/json;charset=UTF-8";

	int ret = sendPostFile(fileName,hostname,header);

	if (ret == -1)

	{

		printf("sendPostFile failed\n");

		return -1;

	}

	else

		printf("sendPostFile success\n");

	return 0;

}

 

#endif









