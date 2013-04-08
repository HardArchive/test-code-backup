#ifndef _COMM_PROTOCOL_h_
#define _COMM_PROTOCOL_h_
/******************************************************************************* 
* 1�� Class      �� comm_protocol
* 2�� Version    �� 1.00
* 3�� Description�� CultureBJͨѶЭ��
* 4�� Author     �� 
* 5�� Created    �� 2012-11-20 17:14:30
* 6�� History    �� 1.00					
* 7�� Remark     �� 
********************************************************************************/ 

//����
#define PACKAGE_MARK                     "RS01"        //���ݰ���־
#define PACKAGE_MARK_LEN                 4             //��ǳ���
#define PROTOCOL_VERSION                 0x0001       //Э��汾��
//���������¼��ּ�������� ����ͷ+ ��������                         ÿ��������                   ͬʱ������(MAX)      ����
//1������(0)-��֤(16)-��������(8)-����ظ�(0)                      (12960��+��������15000��)            1             64
//2����Ӫ��������(128)Զ�̿�������(460)���ɽ�����Ϣ(140)            (����)                              1             512
//3������(328)����������Ϣ(312)���»���־(524)                     (150��+δ֪ ���ܺܶ� Ҳ����û��)   0-1500-40 0000   1024
//4���û���Ϣ(424ImageLen[200*1024])                              (50��)                           0-1500           256*1024
//5������(8+ImageLen[1024*1024])                                 (����^�����ɺ���)                   0-1             1024*1024
#define RECVICE_DATA_MAX_LEN             1024         //����������󳤶�1K ���������ֵ� �ֱ�*[(1-3)*1��(4)*256��(5)*1024]
#define POLICY_BUFSIZE_PERIOD            128*4        //ʱ�β��Ի�������󳤶� 4��
#define POLICY_BUFSIZE_HTTP              256*1024*8   //URL���Ի�������󳤶� 1024*8��
#define POLICY_BUFSIZE_GAME              1024*1024*4  //��Ϸ���Ի�������󳤶� 1024*4��
//��������
#define TYPE_DATA_INT                    0x01       //int��
#define TYPE_DATA_DATE                   0x02       //DATE
#define TYPE_DATA_CHAR_ARRPAY            0x03       //char����
#define TYPE_DATA_BYTE                   0x04       //BYTE
//��������
#define TYPE_POLICY_GAME                 0x01       //��Ϸ����
#define TPYE_POLICY_HTTP                 0x02       //URL����
#define TYPE_POLICY_PERIOD               0x03       //ʱ�β���


//���ݰ�����
#define TYPE_OK        0x01     //�ظ�ȷ�ϣ���ʾ�Ѿ����ջ���ɹ�
#define TYPE_ERROR     0x02     //�ظ�ȷ�ϣ���ʾ���ջ���ʧ��

#define TIMEOUT_MINUTE   0x01   //��ʱʱ�� ����Ϊ1���ӣ���ʽΪ10����

//ǰ�����̨ͨ����Ū
#define TYPE_PACKET_VERIFY                0xA0        //��֤������
//ǰ����ͻ���ͨ�Ű�����
#define TYPE_PACKET_HEARTBEAT             0xA1        //����������
#define TYPE_PACKET_REPLAY_COMMAND        0xA2        //����������ظ�  ��ʾ�Ѿ��յ����������·�  ������󸽴� ������ظ�
#define TYPE_PACKET_REQUEST_POLICY        0xA3        //�������
#define TYPE_PACKET_ISSUED_POLICY         0xA4        //�����·�
//�ͻ������̨ͨ��
#define TYPE_PACKET_POLICY_ALERT          0xA5        //����
#define TYPE_PACKET_SCREENSHOT     	      0xA6        //����


