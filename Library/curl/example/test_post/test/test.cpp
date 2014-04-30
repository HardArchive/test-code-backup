// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}


#include <stdio.h>
#include <string.h>
//#include <curl.h>
#include "curl.h"

#include <stdlib.h>
// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}



#include "curl.h"
#pragma comment(lib, "libcurl.lib")
#include <string>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl.h>


////使用libcurl的大概流程
////curl_easy_init()
////curl_easy_setopt()
////curl_easy_perform()
////curl_easy_cleanup()
////呵呵~超简单的吧，具体的意思这里就不详细说

#define POSTURL "http://bbs.pediy.com/login.php?do=login"
#define POSTFIELDS "vb_login_username=irc&cookieuser=1&vb_login_password=&s=&securitytoken=guest&do=login&vb_login_md5password=cec9b1c9fe98858a7fd9830fee256e3e&vb_login_md5password_utf=cec9b1c9fe98858a7fd9830fee256e3e"
#define FILENAME "pediy_login.html"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

int main12(int argc, char *argv[]) {
	CURL *curl;
	CURLcode res;
	FILE *fptr;
	struct curl_slist *http_header = NULL;

	if ((fptr = fopen(FILENAME, "w")) == NULL) {
		fprintf(stderr, "fopen file error: %s\n", FILENAME);
		exit(1);
	}

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL);                          // CURLOPT_URL: URL地址
	
	struct curl_slist *chunk = NULL;
	chunk = curl_slist_append(chunk, "Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/QVOD, application/QVOD, application/xaml+xml, application/x-ms-xbap, application/x-ms-application, application/vnd.ms-xpsdocument, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*");  
	chunk = curl_slist_append(chunk, "User-Agent: Mo");
	chunk = curl_slist_append(chunk, "Connection: Keep-Alive");  

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTFIELDS);                // CURLOPT_POSTFIELDS: POST参数
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);             // CURLOPT_WRITEFUNCTION: 对返回的数据进行操作的函数地址
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);                       // CURLOPT_WRITEDATA: 设置WRITEFUNCTION的第四个参数值
	curl_easy_setopt(curl, CURLOPT_POST, 1);                               // CURLOPT_POST: 设置为非0表示本次操作为POST
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);                            // CURLOPT_VERBOSE: 设置为非0在执行时打印请求信息
	curl_easy_setopt(curl, CURLOPT_HEADER, 1);                             // CURLOPT_HEADER: 设置为非0将响应头信息同响应体一起传给WRITEFUNCTION
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);                     // CURLOPT_FOLLOWLOCATION: 设置为非0，响应头信息Location
	// CURLOPT_COOKIEFILE: 哈哈，这个实在是太重要了，我之前尝试了好多次没法验证该次POST是否成功的原因就是没有设置这个罗。
	//		               设置对应的COOKIEFILE路径，该路径文件并不一定需要在物理磁盘上实际存在
	//		               会使curl下一次发请求时从指定的文件中读取cookie
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/zhu/CProjects/curlposttest.cookie");


	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	//显示已登录页面
	ShellExecute(NULL, NULL, FILENAME, NULL, NULL, SW_SHOWNORMAL);
	return 1;
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) 
{
	FILE *fptr = (FILE*)userp;
	return fwrite(buffer, size, nmemb, fptr);
}