#include"NSPClient.h"
#include"zlib.h"

nspBasicInfo nBI;

/* HTTP gzip decompress */
/* 参数1.压缩数据 2.数据长度 3.解压数据 4.解压后长度 */
int httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] = 
    {
        0x8 + 0x7 * 0x10,
        (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;
    if(inflateInit2(&d_stream, 47) != Z_OK) return -1;
    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) 
    {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) break;
        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
                {
                    return -1;
                }
            }
            else return -1;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) return -1;
    *ndata = d_stream.total_out;
    return 0;
} 

//urlencode
char *php_url_encode(char const *s, int len, int *new_length)
{
    #define safe_emalloc(nmemb, size, offset)    malloc((nmemb) * (size) + (offset))
    static unsigned char hexchars[] = "0123456789ABCDEF";
    register unsigned char c;
    unsigned char *to, *start;
    unsigned char const *from, *end;
    
    from = (unsigned char *)s;
    end = (unsigned char *)s + len;
    start = to = (unsigned char *) safe_emalloc(3, len, 1);

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            *to++ = '+';
#ifndef CHARSET_EBCDIC
        } else if ((c < '0' && c != '-' && c != '.') ||
                   (c < 'A' && c > '9') ||
                   (c > 'Z' && c < 'a' && c != '_') ||
                   (c > 'z')) {
            to[0] = '%';
            to[1] = hexchars[c >> 4];
            to[2] = hexchars[c & 15];
            to += 3;
#else /*CHARSET_EBCDIC*/
        } else if (!isalnum(c) && strchr("_-.", c) == NULL) {
            /* Allow only alphanumeric chars and '_', '-', '.'; escape the rest */
            to[0] = '%';
            to[1] = hexchars[os_toascii[c] >> 4];
            to[2] = hexchars[os_toascii[c] & 15];
            to += 3;
#endif /*CHARSET_EBCDIC*/
        } else {
            *to++ = c;
        }
    }
    *to = 0;
    if (new_length) {
        *new_length = to - start;
    }
    return (char *) start;
}

#if defined(WIN32)
    int convert(char *desc,char *src,const char *input,size_t ilen,char *output,size_t olen)
    {
        const char **pin=&input;
        char **pout=&output;

        iconv_t cd=iconv_open(desc,src);
        if(cd==(iconv_t)-1){
                return -1;
        }
        memset(output,0,olen);
        if(iconv(cd,pin,&ilen,pout,&olen)){
        }
        iconv_close(cd);
        return 0;
    }
#endif

    size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
    {
		char dec[SMALLBUFSIZE];
		int declen;
		httpgzdecompress((Byte *)ptr,size*nmemb,(Byte *)dec,(uLong *)&declen);
		if(declen<0){
			strcpy(dec,(char *)ptr);
			declen=nmemb*size;
		}
		else{
			dec[declen] = '\0';
		}
        if (strlen((char *)stream) + declen > BIGBUFSIZE-1) return 0;

        char reci[SMALLBUFSIZE];
        memset(reci,0,sizeof(reci));
#if defined(WIN32)
        convert("GBK","UTF-8",(const char *)dec,strlen((char *)dec),reci,SMALLBUFSIZE);
#else
        sprintf(reci,"%s",ptr);
#endif
        strcat((char *)stream,reci);

        return size*nmemb;
    }

