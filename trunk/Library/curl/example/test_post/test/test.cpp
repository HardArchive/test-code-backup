// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
// test.cpp : �������̨Ӧ�ó������ڵ㡣
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


////ʹ��libcurl�Ĵ������
////curl_easy_init()
////curl_easy_setopt()
////curl_easy_perform()
////curl_easy_cleanup()
////�Ǻ�~���򵥵İɣ��������˼����Ͳ���ϸ˵

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
	curl_easy_setopt(curl, CURLOPT_URL, POSTURL);                          // CURLOPT_URL: URL��ַ
	
	struct curl_slist *chunk = NULL;
	chunk = curl_slist_append(chunk, "Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/QVOD, application/QVOD, application/xaml+xml, application/x-ms-xbap, application/x-ms-application, application/vnd.ms-xpsdocument, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*");  
	chunk = curl_slist_append(chunk, "User-Agent: Mo");
	chunk = curl_slist_append(chunk, "Connection: Keep-Alive");  

	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTFIELDS);                // CURLOPT_POSTFIELDS: POST����
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);             // CURLOPT_WRITEFUNCTION: �Է��ص����ݽ��в����ĺ�����ַ
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);                       // CURLOPT_WRITEDATA: ����WRITEFUNCTION�ĵ��ĸ�����ֵ
	curl_easy_setopt(curl, CURLOPT_POST, 1);                               // CURLOPT_POST: ����Ϊ��0��ʾ���β���ΪPOST
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);                            // CURLOPT_VERBOSE: ����Ϊ��0��ִ��ʱ��ӡ������Ϣ
	curl_easy_setopt(curl, CURLOPT_HEADER, 1);                             // CURLOPT_HEADER: ����Ϊ��0����Ӧͷ��Ϣͬ��Ӧ��һ�𴫸�WRITEFUNCTION
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);                     // CURLOPT_FOLLOWLOCATION: ����Ϊ��0����Ӧͷ��ϢLocation
	// CURLOPT_COOKIEFILE: ���������ʵ����̫��Ҫ�ˣ���֮ǰ�����˺ö��û����֤�ô�POST�Ƿ�ɹ���ԭ�����û����������ޡ�
	//		               ���ö�Ӧ��COOKIEFILE·������·���ļ�����һ����Ҫ�����������ʵ�ʴ���
	//		               ��ʹcurl��һ�η�����ʱ��ָ�����ļ��ж�ȡcookie
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/zhu/CProjects/curlposttest.cookie");


	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	//��ʾ�ѵ�¼ҳ��
	ShellExecute(NULL, NULL, FILENAME, NULL, NULL, SW_SHOWNORMAL);
	return 1;
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) 
{
	FILE *fptr = (FILE*)userp;
	return fwrite(buffer, size, nmemb, fptr);
}