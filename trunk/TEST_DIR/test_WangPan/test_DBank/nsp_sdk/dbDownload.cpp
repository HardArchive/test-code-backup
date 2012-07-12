#include"NSPClient.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void (*calldownload_func)(double,double);
int progress_func(void *callback,double t,double d,double ultotal,double ulnow){
    //call callback function to show download percentage
    calldownload_func = (void (*)(double,double))callback;
    if(t!=0){
        calldownload_func(d,t);
    }
    return 0;
}

bool Download(char *objfile,char *savename,void *callback = NULL){

    nsplog("обтьнд╪Ч",objfile);

    //get url of objfile
    nsp_node *send;
    nsp_node *files,*fields;

    nspAction *nA=NSPAction();
    if(nA==NULL){
        return false;
    }
    send = nA->createArray();
    files = nA->createStringArray(1,objfile);
    fields = nA->createStringArray(1,"url");
    nA->appendArray(send,files);
    nA->appendArray(send,fields);

    nsp_node *recv= NSPService("nsp.vfs.getattr",*send);
    if(recv==NULL){
        nA->deleteNode(send);
        NSPActionDelete(nA);
        return false;
    }

    char url[URL_LENGTH];
    nsp_node *find = nA->searchNodeByStr(recv,"successList");
    if(find!=NULL){
        find = nA->searchNodeByPos(find,0);
        if(find!=NULL){
            find = nA->searchNodeByStr(find,"url");
            strcpy(url,find->node_data.valuestring);
        }
    }

    nA->deleteNode(send);
    nA->deleteNode(recv);
    NSPActionDelete(nA);

    if(find==NULL){
        return false;
    }

    //begin to download
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(savename,"wb");
        if (fp) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS,0L);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
            curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, callback);
            res = curl_easy_perform(curl);
            fclose(fp);
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return true;
}

