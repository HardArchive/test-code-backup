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
#define RECVICE_DATA_MAX_LEN             1024         //����������󳤶�1K ���������ֵ� �ֱ�*[(1-3)*1��(4)*256��(5)*1024]
//���ݰ�����
#define TYPE_OK        0x01     //�ظ�ȷ�ϣ���ʾ�Ѿ����ջ���ɹ�
#define TYPE_ERROR     0x02     //�ظ�ȷ�ϣ���ʾ���ջ���ʧ��

#define TIMEOUT_MINUTE   0x01   //��ʱʱ�� ����Ϊ1���ӣ���ʽΪ10����

extern bool g_bIsRujia;

//ʱ������ΪDATA���� COleDateTime  ATL����� MSDN����
//ͨѶЭ��
//########################################################################################
#pragma pack(1)
// ��Ϣͷ �ַ�Э��ͷ
typedef struct DATA_HEADTAG
{
	char  szHeadFlag[4];    // ͷ��ʶ ��RS01��
	DWORD dwSerialNo;       // ��ˮ��  ��1��ʼʹ�ã��������ֵ���ٴ�1��ʼ��ѭ��ʹ��]
	DWORD dwPacketLen;      // ��ͷ+����(XML�ļ�����)
	DWORD dwReturn;     	// ����ֵ  �ɹ���ʧ��
	//�������ͷ�Ƿ����� ��ͷ��������true����false
	inline bool Check()
	{
		//if (g_bIsRujia)	NtohlEx();
		if (dwPacketLen >50*1024*1024) return false;
		return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	}
  //���ð�ͷ�ṹΪ0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}

	////��unsigned long���ʹ������ֽ�˳��ת���������ֽ�˳��
	//inline void NtohlEx()
	//{
	//	dwSerialNo = ntohl(dwSerialNo); 
	//	dwPacketLen = ntohl(dwPacketLen);
	//	dwReturn = ntohl(dwReturn);
	//}

	////��unsigned long���ʹ������ֽ�˳��ת���������ֽ�˳��
	//inline void HtonlEx()
	//{
	//	dwSerialNo = htonl(dwSerialNo); 
	//	dwPacketLen = htonl(dwPacketLen);
	//	dwReturn = htonl(dwReturn);
	//}
}DATAHEAD, *PDATAHEAD;
//const int MESSAGE_HEADER_LEN = sizeof(DATAHEAD);

typedef struct DATA_PACKET_TAG
{
	bool      bIsFill;                         //�Ƿ��Ѿ����� false��һ��  true �ǵ�һ��
	DWORD     dwCurrentSize;                   //��ǰszbyData����Ч���ݳ���	
	DWORD     dwBufLen;                        //szbyDataʵ�ʳ���
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

	inline void Init(DWORD dwInBufLen = RECVICE_DATA_MAX_LEN+16)
	{
		memset(this, 0, sizeof(*this));	
		dwBufLen = dwInBufLen-16;       //��ȥͷ֮���ʵ�ʳ���
		memset(szbyData, 0, dwBufLen);
		pstuDataHead = (PDATAHEAD)szbyData;		
		memcpy(pstuDataHead->szHeadFlag, PACKAGE_MARK, 4);	
	}

	//�������ݰ�����Ϊ0
	inline void Reset()
	{
		DWORD dwTem = dwBufLen;
		memset(this, 0, sizeof(*this));	
		dwBufLen = dwTem;
		memset(szbyData, 0, dwBufLen);	
		pstuDataHead = (PDATAHEAD)szbyData;	
		memcpy(pstuDataHead->szHeadFlag, PACKAGE_MARK, 4);	
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
			TRACE(_T("DATAPACKET�ռ䲻��!!!!!\r\n"));			
			return -2;
		}

		memcpy(szbyData + dwCurrentSize, pbtAddRecvData, dwCopyLen);
		dwCurrentSize += dwCopyLen;
		if (!CheckRecvEnd()) return 0;
		return dwCopyLen;
	}

}DATAPACKET, *PDATAPACKET;

typedef struct CLIENT_BASE_INFO_TAG
{   //���� ��������0��ǰ�� 1����ʱ���Ϊǰ�˳ɹ�(IP��ȷ) 2 ����֤��ʱ���ǰ�˳ɹ�(IP������) 
	DWORD dwConnID;            //����ID          .....����ʱ��ֵ
	char szIP[16];             //����IP           .....����ʱ��ֵ

	CLIENT_BASE_INFO_TAG()
	{
		Reset();
	}
	~CLIENT_BASE_INFO_TAG()
	{
		Reset();
	}

public:
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
		dwConnID = -1;
	}
}CLIENTBASEINFO, * PCLIENTBASEINFO;