//ǰ����Ʒ�ͨ��
//#define TYPE_PACKET_REQUEST_BAR_BASE_INFO       0xB0        //���볡��������Ϣ*�Ѿ��ϳ�*
//#define TYPE_PACKET_UPLOAD_BAR_BASE_INFO        0xB1        //�ϴ�����������Ϣ*�Ѿ��ϳ�*
//#define TYPE_PACKET_REQUEST_TERMINAL_INFO       0xB2        //��������������Ϣ*�Ѿ��ϳ�*
//����TCP������ֱ���ϴ��������� 
#define TYPE_PACKET_UPLOAD_ALL_TERMINAL         0xB3        //�ϴ�����������Ϣ
#define TYPE_PACKET_UPLOAD_SINGLE_TERMINAL      0xB4        //�ϴ�����������Ϣ
#define TYPE_PACKET_UPLOAD_USER_INFO            0xB5        //�ϴ��û���Ϣ
#define TYPE_PACKET_UPLOAD_LOGINOUT_LOG         0xB6        //�ϴ��û����»���־
#define TYPE_PACKET_ISSUED_MESSAGE              0xB7        //�·���Ϣ
#define TYPE_PACKET_CONTROL_COMMAND             0xB8        //��Ӫ���������·�
//ǰ����Ʒ�ͨ��
//ʱ������ΪDATA���� COleDateTime  ATL����� MSDN����
//ͨѶЭ��
//########################################################################################

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
		return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	}
	//���ð�ͷ�ṹΪ0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}
}DATAHEAD, *PDATAHEAD;
//const int MESSAGE_HEADER_LEN = sizeof(DATAHEAD);

typedef struct DATA_PACKET_TAG
{
	bool      bIsFill;                         //�Ƿ��Ѿ�����
	DWORD     dwCurrentSize;                   //��ǰszbyData����Ч���ݳ���	
	DWORD     dwBufLen;                        //Bufferʵ�ʳ���
	PDATAHEAD pstuDataHead;                    //����ͷ
	BYTE      szbyData[RECVICE_DATA_MAX_LEN];  //������Ű�ͷ�Ͱ��� ���ֻ�ܴ��һ����
	DATA_PACKET_TAG()
	{
		Reset();
	}
	~DATA_PACKET_TAG()
	{
		Reset();
	}

	inline void Init()
	{
		memset(this, 0, sizeof(*this));
		dwBufLen = RECVICE_DATA_MAX_LEN;       //��ȥͷ֮���ʵ�ʳ���
		pstuDataHead = (PDATAHEAD)szbyData;		
		memcpy(pstuDataHead->szHeadFlag, PACKAGE_MARK, 4);
		pstuDataHead->dwVersion = 1;
	}

	inline void Init(DWORD dwInBufLen)
	{
		memset(szbyData, 0, dwBufLen);
		memset(this, 0, sizeof(*this));
		dwBufLen = dwInBufLen-16;       //��ȥͷ֮���ʵ�ʳ���
		pstuDataHead = (PDATAHEAD)szbyData;		
		memcpy(pstuDataHead->szHeadFlag, PACKAGE_MARK, 4);
		pstuDataHead->dwVersion = 1;		
	}

	//�������ݰ�����Ϊ0
	inline void Reset()
	{
		memset(szbyData, 0, dwBufLen);
		memset(this, 0, sizeof(*this));		
		pstuDataHead = (PDATAHEAD)szbyData;		
	}

	//��������Ƿ��Ѿ�������� true �Ѿ�������ɣ� false δ��������
	inline bool CheckRecvEnd()
	{
		return dwCurrentSize == pstuDataHead->dwPacketLen;
	}

	//�ý������ݽ�szbyData���Ϊһ�������ݰ� ����ʵ��������ݳ��� ʧ�ܷ��ظ�ֵ
	//-1 ����������� -2 �ռ䲻�� 0 δ�������  ������ ���ر������ӵ����ݳ���
	int AddRecvData(PBYTE pbtAddRecvData, int iAddRecvDataLen)
	{
		DWORD dwCopyLen = 0;       //���β��������ݳ���
		DWORD dwPacketLen = 0;     //Ӧ���յİ�����

		if (!pbtAddRecvData) return -1;
		//�ж��Ƿ�Ϊ��һ�����
		if (!bIsFill)
		{
			dwPacketLen = ((PDATAHEAD)pbtAddRecvData)->dwPacketLen;			  
			bIsFill = true;
		}
		else
		{  //�ٴ����
			dwPacketLen = pstuDataHead->dwPacketLen - dwCurrentSize;
		}

		dwCopyLen = (DWORD)iAddRecvDataLen < dwPacketLen ? iAddRecvDataLen:dwPacketLen;
		if (dwBufLen< dwCurrentSize + dwCopyLen)
		{
			RG::TRACE(_T("DATAPACKET�ռ䲻��!!!!!\r\n"));			
			return -2;
		}

		memcpy(szbyData + dwCurrentSize, pbtAddRecvData, dwCopyLen);
		dwCurrentSize += dwCopyLen;
		if (!CheckRecvEnd()) return 0;
		return dwCopyLen;
	}

}DATAPACKET, *PDATAPACKET;

