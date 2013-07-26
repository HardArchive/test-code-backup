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
//����
#define PACKAGE_MARK                     "RS01"        //���ݰ���־
#define PACKAGE_MARK_LEN                 4             //��ǳ���
#define PROTOCOL_VERSION                 0x0001       //Э��汾��
#define RECVICE_DATA_MAX_LEN             1024         //����������󳤶�1K
//���ݰ�����
#define TYPE_OK        0x01     //�ظ�ȷ�ϣ���ʾ�Ѿ����ջ���ɹ�
#define TYPE_ERROR     0x02     //�ظ�ȷ�ϣ���ʾ���ջ���ʧ��

//��������
#define COMMAND_START                     0x01        //��ʼ����
#define COMMAND_STOP                      0x02        //ֹͣ����
#define COMMAND_PAUSE                     0x03        //��ͣ����
#define COMMAND_CONTINUE                  0x04        //��������
#define COMMAND_EXIT                      0x05        //�˳�����

//��Ϣ����
#define TYPE_PACKET_PATH                  0xA0        //��������ַ�·�
#define TYPE_PACKET_COMMAND               0xA1        //������·�
#define TYPE_PACKET_REPLAY_COMMAND        0xA2        //���������ظ�  ��ʾ�Ѿ��յ����������·� 
#define TYPE_PACKET_MEMSHARE              0xA3        //�ļ�ӳ����Ϣ
#define TYPE_PACKET_REPLAY_MEMSHARE       0xA4        //�ļ�ӳ����Ϣ�ظ�  ֻ�轫ԭ����ͷ���ؼ���


// ��Ϣͷ �ַ�Э��ͷ
typedef struct DATA_HEAD_TAG
{
	char  szHeadFlag[4];    // ͷ��ʶ ��RG01��
	DWORD dwVersion;        // �汾��  0x0001
	DWORD dwSerialNo;       // ��ˮ��  ��1��ʼʹ�ã��������ֵ���ٴ�1��ʼ��ѭ��ʹ��]
	DWORD dwPacketType;     // ����������  ���λΪ1��Ϊ�ظ����ݡ�
	DWORD dwPacketLen;      //sizeof(HEADTAG) + ����
	DWORD dwReturn;     	//����ֵ  �ɹ���ʧ��
	WORD  wHeadLen;         //����ͷ����

	//�������ͷ�Ƿ����� ��ͷ��������true����false
	inline bool Check()
	{
		if (dwPacketType<0xA0 || dwPacketType>0xA2)
		{
			//TRACE(_T("DATAHEAD Check �����Ͳ���ȷ; dwPacketType:0x%X!!!!!\r\n"), dwPacketType);
			return false;
		}
		if (PROTOCOL_VERSION != dwVersion) return false;
		if (wHeadLen != sizeof(DATAHEAD)) return false;
		//if (dwPacketLen >sizeof(DATAHEAD)+sizeof(MEMSHAREINFO)) return false;
		return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	}

	//���ð�ͷ�ṹΪ0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}

	inline void Init()
	{
		Reset();
		dwVersion = PROTOCOL_VERSION;
		wHeadLen = sizeof(DATAHEAD);
		memcmp(szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN);
	}
}DATAHEAD, *PDATAHEAD;

//������
typedef struct DATA_PACKET_TAG
{
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

	//�������ݰ�����Ϊ0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));	
		memset(szbyData, 0, dwBufLen);	
		pstuDataHead = (PDATAHEAD)szbyData;	
		pstuDataHead->Init();
		dwBufLen += sizeof(DATAHEAD);
	}
}DATAPACKET, *PDATAPACKET;
//������������Ϣ

//·����Ϣ
typedef struct PATH_INFO_TAG
{
	TCHAR tszSourcePath[MAX_PATH];        //Դ�ļ�Ŀ¼
	//TCHAR tszTargetPath[MAX_PATH];        //Ŀ��Ŀ¼	
	//TCHAR tszControlEventName[MAX_PATH];  //�����¼���   
}PATHINFO, *PPATHINFO;

typedef struct COMMAND_TAG
{
	int iCommand;
}COMMAND, *PCOMMAND;


//�ͻ��˷�����Ϣ

//�ļ���Ϣ  ע�⵱ΪĿ¼��Ϣʱ��ֻ��tszFilePath��ֵ������Ϊ0��
typedef struct FILE_INFO_TAG
{
	//PVOID pPoint;                   //ָ���ַ
	bool bIsFile;                     //�Ƿ�Ϊ�ļ�  trueΪ�ļ�
	DWORD dwAddrLen;                  //��ַָ�����ݳ���
	//DWORD dwFileLen;                //�ļ�����
	DWORD dwFileAttributes;         
	//TCHAR tszFileName[MAX_PATH];    //�ļ���
	TCHAR tszFilePath[MAX_PATH];      //�ļ�·�� �����ļ�������Ŀ¼·�� eg:(PATH����Ŀ¼)\DecryptionDLP\DecryptionDLP.vcproj

	inline void Reset()
	{
		bIsFile = false;
		dwAddrLen = 0;
		dwFileAttributes = 0;
		memset(tszFilePath, 0, sizeof(TCHAR)*MAX_PATH);
	}

	//�ļ�����true Ŀ¼���� false
	inline bool IsFile()
	{
		return bIsFile;
	}
}FILEINFO, *PFILEINFO;

//�ڴ�ӳ����Ϣ  ��ΪĿ¼ʱtszFileMapName tszEventNameΪ��
typedef struct MEM_SHARE_INFO_TAG
{
	FILEINFO stuFileInfo;           //�ļ���Ϣ
	TCHAR tszFileMapName[MAX_PATH]; //�ļ�ӳ�������
	TCHAR tszEventName[MAX_PATH];   //�ȴ��¼� �ļ�������Ͻ��¼���Ϊ���ź� DLL�߳���ִ��������

	inline void Reset()
	{
		stuFileInfo.Reset();
		memset(tszFileMapName, 0, sizeof(TCHAR)*MAX_PATH);
		memset(tszEventName, 0, sizeof(TCHAR)*MAX_PATH);
	}
}MEMSHAREINFO, *PMEMSHAREINFO;


#endif /*__COMM_PROTOCOL_H__*/  