// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>
//#include <stdio.h>
void deleteQ()
{
	char kendi_ismi[MAX_PATH];
	char kendi_ismi2[MAX_PATH];
	char*ptr;
	char basename[126]; 
	char r ='"';
	char k='%';
	GetModuleFileName(NULL, kendi_ismi, MAX_PATH);

	strcpy(kendi_ismi2,kendi_ismi); 
	ptr = strrchr(kendi_ismi2, '\\'); 
	FILE *di; 
	if((di=fopen(strcat(kendi_ismi,".bat"),"w")) == NULL) 
	{ 
		return; 
	} 

	if(ptr != NULL) 
		strcpy(basename,ptr+1); 
	printf("%s",basename); 

	fprintf(di,":1\n"); 
	fprintf(di,"taskkill /F /IM %s\n",basename); 
	fprintf(di,"del %c%s%c\n",r,kendi_ismi2,r); 
	fprintf(di,"if exist %c%s%c goto 1\n",r,kendi_ismi2,r); 
	fprintf(di,"del %c0\n",k); 
	fclose(di); 
	WinExec(kendi_ismi, SW_HIDE); 
}

int main()
{
	int c;
	printf("enter:1\n");
	scanf("%d", &c);
	if(c==1)
	{
		deleteQ();
	}
	system("pause");
}
