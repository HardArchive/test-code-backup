#include "NSPClient.h"
#include "cJSON.h"

//file transfer cgi
#define URL_up_init "/up/up_init"
#define URL_up_retcon "/up/up_retcon"
#define URL_up_blk "/up/up_blk"
#define URL_up_seginfo "/up/up_seginfo"

typedef enum{
    UNKNOWN = 0,
    REQTCON,
    UPSTAT,
    REQTBLK,
    REQTSEG
}nsp_upcmd_e;

typedef enum{
    UNKNOWN_UPSTAT = 0,
    FIN,
    RECV,
    NONE
}nsp_upstat_e;

typedef struct nsp_ret_s        nsp_ret_t;
typedef struct nsp_range_s      nsp_range_t;

struct nsp_range_s{
    int start;
    int end;
};

struct nsp_ret_s{
    nsp_upcmd_e nsp_upcmd;
    nsp_upstat_e nsp_upstat;
    char nsp_fid[33];
    char nsp_path[256];
    int nsp_fstat_rsize;
    nsp_range_t nsp_range;

    long nsp_ts;
    char nsp_size[33];
    char nsp_sig[33];
    long nsp_segsize;
};
//store big file infomation
//fileid(segment md5)
//pfileid(file md5)
struct nsp_seg_info{
    char fileid[33];
    nsp_range_t range;
    struct nsp_seg_info *next;
};
struct nsp_file_info{
    bool isbigfile;
    char pfileid[33];
    char filename[256];
    nsp_seg_info nsi;
};
void nsp_bigfile_clean(nsp_seg_info *nsi){
    if(nsi==NULL){
        return;
    }
    else if(nsi->next!=NULL){
        nsp_bigfile_clean(nsi->next);
    }
    free(nsi);
    nsi=NULL;
}

long sent_bytes=0;

bool parse_range(const char * range_str ,nsp_range_t *nsp_range)
{
    char        *p;
    int       start, end;
    nsp_range->start = 0;
    nsp_range->end = 0;

    p = (char *)range_str;
    while (*p == ' ') { p++; }

    for ( ;; ) {
        start = 0;
        end = 0;
        while(*p != '-'){
            if (*p < '0' || *p > '9') {
                return false;
            }

            while (*p >= '0' && *p <= '9') {
                start = start * 10 + *p++ - '0';
            }

            while (*p == ' ') { p++; }
        }
        if (*p++ != '-') {
            return false;
        }

        while (*p == ' ') { p++; }
        if (*p < '0' || *p > '9') {
            return false;
        }

        while (*p >= '0' && *p <= '9') {
            end = end * 10 + *p++ - '0';
        }
        if (start > end) {
            return false;
        }
        nsp_range->start = start;
        nsp_range->end = end;
        return true;
    }
}

void parse_ret(char *ptr,nsp_ret_t *ret_t){
    cJSON *json;
    json=cJSON_Parse((char *)ptr);
    if(!json || !json->child){ret_t->nsp_upcmd=UNKNOWN;return ;}

    cJSON *obj=json->child;

    bool has_upcmd=false;
    while(obj!=NULL){
        if(strcmp(obj->string,"nsp_upcmd")==0){
            has_upcmd=true;
            if(strcmp(obj->valuestring,"REQTCON")==0)        {ret_t->nsp_upcmd = REQTCON;}
            else if(strcmp(obj->valuestring,"UPSTAT")==0)    {ret_t->nsp_upcmd = UPSTAT;}
            else if(strcmp(obj->valuestring,"REQTBLK")==0)    {ret_t->nsp_upcmd = REQTBLK;}
            else if(strcmp(obj->valuestring,"REQTSEG")==0)    {ret_t->nsp_upcmd = REQTSEG;}
            else                                            {ret_t->nsp_upcmd = UNKNOWN;cJSON_Delete(json);return;}
        }
        else if(strcmp(obj->string,"nsp_fid")==0){
            strncpy(ret_t->nsp_fid,obj->valuestring, 32);
            ret_t->nsp_fid[32] = 0;
        }
        else if(strcmp(obj->string,"nsp_range")==0){
            parse_range(obj->valuestring, &ret_t->nsp_range);
        }
        else if(strcmp(obj->string,"nsp_upstat")==0){
            if(strcmp("FIN",obj->valuestring) == 0)            {ret_t->nsp_upstat = FIN;}
            else if(strcmp("RECV",obj->valuestring) == 0)    {ret_t->nsp_upstat = RECV;}
            else if(strcmp("NONE",obj->valuestring) == 0)    {ret_t->nsp_upstat = NONE;}
            else                                            {ret_t->nsp_upstat = UNKNOWN_UPSTAT;cJSON_Delete(json);return;}
        }
        else if(strcmp(obj->string,"nsp_fstat")==0){
            cJSON *chi = obj->child;
            while(chi!=NULL){
                if(strcmp(chi->string,"RSIZE")==0){
                    ret_t->nsp_fstat_rsize = chi->valueint;
                }
                chi=chi->next;
            }
            ret_t->nsp_fstat_rsize = 0;
        }
        else if(strcmp(obj->string,"nsp_path")==0){
            strcpy(ret_t->nsp_path,obj->valuestring);
        }
        else if(strcmp(obj->string,"nsp_ts")==0){
            ret_t->nsp_ts=obj->valueint;
        }
        else if(strcmp(obj->string,"nsp_size")==0){
            strcpy(ret_t->nsp_size,obj->valuestring);
        }
        else if(strcmp(obj->string,"nsp_sig")==0){
            strcpy(ret_t->nsp_sig,obj->valuestring);
        }
        else if(strcmp(obj->string,"nsp_segsize")==0){
            ret_t->nsp_segsize=obj->valueint;
        }
        else{}
        obj=obj->next;
    }
    if(!has_upcmd){
        ret_t->nsp_upcmd = UNKNOWN;
    }
    cJSON_Delete(json);
}