bool CurlAction(char *url,char *sendbuf,nsp_node *ret,int &time_adjust)
{
    CURL *curl;
    CURLcode res;

	nsplog("发送数据",sendbuf);
    char *postthis=sendbuf;

    curl = curl_easy_init();

    if(curl){
        
        char nsp_reci[BIGBUFSIZE];    //used for allocating information returned by server
        memset(nsp_reci,0,sizeof(nsp_reci));

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postthis));
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, nsp_reci);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT,CURLTIMEOUT);
        res = curl_easy_perform(curl);
        if(CURLE_OK == res) {
            long code;
            curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &code);
            
            if(code != 200){
                char errorMsg[50];
                sprintf(errorMsg,"%s %d",RET_SERVER_MSG,code);
                nsplog("内部错误",errorMsg);
                curl_easy_cleanup(curl);
                return false;
            }
            //unserialize information returned by server

            nsp_node *res = NULL;
            char *split = strtok(nsp_reci,"\r\n");
            time_t local_time=time(0);
            time_t server_time=local_time;

            if(split!=NULL){
                while(split = strtok(NULL,"\r\n")){
                    if(strcmp(split,"NSP_STATUS: 109")==0){
                        time_adjust=server_time-local_time;
                        nsplog("内部错误",RET_INVALID_DATA_MSG);curl_easy_cleanup(curl);return false;
                    }
                    else if(strncmp(split,"Date:",5)==0){
                        tm newtime;
                        memset(&newtime,0,sizeof(struct tm));
                        strptime(split+6,"%a, %d %b %Y %H:%M:%S",&newtime);
                        newtime.tm_year+=70;
                        server_time=mktime(&newtime)+8*60*60;
                    }
                    if(strncmp(split,"a:",2)==0){
                        res = nspUnserialize(split);
						nsplog("接收数据",split);
                    }
                }
            }

            if(res!=NULL){
                memcpy(ret,res,sizeof(nsp_node));
                free(res);res=NULL;
            }
            else{nsplog("内部错误",RET_INVALID_DATA_MSG);curl_easy_cleanup(curl);return false;}
        }
        curl_easy_cleanup(curl);
    }
    return true;
}


bool NSPSendByAppId(char *svcname,nsp_node &send,nsp_node *ret,int &time_adjust){

    char times[20];
    time_t ttimes = time(0);
    ttimes += time_adjust;
    sprintf(times,"%d",ttimes);

    //serialize parameters
    char *sebuf=nspSerialize(&send);
    if(sebuf==NULL){
        nsplog("内部错误","Fail to serialize data");
        return false;
    }

    int lenl;
    char *encodebuf=php_url_encode(sebuf,strlen(sebuf),&lenl);

    //calculate md5 for nsp_key
    char *md5str = (char *)malloc(strlen(sebuf)+1000);
    sprintf(md5str,"%snsp_app%dnsp_fmt%snsp_params%snsp_svc%snsp_ts%s",nBI.nsp_appkey,nBI.nsp_appid,"php-rpc",sebuf,svcname,times);
    char mdresult[33];
    md5(mdresult,md5str);
    //combine post information
    char *sendbuf = (char *)malloc(strlen(encodebuf)+1000);
    sprintf(sendbuf,"nsp_fmt=%s&nsp_svc=%s&nsp_app=%d&nsp_ts=%s&nsp_params=%s&nsp_key=%s","php-rpc",svcname,nBI.nsp_appid,times,encodebuf,mdresult);
    
    free(md5str);md5str=NULL;
    free(encodebuf);encodebuf=NULL;
    free(sebuf);sebuf=NULL;
    if(CurlAction(RESTURL,sendbuf,ret,time_adjust)==false){
        free(sendbuf);sendbuf=NULL;
        return false;
    }
    free(sendbuf);sendbuf=NULL;
    return true;
}

bool NSPSendBySessionId(char *svcname,nsp_node &send,nsp_node *ret,int &time_adjust){

    char times[20];
    time_t ttimes = time(0);
    ttimes += time_adjust;
    sprintf(times,"%d",ttimes);

    //serialize parameters
    char *sebuf=nspSerialize(&send);
    if(sebuf==NULL){
        return false;
    }

    int lenl;
    char *encodebuf=php_url_encode(sebuf,strlen(sebuf),&lenl);

    //calculate md5 for nsp_key
    char *md5str = (char *)malloc(strlen(sebuf)+1000);
    sprintf(md5str,"%snsp_fmt%snsp_params%snsp_sid%snsp_svc%snsp_ts%s",nBI.nsp_appsecret,"php-rpc",sebuf,nBI.nsp_appsid,svcname,times);
    char mdresult[33];
    md5(mdresult,md5str);
    strcpy(mdresult,mdresult);
    //combine post information
    char *sendbuf = (char *)malloc(strlen(encodebuf)+1000);
    sprintf(sendbuf,"nsp_fmt=%s&nsp_svc=%s&nsp_ts=%s&nsp_params=%s&nsp_sid=%s&nsp_key=%s","php-rpc",svcname,times,encodebuf,nBI.nsp_appsid,mdresult);


    free(md5str);md5str=NULL;
    free(encodebuf);encodebuf=NULL;
    free(sebuf);sebuf=NULL;
    if(CurlAction(RESTURL,sendbuf,ret,time_adjust)==false){
        free(sendbuf);sendbuf=NULL;
        return false;
    }
    free(sendbuf);sendbuf=NULL;
    return true;
}

