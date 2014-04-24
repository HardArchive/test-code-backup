// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}



#include "curl.h"
#pragma comment(lib, "libcurld_imp.lib")
#include <string>
using namespace std;

long writer(void *data, int size, int nmemb, string &content);
bool  CurlInit(CURL *&curl, const char* url,string &content);
bool  GetURLDataBycurl(const char* URL, string &content);

void main()
{
	char *url ="http://www.baidu.com/img/baidu.gif";
	string content;
	if ( GetURLDataBycurl(url,content))
	{
		printf("%s\n",content);

	}
	getchar();
}

/*curl_easy_setopt
 *1. �����������ݵĻص�����.  
 *	option:      
 *		CURLOPT_WRITEFUNCTION //���ûص�����
 *			�ص�����ԭ��Ϊ: size_t function( void *ptr, size_t size, size_t nmemb, void *userp);           ��������libcurl���յ����ݺ󱻵��á�
 *				void *ptr�����ػ���������.
 *				void *userp���û�ָ��, �û�ͨ�����ָ�봫���Լ�������.
 *		CURLOPT_WRITEDATA 
 *			���ûص������е�void *userpָ�����Դ��
 *2. ���ؽ��ȿ���.
 *	option:
 *		CURLOPT_NOPROGRESS  
 *			Ϊ��ʹCURLOPT_PROGRESSFUNCTION������. CURLOPT_NOPROGRESS���뱻����Ϊfalse.
 *		CURLOPT_PROGRESSFUNCTION
 *		CURLOPT_PROGRESSFUNCTION ָ���ĺ������������ÿ�뱻libcurl����һ��.
 *		CURLOPT_PROGRESSDATA
 *		CURLOPT_PROGRESSDATAָ���Ĳ�������ΪCURLOPT_PROGRESSFUNCTIONָ�������Ĳ���. 
 *		�����������������ݻص��Ĵ�����ͬ. 
 *3. ������������. 
 *	option:
 *		CURLOPT_URL 					 ���÷��ʵ�URI.
 *		CURLOPT_NOSIGNAL				 ���������ź�.
 *		CURLOPT_HEADER					 ȡ����ʱ��ͬHTTPͷ��һ��ȡ��.
 *		CURLOPT_HEADERFUNCTION
 *		CURLOPT_HEADERDATA				 ֻȡHTTPͷ������, �������������ݻص��Ĵ�����ͬ. 
 *		CURLOPT_TIMEOUT					 ��ʱʱ��.
 *		CURLOPT_CONNECTIONTIMEOUT		 ���ӵȴ�ʱ��.
 *		CURLOPT_FOLLOWLOCATION	  ����   ����֧��302�ض���
 *		CURLOPT_RANGE   			  �� �ϵ�����, ָ�������Ƭ, ��ʽ:"0-200"
 */
bool CurlInit(CURL *&curl, const char* url,string &content)
{
	CURLcode code;
	string error;
	//��ʼ������handle
	curl = curl_easy_init();
	if (curl == NULL)
	{
		printf( "Failed to create CURL connection\n");
		return false;
	}
	//������������. �����ò���
	code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);
	if (code != CURLE_OK)
	{
		printf( "Failed to set error buffer [%d]\n", code );
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	code = curl_easy_setopt(curl, CURLOPT_URL, url);
	if (code != CURLE_OK)
	{
		printf("Failed to set URL [%s]\n", error);
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	if (code != CURLE_OK)
	{
		printf( "Failed to set redirect option [%s]\n", error );
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK)
	{
		printf( "Failed to set writer [%s]\n", error);
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
	if (code != CURLE_OK)
	{
		printf( "Failed to set write data [%s]\n", error );
		return false;
	}
	return true;
}

long writer(void *data, int size, int nmemb, string &content)
{
	long sizes = size * nmemb;
	string temp((char*)data, sizes);
	content += temp; 
	return sizes;
}

bool GetURLDataBycurl(const char* URL,  string &content)
{
	CURL *curl = NULL;
	CURLcode code;
	string error;

	//curl_global_init libcurl��ȫ�ֳ�ʼ��
	// Ӧ���ڳ���ʼʱ���ó�ʼ������. ��Ȼ�����������ʼ������, libcurl����curl_easy_init()�������Զ�����. ���ڶ��̴߳���ʱ, ���ܻ���ֶ���Զ����õ����.
	//flags: CURL_GLOBAL_ALL     //��ʼ�����еĿ��ܵĵ��á�
	//       CURL_GLOBAL_SSL     //��ʼ��֧�� ��ȫ�׽��ֲ㡣
	//       CURL_GLOBAL_WIN32   //��ʼ��win32�׽��ֿ⡣
	//       CURL_GLOBAL_NOTHING //û�ж���ĳ�ʼ����
	code = curl_global_init(CURL_GLOBAL_DEFAULT);  
	if (code != CURLE_OK)
	{
		printf( "Failed to global init default [%d]\n", code );
		return false;
	} 

	if ( !CurlInit(curl,URL,content) )
	{
		printf( "Failed to global init default [%d]\n" );
		//return PM_FALSE;
		return FALSE;
	}
	//��ʼ����
	code = curl_easy_perform(curl);
	if (code != CURLE_OK)
	{
		printf( "Failed to get '%s' [%s]\n", URL, error);
		return false;
	}
	long retcode = 0;
	code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE , &retcode); 
	if ( (code == CURLE_OK) && retcode == 200 )
	{
		double length = 0;
		code = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &length); 
		printf("%d",retcode);
		FILE * file = fopen("1.gif","wb");
		fseek(file,0,SEEK_SET);
		fwrite(content.c_str(),1,length,file);
		fclose(file);

		//struct curl_slist *list;
		//code = curl_easy_getinfo(curl,CURLINFO_COOKIELIST,&list);
		//curl_slist_free_all (list);

		return true;
	}
	else
	{
		//    debug1( "%s \n ",getStatusCode(retcode));
		return false;
	}
	//����handle���ͷ�	
	curl_easy_cleanup(curl);
	// libcurl��ȫ���ͷ�
	curl_global_cleanup();
	return false;
}