size_t process_nsp_ret( void *ptr, size_t size, size_t nmemb, void *userdata)
{
    if (strlen((char *)userdata) + strlen((char *)ptr) > SMALLBUFSIZE-1) return 0;
    strcat((char *)userdata,(char *)ptr);
    return size * nmemb;
}

//big file is splited into several segments
//get file range of a segment by its md5
void getSeginfoByfid(nsp_seg_info *nsi,char *fid,nsp_range_t &segpos){
    while(nsi!=NULL){
        if(strcmp(nsi->fileid,fid)==0){
            segpos.start=nsi->range.start;
            segpos.end=nsi->range.end;
            return;
        }
        nsi=nsi->next;
    }
}

void (*callupload_func)(double,double);
bool up_blk(char *appid,char * host, char *tstr, char *secret, char *filename, 
                    int nspfsize, nsp_ret_t *nsp_ret, nsp_file_info *file_info,void *callfunc=NULL)
{

    CURL *curl;
    CURLcode ret;
    char url[1024];
    char tmp[1024];
    time_t nspts = time(0);

    nsp_range_t seg_info_range;
    if(file_info->isbigfile){        //big file > 100M    fid,fsize,pfid,pfsize shall be different
        getSeginfoByfid(&file_info->nsi,nsp_ret->nsp_fid,seg_info_range);
        sprintf(tmp, "%snsp_app%snsp_fid%snsp_fmtJSONnsp_fsize%dnsp_pfid%snsp_pfsize%dnsp_range%d-%dnsp_ts%dnsp_tstr%snsp_ver1.0", 
            secret,appid,nsp_ret->nsp_fid, seg_info_range.end-seg_info_range.start+1 , file_info->pfileid , nspfsize, 
                nsp_ret->nsp_range.start, nsp_ret->nsp_range.end, (int)nspts, tstr);
    }
    else{//file < 100M    no segments
        sprintf(tmp, "%snsp_app%snsp_fid%snsp_fmtJSONnsp_fsize%dnsp_range%d-%dnsp_ts%dnsp_tstr%snsp_ver1.0", 
                secret, appid, nsp_ret->nsp_fid, nspfsize, 
                nsp_ret->nsp_range.start, nsp_ret->nsp_range.end, (int)nspts, tstr);
    }

    char pwdmd5[33];
    md5(pwdmd5,tmp);
    for(int i=0;i<32;i++){pwdmd5[i]=tolower(pwdmd5[i]);}
    if(file_info->isbigfile){        //big file > 100M    fid,fsize,pfid,pfsize shall be changed
        sprintf(url, "http://%s%s?nsp_app=%s&nsp_ts=%d&nsp_fmt=JSON&nsp_ver=1.0&nsp_key=%s" \
                     "&nsp_fid=%s&nsp_fsize=%d&nsp_range=%d-%d&nsp_tstr=%s&nsp_pfid=%s&nsp_pfsize=%d", 
                    host, URL_up_blk, appid, (int)nspts, pwdmd5 , nsp_ret->nsp_fid, seg_info_range.end-seg_info_range.start+1 ,
                    nsp_ret->nsp_range.start, nsp_ret->nsp_range.end, tstr , file_info->pfileid , nspfsize);
    }
    else{//file < 100M no segments
        sprintf(url, "http://%s%s?nsp_app=%s&nsp_ts=%d&nsp_fmt=JSON&nsp_ver=1.0&nsp_key=%s" \
                     "&nsp_fid=%s&nsp_fsize=%d&nsp_range=%d-%d&nsp_tstr=%s", 
                    host, URL_up_blk, appid, (int)nspts, pwdmd5 , nsp_ret->nsp_fid, nspfsize,
                    nsp_ret->nsp_range.start, nsp_ret->nsp_range.end, tstr);
    }
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    static const char buf[] = "Expect:";
    headerlist = curl_slist_append(headerlist, buf);

    curl_global_init(CURL_GLOBAL_ALL);
 
    long buffer_len = nsp_ret->nsp_range.end - nsp_ret->nsp_range.start + 1;
    char *buffer = (char *)malloc(buffer_len+1);
    if(!buffer){return false;}
    memset(buffer,0,buffer_len+1);
    FILE *fp; 
    if( (fp = fopen(filename, "rb") ) == NULL ){ 
        nsplog("内部错误",RET_FILE_MSG);
        return false;
    }

    size_t n;
    if(file_info->isbigfile){        //big file > 100M    start pos shall be different
        fseek(fp, seg_info_range.start+nsp_ret->nsp_range.start, SEEK_SET);
    }
    else{
        fseek(fp, nsp_ret->nsp_range.start, SEEK_SET);
    }
    n = fread(buffer, buffer_len, 1, fp) * buffer_len;
    fclose(fp);

    if(callfunc!=NULL){
        sent_bytes+=n;
        callupload_func = (void (*)(double,double))callfunc;
        callupload_func((double)sent_bytes,(double)nspfsize);
    }

    curl_formadd(&formpost,
                &lastptr,
                CURLFORM_COPYNAME, "file",
                CURLFORM_BUFFER, filename,
                CURLFORM_BUFFERPTR, buffer,
                CURLFORM_BUFFERLENGTH, buffer_len,
                CURLFORM_END);

    curl = curl_easy_init();
    curl_easy_setopt (curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    char ret_data[SMALLBUFSIZE]="";

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, process_nsp_ret);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, ret_data);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    ret = curl_easy_perform(curl);


    if(ret != CURLE_OK){
        nsplog("内部错误",RET_CURL_MSG);
        goto error;
    }
    long code;
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &code);
    if(code != 200){
        char errorMsg[100];
        sprintf(errorMsg,"curl连接错误 %s %d",RET_SERVER_MSG,code);
        nsplog("内部错误",errorMsg);
        goto error;
    }
    parse_ret(ret_data,nsp_ret);

    curl_easy_cleanup(curl);
    curl_formfree(formpost);
    curl_slist_free_all (headerlist);
    free(buffer);
    return true;

