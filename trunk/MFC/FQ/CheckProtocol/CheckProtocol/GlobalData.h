#pragma once
using namespace std;

class CGlobalData
{
public:
	CGlobalData(void);
	~CGlobalData(void);
	static CGlobalData **GetInstance();
	char *GetProcessPath();                                                                //Xt: ��ý���·��

	bool InitInstance(const char *pFilePath = NULL);                                       //Xt: ��ʼ��
	static bool LoadHttpRuleFile(const char *pFilePath = NULL);		                       //Xt: ��ȡHTML���������ļ�
	static bool LoadPackRuleFile(const char *pFilePath = NULL);                            //Xt: ��ȡ���еĹ��˹����ļ�

	static bool WriteHttpRuleFile(LoginFileContentInfo *pFileContentInfo,FILE *pFile);	   //Xt: д��HTML���������ļ�
	static bool WritePackRuleFile(PACKRULEFILE *pPackRuleFile,FILE *pFile);		           //Xt: д����еĹ��˹����ļ�
	static bool LoadFindDLL();										                       //Xt: ����HTML�������п�  ?????????
	static bool LoadRegedit();                                                             //Xt: ����ע����е���Ϣ

	
	static bool FindHttpFileContentKey(char *pStr,LoginFileContentInfo *pFileContentInfo); //Xt: ����HTTP�ļ��еĹؼ���
	static bool FindPackFileContentKey(char *pStr,PACKRULEFILE *pPackRuleFile);            //Xt: ����Pack�ļ��еĹؼ���
	static bool FindInterceptedFile(char *pStr,RADE_FILE *RadeFileInfo);                   //Xt: ���Ҳ�����ļ��е���Ϣ
	static bool FindRuleFileInfo(char *pStr,PACKRULEFILE *pPackRuleFile);                  //Xt: ���Ҳ�����ļ��а�����ؼ���
	static bool FindKeyWord(char *pStrData,const char *pStrKey,char *pStrSeve,int &nLeng); //Xt: ���ҹؼ���(Ҫ���ҵ��ַ��������ҵĹؼ���,�������ָ��)   

	static bool  printPcapMSG(pack_info *pi,FILE *pFile);                                  //Xt: ��ӡ�ػ���������Ϣ�����ļ�
	static bool  printPackRule(PACKRULEFILE *pPackRule,FILE *pFile);                       //Xt: ��ӡ���Ĺ�����Ϣ�����ļ�
	static void  DeleteInterCeptedFile();                                                  //Xt: ɾ���������Ϣ�ļ�
	static char *GetError()	{ return m_strError; }
	//Xt: ��ӡ��־�ļ�
	static char *PrintLogInfo(char *pThreadName, char *pErrorInfo, char cType = 'E',int iErrorID = 0); 
    static bool  SaveFindInf(FindInfo &Findinfo);	                                       //Xt: �洢��ӡ������Ϣ(����)				
public:	
	char                       m_strProcessPath[MAX_PATH];						           //Xt: ����·��	
	static char                m_strRulePackPath[MAX_PATH];                                //Xt: ����ץȡ�������������Ϣ
	static char                m_strRulePackInfoPath[MAX_PATH];                            //Xt: ���Ĺ�����Ϣ
	static char                m_strPrintLogPath[MAX_PATH];                                //Xt: ��ӡ��־·��
	static char                m_strHttpFilePath[MAX_PATH];						           //Xt: ��ȡ�ļ�·��
	static char                m_strPackRulePath[MAX_PATH];                                //Xt: ץ������·��
	static char                m_strOfflinePackPath[MAX_PATH];                             //Xt: �����ļ���·��
	static char                m_strClientVersionNum[10];                                  //Xt: ��ð汾����������
	static char				   m_strServerVersionNum[10];
	static char                m_strInterceptedInfoPath[MAX_PATH];                         //Xt: ץ���ػ����Ϣ���λ��
	static char                m_strError[1024];                                           //Xt: ������Ϣ
	static char                m_strServiceIP[16];                                         //Xt: ������IP��ַ
//	static FindInfo            m_findInfo;                                                 //Xt: Ҫ���ҵ���Ϣ
	static int                 m_nServicePort;                                             //Xt: �������˿ں�
	static int                 m_nPackRuleLen;                                             //Xt: ��ȡ������ĳ���
	static int                 m_nPackHeadRuleLen;                                         //Xt: ���û�ȡ��ͷ�ĳ��� 
	static int                 m_nSeparatePackNum;                                         //Xt: �ػ�ְ��ĸ���(1~3)֮��
	static CRITICAL_SECTION    m_writeErrorFileLock;                                       //Xt: ��ӡ�ļ��ٽ���
	static CGlobalData        *m_pGlbalData;                                               //Xt: ȫ��ָ��
public:
	static vector<string>                    m_vtOfflinePackPath;                          //Xt: ���߰�·��
	static vector<FindInfo>                  m_vtFindInfo;                                 //Xt: �洢��ӡ������Ϣ
    static map<string,DWORD>                 m_mapCloseWebInfo;                            //Xt: ���ڹر���ҳʱ��<����������ID>
	static list<INTERCEPTEDINFO>	         m_listInterceptedInfo;                        //Xt: ������HTTP������Ϣ�еļ�����Ϣ�����ں�ץ����Ϣƥ��
	static map<string,PACKRULEFILE>          m_mapPackRuleFileInfo;                        //Xt: ���ļ��ж�ȡPACK������Ϣ
	static map<string,LoginFileContentInfo>  m_mapHttpFileInfo;                            //Xt: ���ļ��ж�ȡHTTP������Ϣ
	static vector<string>                    m_vectAdapterName;                            //Xt: ����������ѡ��
	static map<string,pcap_if_t *>           m_mapAdapterInfo;                             //Xt: ͨ�����������������Ϣ 
};