nsp_node* NSPService(char *svcname,nsp_node &send){

    nsplog("调用平台服务",svcname);
    nsp_node *ret=(nsp_node *)malloc(sizeof(nsp_node));
    if(ret){memset(ret,0,sizeof(nsp_node));}
    else return NULL;

    static int time_adjust = 0;
    int t_adjust = time_adjust;
    while(true){
        bool res = false;
        if(nBI.nsp_isAppID == true){
            res = NSPSendByAppId(svcname,send,ret,t_adjust);
        }
        else
        {
            res = NSPSendBySessionId(svcname,send,ret,t_adjust);
        }
        if(res == true){
            break;
        }
        else if(res == false && t_adjust!=time_adjust){
            time_adjust = t_adjust;
        }
        else{
            free(ret);return NULL;
        }
    }
    return ret;
}

nsp_node* Invoke(char *appsvc,void *params){
    nspAction *nA=NSPAction();
    if(nA==NULL){return NULL;}

    nsp_node *send;
    if(params!=NULL){
        send=(nsp_node *)params;
    }
    else{
        send = nA->createArray();
    }

    NSPActionDelete(nA);
    if(send==NULL){return NULL;}
    return NSPService(appsvc,*send);
}

//check whether NSPClient(param1,param2) shall be a system level if true
bool IsInteger(char *str){
    char *p=str,*q=str+strlen(str);
    while(p!=q)
    {
        if(!isdigit(*p)){
            return false;
        }
        p++;
    }
    return true;
}
nspClient* NSPClient(char *str1,char *str2){
    nspClient *nC = (nspClient *)malloc(sizeof(nspClient));
    if(nC==NULL){return NULL;}
    nC->invoke =&Invoke;
    nC->upload =&Upload;
	nC->upload_progress =&UploadProgress;
	nC->download =&Download;
    nC->action=NSPAction();
    if(nC->action==NULL){
        free(nC);nC=NULL;return NULL;
    }
    if(IsInteger(str1)){            //system level(using appid and appkey)
        nBI.nsp_isAppID = true;
        nBI.nsp_appid = atoi(str1);
        nBI.nsp_appkey = (char *)malloc(strlen(str2)+1);
        if(nBI.nsp_appkey==NULL){
            NSPActionDelete(nC->action);free(nC);nC=NULL;return NULL;
        }
        strcpy(nBI.nsp_appkey,str2);
    }
    else{                            //user level(using session id and secret)
        nBI.nsp_isAppID = false;
        nBI.nsp_appsid = (char *)malloc(strlen(str1)+1);
        if(nBI.nsp_appsid==NULL){
            NSPActionDelete(nC->action);free(nC);nC=NULL;return NULL;
        }
        nBI.nsp_appsecret = (char *)malloc(strlen(str2)+1);
        if(nBI.nsp_appsecret==NULL){
            free(nBI.nsp_appsid);NSPActionDelete(nC->action);free(nC);nC=NULL;return NULL;
        }
        strcpy(nBI.nsp_appsid,str1);
        strcpy(nBI.nsp_appsecret,str2);
    }
    return nC;
}
//please delete client object when don't use it anymore
bool NSPClientDelete(nspClient *nC){
    if(nC!=NULL){
        if(nBI.nsp_isAppID){
            nBI.nsp_appid=0;
            free(nBI.nsp_appkey);
            nBI.nsp_appkey=NULL;
        }
        else
        {
            free(nBI.nsp_appsecret);
            nBI.nsp_appsecret=NULL;
            free(nBI.nsp_appsid);
            nBI.nsp_appsid=NULL;
        }
        NSPActionDelete(nC->action);
        free(nC);
        nC=NULL;
    }
    return true;
}
