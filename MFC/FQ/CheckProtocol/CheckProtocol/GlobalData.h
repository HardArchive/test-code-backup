#pragma once
using namespace std;

class CGlobalData
{
public:
	CGlobalData(void);
	~CGlobalData(void);
	static CGlobalData **GetInstance();
	char *GetProcessPath();                                                                //Xt: 获得进程路径

	bool InitInstance(const char *pFilePath = NULL);                                       //Xt: 初始化
	static bool LoadHttpRuleFile(const char *pFilePath = NULL);		                       //Xt: 读取HTML解析规则文件
	static bool LoadPackRuleFile(const char *pFilePath = NULL);                            //Xt: 读取包中的过滤规则文件

	static bool WriteHttpRuleFile(LoginFileContentInfo *pFileContentInfo,FILE *pFile);	   //Xt: 写入HTML解析规则文件
	static bool WritePackRuleFile(PACKRULEFILE *pPackRuleFile,FILE *pFile);		           //Xt: 写入包中的过滤规则文件
	static bool LoadFindDLL();										                       //Xt: 加载HTML搜索运行库  ?????????
	static bool LoadRegedit();                                                             //Xt: 加载注册表中的信息

	
	static bool FindHttpFileContentKey(char *pStr,LoginFileContentInfo *pFileContentInfo); //Xt: 查找HTTP文件中的关键字
	static bool FindPackFileContentKey(char *pStr,PACKRULEFILE *pPackRuleFile);            //Xt: 查找Pack文件中的关键字
	static bool FindInterceptedFile(char *pStr,RADE_FILE *RadeFileInfo);                   //Xt: 查找捕获后文件中的信息
	static bool FindRuleFileInfo(char *pStr,PACKRULEFILE *pPackRuleFile);                  //Xt: 查找捕获后文件中包规则关键字
	static bool FindKeyWord(char *pStrData,const char *pStrKey,char *pStrSeve,int &nLeng); //Xt: 查找关键字(要查找的字符串，查找的关键字,保存变量指针)   

	static bool  printPcapMSG(pack_info *pi,FILE *pFile);                                  //Xt: 打印截获分析后的信息存于文件
	static bool  printPackRule(PACKRULEFILE *pPackRule,FILE *pFile);                       //Xt: 打印包的规则信息存于文件
	static void  DeleteInterCeptedFile();                                                  //Xt: 删除捕获的信息文件
	static char *GetError()	{ return m_strError; }
	//Xt: 打印日志文件
	static char *PrintLogInfo(char *pThreadName, char *pErrorInfo, char cType = 'E',int iErrorID = 0); 
    static bool  SaveFindInf(FindInfo &Findinfo);	                                       //Xt: 存储打印规则信息(多码)				
public:	
	char                       m_strProcessPath[MAX_PATH];						           //Xt: 进程路径	
	static char                m_strRulePackPath[MAX_PATH];                                //Xt: 保存抓取规则包的完整信息
	static char                m_strRulePackInfoPath[MAX_PATH];                            //Xt: 包的规则信息
	static char                m_strPrintLogPath[MAX_PATH];                                //Xt: 打印日志路径
	static char                m_strHttpFilePath[MAX_PATH];						           //Xt: 读取文件路径
	static char                m_strPackRulePath[MAX_PATH];                                //Xt: 抓包规则路径
	static char                m_strOfflinePackPath[MAX_PATH];                             //Xt: 离线文件夹路径
	static char                m_strClientVersionNum[10];                                  //Xt: 获得版本号用于升级
	static char				   m_strServerVersionNum[10];
	static char                m_strInterceptedInfoPath[MAX_PATH];                         //Xt: 抓包截获的信息存放位置
	static char                m_strError[1024];                                           //Xt: 错误信息
	static char                m_strServiceIP[16];                                         //Xt: 服务器IP地址
//	static FindInfo            m_findInfo;                                                 //Xt: 要查找的信息
	static int                 m_nServicePort;                                             //Xt: 服务器端口号
	static int                 m_nPackRuleLen;                                             //Xt: 获取包规则的长度
	static int                 m_nPackHeadRuleLen;                                         //Xt: 设置获取包头的长度 
	static int                 m_nSeparatePackNum;                                         //Xt: 截获分包的个数(1~3)之间
	static CRITICAL_SECTION    m_writeErrorFileLock;                                       //Xt: 打印文件临界区
	static CGlobalData        *m_pGlbalData;                                               //Xt: 全局指针
public:
	static vector<string>                    m_vtOfflinePackPath;                          //Xt: 离线包路径
	static vector<FindInfo>                  m_vtFindInfo;                                 //Xt: 存储打印规则信息
    static map<string,DWORD>                 m_mapCloseWebInfo;                            //Xt: 用于关闭网页时用<项名、进程ID>
	static list<INTERCEPTEDINFO>	         m_listInterceptedInfo;                        //Xt: 挑出从HTTP规则信息中的检索信息，用于和抓包信息匹配
	static map<string,PACKRULEFILE>          m_mapPackRuleFileInfo;                        //Xt: 从文件中读取PACK规则信息
	static map<string,LoginFileContentInfo>  m_mapHttpFileInfo;                            //Xt: 从文件中读取HTTP规则信息
	static vector<string>                    m_vectAdapterName;                            //Xt: 网卡名用于选择
	static map<string,pcap_if_t *>           m_mapAdapterInfo;                             //Xt: 通过网卡名获得网卡信息 
};