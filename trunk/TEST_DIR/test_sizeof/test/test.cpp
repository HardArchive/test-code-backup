// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
typedef struct DomainInfoTag
{
	DWORD dwIPId;//IP�ε�IDֵ ���Ψһһ��IP��
	DWORD dwIP;//IPֵ
	//������Ϣ ���Ը���ʵ�������ĸ����������ȶ���˻�����,������Ϣ�Ĵ�Ÿ�ʽΪ ��������:����
	//��������ռһ���ֽ�,�������������ȵ���������Ϣ Ȼ������һ���������Ⱥ�������Ϣ ���˸�ʽһֱ����ȥ
	char buff[29];  //���buff���Ȳ��̶�
	WORD AA;

}DomainInfo;

typedef struct BAR_TERMINAIL_INFO
{
	int iID;                              //����ID
	char szBarCode[16];                   //�������
	char szHostIP[16];                    //����IP
	char szHostMac[32];                   //����MAC
	char szHostName[32];                  //��������
	char szHostNum[32];                   //�������
	int iIsCamra;                         //��Ƶ״̬
	int iCurrentStatus;                   //����״̬
	DATE dtLastUpdate;  
};
int _tmain(int argc, _TCHAR* argv[])
{
	char szbuf[1024] = {0};
	int iLen1 = sizeof(DWORD);
	int iLen3 = sizeof(WORD);
	int iLen2 = sizeof(DomainInfo);
	int iLen4 = sizeof(BAR_TERMINAIL_INFO);

	struct BAR_TERMINAIL_INFO aa = {0};
	memset(&aa, 0xaa, sizeof(aa));
	memset(&aa.dtLastUpdate, 0xff, 8);

	DomainInfo* pDomainInfo = (DomainInfo*)szbuf;

	pDomainInfo->dwIPId = 100;
	pDomainInfo->dwIP = 115;

	strcpy(pDomainInfo->buff, "asdgkladfjghkladfjgkldfgorthdfkalgafklgkldfgkldf");
	strcpy(pDomainInfo->buff, "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");

	//��Ч����
	iLen3 = sizeof(DomainInfo)-32 + strlen("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");



	return 0;
}