error:
    curl_easy_cleanup(curl);
    curl_formfree(formpost);
    curl_slist_free_all (headerlist);
    free(buffer);
    return false;
}

bool up_retcon(char *appid,char * host, char *tstr, char *secret, char *filename, 
                    int nspfsize, nsp_ret_t *nsp_ret, nsp_file_info *file_info)
{
    FILE *fp;
    char pwdmd5[33];
    int i;
    nsp_range_t seg_info_range;
    if(file_info->isbigfile){//find which segments has the same md5 value with nsp_fid and find its range
        getSeginfoByfid(&file_info->nsi,nsp_ret->nsp_fid,seg_info_range);
    }
    if( (fp = fopen(filename, "rb") ) != NULL ){
        if(file_info->isbigfile){
            fseek(fp,seg_info_range.start+nsp_ret->nsp_range.start, SEEK_SET);
        }
        else{
            fseek(fp, nsp_ret->nsp_range.start, SEEK_SET);
        }
        int size = nsp_ret->nsp_range.end - nsp_ret->nsp_range.start + 1;
        char *buf = (char *)malloc(size);
        if(buf == NULL){
            fclose(fp);fp=NULL;
            return false;
        }
        int n = fread(buf, size, 1, fp);
        if (n != 1){
            nsplog("内部错误",RET_FILE_MSG);
            fclose(fp);fp=NULL;
            return false;
        }
        //buf[size] = 0;
        md5bytes(pwdmd5,(unsigned char *)buf,size);
        free(buf);
        fclose(fp);
    }else{
        nsplog("内部错误",RET_FILE_MSG);
        return false;
    }

    CURL *curl;
    CURLcode ret;
    char url[1024];
    char tmp[1024];
    time_t nspts = time(0);
    char nspkey[33];
    char nspfcon[33];

    for(i=0;i<32;i++){pwdmd5[i]=tolower(pwdmd5[i]);}
    strncpy(nspfcon, pwdmd5, 32);
    nspfcon[32] = 0;

    if(file_info->isbigfile){        //big file > 100M    fid,fsize,pfid,pfsize shall be different
        sprintf(tmp, "%snsp_app%snsp_fcon%snsp_fid%snsp_filename%snsp_fmtJSONnsp_fsize%dnsp_pfid%snsp_pfsize%dnsp_range%d-%dnsp_ts%dnsp_tstr%snsp_ver1.0", 
            secret, appid, nspfcon, nsp_ret->nsp_fid, file_info->filename , seg_info_range.end-seg_info_range.start+1,file_info->pfileid,nspfsize,
                nsp_ret->nsp_range.start, nsp_ret->nsp_range.end, (int)nspts, tstr);
    }
    else{
        sprintf(tmp, "%snsp_app%snsp_fcon%snsp_fid%snsp_fmtJSONnsp_fsize%dnsp_range%d-%dnsp_ts%dnsp_tstr%snsp_ver1.0", 
                secret, appid, nspfcon, nsp_ret->nsp_fid, nspfsize, 
                nsp_ret->nsp_range.start, nsp_ret->nsp_range.end, (int)nspts, tstr);
    }
    md5(pwdmd5,tmp);
    for(i=0;i<32;i++){pwdmd5[i]=tolower(pwdmd5[i]);}
    strncpy(nspkey, pwdmd5, 32);
    nspkey[32] = 0;

    if(file_info->isbigfile){        //big file > 100M    fid,fsize,pfid,pfsize shall be changed
        sprintf(url, "http://%s%s?nsp_app=%s&nsp_ts=%d&nsp_fmt=JSON&nsp_ver=1.0&nsp_key=%s" \
                     "&nsp_fid=%s&nsp_fsize=%d&nsp_range=%d-%d&nsp_fcon=%s&nsp_tstr=%s&nsp_pfid=%s&nsp_pfsize=%d&nsp_filename=%s", 
                    host, URL_up_retcon, appid, (int)nspts, nspkey, nsp_ret->nsp_fid, seg_info_range.end-seg_info_range.start+1,
                    nsp_ret->nsp_range.start, nsp_ret->nsp_range.end, nspfcon, tstr , file_info->pfileid , nspfsize,file_info->filename);
    }
    else{
        sprintf(url, "http://%s%s?nsp_app=%s&nsp_ts=%d&nsp_fmt=JSON&nsp_ver=1.0&nsp_key=%s" \
                     "&nsp_fid=%s&nsp_fsize=%d&nsp_range=%d-%d&nsp_fcon=%s&nsp_tstr=%s", 
                    host, URL_up_retcon, appid, (int)nspts, nspkey, nsp_ret->nsp_fid, nspfsize,
                    nsp_ret->nsp_range.start, nsp_ret->nsp_range.end, nspfcon, tstr);
    }
    char ret_data[SMALLBUFSIZE]="";

    curl = curl_easy_init();
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, process_nsp_ret);
    curl_easy_setopt (curl, CURLOPT_URL, url);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, ret_data);

    ret = curl_easy_perform (curl);
    if(ret != CURLE_OK){
        nsplog("内部错误",RET_CURL_MSG);
        curl_easy_cleanup (curl);
        return false;
    }
    long code;
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup (curl);

    if(code != 200){
        char errorMsg[50];
        sprintf(errorMsg,"%s %d",RET_SERVER_MSG,code);
        nsplog("内部错误",errorMsg);
        return false;
    }
    parse_ret(ret_data,nsp_ret);
    return true;
}

