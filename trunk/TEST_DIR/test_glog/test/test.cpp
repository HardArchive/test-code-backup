// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#pragma pack(1)
typedef struct CUSTOMER_LIGIN_OUT_LOG
{
	long lID;                   //ID�������ԣ����ĳ����Σ�
	char szBarCode[16];         //�������
	char szCusName[32];         //�û�����
	int iCusGender;             //�û��Ա�
	int iCardType;              //������
	int iCertType;              //֤������
	char szCertNo[32];          //֤������
	DATE dtLoginTime;           //�ϻ�ʱ��
	char szHostIP[16];          //��¼����IP
	char szHostName[32];        //��¼��������
	char szHostMac[32];         //����MAC��ַ
	DATE dtRcvTime;             //����ʱ��
	DATE dtLogoutTime;          //�»�ʱ��
	int iIsUpdate;              //�Ƿ��ϴ�
	char szComment[256];        //��ע
	char szCusBarCode[32];      //�û�����
	char szCusPeople[32];       //�û�����
	CUSTOMER_LIGIN_OUT_LOG()
	{
		Reset();
	}
	~CUSTOMER_LIGIN_OUT_LOG()
	{
		Reset();
	}

	//�������ݰ�����Ϊ0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));	
	}
}CUSTOMERLOGINOUTLOG, *PCUSTOMERLOGINOUTLOG;

typedef struct DATA_HEADTAG
{
	char  szHeadFlag[4];    // ͷ��ʶ ��RS01��
	DWORD dwVersion;        // �汾��  0x0001
	DWORD dwSerialNo;       // ��ˮ��  ��1��ʼʹ�ã��������ֵ���ٴ�1��ʼ��ѭ��ʹ��]
	DWORD dwPacketType;     // ����������  ���λΪ1��Ϊ�ظ����ݡ�
	DWORD dwPacketLen;      //sizeof(HEADTAG) + ����
	DWORD dwReturn;     	//����ֵ  �ɹ���ʧ��
	WORD  wEncode;          // �����㷨[0=δ���ܣ�1=�����ܣ�]	
	WORD  wHeadLen;         // ����ͷ����

	//�������ͷ�Ƿ����� ��ͷ��������true����false
	//inline bool Check()
	//{
	//	return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	//}
	////���ð�ͷ�ṹΪ0
	//inline void Reset()
	//{
	//	memset(this, 0, sizeof(*this));
	//}
}DATAHEAD, *PDATAHEAD;
#pragma pack()
int _tmain(int argc, _TCHAR* argv[])
{
	size_t szLen = sizeof(CUSTOMERLOGINOUTLOG);
	size_t szLen2 = sizeof(DATAHEAD);

	char szTem[1024] = {0};
	//CUSTOMERLOGINOUTLOG stuCustomerLoginoutLog = {0};  //�û���Ϣ��
	//sprintf_s(szTem, 1024, "%l\a %s\a %s\a  %d\a%d\a%d\a  %s\a%s\a%s\a%s\a%s\a%s\a %s\a  %d\a %s\a%s\a%s\a\r\n",
	//	stuCustomerLoginoutLog.lID,
	//	stuCustomerLoginoutLog.szBarCode,
	//	stuCustomerLoginoutLog.szCusName,
	//	stuCustomerLoginoutLog.iCusGender,
	//	stuCustomerLoginoutLog.iCardType,
	//	stuCustomerLoginoutLog.iCertType,
	//	stuCustomerLoginoutLog.szCertNo,
	//	DateTimeToString(stuCustomerLoginoutLog.dtLoginTime),
	//	stuCustomerLoginoutLog.szHostIP,
	//	stuCustomerLoginoutLog.szHostName,
	//	stuCustomerLoginoutLog.szHostMac,
	//	DateTimeToString(stuCustomerLoginoutLog.dtRcvTime),
	//	DateTimeToString(stuCustomerLoginoutLog.dtLogoutTime),
	//	stuCustomerLoginoutLog.iIsUpdate,
	//	stuCustomerLoginoutLog.szComment,
	//	stuCustomerLoginoutLog.szCusBarCode,
	//	stuCustomerLoginoutLog.szCusPeople
	//	);
	return 0;
}

//
//
//#include <glog/logging.h>
//#include <gflags/gflags.h>
//
//DEFINE_int32(port, 0 ,"what port to listen on"); # ʵ�ʿ���ʹ��FLAGS_port 
//DEFINE_string(log_dir, "./", "path");#ʵ�ʿ���ʹ��FLAGS_log_dir
//
//int main(int argc, char* argv[]) 
//{
//	//Initialize Google's logging library.
//	google::InitGoogleLogging(argv[0]);
//	int num_cookies = 10;
//	LOG(INFO) << "Found " << num_cookies << " cookies";
//	return 0;
//}