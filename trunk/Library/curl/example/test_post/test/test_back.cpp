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
#pragma comment(lib, "libcurld_imp.lib")
#include <string>
using namespace std;

long writer(void *data, int size, int nmemb, string &content);
bool  CurlInit(CURL *&curl, const char* url,string &content);
bool  GetURLDataBycurl(const char* URL, string &content);


int post_page_content(char* url)
{

	struct curl_slist *chunk = NULL;
	chunk.memory = (char*)malloc(1);   
	chunk.size = 0; 
	CURL *curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 1);
	curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);
	curl_easy_setopt(curl_handle, CURLOPT_COOKIEJAR, "mycookie.txt"); 
	curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "mycookie.txt"); 


	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);


	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&m_chunk);


	curl_easy_setopt(curl_handle,          CURLOPT_POSTFIELDS,"action=Signin&eventSubmitDoPost=any&Done=http://china.abc.com/&urlType=&formSubmit=Y&LoginId="�û���"&Password="����"");
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");


	curl_easy_perform(curl_handle); 
	curl_easy_cleanup(curl_handle);
	return 1;
}
char *myurl = "https://loginchina.abc.com/member/signin.htm";
post_page_content(myurl);

int main()
{ 

	curl_global_init(CURL_GLOBAL_WIN32); 
	CURL *easy_handle = curl_easy_init();

	// ʹ��multi-parts form post
	curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:2210/Default.aspx"); 
	 curl_httppost *post = NULL; curl_httppost *last = NULL; 
	// �ı�����
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "name", CURLFORM_COPYCONTENTS, "JGood", CURLFORM_END); 
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "address", CURLFORM_COPYCONTENTS, "HangZhou", CURLFORM_END);
	// �ı��ļ��е�����
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "file", CURLFORM_FILECONTENT, "ReadMe.txt", CURLFORM_END);
	curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, post);
	curl_easy_perform(easy_handle);
	curl_formfree(post);
	curl_easy_cleanup(easy_handle);
	curl_global_cleanup();

	return 0;
}  


static char data[]="dummy";

struct WriteThis {
	char *readptr;
	size_t sizeleft;
};

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct WriteThis *pooh = (struct WriteThis *)userp;

	if(size*nmemb < 1)
		return 0;

	if(pooh->sizeleft) {
		*(char *)ptr = pooh->readptr[0]; /* copy one single byte */
		pooh->readptr++;                 /* advance pointer */
		pooh->sizeleft--;                /* less data left */
		return 1;                        /* we return 1 byte at a time! */
	}

	return 0;                         /* no more data left to deliver */
}

int test(char *URL)
{
	CURL *curl;
	CURLcode result = CURLE_OK;
	int res = 0;
	struct WriteThis pooh = { data, sizeof(data)-1 };

	curl_global_init(CURL_GLOBAL_ALL);

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	/* Purposely omit to set CURLOPT_POSTFIELDSIZE */
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);

	result = curl_easy_perform(curl);

test_cleanup:

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return (int)result;
}


void main1()
{
	char *url ="http://bbs.pediy.com/login.php?do=login";

	test(url);
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
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);        //����ʱ��㱨���е���Ϣ�������STDERR��ָ����CURLOPT_STDERR��
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

	struct curl_slist *chunk = NULL;

	chunk = curl_slist_append(chunk, "Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/QVOD, application/QVOD, application/xaml+xml, application/x-ms-xbap, application/x-ms-application, application/vnd.ms-xpsdocument, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*");  
	chunk = curl_slist_append(chunk, "User-Agent: Mo");

	chunk = curl_slist_append(chunk, "Content-Length: 200");  
	chunk = curl_slist_append(chunk, "Connection: Keep-Alive");  


	chunk = curl_slist_append(chunk, "vb_login_username=%69%72%63&cookieuser=1&vb_login_password=&securitytoken=guest&do=login&vb_login_md5password=cec9b1c9fe98858a7fd9830fee256e3e&vb_login_md5password_utf=cec9b1c9fe98858a7fd9830fee256e3e"); 
	//�����ļ�
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);     
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