bool _calc_md5info(char *filename,nsp_file_info &file_info,int segmentsize);
//big file transfer
bool up_seginfo(char *appid,char *host, char *tstr,char *secret,char *filename,int nspfsize,nsp_ret_t *nsp_ret,nsp_file_info *file_info){

 
    char pwdmd5[33];
    int i;
    int segnums=0;
    nsp_seg_info *head_seg = &file_info->nsi;

    if(nsp_ret->nsp_segsize!=SEGMENTSIZE){    //shall recalculate segments md5

        nsp_bigfile_clean(file_info->nsi.next);//delete old md5 result
        if(_calc_md5info(filename,*file_info,nsp_ret->nsp_segsize)==false){return false;}
    }

    //make seg infomation into json str
    char *nsp_seglist=(char *)malloc(int(nspfsize/SEGMENTSIZE+1)*100);//has nearly nspfsize/SEGMENTSIZE segments
	if(nsp_seglist == NULL){return false;}
    strcpy(nsp_seglist,"[");
    char nsp_seg[100];
    head_seg = file_info->nsi.next;
    while(head_seg!=NULL){
        sprintf(nsp_seg,"{\"fileid\":\"%s\",\"range\":\"%d-%d\"},",head_seg->fileid,head_seg->range.start,head_seg->range.end);
        strcat(nsp_seglist,nsp_seg);
        head_seg=head_seg->next;
    }
    nsp_seglist[strlen(nsp_seglist)-1]=']';
    //finish

    CURL *curl;
    CURLcode ret;
    char url[URL_LENGTH*4];
    char tmp[URL_LENGTH];
    time_t nspts = time(0);
    char nspkey[33];

    sprintf(tmp, "%snsp_app%snsp_fid%snsp_fmtJSONnsp_fsize%dnsp_seglist%snsp_ts%dnsp_tstr%snsp_ver1.0", 
            secret, appid, nsp_ret->nsp_fid, nspfsize, nsp_seglist, (int)nspts, tstr);

    md5(pwdmd5,tmp);
    for(i=0;i<32;i++){pwdmd5[i]=tolower(pwdmd5[i]);}
    strncpy(nspkey, pwdmd5, 32);
    nspkey[32] = 0;

    sprintf(url, "http://%s%s?nsp_app=%s&nsp_ts=%d&nsp_fmt=JSON&nsp_ver=1.0&nsp_key=%s" \
                 "&nsp_fid=%s&nsp_fsize=%d&nsp_tstr=%s&nsp_seglist=%s", 
                host, URL_up_seginfo, appid, (int)nspts, nspkey, nsp_ret->nsp_fid, nspfsize, tstr , nsp_seglist);

    //no longer need json str
    free(nsp_seglist);
    nsp_seglist=NULL;

    char ret_data[SMALLBUFSIZE]="";

    curl = curl_easy_init();
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, process_nsp_ret);
    curl_easy_setopt (curl, CURLOPT_URL, url);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, ret_data);

    ret = curl_easy_perform (curl);
    if(ret != CURLE_OK){
        nsplog("内部错误",RET_CURL_MSG);
        curl_easy_cleanup (curl);
        return false;
    }
    long code;
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup (curl);

    if(code != 200){
        char errorMsg[100];
        sprintf(errorMsg,"curl连接出错 %s %d",RET_SERVER_MSG,code);
        nsplog("内部错误",errorMsg);
        return false;
    }
    parse_ret(ret_data,nsp_ret);

    return true;
}

bool up_init(char *appid, char *host, char *tstr, char *secret, char *filename, int nspfsize, nsp_ret_t *nsp_ret ,char *nspfid)
{
    CURL *curl;
    CURLcode ret;
    char url[1024];
    char tmp[1024];

    char nspkey[33];
    char pwdmd5[33];

    time_t nspts = time(0);

    sprintf(tmp, "%snsp_app%snsp_fid%snsp_fmtJSONnsp_fsize%dnsp_ts%dnsp_tstr%snsp_ver1.0", 
            secret, appid, nspfid, nspfsize, (int)nspts, tstr);

    md5(pwdmd5,tmp);
    for(int i=0;i<32;i++){pwdmd5[i]=tolower(pwdmd5[i]);}

    strncpy(nspkey, pwdmd5, 32);
    nspkey[32] = 0;

    sprintf(url, "http://%s%s?nsp_app=%s&nsp_ts=%d&nsp_fmt=JSON&nsp_ver=1.0&nsp_key=%s&nsp_fid=%s&nsp_fsize=%d&nsp_tstr=%s", 
                host, URL_up_init, appid, (int)nspts, nspkey, nspfid, nspfsize, tstr);

    char ret_data[SMALLBUFSIZE]="";

    curl = curl_easy_init();
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, process_nsp_ret);
    curl_easy_setopt (curl, CURLOPT_URL, url);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, ret_data);

    ret = curl_easy_perform (curl);

    if(ret != CURLE_OK){
        nsplog("内部错误",RET_CURL_MSG);
        curl_easy_cleanup (curl);
        return false;
    }
    long code;
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup (curl);
    if(code != 200){
        char errorMsg[100];
        sprintf(errorMsg,"curl连接错误 %s %d",RET_SERVER_MSG,code);
        nsplog("内部错误",errorMsg);
        return false;
    }
    parse_ret(ret_data,nsp_ret);
    return true;
}

long get_file_size(char * filename)    
{    
    struct   stat   f_stat;    
    if(stat(filename, &f_stat) == -1){    
        return   -1;    
    }    
    return   (long)f_stat.st_size;    
}  

