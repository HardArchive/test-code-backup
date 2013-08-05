//file protocol.h
/***************************************************************************************************
* 1�� Class      �� comm_define
* 2�� Version    �� *.*
* 3�� Description�� ��������Ĺ���ͷ�ļ�
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-10-21 14:14:03
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__




enum SAVE_TYPE_TAG
{
	sKeyWord1 = 1,         //��������tCompanyLog  sKeyWord1
	sKeyWord2 = 2,         //�������ݿ�tCompanyLog  sKeyWord2
	sMemoChar = 3,         //�������ݿ�tCompanyLog  sMemoChar
	File = 4               //�����ļ�
}SAVETYPE, *PSAVETYPE;


/*
* Encoding type
*/
#define ENCT_URL             0x0001
#define ENCT_GB1212          0x0002
#define ENCT_UTF_8           0x0008
#define ENCT_UNICODE         0x0020

/*
* marktype type  XML�е�marktype���ֵ����  0Ϊ��������  ����Ҫת��
*/
#define MT_START_TAG          0x0001                     //��ǿ�ʼ���Ϊ16�����ַ����� ��ת���ɶ���������
#define MT_END_TAG            0x0002                     //��ǽ������Ϊ16�����ַ����� ��ת���ɶ���������
#define MT_ALL_TAG            0x0008                     //��ǿ�ʼ�ͽ�����Ƕ�Ϊ16�����ַ����� ��ת���ɶ���������
#define MT_START_OFFSET       0x0020                     //��ǿ�ʼ���Ϊƫ����  �����ݰ���ʼ+ƫ����+����(ȡ����������)
//#define CS_CLASSDC            0x0040

//���ұ�����ṹ��  ����ṹ�岻�غ�Э�� ��������Ҫ��ȡ���ݵĹؼ����� ��һ�ֹؼ���һ������
typedef struct MARK_FIND_TAG
{
	int iEncodingType; 	                  //�������� URL GB1212 UTF_8 UNICODE ��Ϊ�е�����ǰ���ǲ�һ�� Coding typeֵ
	int iPacketNum;                       //�������ڵ�ǰ��֮��ڼ�����֮�ڣ� ��Ҫ����������
	int iMarkType;                        //����������� 0Ϊ�����ַ��� 1��ʱ��ʼ���Ϊ16�����ַ�����ת����2�����������  8����������Ƕ���  4���� �ɸ��� marktype type����λ��
	int ibyStartBufLen;                   //���ַ������͵����ݱ����г��� ��ʼ��ǳ���
	int ibyEndBufLen;                     //��������
	char szMarkStart[64];                 //��ʼ���
	char szMarkEnd[16];                   //�������
	SAVE_TYPE_TAG enumSaveType;           //����е����ݱ�������
	MARK_FIND_TAG* pstuMarkFindNext;

	MARK_FIND_TAG()
	{
		memset(this, 0, sizeof(*this));
	}

	~MARK_FIND_TAG()
	{
		Reset();
	}

	//����һ���µı�ǲ��ҽṹ������ʼ��Ϊ0  �ɹ����� ����ָ�� ʧ�ܷ���NULL
	inline MARK_FIND_TAG* NewMarkFind()
	{
		MARK_FIND_TAG* pstuMarkFind = new MARK_FIND_TAG;
		if (pstuMarkFind)
		{
			memset(pstuMarkFind, 0, sizeof(MARK_FIND_TAG));
			return pstuMarkFind;
		}
		return NULL;
	}

	//����ǲ��ҽṹ��������һ���µı�ǽṹ������ʼ��Ϊ0  ʧ�ܻط�false
	inline bool AddMarkFind()
	{
		MARK_FIND_TAG* pstuMarkFind = new MARK_FIND_TAG;
		if (pstuMarkFind)
		{
			memset(pstuMarkFind, 0, sizeof(MARK_FIND_TAG));
			pstuMarkFindNext = pstuMarkFind;
			return true;
		}
		return false;
	}

	//�����ǰ�ṹ������ָ����һ���ṹ����
	inline void ClearNext()
	{
		//�����ǰ�ṹ������ָ����һ���ṹ����
		if (pstuMarkFindNext)
		{
			//�����һ���ṹ����Ľṹ
			while(pstuMarkFindNext->pstuMarkFindNext)
			{
				pstuMarkFindNext->ClearNext();
			}

			memset(pstuMarkFindNext, 0, sizeof(MARK_FIND_TAG));
			delete pstuMarkFindNext;
			pstuMarkFindNext = NULL;
		}
	}

	//���õ�ǰ��㿪ʼ������
	inline void Reset()
	{
		ClearNext();
		memset(this, 0, sizeof(*this));
	}
}MARKFIND, *PMARKFIND;


