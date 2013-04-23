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
	char           *FindKey(char *p1,char *p2,int nLength,int &nReturnOffset);          //Xt:查找包中关键字(数据源、要查找信息，查找范围长度，返回找到与数据的偏移量)                                                                  
    void            FindCode(char *pStrCodeType,char *strSrc,char *pStrDest);           //Xt:解码关键字

	static          bool   GetAdapterInfo();				                            //Xt:获得网卡信息
	static          bool   SetAdapterInfo(string strAdapterName);                       //Xt:设置网卡信息                    
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

	bool            OnLineCapturePack(int nOpertionType);                                //Xt: 在线抓包(抓包的内容，或包的规则)
	bool            OffLineCapturePack(int nOpertionType,const char *pPackPath);         //Xt: 离线抓包
    bool            ResolvePackRun(pack_info *pPackInfo);                    
	bool			GetResolvePackInfo(pack_info *pPackInfo);                            //Xt:(在线下\离线下)解析包信息
	//Xt:   数据包信息、规则信息、存储内容信息
	bool            IsRulePack(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile);                             
	bool            GetPackUserInfo(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile,INTERCEPTEDINFO *pInterceptedInfo);        //Xt:获得用户名密码信息
	bool            GetPackContentInfo(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile,INTERCEPTEDINFO *pInterceptedInfo);     //Xt:获得发帖内容信息

	//Xt:   查找包规则信息
	bool            ResolvePackRuleRun(pack_info *pPackInfo);
	bool            GetResolvePackRuleInfo(pack_info *pPackInfo);                                                  //Xt:(在线解析)解析包规则
	int             FindPackUserInfo(pack_info *pi,FindInfo *pFindInfo,PackRuleFile *pPackRuleInfo,int nRuleLen);  //Xt:查找包规则信息
	bool            FindPackHostInfo(char *pBuffer,PackRuleFile *pPackRuleInfo,int nRuleLen);                      //Xt:获取域名,和包头信息
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
	char                                    m_strSaveGetOrPostPackHead[10240];   //Xt:保存Get或Post包信息
	pcap_pkthdr                            *m_pheader;						     //Xt:包的时间，抓到包的长度、包的实际长度
public:
//	static char                             m_pRecvAdapterName[MAX_PATH];        //Xt:网卡名信息  
	static pcap_t                          *m_pRecvHandle;					     //Xt:网卡信息   
	static pcap_if_t                       *m_pRecvAdapterInfo;                  //Xt:选择的网卡信息
	static PackRuleFile                     m_PackRuleInfo;                      //Xt:存储包截获后的规则信息
// 	static vector<string>                   m_vectAdapterName;                   //Xt:网卡名用于选择
// 	static map<string,pcap_if_t *>          m_mapAdapterInfo;                    //Xt:通过网卡名获得网卡信息
	static multimap<string,INTERCEPTEDINFO> m_MulMapInterceptedInfo;             //Xt:包的信息，长度、端口、IP、截获内容
public:
	CALLLOGINSTATE	                        calLoginSate;
	CALLCAPTRUEDLGFUCTION                   calAddPackListInfo;                  //Xt:回调CaptruePackInfoDlg通知加载列表
	CALLFINDSTOPCAPTURERULE					callFindDlgStopCaptrue;
	CALLLOGSTOPCAPTRUE						callLoginDlgStopCaptrue;             
};