bool _load_md5info_from_cache(char *cachename,nsp_file_info &file_info){
    FILE * fp;
    int n;
    char buf[1024];
    if( (fp = fopen(cachename, "r") ) != NULL ){//file exsit read information
        if(!fscanf(fp,"%s",buf)){ remove(cachename); return false;}
        if(strncmp(buf,"bigfile",7)==0){
            file_info.isbigfile=true;
        }
        else{
            file_info.isbigfile=false;
        }
        if(!fscanf(fp,"%s",&file_info.filename)){ remove(cachename); return false;}
        if(!fscanf(fp,"%s",&file_info.pfileid)){ remove(cachename); return false;}
        file_info.pfileid[32]=0;
        if(file_info.isbigfile){
            nsp_seg_info *head=&file_info.nsi;
            while(true){
                nsp_seg_info *nsi = (nsp_seg_info *)malloc(sizeof(nsp_seg_info));
                if(!nsi){return false;}
                nsi->next=NULL;
                n=fscanf(fp,"%s %d %d",&nsi->fileid,&nsi->range.start,&nsi->range.end);
                if(n==EOF){
                    free(nsi);nsi=NULL;
                    break;
                }
                head->next=nsi;
                head=head->next;
            }
        }
        fclose(fp);
        return true;
    }
    return false;
}

bool _save_md5info_to_cache(char *cachename,nsp_file_info &file_info){
    FILE * fp;
    if( (fp = fopen(cachename, "w") ) != NULL ){//write information to cache file
        if(file_info.isbigfile)    {fputs("bigfile\n",fp);}
        else                    {fputs("smallfile\n",fp);}
        fputs(file_info.filename,fp);
        fputs("\n",fp);
        fputs(file_info.pfileid,fp);
        fputs("\n",fp);
        nsp_seg_info *curr = file_info.nsi.next;
        while(curr!=NULL){
            fprintf(fp,"%s %d %d\n",curr->fileid,curr->range.start,curr->range.end);
            curr=curr->next;
        }
        fclose(fp);
        return true;
    }
    else{return false;}
}

bool _calc_md5info(char *filename,nsp_file_info &file_info,int segmentsize){
    char pwdmd5[33],pwdmd5_seg[33];
    memset(pwdmd5,0,sizeof(pwdmd5));
    memset(pwdmd5_seg,0,sizeof(pwdmd5_seg));

    //calc md5c for file and md5c_seg for segments
    MD5_CTX md5c,md5c_seg;
    unsigned char ss[16],ss_seg[16];
    char tmp[3]={'\0'};
    int i;
    MD5Init(&md5c);
    MD5Init(&md5c_seg);

    FILE *infile;
    infile=fopen(filename,"rb");
    if(!infile){return false;}
    long length;
    long currpos=0;
    char buffer[READFILEBUF];

    nsp_seg_info *head=&file_info.nsi;

    int readsize=READFILEBUF;
    bool finalseg=false;
    long currstart=0;
    while ((length=fread(buffer,sizeof(unsigned char),readsize,infile))!=0) {
        if (length > 0) {
            MD5Update(&md5c,(unsigned char *)buffer,length);
            MD5Update(&md5c_seg,(unsigned char *)buffer,length);
            currpos+=length;
            finalseg=true;
        }
        int rest = segmentsize - (currpos%segmentsize);
        if(rest<READFILEBUF && rest != 0){
            readsize=segmentsize - (currpos%segmentsize);
        }
        else if(rest == segmentsize){

            nsp_seg_info *nsi = (nsp_seg_info *)malloc(sizeof(nsp_seg_info));
            if(!nsi){return false;}

            MD5Final(&md5c_seg,ss_seg);
            char tmp_seg[3]={'\0'};
            for(i=0;i<16;i++)
            {
                sprintf(tmp_seg,"%02X",ss_seg[i]);
                strcat(pwdmd5_seg,tmp_seg);
            }
            for(i=0;i<32;i++){pwdmd5_seg[i]=tolower(pwdmd5_seg[i]);}

            strcpy(nsi->fileid,pwdmd5_seg);
            nsi->fileid[32]=0;
            nsi->range.start=currpos-segmentsize;
            nsi->range.end=currpos-1;
            currstart=currpos;
            nsi->next=NULL;

            head->next=nsi;
            head=head->next;

            memset(pwdmd5_seg,0,sizeof(pwdmd5_seg));
            MD5Init(&md5c_seg);
            readsize=READFILEBUF;
            finalseg=false;
        }
    }
    if(finalseg && file_info.isbigfile){
            nsp_seg_info *nsi = (nsp_seg_info *)malloc(sizeof(nsp_seg_info));
            if(!nsi){return false;}

            MD5Final(&md5c_seg,ss_seg);
            char tmp_segf[3]={'\0'};
            for(i=0;i<16;i++)
            {
                sprintf(tmp_segf,"%02X",ss_seg[i]);
                strcat(pwdmd5_seg,tmp_segf);
            }
            for(i=0;i<32;i++){pwdmd5_seg[i]=tolower(pwdmd5_seg[i]);}

            strcpy(nsi->fileid,pwdmd5_seg);
            nsi->fileid[32]=0;
            nsi->range.start=currstart;
            nsi->range.end=currpos-1;
            nsi->next=NULL;

            head->next=nsi;
            head=head->next;
    }

    fclose(infile);
    MD5Final(&md5c,ss);
    
    for(i=0;i<16;i++)
    {
        sprintf(tmp,"%02X",ss[i]);
        strcat(pwdmd5,tmp);
    }
    for(i=0;i<32;i++){pwdmd5[i]=tolower(pwdmd5[i]);}
        
    strcpy(file_info.filename,strrchr(filename,'/')+1);
    strcpy(file_info.pfileid,pwdmd5);
    file_info.pfileid[32]=0;

    return true;
}

