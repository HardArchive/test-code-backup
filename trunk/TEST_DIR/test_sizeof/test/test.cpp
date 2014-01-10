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
	DATE dtLastUpdate;                    //LastUpdate
	char szDistrictID[8];                 //���ر��  - �鳡�����
	char szCityID[8];                     //���б��
	char szProvinceID[8];                 //ʡ�ݱ��
	char szClientVersion[32];             //�ͻ��˼ƷѰ汾��
	char szBarCusName[32];                //�û�����
	char szBarCusCertNO[32];              //�û�֤����
	int iBarCusCertType;                  //�û�֤�����
	char szBarUserNO[32];                 //�û�����
	DATE dtBarCusLoginTime;               //�ϻ�ʱ��
}BARTERMINALINFO, *PBARTERMINALINFO;
#define PACKAGE_MARK                     "RS01"        //���ݰ���־
#define PACKAGE_MARK_LEN                 4             //��ǳ���
#define PROTOCOL_VERSION                 0x0001       //Э��汾��

// ��Ϣͷ �ַ�Э��ͷ
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
	inline bool Check()
	{
		if (dwPacketType<0xA0 || dwPacketType>0xB8)
		{
			//TRACE(_T("DATAHEAD Check �����Ͳ���ȷ; dwPacketType:0x%X!!!!!\r\n"), dwPacketType);
			return false;
		}
		if (PROTOCOL_VERSION != dwVersion) return false;
		if (wHeadLen != sizeof(DATAHEAD)) return false;
		if (dwPacketLen >50*1024*1024) return false;
		return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	}
	//���ð�ͷ�ṹΪ0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}
}DATAHEAD, *PDATAHEAD;
//const int MESSAGE_HEADER_LEN = sizeof(DATAHEAD);


typedef struct CUSTOMER_INFO
{
	char szBarCode[16];       //�������
	char szCusName[32];       //�û�����
	int iCusGender;           //�û��Ա�
	int iCardType;            //�����ͣ���ʱ  ��Ա��
	DATE dtRegDate;           //�Ǽ�ʱ��
	int iCertType;            //֤������
	char szCertNo[32];        //֤������
	char szIssueOffice[64];   //��֤��λ
	char szCountryID[64];     //����
	char szUserBarCode[32];   //�˿��ʺţ����ţ�
	char szCusPeople[32];     //����
	char szCusOfficeName[64]; //��λ����
	char szInfoDesc[64];      //����
	DWORD dwImageLen;         //���֤ɨ��ͼƬ����
}CUSTOMERINFO, *PCUSTOMERINFO;

//�û����»���־ Bar_CustomerLoginOutLog ֻ�����
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

//���Ա��� ������ײ��¼ Mgt_TacticsMatchRecord
typedef struct POLICY_ALERT
{
	//int iID;                    //ID
	int iTacticsType;          //��������
	int TacticsID;             //���Ա��
	char szBarCode[16];        //���ڳ���
	//int iBarHostName;          //��������
	char szBarHostIP[16];      //����IP
	char szCusName[32];        //�û�����
	char szCusCertNO[32];      //�û�֤������	
	char szUserBarCode[32];    //�û�����
	DATE dtCreateDate;  //����ʱ��
	char szBarHostMac[32];     //����MAC
	//ץ������	CaptureImg	image			FALSE	FALSE	FALSE
	char szBarCusNo[16];       //�˿��˺�
	int iCusCertType;          //�˿�֤������
	char szIssueOrg[64];       //��֤����
	char szAlarmType[8];       //�������
	char szAlarmDesc[64];      //��������
}POLICYALERT, *PPLICYALERT;


int _tmain(int argc, _TCHAR* argv[])
{
	char szbuf[1024] = {0};
	int iLen1 = sizeof(DWORD);
	int iLen3 = sizeof(WORD);
	int iLen5 = sizeof(DATAHEAD);
	int iLen2 = sizeof(DomainInfo);
	int iLen4 = sizeof(BAR_TERMINAIL_INFO);
	int iLen6 = sizeof(CUSTOMERINFO);
	int iLen7 = sizeof(CUSTOMERLOGINOUTLOG);
	int iLen8 = sizeof(POLICYALERT);
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

