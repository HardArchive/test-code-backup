#pragma once
#include "../resource.h"
#include "ThreadBase.h"
#include "../Dialog/CaptruePackInfoDlg.h"

typedef void (__stdcall *CALLCAPTRUEDLGFUCTION)();
typedef void (__stdcall *CALLLOGINSTATE)(char *pItemName);
typedef void (__stdcall *CALLFINDSTOPCAPTURERULE)();
typedef void (__stdcall *CALLLOGSTOPCAPTRUE)();

class CPackThread: public CThreadBase
{
public:
	CPackThread(void);
	~CPackThread(void);

	virtual  bool   Initialization();
	virtual	 void   WorkThread(LPVOID pParam = NULL);

public:
	void            Close();	
	bool            GetLocalIP();
	char           *FindKey(char *p1,char *p2,int nLength,int &nReturnOffset);          //Xt:���Ұ��йؼ���(����Դ��Ҫ������Ϣ�����ҷ�Χ���ȣ������ҵ������ݵ�ƫ����)                                                                  
    void            FindCode(char *pStrCodeType,char *strSrc,char *pStrDest);           //Xt:����ؼ���

	static          bool   GetAdapterInfo();				                            //Xt:���������Ϣ
	static          bool   SetAdapterInfo(string strAdapterName);                       //Xt:����������Ϣ                    
private:
	bool            OpenPacp();
	int             Decode(pack_info &pi);
	int             DecodeIP( pack_info &pi );
	int             DecodeTcp( pack_info &pi );
	int             DecodeUdp( pack_info &pi );
//rg
 	u_short			Checksum( const u_short* p, const int nLen );
 	u_short			TcpChecksum( const ip_header* p );
//rg
	bool            IsReservationIP( in_addr ip, bool bIsNetStream = true);		

	bool            OnLineCapturePack(int nOpertionType);                                //Xt: ����ץ��(ץ�������ݣ�����Ĺ���)
	bool            OffLineCapturePack(int nOpertionType,const char *pPackPath);         //Xt: ����ץ��
    bool            ResolvePackRun(pack_info *pPackInfo);                    
	bool			GetResolvePackInfo(pack_info *pPackInfo);                            //Xt:(������\������)��������Ϣ
	//Xt:   ���ݰ���Ϣ��������Ϣ���洢������Ϣ
	bool            IsRulePack(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile);                             
	bool            GetPackUserInfo(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile,INTERCEPTEDINFO *pInterceptedInfo);        //Xt:����û���������Ϣ
	bool            GetPackContentInfo(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile,INTERCEPTEDINFO *pInterceptedInfo);     //Xt:��÷���������Ϣ

	//Xt:   ���Ұ�������Ϣ
	bool            ResolvePackRuleRun(pack_info *pPackInfo);
	bool            GetResolvePackRuleInfo(pack_info *pPackInfo);                                                  //Xt:(���߽���)����������
	int             FindPackUserInfo(pack_info *pi,FindInfo *pFindInfo,PackRuleFile *pPackRuleInfo,int nRuleLen);  //Xt:���Ұ�������Ϣ
	bool            FindPackHostInfo(char *pBuffer,PackRuleFile *pPackRuleInfo,int nRuleLen);                      //Xt:��ȡ����,�Ͱ�ͷ��Ϣ
	inline   string	NewGuid()
	{
		int  nLen = 0;
		GUID m_guid; 
		char pOut[40]={0};
		if(S_OK ==::CoCreateGuid(&m_guid))   
		{         
			nLen = ::sprintf_s(pOut, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",\
				m_guid.Data1,  m_guid.Data2,   m_guid.Data3 ,\
				m_guid.Data4[0],   m_guid.Data4[1],\
				m_guid.Data4[2],   m_guid.Data4[3],\
				m_guid.Data4[4],   m_guid.Data4[5],\
				m_guid.Data4[6],   m_guid.Data4[7] );   
		}
		return nLen == 0 ? string("") : string(pOut);
	}

public:
	static char                             m_strLocalIP[16];
	char                                    m_strSaveGetOrPostPackHead[10240];   //Xt:����Get��Post����Ϣ
	pcap_pkthdr                            *m_pheader;						     //Xt:����ʱ�䣬ץ�����ĳ��ȡ�����ʵ�ʳ���
public:
//	static char                             m_pRecvAdapterName[MAX_PATH];        //Xt:��������Ϣ  
	static pcap_t                          *m_pRecvHandle;					     //Xt:������Ϣ   
	static pcap_if_t                       *m_pRecvAdapterInfo;                  //Xt:ѡ���������Ϣ
	static PackRuleFile                     m_PackRuleInfo;                      //Xt:�洢���ػ��Ĺ�����Ϣ
// 	static vector<string>                   m_vectAdapterName;                   //Xt:����������ѡ��
// 	static map<string,pcap_if_t *>          m_mapAdapterInfo;                    //Xt:ͨ�����������������Ϣ
	static multimap<string,INTERCEPTEDINFO> m_MulMapInterceptedInfo;             //Xt:������Ϣ�����ȡ��˿ڡ�IP���ػ�����
public:
	CALLLOGINSTATE	                        calLoginSate;
	CALLCAPTRUEDLGFUCTION                   calAddPackListInfo;                  //Xt:�ص�CaptruePackInfoDlg֪ͨ�����б�
	CALLFINDSTOPCAPTURERULE					callFindDlgStopCaptrue;
	CALLLOGSTOPCAPTRUE						callLoginDlgStopCaptrue;             
};