bool load_fileinfo(nsp_file_info &file_info,char *filename){

    char cachename[1024]="cache/";
    strcat(cachename,strrchr(filename,'/')+1);
    strcat(cachename,".cache");

    //try to get file info from cache
    if(_load_md5info_from_cache(cachename,file_info)){
        return true;
    }
    else//if there is no cache,calculate its info and save into cache
    {
        if(_calc_md5info(filename,file_info,SEGMENTSIZE))
        {
            if(file_info.isbigfile)
            {
                _save_md5info_to_cache(cachename,file_info);
            }
            return true;
        }
        else{
            return false;
        }
    }
}

//remove md5 information of bigfile after upload
void remove_cache(char *filename)
{
	if(strlen(filename)==0){return;}
    char file[MAX_PATH]="cache/";
    strcat(file,filename);
    strcat(file,".cache");
    remove(file);
}

//space+spaceret-usedspace
bool hasspace(char *rest,char *restret,char *used,int filesize)
{
    int L1=strlen(rest),L2=strlen(restret),L3=strlen(used);
    while(int(filesize/10)!=0){
        L1--;
        L2--;
        L3--;
        filesize/=10;
    }
    char re[10]={'\0'},rer[10]={'\0'},us[10]={'\0'};
    if(L1>0){strncpy(re,rest,L1);}
    if(L2>0){strncpy(rer,restret,L2);}
    if(L3>0){strncpy(us,used,L3);}
    int restsize=atoi(re)+atoi(rer)-atoi(us)-filesize;
    return restsize>filesize;
}

bool abletoUpload(int filesize){

    nspAction *nA=NSPAction();
	if(nA==NULL){return false;}
    nsp_node *send = nA->createArray();
    nsp_node *param = nA->createArray();
    if(send == NULL || param == NULL){NSPActionDelete(nA);return false;}
    nA->appendString(param,"product.spacecapacity");
    nA->appendString(param,"profile.spaceextcapacity");
    nA->appendString(param,"profile.usedspacecapacity");
    nA->appendString(param,"product.fileuploadsize");
    nA->appendArray(send,param);

    nsp_node *space = NULL;
    nsp_node *spaceext = NULL;
    nsp_node *usedspace = NULL;
    nsp_node *uploadsize = NULL;
    //send information and receive result and print
    nsp_node *recv = NSPService("nsp.user.getInfo",*send);
    if(recv==NULL){goto finish_able;}

    space = nA->searchNodeByStr(recv,"product.spacecapacity");
    spaceext = nA->searchNodeByStr(recv,"profile.spaceextcapacity");
    usedspace = nA->searchNodeByStr(recv,"profile.usedspacecapacity");
    uploadsize = nA->searchNodeByStr(recv,"product.fileuploadsize");

    if(space==NULL || spaceext==NULL || usedspace==NULL || uploadsize==NULL){
        nA->deleteNode(recv);
        goto finish_able;
    }
    
    if(!hasspace(nA->getNodeStringValue(space),nA->getNodeStringValue(spaceext),nA->getNodeStringValue(usedspace),filesize) || atoi(nA->getNodeStringValue(uploadsize))<filesize){
        nA->deleteNode(recv);
        goto finish_able;    
    }

    //free memory
    nA->deleteNode(recv);
    nA->deleteNode(send);
    NSPActionDelete(nA);
    return true;

finish_able:
    nA->deleteNode(send);
    NSPActionDelete(nA);
    return false;
}

