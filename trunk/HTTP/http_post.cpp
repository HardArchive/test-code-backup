/*
 *                                 http_post.cpp 
 *
 * by Uday Chitragar - 2004/Dec/01
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 * not claim that you wrote the original software. If you use this
 * software in a product, an acknowledgment in the product documentation
 * would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 * 
 * */

/* 
 * Notes:
 * This source demonstrates sending HTTP POST request to webserver from C++
 * This uses sockets hence can be compiled on Linux, UNIX, Win
 */

#define LINUX_OS
// #define WIN_OS
#define _DEBUG_PRINT(X)   /* X */

//For commn
#include <iostream>
#include <string>
#include <stdlib.h>
#include <assert.h>

#ifdef LINUX_OS
 #include <netdb.h>
#endif

#ifdef WIN_OS
 #include <Winsock2.h>
#endif


#define SEND_RQ(MSG) \
                /*cout<<send_str;*/ \
  send(sock,MSG,strlen(MSG),0);


using namespace std;
//<exe> hostname api parameters
int request (char* hostname, char* api, char* parameters, string& message)
{

	#ifdef WIN_OS
	{
		WSADATA	WsaData;
		WSAStartup (0x0101, &WsaData);
	}
	#endif

    sockaddr_in       sin;
    int sock = socket (AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
		return -100;
	}
    sin.sin_family = AF_INET;
    sin.sin_port = htons( (unsigned short)80);

    struct hostent * host_addr = gethostbyname(hostname);
    if(host_addr==NULL) {
      _DEBUG_PRINT( cout<<"Unable to locate host"<<endl );
      return -103;
    }
    sin.sin_addr.s_addr = *((int*)*host_addr->h_addr_list) ;
    _DEBUG_PRINT( cout<<"Port :"<<sin.sin_port<<", Address : "<< sin.sin_addr.s_addr<<endl);

    if( connect (sock,(const struct sockaddr *)&sin, sizeof(sockaddr_in) ) == -1 ) {
     _DEBUG_PRINT( cout<<"connect failed"<<endl ) ;
     return -101;
    }

 string send_str;

 SEND_RQ("POST ");
 SEND_RQ(api);
 SEND_RQ(" HTTP/1.0\r\n");
 SEND_RQ("Accept: */*\r\n");
 SEND_RQ("User-Agent: Mozilla/4.0\r\n");

 char content_header[100];
 sprintf(content_header,"Content-Length: %d\r\n",strlen(parameters));
 SEND_RQ(content_header);
 SEND_RQ("Accept-Language: en-us\r\n");
 SEND_RQ("Accept-Encoding: gzip, deflate\r\n");
 SEND_RQ("Host: ");
 SEND_RQ("hostname");
 SEND_RQ("\r\n");
 SEND_RQ("Content-Type: application/x-www-form-urlencoded\r\n");
 
 //If you need to send a basic authorization
 //string Auth        = "username:password";
 //Figureout a way to encode test into base64 !
 //string AuthInfo    = base64_encode(reinterpret_cast<const unsigned char*>(Auth.c_str()),Auth.length());
 //string sPassReq    = "Authorization: Basic " + AuthInfo;
 //SEND_RQ(sPassReq.c_str());

 SEND_RQ("\r\n");
 SEND_RQ("\r\n");
 SEND_RQ(parameters);
 SEND_RQ("\r\n");

 _DEBUG_PRINT(cout<<"####HEADER####"<<endl);
 char c1[1];
 int l,line_length;
 bool loop = true;
 bool bHeader = false;

 while(loop) {
   l = recv(sock, c1, 1, 0);
   if(l<0) loop = false;
   if(c1[0]=='\n') {
       if(line_length == 0) loop = false;

       line_length = 0;
       if(message.find("200") != string::npos)
	       bHeader = true;

   }
   else if(c1[0]!='\r') line_length++;
   _DEBUG_PRINT( cout<<c1[0]);
   message += c1[0];
 }

 message="";
 if(bHeader) {

     _DEBUG_PRINT( cout<<"####BODY####"<<endl) ;
     char p[1024];
     while((l = recv(sock,p,1023,0)) > 0)  {
         _DEBUG_PRINT( cout.write(p,l)) ;
	     p[l] = '\0';
	     message += p;
     }

     _DEBUG_PRINT( cout << message.c_str());
 } else {
	 return -102;
 }


 #ifdef WIN_OS
   WSACleanup( );
 #endif

 return 0;
}


int main(){
  string message;
  int request ("www.somesite.com", "/post_url.pl", "search=hello&date=todat", string& message);
  // message contains response!

}