//�� ���ṹ ����   �ڶ��ֹغ���������  ��http��  �ɼ����ֿ��� 
typedef struct FIND_INFO_TAG
{
	bool bIsExtendFind;           //�Ƿ���Ҫ�������(���ڵڶ���������)  true��  false��	
	int iHeadTagType;             //��ͷ����������� 1Ϊ�ַ��� 2Ϊ16�����ַ��� ��תΪ����������
	int ibyBufLen;                //���ַ������͵����ݱ����г���
	char szHost[128];             //http���������� ������Ϊ��httpЭ��ʱΪ�� ���Ϊ128λ
	char szPacketHeadTag[64];     //��ͷ���  ���Ϊ64λ             ͷ����������ȷ�������ݵ����� 
	PMARKFIND pstuMarkFind;       //���ұ������
	FIND_INFO_TAG* pstuFindInfoNext;

	FIND_INFO_TAG()
	{
		memset(this, 0, sizeof(*this));
	}

	~FIND_INFO_TAG()
	{
		Reset();
	}

	//����ͷ���
	bool CheckPacketHeadTag(BYTE* pInBuffer, int iBufLen)
	{
		if (strlen(szPacketHeadTag)>iBufLen) return false;
		//�������ﻹ��Ҫ���ü���ͷ����

	}	

	//����һ���µı�ǲ��ҽṹ������ʼ��Ϊ0  �ɹ����� ����ָ�� ʧ�ܷ���NULL
	inline FIND_INFO_TAG* NewFindInfo()
	{
		FIND_INFO_TAG* pstuFindInfo = new FIND_INFO_TAG;
		if (pstuFindInfo)
		{
			memset(pstuFindInfo, 0, sizeof(FIND_INFO_TAG));
			return pstuFindInfo;
		}
		return NULL;
	}

	//����ǲ��ҽṹ��������һ���µı�ǽṹ������ʼ��Ϊ0  ʧ�ܻط�false
	inline bool AddFindInfo()
	{
		FIND_INFO_TAG* pstuFindInfo = new FIND_INFO_TAG;
		if (pstuFindInfo)
		{
			memset(pstuFindInfo, 0, sizeof(FIND_INFO_TAG));
			pstuFindInfoNext = pstuFindInfo;
			return true;
		}
		return false;
	}

	//�����ǰ�ṹ������ָ����һ���ṹ����
	inline void ClearNext()
	{
		//������ṹ��ָ����һ���ṹ����
		if (pstuFindInfoNext)
		{
			//�����һ���ṹ����Ľṹ
			while(pstuFindInfoNext->pstuFindInfoNext)
			{
				pstuFindInfoNext->ClearNext();
			}

			//����ղ��ұ������
			if (pstuFindInfoNext->pstuMarkFind)
			{
				pstuFindInfoNext->pstuMarkFind->Reset();
				delete pstuFindInfoNext->pstuMarkFind;
				pstuFindInfoNext->pstuMarkFind = NULL;
			}

			//����հ� ���ṹ ����
			memset(pstuFindInfoNext, 0, sizeof(FIND_INFO_TAG));
			delete pstuFindInfoNext;
			pstuFindInfoNext = NULL;
		}
	}

	//���õ�ǰ��㿪ʼ������
	inline void Reset()
	{		
		ClearNext();
		//��ղ��ұ������
		if (pstuMarkFind)
		{
			pstuMarkFind->Reset();
			delete pstuMarkFind;
			pstuMarkFind = NULL;
		}
		//��հ����ṹ����
		memset(this, 0, sizeof(*this));
	}
}FINDINFO, *PFINDINFO;