bool upload_file(char *path,char * filename,nsp_node &info,nsp_node &recv,bool simpletype=false,void *callfunc=NULL){

    nsplog("上传文件",filename);
    char cfile[MAX_PATH];
    char appid[33];
    char server[32];
    char tstr[32];
    char secret[33];
    nsp_node *nsp_search;

    nspAction *nA=NSPAction();
    if(nA==NULL){return false;}

    nsp_file_info file_info; 
	memset(file_info.filename,0,sizeof(file_info.filename));
	file_info.nsi.next=NULL;

    int nspfsize = -1;
    //upauth information
    nsp_search = nA->searchNodeByStr(&info,"nsp_tapp");
    if(nsp_search!=NULL){strcpy(appid,nsp_search->node_data.valuestring);}else{nsplog("内部错误","文件上传鉴权出错");goto upload_fail;}
    nsp_search = nA->searchNodeByStr(&info,"nsp_host");
    if(nsp_search!=NULL){strcpy(server,nsp_search->node_data.valuestring);}else{nsplog("内部错误","文件上传鉴权出错");goto upload_fail;}
    nsp_search = nA->searchNodeByStr(&info,"nsp_tstr");
    if(nsp_search!=NULL){sprintf(tstr,"%d",nsp_search->node_data.valueint);}else{nsplog("内部错误","文件上传鉴权出错");goto upload_fail;}
    nsp_search = nA->searchNodeByStr(&info,"secret");
    if(nsp_search!=NULL){strcpy(secret,nsp_search->node_data.valuestring);}else{nsplog("内部错误","文件上传鉴权出错");goto upload_fail;}

    strcpy(cfile, filename);

    nspfsize = get_file_size(cfile); //get length of file
    if(nspfsize == -1){
        nsplog("内部错误","获取上传文件大小出错");
        goto upload_fail;
    }
    if(nspfsize>BIGFILESIZE){
        file_info.isbigfile=true;
    }
    else{file_info.isbigfile=false;}
    // store file(>100M) seginfo 

    // control big file size
    if(!abletoUpload(nspfsize)){
        nsplog("内部错误","网盘空间已满或者无权限上传大文件");
        goto upload_fail;
    }

    if(!load_fileinfo(file_info,filename)){
        nsplog("内部错误","获取/计算文件信息出错");
        goto upload_fail;
    }
    sent_bytes = 0;


    nsp_ret_t nsp_ret;
    if(!up_init(appid,server, tstr, secret, cfile, nspfsize, &nsp_ret ,file_info.pfileid )){
        nsplog("内部错误","上传初始化失败");
        goto upload_fail;
    }
    while(1){
        switch (nsp_ret.nsp_upcmd){
        case REQTCON:
            if(!up_retcon(appid,server, tstr, secret, cfile, nspfsize, &nsp_ret, &file_info)){
                nsplog("内部错误","飞速上传出错");
                goto upload_fail;
            }
            break;
        case REQTBLK:
            if(!up_blk(appid,server, tstr, secret, cfile, nspfsize, &nsp_ret, &file_info, callfunc)){
                nsplog("内部错误","文件分块上传出错");
                goto upload_fail;
            }
            break;
        case UPSTAT:
            if(nsp_ret.nsp_upstat == NONE){
                nsplog("内部错误","未知的上传状态");
                goto upload_fail;
            }else if(nsp_ret.nsp_upstat == FIN){
                //begin to mkfile
                nA->appendMap_Str_Str(&recv,"nsp_fid",nsp_ret.nsp_fid);
                nA->appendMap_Str_Str(&recv,"nsp_path",nsp_ret.nsp_path);
                nA->appendMap_Str_Str(&recv,"nsp_size",nsp_ret.nsp_size);
                nA->appendMap_Str_Str(&recv,"nsp_sig",nsp_ret.nsp_sig);
                nA->appendMap_Str_Int(&recv,"nsp_ts",nsp_ret.nsp_ts);
                NSPActionDelete(nA);
                remove_cache(file_info.filename);
                if(file_info.isbigfile && file_info.nsi.next!=NULL){
                    nsp_bigfile_clean(file_info.nsi.next);
                }
                if(callfunc!=NULL){
                    callupload_func = (void (*)(double,double))callfunc;
                    callupload_func((double)nspfsize,(double)nspfsize);
                }
                return true; 
            }
            nsplog("内部错误","未知的上传状态");
            goto upload_fail;
        case REQTSEG:
            if(!up_seginfo(appid,server, tstr, secret, cfile, nspfsize, &nsp_ret ,&file_info)){
                nsplog("内部错误","文件分块信息出错");
                goto upload_fail;
            }
            file_info.isbigfile=true;
            break;
        default:
            nsplog("内部错误","未知的上传状态");
            goto upload_fail;
        }
    }
    return true;

upload_fail:
    NSPActionDelete(nA);
    remove_cache(file_info.filename);
    if(file_info.isbigfile && file_info.nsi.next!=NULL){
        nsp_bigfile_clean(file_info.nsi.next);
    }
    return false;
}

Result* Upload(char *dirpath,int count,...){
    
    nspAction *nA=NSPAction();
    if(nA==NULL){return NULL;}

    nsp_node *send=nA->createArray();
    nsp_node *info;

    va_list lst;
    char *arg=NULL;
    va_start(lst, count);
    int tag=0;
    int successpos=0,failpos=0;

    char *mkfile = (char *)malloc(count*1024);//assume average path of each file has 1024 characters
    strcpy(mkfile,"[");

    //fetch vfs.upauth information
    if((info=NSPService("nsp.vfs.upauth",*send))==NULL){
        return NULL;
    }

    //record files which failed to upload
    nsp_node *failfiles = nA->createArray();
    if(failfiles==NULL){free(mkfile);nA->deleteNode(info);nA->deleteNode(send);NSPActionDelete(nA);return NULL;}

    while((arg=va_arg(lst,char *))!=NULL && tag<count)
    {
            nsp_node *recv=nA->createArray();
            if(recv==NULL){free(mkfile);nA->deleteNode(failfiles);nA->deleteNode(info);nA->deleteNode(send);NSPActionDelete(nA);return NULL;}

            //begin to upload
            if(!upload_file(dirpath,arg,*info,*recv,false)){
                nA->deleteNode(recv);
                tag++;
                nsp_node *failfile = nA->createArray();
                if(!failfile){
                    free(mkfile);nA->deleteNode(failfiles);nA->deleteNode(info);nA->deleteNode(send);NSPActionDelete(nA);
                    return NULL;
                }

                nA->appendMap_Str_Str(failfile,"name",arg);
                nA->appendMap_Str_Int(failfile,"errCode",999);
                nA->appendMap_Str_Str(failfile,"errMsg","failed to upload file");

                nA->appendArray(failfiles,failfile);
                continue;
            }
            char mkf[1024];

            //begin to strcat mkfile string

            char fid[33];
            char path[256];
            char size[33];
            char sig[33];
            int ts;
            nsp_node *nsp_search = nA->searchNodeByStr(recv,"nsp_fid");
            if(nsp_search!=NULL){strcpy(fid,nsp_search->node_data.valuestring);}
            nsp_search = nA->searchNodeByStr(recv,"nsp_path");
            if(nsp_search!=NULL){strcpy(path,nsp_search->node_data.valuestring);}
            nsp_search = nA->searchNodeByStr(recv,"nsp_size");
			if(nsp_search!=NULL){strcpy(size,nsp_search->node_data.valuestring);}
            nsp_search = nA->searchNodeByStr(recv,"nsp_sig");
			if(nsp_search!=NULL){strcpy(sig,nsp_search->node_data.valuestring);}
            nsp_search = nA->searchNodeByStr(recv,"nsp_ts");
			if(nsp_search!=NULL){ts=nsp_search->node_data.valueint;}

            sprintf(mkf,"{\"name\":\"%s\",\"md5\":\"%s\",\"type\":\"File\",\"url\":\"%s\",\"size\":\"%s\",\"sig\":\"%s\",\"ts\":%d},",
                    (strrchr(arg,'/')+1),
                    fid,
                    path,
                    size,
                    sig,
                    ts);
            strcat(mkfile,mkf);
            tag++;

            nA->deleteNode(recv);
    }
    nA->deleteNode(info);
    nA->deleteNode(send);

    nsp_node *ret = NULL;
    if(strlen(mkfile)==1){
        ret = nA->createArray();
        if(ret!=NULL){
            nA->appendMap_Str_Node(ret,"successList",nA->createArray());
            nA->appendMap_Str_Node(ret,"failList",failfiles);
        }
        
        goto finish_upload;
    }
    mkfile[strlen(mkfile)-1]=']';

    //begin to mkfile

    send=nA->createArray();
    nA->appendMap_Str_Str(send,"files",mkfile);
    nA->appendMap_Str_Str(send,"path",dirpath);
    ret = NSPService("nsp.vfs.mkfile",*send);

    if(ret!=NULL){
        nA->getArrayNodeSize(failfiles);
        nsp_node *tmpnode = nA->searchNodeByStr(ret,"failList");
        if(tmpnode!=NULL && tmpnode->node_data.child!=NULL && tmpnode->node_data.child->key==NULL)
        {
            free(tmpnode->node_data.child);tmpnode->node_data.child=NULL;
        }
        valuemap *vm=failfiles->node_data.child;
        while(vm!=NULL)
        {
            nA->appendArray(tmpnode,vm->value);
            vm=vm->next;
        }
        free(failfiles);
    }

    nA->deleteNode(send);

finish_upload:
    free(mkfile);mkfile=NULL;
    Result *res = NSPWrapper::convert_to_result(nA,ret);
    nA->deleteNode(ret);
    NSPActionDelete(nA);
    return res;
}

