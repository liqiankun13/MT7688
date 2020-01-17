

#include <ftp.h>
#include <includes.h>
#include <display.h>


static Bool isError = True;
/* parse headers for Content-Length */
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{

    int r, errCode;
    long len = 0;
    /* _snscanf() is Win32 specific */

    //r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
	r = sscanf((const char*)ptr, "HTTP/1.1 %d", &errCode);
	if(r)
	{
		LOG_INFO("errCode = %d\r\n", errCode);
	}
    r = sscanf((const char*)ptr, "Content-Length: %ld\n", &len);
    if (r) /* Microsoft: we don't read the specs */
        *((long *) stream) = len;
	if(errCode == 200)
		isError = False;
    return size * nmemb;
}
/* discard downloaded data */

size_t discardfunc(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    return size * nmemb;
}

//write data to upload
size_t writefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    return fwrite(ptr, size, nmemb, (FILE*)stream);
}
int my_progress_func(char *progress_data,  
                     double dltotal, /* dltotal */  
                     double dlnow, /* dlnow */  
                     double ultotal,  
                     double ulnow)  
{  
  printf("%s %g / %g (%g %%)\n", progress_data, dltotal, dlnow, dlnow*100.0/dltotal); 
  char text[64];
  sprintf(text, "upgrage 02d%", (int)(dlnow*100/dltotal));
  GUIDrawText(20,100,text, LCD_FONT_BIG, LCD_FILL_WHITE, LCD_FILL_BLACK);
  return 0;  
}  

// 下载
int download(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries)
{

    FILE *f;
    curl_off_t local_file_len = -1 ;
    long filesize =0 ;
    CURLcode r = CURLE_GOT_NOTHING;
    struct stat file_info;
    int use_resume = 0;
	char *progress_data = "* ";  
	 
    //获取本地文件大小信息

    if(stat(localpath, &file_info) == 0)
    {
        local_file_len = file_info.st_size; 
        use_resume = 1;
    }
    //追加方式打开文件，实现断点续传
    f = fopen(localpath, "ab+");
    if (f == NULL) {
        LOG_ERR("fopen %s\r\n", strerror(errno));
        return 0;
    }
    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
    curl_easy_setopt(curlhandle, CURLOPT_USERPWD, "spider:spider");   
    //连接超时设置
    curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);
    //设置头处理函数

    curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
    curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
    // 设置断点续传
    curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
    curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 0L); 
	curl_easy_setopt(curlhandle, CURLOPT_PROGRESSFUNCTION, my_progress_func);
	curl_easy_setopt(curlhandle, CURLOPT_PROGRESSDATA, progress_data);  
    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);
    r = curl_easy_perform(curlhandle);
    fclose(f);
    if (r == CURLE_OK)
        return 1;
    else {
        fprintf(stderr, "%s\n", curl_easy_strerror(r));
        return 0;
    }
}


int download_file(const char *url, const char * localpath)
{

    CURL *curldwn = NULL;
	isError = True;
    curl_global_init(CURL_GLOBAL_ALL);
    curldwn = curl_easy_init();
    download(curldwn, url, localpath, 1, 3);
    curl_easy_cleanup(curldwn);
    curl_global_cleanup();
	if(isError)
		unlink(localpath);
		
	
}