enum TransmissionProtocolType { Tcp, Udp };
//�û�״̬
enum USER_STATUS
{
	ErrorStatus,  //����
	CheckIn,      //��ס
	CheckOut,     //�˷�
	OnLine,       //����
	OutLine       //����
};

//֤������
enum CARD_TYPE
{
	GID,	//����֤
	ID,		//���֤
	JID,	//����֤
	JLZ,	//����˾���֤
	JZ,		//����
	TXZ,	//ͨ��֤
	VSA,	//����
	XZ,		//����֤
	ZQT		//����֤��
};
////֤������
//enum CARD_TYPE
//{
//	"GID",     //����֤
//	"ID",      //���֤
//	"JID",     //����֤
//	"JLZ",     //����˾���֤
//	"JZ",      //����
//	"TXZ",     //ͨ��֤
//	"VSA",     //����
//	"XZ",      //����֤
//	"ZQT"      //����֤��
//};

//�û�״̬��Ϣ
typedef struct USER_STATUS_INFO
{
	USER_STATUS emUserStatus;          //�û�״̬
	TCHAR tszDescription[128];         //״̬����
	//CARD_TYPE emCardType;            //֤������
	TCHAR tszCardType[8];
	TCHAR tszCardID[128];              //֤������
	TCHAR tszUserName[MAX_PATH];       //�û�����
	TCHAR tszRoomName[8];              //�����
	TCHAR tszIP[32];                   //IP
	TCHAR tszMAC[32];                  //MAC
	DATE dtTime;                     //ʱ��
	//TCHAR tszTime[32];                 //ʱ��

	inline bool Check()
	{
		if (emUserStatus>ZQT || emUserStatus<GID)
		{
			TRACE(_T("�û�״̬��Ϣ��飻�û�״̬����;emUserStatus:%d\r\n"), emUserStatus);
			return false;
		}
		//if (!(_tcslen(tszCardType) && _tcslen(tszCardID) && _tcslen(tszUserName) && _tcslen(tszIP) && _tcslen(tszMAC) && _tcslen(tszTime)))
		//{
		//	TRACE(_T("�û�״̬��Ϣ��飻���ݳ��ȳ���;tszCardType:%d; tszCardID:%d; tszUserName:%d; tszIP:%d; tszMAC:%d; tszTime:%d;\r\n"),
		//		_tcslen(tszCardType),  _tcslen(tszCardID), _tcslen(tszUserName), _tcslen(tszIP), _tcslen(tszMAC), _tcslen(tszTime));
		//	return false;
		//}
		//if (!(!_tcscmp(tszCardType, _T("GID")) || !_tcscmp(tszCardType, _T("ID")) || !_tcscmp(tszCardType, _T("JID"))  || 
		//	!_tcscmp(tszCardType, _T("JLZ")) || !_tcscmp(tszCardType, _T("JZ")) || !_tcscmp(tszCardType, _T("TXZ")) || 
		//	!_tcscmp(tszCardType, _T("VSA"))  || !_tcscmp(tszCardType, _T("XZ")) || !_tcscmp(tszCardType, _T("ZQT"))))
		//{
		//	TRACE(_T("�û�״̬��Ϣ��飻֤�����ͼ�����;tszCardType:%s\r\n"), tszCardType);
		//	return false;
		//}
		//if (iRoomID<0) 
		//{
		//	TRACE(_T("�û�״̬��Ϣ��飻����ż�����;iRoomID:%s\r\n"), iRoomID);
		//	return false;
		//}

		return true;
	}
}USERSTATUSINFO, * PUSERSTATUSINFO;


#define DateTimeToString(dt) COleDateTime(dt).Format(_T("%Y-%m-%d %H:%M:%S"))
#define DateToString(dt) COleDateTime(dt).Format(_T("%Y-%m-%d"))
#define CURRENTDATE COleDateTime::GetCurrentTime().Format(_T("%Y%m%d"))
#define CURRENTTIME COleDateTime::GetCurrentTime().Format(_T("%H%M"))
#define GETCURRENTTIME COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"))
#endif  