//������ṹ����  ����ṹֻ�غ�Э��
typedef struct SERVICES_FIND_INFO_TAG
{
	int iServicesType;                                                     //��������,��enum ApplicationProtocolType���Ӧ  eg baidu
	PFINDINFO pstuFindInfo;                                                //��ͷ���ṹ ��
	SERVICES_FIND_INFO_TAG* pstuServicesFindInfoNext;                      //

	SERVICES_FIND_INFO_TAG()
	{
		memset(this, 0, sizeof(*this));
	}

	~SERVICES_FIND_INFO_TAG()
	{
		Reset();
	}

	//����һ���µı�ǲ��ҽṹ������ʼ��Ϊ0  �ɹ����� ����ָ�� ʧ�ܷ���NULL
	inline SERVICES_FIND_INFO_TAG* NewServicesFindInfo()
	{
		SERVICES_FIND_INFO_TAG* pstuServicesFindInfo = new SERVICES_FIND_INFO_TAG;
		if (pstuServicesFindInfo)
		{
			memset(pstuServicesFindInfo, 0, sizeof(SERVICES_FIND_INFO_TAG));
			//���� 
			if (!(pstuServicesFindInfo->pstuFindInfo = pstuServicesFindInfo->pstuFindInfo->NewFindInfo()))
			{
				Reset();
				pstuServicesFindInfo = NULL;
			}
			return pstuServicesFindInfo;
		}
		return NULL;
	}

	//����ǲ��ҽṹ��������һ���µı�ǽṹ������ʼ��Ϊ0  ʧ�ܻط�false
	inline bool AddServicesFindInfo()
	{
		SERVICES_FIND_INFO_TAG* pstuServicesFindInfo = new SERVICES_FIND_INFO_TAG;
		if (pstuServicesFindInfo)
		{
			memset(pstuServicesFindInfo, 0, sizeof(SERVICES_FIND_INFO_TAG));
			pstuServicesFindInfoNext = pstuServicesFindInfo;
			return true;
		}
		return false;
	}

	//�����ǰ�ṹ������ָ����һ���ṹ����
	inline void ClearNext()
	{
		//�����ǰ�ṹ������ָ����һ���ṹ����
		if (pstuServicesFindInfoNext)
		{
			//�����һ���ṹ����Ľṹ
			while(pstuServicesFindInfoNext->pstuServicesFindInfoNext)
			{
				pstuServicesFindInfoNext->ClearNext();
			}

			//����ղ��ұ������
			if (pstuServicesFindInfoNext->pstuFindInfo)
			{
				pstuServicesFindInfoNext->pstuFindInfo->Reset();
				delete pstuServicesFindInfoNext->pstuFindInfo;
				pstuServicesFindInfoNext->pstuFindInfo = NULL;
			}

			memset(pstuServicesFindInfoNext, 0, sizeof(SERVICES_FIND_INFO_TAG));
			delete pstuServicesFindInfoNext;
			pstuServicesFindInfoNext = NULL;
		}
	}

	//���õ�ǰ��㿪ʼ������
	inline void Reset()
	{
		ClearNext();
		if (pstuFindInfo)
		{
			pstuFindInfo->Reset();
			delete pstuFindInfo;
			pstuFindInfo = NULL;
		}
		memset(this, 0, sizeof(*this));
	}
}SERVICESFINDINFO, *PSERVICESFINDINFO;

//�����XMLû�� ���ǿ��Ը��ݳ��ֵ�Ƶ�ʵ���������˳��
typedef struct FILTER_FIND_INFO_TAG
{
	int iFilterType;                        //ɸѡ���� eg http  Smtp
	PSERVICESFINDINFO pstuServicesFindInfo; //���¶�Ӧ���ַ��� eg baidu mop 163 126
}FILTERFINDINFO, *PFILTERFINDINFO;
#endif /*__PROTOCOL_H__*/