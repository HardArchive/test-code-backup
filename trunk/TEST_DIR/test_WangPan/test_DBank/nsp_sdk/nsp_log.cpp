#include<fstream>
#include<stdio.h>
#include<time.h>
#include"NSPClient.h"

void nsplog(char *logtitle,char *detail){
#ifdef NSP_LOG
    FILE *fp;
    if((fp=fopen("nsp_sdk.log","a"))!=NULL){
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo=localtime(&rawtime);
		fprintf(fp,"# %d-%02d-%02d %02d:%02d:%02d # %s %s\n",(1900+timeinfo->tm_year),(1+timeinfo->tm_mon),
            timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec,logtitle,detail);
        fclose(fp);
    }
#endif
}