typedef struct CLIENT_INFO_TAG
{
	int iIsFrontend;           //0��ǰ�� 1����ʱ���Ϊǰ�˳ɹ�(IP��ȷ) 2 ����֤��ʱ���ǰ�˳ɹ�(IP������)
	DWORD dwConnID;            //����ID
	char szBarCode[16];        //�Ļ�����ID
	char szCompanyID[16];      //���ɼƷ�ID
	char szIP[16];             //����IP

	CLIENT_INFO_TAG()
	{
		Reset();
	}
	~CLIENT_INFO_TAG()
	{
		Reset();
	}

	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
		iIsFrontend = 0;
		dwConnID = -1;
	}
}CLIENTINFO, * PCLIENTINFO;
//��֤�� dwTotalLen = HEAD+DATAHEART
typedef struct DATA_VERIFY
{
	char szBarCode[16];        //���ڳ���
}DATAVERIFY, *PDATAVERIFY;


//�·�(�ظ�)���� dwTotalLen = HEAD + COMMONDISSUED
typedef struct COMMOND_ISSUED
{
	int iCommandID;           //������
	int iCommandType;         //��������
	char szBackgroundIP[64];  //��̨ip
	UINT uiBackgroundPort;    //��̨�˿�
	char szClientIP[64];      //�ͻ���IP
}COMMONDISSUED, *PCOMMONDISSUED;



//�������� dwTotalLen = HEAD + POLICYREQ
typedef struct POLICY_REQUEST
{
	int iType;             //��������
	DATE dtTime;           //ʱ���
}POLICYREQ, *PPOLICYREQ;

//���Իظ�acknowledge
typedef struct POLICY_ACK
{
	int iType;               //�������� 
	int iCount;              //����
	DATE dtTime;             //ʱ���
	char szIP[64];           //��̨ip
	UINT uiPort;             //��̨�˿�
	POLICY_ACK()
	{
		Reset();
	}
	~POLICY_ACK()
	{
		Reset();
	}

	//�������ݰ�����Ϊ0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}
}POLICYACK, *PPOLICYACK;

//���ز��Ի�����
typedef struct POLICY_CONTENT
{
	DWORD dwContentLen;        //��Ч���ݳ���
	DWORD dwBufLen;            //Buffer��ʵ�ʳ���
	PPOLICYACK pstuPolicyAck;	
	PBYTE pszbyData;           //�������� ��������ͷ + N���������� �Ӳ���ͷ  ��󳤶�Ϊ128K
	POLICY_CONTENT()
	{
		Reset();
	}
	~POLICY_CONTENT()
	{
		Reset();
	}

	//��ʼ��
	void Init(int iType)
	{		
		dwBufLen = TYPE_POLICY_PERIOD==iType ? POLICY_BUFSIZE_PERIOD:(TPYE_POLICY_HTTP==iType ? POLICY_BUFSIZE_HTTP:POLICY_BUFSIZE_GAME);
		pszbyData = new BYTE[dwBufLen];
		memset(pszbyData, 0, dwBufLen);
		pstuPolicyAck = (PPOLICYACK)pszbyData;
		pstuPolicyAck->iType = iType;
	}

	//�������ݰ�����Ϊ0
	inline void Reset()
	{
		if (pszbyData) {delete pszbyData;}
		pszbyData = NULL;
		pstuPolicyAck = NULL;	
		memset(this, 0, sizeof(*this));		
	}

	inline void RestPolicy()
	{
		memset(pszbyData + sizeof(POLICYACK), 0, dwBufLen - sizeof(POLICYACK));
	}

	//���ʱ��� ��ͬ����true ��ͬ����false
	inline bool CheckdtTime(DATE dtTime)
	{
		return dtTime == pstuPolicyAck->dtTime;
	}
}POLICYCONTENT, *PPOLICYCONTENT;

