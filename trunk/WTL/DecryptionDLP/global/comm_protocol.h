//file comm_protocol.h
#ifndef __COMM_PROTOCOL_H__
#define __COMM_PROTOCOL_H__
/***************************************************************************************************
* 1�� File       �� comm_protocol.h
* 2�� Version    �� 1.0
* 3�� Description�� ����ͨѶЭ��--���ܹ���ͨѶЭ��
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-5-2 9:56:34
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
//��ַ��Ϣ
typedef struct ADDR_INFO
{
	PVOID pPoint;           //ָ���ַ
	DWORD dwAddrLen;        //��ַָ�����ݳ���
}ADDRINFO, *PADDRINFO;

//·����Ϣ
typedef struct PATH_INFO
{
	TCHAR tszSourcePath[MAX_PATH];        //ԴĿ¼
	TCHAR tszTargetPath[MAX_PATH];        //Ŀ��Ŀ¼	
	TCHAR tszControlEventName[MAX_PATH];  //�����¼���
}PATHINFO, *PPATHINFO;

//�ļ���Ϣ
typedef struct FILE_INFO
{
	DWORD dwFileLen;                //�ļ�����
	TCHAR tszFileName[MAX_PATH];    //�ļ���
	TCHAR tszFilePath[MAX_PATH];    //�ļ�·�� ��Ŀ¼·��
	//����Ҳ����Ҫ��һ���ļ�����
	TCHAR tszFileMapName[MAX_PATH]; //�ļ�ӳ�������
	TCHAR tszEventName[MAX_PATH];   //�ȴ��¼� �ļ�������Ͻ��¼���Ϊ���ź� DLL�߳���ִ��������
}FILEINFO, *PFILENIFO;


#endif /*__COMM_PROTOCOL_H__*/  