Result* UploadProgress(char *dirpath,char *filename,void *callfunc){
    
    nsplog("调用上传回调服务",filename);
    nspAction *nA=NSPAction();
    if(nA==NULL){return NULL;}

    nsp_node *send=nA->createArray();
    nsp_node *info;

    int successpos=0,failpos=0;

    char *mkfile = (char *)malloc(1024);//assume average path of each file has 1024 characters
    strcpy(mkfile,"[");

    //fetch vfs.upauth information
    if((info=NSPService("nsp.vfs.upauth",*send))==NULL){
        return NULL;
    }

    //record files which failed to upload
    nsp_node *failfiles = nA->createArray();
    if(failfiles==NULL){free(mkfile);nA->deleteNode(info);nA->deleteNode(send);NSPActionDelete(nA);return NULL;}

    {
            nsp_node *recv=nA->createArray();
            if(recv==NULL){free(mkfile);nA->deleteNode(failfiles);nA->deleteNode(info);nA->deleteNode(send);NSPActionDelete(nA);return NULL;}

            char mkf[1024];
            //begin to upload
            if(!upload_file(dirpath,filename,*info,*recv,false,callfunc)){
                nA->deleteNode(recv);
                nsp_node *failfile = nA->createArray();
                if(!failfile){
                    free(mkfile);nA->deleteNode(failfiles);nA->deleteNode(info);nA->deleteNode(send);NSPActionDelete(nA);
                    return NULL;
                }

                nA->appendMap_Str_Str(failfile,"name",filename);
                nA->appendMap_Str_Int(failfile,"errCode",999);
                nA->appendMap_Str_Str(failfile,"errMsg","failed to upload file");

                nA->appendArray(failfiles,failfile);
            }
			else{
            //begin to strcat mkfile string

            char fid[33]="\0";
            char path[256]="\0";
            char size[33]="\0";
            char sig[33]="\0";
            int ts=0;
            nsp_node *nsp_search = nA->searchNodeByStr(recv,"nsp_fid");
            if(nsp_search!=NULL){strcpy(fid,nsp_search->node_data.valuestring);}
            nsp_search = nA->searchNodeByStr(recv,"nsp_path");
            if(nsp_search!=NULL){strcpy(path,nsp_search->node_data.valuestring);}
            nsp_search = nA->searchNodeByStr(recv,"nsp_size");
			if(nsp_search!=NULL){strcpy(size,nsp_search->node_data.valuestring);}
            nsp_search = nA->searchNodeByStr(recv,"nsp_sig");
			if(nsp_search!=NULL){strcpy(sig,nsp_search->node_data.valuestring);}
            nsp_search = nA->searchNodeByStr(recv,"nsp_ts");
			if(nsp_search!=NULL){ts=nsp_search->node_data.valueint;}

            sprintf(mkf,"{\"name\":\"%s\",\"md5\":\"%s\",\"type\":\"File\",\"url\":\"%s\",\"size\":\"%s\",\"sig\":\"%s\",\"ts\":%d},",
                    (strrchr(filename,'/')+1),
                    fid,
                    path,
                    size,
                    sig,
                    ts);
            strcat(mkfile,mkf);

            nA->deleteNode(recv);
			}
    }
    nA->deleteNode(info);
    nA->deleteNode(send);

	nsp_node *ret=NULL;
    if(strlen(mkfile)==1){
        ret = nA->createArray();
        if(ret!=NULL){
            nA->appendMap_Str_Node(ret,"successList",nA->createArray());
            nA->appendMap_Str_Node(ret,"failList",failfiles);
        }

        goto finish_upload_pro;
    }
    mkfile[strlen(mkfile)-1]=']';

    //begin to mkfile

    send=nA->createArray();
    nA->appendMap_Str_Str(send,"files",mkfile);
    nA->appendMap_Str_Str(send,"path",dirpath);
    ret = NSPService("nsp.vfs.mkfile",*send);

    nA->deleteNode(send);

finish_upload_pro:
    free(mkfile);mkfile=NULL;
    Result *res = NSPWrapper::convert_to_result(nA,ret);
    nA->deleteNode(ret);
    NSPActionDelete(nA);
    return res;
}
