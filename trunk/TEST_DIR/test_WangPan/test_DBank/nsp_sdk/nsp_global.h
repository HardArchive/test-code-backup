#ifndef __NSP_GLOBAL_H__
#define __NSP_GLOBAL_H__

#if defined(WIN32)
    #include <iconv.h>
#else
    #include <unistd.h>
    #include <errno.h>
    #include <stdarg.h>
    #include <iconv.h>
    #include <ctype.h>
#endif

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <math.h>

//some constant value
#define CURLTIMEOUT        10        //curl链接超时限制    
#define BIGFILESIZE        104857600
#define SEGMENTSIZE        67108864
#define READFILEBUF        10240     //calc md5 , for each time read 10240 bytes of a big file
#define MAX_PATH           260    
#define SEND_BUFSIZ        1024
#define URL_LENGTH         1024
#define BIGBUFSIZE         100000
#define SMALLBUFSIZE       10000

//errno

#define RET_OK_MSG                            "Ok"
#define RET_MEMORY_MSG                        "Cannot malloc memory"
#define RET_INVALID_DATA_MSG                  "Invalid data(not json or php-rpc) from server"
#define RET_CONNECT_MSG                       "Cannot connect to server"
#define RET_FILE_MSG                          "Cannot open or read file when uploading"
#define RET_CURL_MSG                          "Libcurl encounted an error"
#define RET_SERVER_MSG                        "Server failed to return result, error number : "
#define RET_LONG_MSG                          "String too long"
#define RET_MD5_MSG                           "Error occured when calclulating md5 value of file on uploading"
#define RET_UPLOAD_MSG                        "Upload file error"

#define RESTURL "http://api.dbank.com/rest.php"            //

#if defined(WIN32)
#ifdef NSP_SDK_EXPORTS
#define NSP_EXPORT_DLL __declspec(dllexport)
#else
#define NSP_EXPORT_DLL __declspec(dllimport)
#endif

#else
#define NSP_EXPORT_DLL 
#endif


class Result;

struct nspBasicInfo{
    //
    bool    nsp_isAppID;
    //system level
    int     nsp_appid;
    char*    nsp_appkey;
    //customer level
    char*    nsp_appsid;
    char*    nsp_appsecret;

};
extern nspBasicInfo nBI;

nsp_node* Invoke(char *appsvc,void *params);
Result* Upload(char *path,int count,...);
Result* UploadProgress(char *dirpath,char *filename,void *callfunc);
bool Download(char *objfile,char *savename,void *callback);
nsp_node* NSPService(char *svcname,nsp_node &send);

char *strptime(const char *buf, const char *fmt, struct tm *tm);

int convert(char *desc,char *src,const char *input,size_t ilen,char *output,size_t olen);
nsp_node* nspUnserialize(char buf[]);
char* nspSerialize(nsp_node *nn);

void nsplog(char *logtitle,char *logmsg);

#endif