//��ֹ��Ϸ���� Dic_ForbidedGames
typedef struct POLICY_CONTENT_GAME
{
	int iID;                        //����ID
	char szGameEName[64];           //GameEName
	char szGameCName[64];           //GameCName
	char szProductsByCorp[64];      //��Ʒ��˾
	char szProcessName[64];         //��Ϸ��������
	char szGameType[32];            //��Ϸ���ͣ����� ������
	int iHarmType;                  //��ϷΣ�����ͣ������ԣ�
	char szGameVersion[32];         //�汾��
	char szGameSampling[256];       //��Ϸ����
	char szGameSamplingFile[256];   //�ļ�ȡ��
	DATE dtStartTime;               //��ʼִ��ʱ��	StartDate	datetime
	int iCreator;
	DATE dtCreatTime;               //CreateDate
	int iEditor;
	DATE dtEditDateTime;
	BYTE byIsDeleted;
	BYTE byIsAlarm;               //�Ƿ��ϴ�����
	int iAlarmType;				  //��������		
	char szGameFileName[64];      //��Ϸ�ļ���
}POLICYGAME, *PPOLICYGAME;

//��ֹ��ҳ����
typedef struct POLICY_CONTENT_HTTP
{
	int iID;                        //����ID
	char szSiteName[64];            //SiteName
	char szUrl[32];                 //URL
	int iHarmType;                  //��վΣ�����ͣ������ԣ�
	DATE dtStartTime;               //��ʼִ��ʱ��
	int iCreator;
	DATE dtCreatTime;               //CreateDate
	int iEditor;
	DATE dtEditDateTime;
	BYTE byIsDeleted;
	BYTE byIsAlarm;
	int iAlarmType;				    //��������
	char szSiteType[32];            //��վ���
}POLICYHTTP, *PPOLICYHTTP;

//tcpͨѶ
//������ԣ��ͻ���-->dwTotalLen = HEAD + POLICYREQ -->ǰ��
//�·����ԣ�ǰ��-->HEAD + POLICYACK + POLICYACK::iCount*POLICYGAME-->�ͻ���
//�·����ԣ�ǰ��-->HEAD + POLICYACK + POLICYACK::iCount*POLICYHTTP-->�ͻ���

//��Ӫʱ����� Mgt_BarBusinessTime
typedef struct POLICY_TIME
{
	int iID;                        //����ID
	char szCityID[8];               //�������
	DATE dtStartTime;               //��ʼʱ��
	DATE dtEndTime;                 //����ʱ��
	DATE dtCreatTime;               //CreateDate
	int iCreator;
	DATE dtEditDateTime;
	int iEditor;
	int iIsDeleted;
	int iAlarmType;                //��������
}POLICYTIME, *PPOLICYTIME;

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
	char szAlarmType[4];       //�������
	char szAlarmDesc[64];      //��������
}POLICYALERT, *PPLICYALERT;
//tcpͨѶ
//���ͱ������ͻ���-->HEAD + POLICYALERT-->��̨
//�ظ�ȷ�ϣ���̨-->HEAD-->�ͻ���

//����  ͼ�����ݸ���������
typedef struct SCREEN_SHOT
{
	int iID;//������
	DWORD dwImageLen;     //Image���ݳ���Image���ڱ��ṹ����
	//ץ������	CaptureImg	image			FALSE	FALSE	FALSE
}SCREENSHOT, *PSCREENSHOT;
//tcpͨѶ
//���ͽ������ͻ���-->HEAD + SEREENSHOT-->��̨
//�ظ�ȷ�ϣ���̨-->HEAD-->�ͻ���


//########################################################################################
//��Ӫ����������Ϣ Bar_BaseInfo
typedef struct BAR_BASE_INFO
{
	char szBarCode[16];             //BarCode
	char szBarName[128];            //BarName
	char szBoss[32];                //���˴���
	char szManager[32];             //������
	char szNetAD[32];               //����
	char szBarAddress[128];         //������ַ
	char szManagerTel[32];          //�����˵绰
	char szADTel[32];               //���ܵ绰
	char szBarTel[32];              //���ɹ̶��绰
	char szLicenseNum[16];          //���֤����
	char szIssueOrg[64];            //��������
	DATE dtLicenseExpiredData;      //���֤��Ч��
	int iServerQuantity;            //���ɷ���������
	char szBillingName[64];         //���ɼƷ�ϵͳ
	char szBillingVersion[32];      //�Ʒ�����ͺ�
	int iRealTerminalQuantity;      //ʵ��������
	int iFilingTerminalQuantity;    //����������
	char szEmpQuantity[32];         //������Ա����
	char szBarIP[16];               //IP��ַ	BarIP
	//int iBarStatus;               //Ӫҵ״̬*****************
	DATE dtStartDate;               //��ʼ����ʱ��
	char szProvinceID[16];          //����ʡ��
	char szCityID[16];              //��������
	char szDistrctID[16];           //��������
	DATE dtLastUpdate;              //���ͨѶʱ��**************
	DATE dtCreateDate;              //����ʱ��
	DATE dtEditDate;                //�޸�ʱ��      #############
	int iCreator;                   //�����
	int iEditor;                    //�޸���
	int iIsPermit;                  //���֤״̬
	int iIsOpened;                  //IsOpened
	int iIsOnLine;                  //����״̬ 1/0  ##############
	int iIsInstalled;               //����ϵͳ��װ״̬
}BARBASEINFO, *PBARBASEINFO;
//����������Ϣ����ظ�
typedef struct TERMINAIL_ACK
{
	int iCount;              //��������
}TERMINAILACK, *PTERMINAILACK;
#pragma pack(1)
//��Ӫ����������Ϣ ��������������ϢBar_TerminalInfo delete Insert
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
#pragma pack()
//�����û���Ϣ  �����û���־Cus_CustomerInfo  ֻ�����
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
//�Ʒ�-->HEAD + CUSTOMERINFO + ͼƬ--> ǰ�� 0xB5
#pragma pack(1)
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
#pragma pack()
//��Ӫ�������� Mgt_BarBusinessControl
typedef struct BAR_BUSINESSCONTROL
{
	int iID;                    //����ID
	char szBarCode[16];         //BarCode
	char szBarServerIP[16];     //����IP��ַ
	char szActionType[32];      //�������ͣ�Ϊ��չ��
	int iActionID;              //���Ʊ��
	DATE dtStartDate;           //���ƿ�ʼʱ��
	DATE dtEndDate;             //���ƽ���ʱ��
	int iExeState;              //����״̬������ ������
	DATE dtCreateDate;          //CreateDate
	int iCreator;               //Creator
	int iEditor;                //Editor
	DATE dtEditDate;            //EditDate
	int iIsSuccess;             //���ͳɹ���־
}BARBUSINESSCONTROL, *PBAR_BUSINESSCONTROL;


//�·���Ϣ  Mgt_MessageForBar
typedef struct ISSUED_MESSAGE
{
	int iID;            //��Ϣ���
	char szTitle[128];  //��Ϣ����
	char szURL[256];    //URL
	int iIsUrgency;     //�����̶�
	int iMessageType;   //��Ϣ����
	char szBarIP[64];   //����IP��ַ	BarIP
}ISSUEDMESSAGE, *PISSUEDMESSAGE;
//ǰ��-->HEAD + ISSUEDMESSAGE -->�Ʒ����� 0xB7
/////////////////////////////////////////////////////////////////////////////////////////////
//���ݿ��ֶ���Ϣ
typedef struct FIELD_PARAMETER
{	
	int FiledLen;           //�ֶ����ݳ���
	int iFiledType;         //�ֶ�����
	char* pszFieldName;     //�ֶ���	
	char* pszFieldDesc;     //����
}FIELDPARAMETER, *PFIELDPARAMETER;

//����������
typedef struct COMMOND_RESULE
{
	char szPlaceIP[64];              //����IP
	COMMONDISSUED stuCommondIssued;  //����ṹ
}COMMONDRESULE,* PCOMMONDRESULE;

//extern map<string, string> g_mapCompanyIDBarcode;   //Barcode��Company��Ӧ��ͼ
#endif  