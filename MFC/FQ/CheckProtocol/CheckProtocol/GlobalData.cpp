#include "StdAfx.h"
#include <direct.h>
#include "RegistryOperate.h"

int  CGlobalData::m_nPackRuleLen;
int  CGlobalData::m_nSeparatePackNum = 0;
int  CGlobalData::m_nPackHeadRuleLen = 0;
int  CGlobalData::m_nServicePort = 0;
char CGlobalData::m_strServiceIP[16] = {0};

CGlobalData *CGlobalData::m_pGlbalData = NULL;
list <INTERCEPTEDINFO> CGlobalData::m_listInterceptedInfo;
vector  <FindInfo>  CGlobalData::m_vtFindInfo;

vector<string> CGlobalData::m_vectAdapterName;
map<string,pcap_if_t *> CGlobalData::m_mapAdapterInfo;

map <string,DWORD> CGlobalData::m_mapCloseWebInfo;                         //????????
map <string,PACKRULEFILE> CGlobalData::m_mapPackRuleFileInfo;
map <string,LoginFileContentInfo> CGlobalData::m_mapHttpFileInfo;

char  CGlobalData::m_strError[1024] = {0};
char  CGlobalData::m_strClientVersionNum[10] = {0};
char  CGlobalData::m_strServerVersionNum[10] = {0};
char  CGlobalData::m_strRulePackPath[MAX_PATH] = {0};
char  CGlobalData::m_strHttpFilePath[MAX_PATH] = {0};
char  CGlobalData::m_strPackRulePath[MAX_PATH] = {0};                         
char  CGlobalData::m_strPrintLogPath[MAX_PATH] = {0}; 
char  CGlobalData::m_strOfflinePackPath[MAX_PATH] = {0};
char  CGlobalData::m_strRulePackInfoPath[MAX_PATH] = {0};
char  CGlobalData::m_strInterceptedInfoPath[MAX_PATH] ={0};

CRITICAL_SECTION  CGlobalData::m_writeErrorFileLock;
vector <string> CGlobalData::m_vtOfflinePackPath;

CGlobalData::CGlobalData(void)
{
	m_pGlbalData = this;
	memset(m_strError,0,sizeof(m_strError));
	memset(m_strProcessPath,0,sizeof(m_strProcessPath));
	::InitializeCriticalSection(&m_writeErrorFileLock);
}

CGlobalData::~CGlobalData(void)
{	
	m_pGlbalData = NULL;
	memset(CGlobalData::m_strError,0,sizeof(CGlobalData::m_strError));
	memset(CGlobalData::m_strRulePackPath,0,sizeof(CGlobalData::m_strRulePackPath));
	memset(CGlobalData::m_strHttpFilePath,0,sizeof(CGlobalData::m_strHttpFilePath));
	memset(CGlobalData::m_strPackRulePath,0,sizeof(CGlobalData::m_strPackRulePath));                         
	memset(CGlobalData::m_strPrintLogPath,0,sizeof(CGlobalData::m_strPrintLogPath)); 
	memset(CGlobalData::m_strOfflinePackPath,0,sizeof(CGlobalData::m_strOfflinePackPath));
	memset(CGlobalData::m_strRulePackInfoPath,0,sizeof(CGlobalData::m_strRulePackInfoPath));
	memset(CGlobalData::m_strInterceptedInfoPath,0,sizeof(CGlobalData::m_strInterceptedInfoPath));
	::DeleteCriticalSection(&m_writeErrorFileLock);
}

CGlobalData ** CGlobalData::GetInstance()
{
	return &m_pGlbalData;
}

bool CGlobalData::InitInstance(const char *pFilePath)              //??????参数
{
	if(!LoadRegedit())                                             
	{
		PrintLogInfo("CRegistryOperate",CRegistryOperate::m_strError,'E');
	}

	if(!GetProcessPath())
	{
		PrintLogInfo("CGlobalData","获得当前进程路径错误!",'E');
		return false;
	}

	if('\0' == CGlobalData::m_strHttpFilePath[0])
		sprintf_s(CGlobalData::m_strHttpFilePath,sizeof(CGlobalData::m_strHttpFilePath),"%s\\LoginKey\\LoginKey.txt",m_strProcessPath);
	if('\0' == CGlobalData::m_strPackRulePath[0])
		sprintf_s(CGlobalData::m_strPackRulePath,sizeof(CGlobalData::m_strPackRulePath),"%s\\PackRuleFile\\PackRule.txt",m_strProcessPath);
    if('\0' == CGlobalData::m_strInterceptedInfoPath[0])
		sprintf_s(CGlobalData::m_strInterceptedInfoPath,sizeof(CGlobalData::m_strInterceptedInfoPath),"%s\\InterceptedInfo\\PackInfo.txt",m_strProcessPath);
	if('\0' == CGlobalData::m_strPrintLogPath[0])
		sprintf_s(CGlobalData::m_strPrintLogPath,sizeof(CGlobalData::m_strPrintLogPath),"%s\\LogInfo\\CheckProtocolLog.log",m_strProcessPath);
	if('\0' == CGlobalData::m_strOfflinePackPath[0])
		sprintf_s(CGlobalData::m_strOfflinePackPath,sizeof(CGlobalData::m_strOfflinePackPath),"%s\\OfflinePackPath",m_strProcessPath);
	if('\0' == CGlobalData::m_strRulePackPath[0])
		sprintf_s(CGlobalData::m_strRulePackPath,sizeof(CGlobalData::m_strRulePackPath),"%s\\InterceptedInfo\\RulePack.txt",m_strProcessPath);
	if('\0' == CGlobalData::m_strRulePackInfoPath[0])
		sprintf_s(CGlobalData::m_strRulePackInfoPath,sizeof(CGlobalData::m_strRulePackInfoPath),"%s\\InterceptedInfo\\RulePackInfo.txt",m_strProcessPath);
	if('\0' == CGlobalData::m_strServiceIP[0])
		memcpy(CGlobalData::m_strServiceIP,"192.168.30.234",strlen("192.168.30.234"));
	if(0 == m_nServicePort)
		m_nServicePort = 8886;

	if(!LoadHttpRuleFile() || !LoadPackRuleFile())
	{
		return false;
	}
	return TRUE;
}

//Xt: Release修改
char *CGlobalData::GetProcessPath()
{
	if(!GetModuleFileNameA(NULL,m_strProcessPath,sizeof(m_strProcessPath)))
	{
		sprintf_s(m_strError,sizeof(m_strError),"获得进程路径错误!\n 错误代码%d",GetLastError());
		return false;
	}	
	for(int i = strlen(m_strProcessPath)-1 ; i>0 ;i--)
	{
		if('\\' == m_strProcessPath[i])
		{
// 			i -= 1;
// 			for(;i>0;i--)				
// 			{
// 				if('\\' == m_strProcessPath[i])
// 				{
					m_strProcessPath[i] = '\0';
					return m_strPackRulePath;
// 				}
// 			}
	    	return NULL;
		}
	}
	return NULL;
}

bool CGlobalData::LoadRegedit()
{
	HKEY hKey;
	CRegistryOperate RegOpt;
	if(RegOpt.OpenRegistry(HKEY_LOCAL_MACHINE,"SOFTWARE\\CaptureTest",&hKey))
	{
		RegOpt.GetRegistryValue("PrintLogPath",m_strPrintLogPath,&hKey);
		RegOpt.GetRegistryValue("HttpFilePath",m_strHttpFilePath,&hKey);
		RegOpt.GetRegistryValue("PackRulePath",m_strPackRulePath,&hKey);
		RegOpt.GetRegistryValue("InterceptedInfoPath",m_strInterceptedInfoPath,&hKey);
		RegOpt.GetRegistryValue("OfflinePackPath",m_strOfflinePackPath,&hKey);
		RegOpt.GetRegistryValue("RulePackPath",m_strRulePackPath,&hKey);
		RegOpt.GetRegistryValue("RulePackInfoPath",m_strRulePackInfoPath,&hKey);
		RegOpt.GetRegistryValue("Version",m_strClientVersionNum,&hKey);
		RegOpt.GetRegistryValue("ServiceIP",m_strServiceIP,&hKey);
		char   strServicePort[6] = {0};
		RegOpt.GetRegistryValue("ServicePort",strServicePort,&hKey);
		if(*strServicePort != '\0')
			m_nServicePort = atoi(strServicePort);
		else
			m_nServicePort = 0;

		RegCloseKey(hKey);
		return true;
	}
	else if(RegOpt.CreateRegistry(HKEY_LOCAL_MACHINE,"SOFTWARE\\CaptureTest",&hKey))
	{
		RegOpt.SetRegistryValue("PrintLogPath","",&hKey);
		RegOpt.SetRegistryValue("HttpFilePath","",&hKey);
		RegOpt.SetRegistryValue("PackRulePath","",&hKey);
		RegOpt.SetRegistryValue("InterceptedInfoPath","",&hKey);
		RegOpt.SetRegistryValue("OfflinePackPath","",&hKey);
		RegOpt.SetRegistryValue("RulePackPath","",&hKey);
		RegOpt.SetRegistryValue("RulePackInfoPath","",&hKey);
		RegOpt.SetRegistryValue("Version",m_strClientVersionNum,&hKey);
		RegOpt.SetRegistryValue("ServiceIP",m_strServiceIP,&hKey);
		RegOpt.SetRegistryValue("ServicePort","",&hKey);
		RegCloseKey(hKey);
		return false;
	}
	return true;
}

bool CGlobalData::LoadHttpRuleFile(const char *pFilePath)
{
	char strFileContentInfo[1024] = {0};
	FILE *pFile = fopen(m_strHttpFilePath,"rb+");
	if(!pFile)	
	{
		sprintf(m_strError,"\"%s\" 文件夹下无配置文件文件",m_strHttpFilePath);
		return false;
	}
	else
	{	
		int nNumber = 0;
		while(fgets(strFileContentInfo,sizeof(strFileContentInfo),pFile))
		{
			if(*strFileContentInfo == EOF)
				break;
			INTERCEPTEDINFO iio;
			LoginFileContentInfo FileContentInfo;
			if(FindHttpFileContentKey(strFileContentInfo,&FileContentInfo))
			{
				m_mapHttpFileInfo.insert(make_pair(FileContentInfo.Item,FileContentInfo));
				
				memcpy(iio.Item,FileContentInfo.Item,sizeof(iio.Item));                                  //Xt:?????????改建
				memcpy(iio.UserName,FileContentInfo.UserName,sizeof(iio.UserName));
				memcpy(iio.SendContent,FileContentInfo.SendContent,sizeof(iio.SendContent));
				m_listInterceptedInfo.push_back(iio);
			}
			else
			{
				sprintf(m_strError,"读取登录文件的第%d行错误!!!",nNumber);
			}
				nNumber++;
		}
		if(m_mapHttpFileInfo.empty())
		{
			sprintf(m_strError,"\"%s\" 文件下无有效信息",m_strHttpFilePath);
		}
		else
		{
			//CGlobalData::m_listInterceptedInfo.sort();                   //????????????排序
		}
	}
	fclose(pFile);
	return true;
}

bool CGlobalData::LoadPackRuleFile(const char *pFilePath)
{
	char strFileContentInfo[512] = {0};
	FILE *pFile = fopen(m_strPackRulePath,"rb+");
	if(!pFile)	
	{ 
		sprintf(m_strError,"\"%s\" 文件夹下无配置文件文件",m_strPackRulePath);
		return false;
	}
	else
	{	
		int nNumber = 0;
		while(fgets(strFileContentInfo,sizeof(strFileContentInfo),pFile))
		{
			if(*strFileContentInfo == EOF)
				break;	
			PackRuleFile PRFile;
			if(FindPackFileContentKey(strFileContentInfo,&PRFile))
			{
				m_mapPackRuleFileInfo.insert(make_pair(PRFile.Item,PRFile));
			}
			else
			{
				sprintf_s(m_strError,sizeof(m_strError),"读取登录文件的第%d行错误!!!",nNumber);
			}
			nNumber++;
		}
		if(m_mapPackRuleFileInfo.empty())
		{
			sprintf(m_strError,"\"%s\" 文件下无有效信息",m_strHttpFilePath);
		}
	}
	fclose(pFile);
	return true;
}

bool CGlobalData::WriteHttpRuleFile(LoginFileContentInfo *pFileContentInfo,FILE *pFile)
{
	if(m_strHttpFilePath)
	{
		if(!fseek(pFile,0,SEEK_END))		
		{
			if(0>fprintf(pFile,"ItemName=\"%s\"; URL=\"%s\"; UserNameKey=\"%s\"; PasswordKey=\"%s\"; OKButtonName=\"%s\"; UserName=\"%s\"; Password=\"%s\"; OperateItem=\"%d\"; SendContent=\"%s\"; SendKey=\"%s\"; \r\n", \
				pFileContentInfo->Item,pFileContentInfo->URL,pFileContentInfo->UserNameMark,pFileContentInfo->PassWordMark,pFileContentInfo->ButtonName,\
				pFileContentInfo->UserName,pFileContentInfo->Password,pFileContentInfo->OperateItem,pFileContentInfo->SendContent,pFileContentInfo->SendKey))
			{
				sprintf_s(m_strError,sizeof(m_strError),"写入\"%s\"项时错误",pFileContentInfo->Item);
				return false;
			}	
		}
		else
		{
			sprintf_s(m_strError,sizeof(m_strError),"\"%s\"路径下读取登录文件记录错误!",m_strHttpFilePath);
			return false;
		}
	}
	return true;
}

bool CGlobalData::FindHttpFileContentKey(char *pStr,LoginFileContentInfo *pFileContentInfo)
{
	int   nSz = 0;
	char  pOperateItem[10] = {0};
	if(!FindKeyWord(pStr,"OperateItem=""",pOperateItem,nSz))	    return false;
	      pFileContentInfo->OperateItem = atoi(pOperateItem);

	nSz = 0;
	if(!FindKeyWord(pStr,"ItemName=""",pFileContentInfo->Item,nSz))		return false;
	if(!FindKeyWord(pStr+nSz,"URL=""",pFileContentInfo->URL,nSz))		return false;
	if((!FindKeyWord(pStr+nSz,"UserNameKey=""",pFileContentInfo->UserNameMark,nSz)) && (pFileContentInfo->OperateItem == 1))
		return false;
	if((!FindKeyWord(pStr+nSz,"PasswordKey=""",pFileContentInfo->PassWordMark,nSz)) && (pFileContentInfo->OperateItem == 1))	
		return false;
	if((!FindKeyWord(pStr+nSz,"OKButtonName=""",pFileContentInfo->ButtonName,nSz)) && (pFileContentInfo->OperateItem == 1))	  
		return false;
	if((!FindKeyWord(pStr+nSz,"UserName=""",pFileContentInfo->UserName,nSz)) && (pFileContentInfo->OperateItem == 1))		
		return false;
	if((!FindKeyWord(pStr+nSz,"Password=""",pFileContentInfo->Password,nSz)) && (pFileContentInfo->OperateItem == 1))		
		return false;
	if((!FindKeyWord(pStr+nSz,"SendContent=""",pFileContentInfo->SendContent,nSz)) && (pFileContentInfo->OperateItem == 2))
		//memset(pFileContentInfo->SendContent,0,sizeof(pFileContentInfo->SendContent));
		return false;
	if((!FindKeyWord(pStr+nSz,"SendKey=""",pFileContentInfo->SendKey,nSz)) && (pFileContentInfo->OperateItem == 2))
		//memset(pFileContentInfo->SendKey,0,sizeof(pFileContentInfo->SendKey));
		return false;
	return true;
}

bool CGlobalData::FindPackFileContentKey(char *pStr,PACKRULEFILE *pPackRuleFile)
{
	int nLeng = 0;
	if(!FindKeyWord(pStr,"ItemName=""",pPackRuleFile->Item,nLeng))	return false;
	if(!FindKeyWord(pStr+nLeng,"PackHeadName=""",pPackRuleFile->PackHead,nLeng))	return false;
	if(!FindKeyWord(pStr+nLeng,"HostName=""",pPackRuleFile->Host,nLeng))	return false;
	if(!FindKeyWord(pStr+nLeng,"UserNameKeyName=""",pPackRuleFile->UserNameKey,nLeng))	return false;
	if(!FindKeyWord(pStr+nLeng,"UserEndNameKeyName=""",pPackRuleFile->UserEndNameKey,nLeng))	return false;
	if(FindKeyWord(pStr+nLeng,"PasswordKeyName=""",pPackRuleFile->PassWordKey,nLeng))
	{
		FindKeyWord(pStr+nLeng,"EndPasswordKey=""",pPackRuleFile->EndPassWordKey,nLeng);
	}	
	if(FindKeyWord(pStr+nLeng,"SendContentKeyName=""",pPackRuleFile->SendContentKey,nLeng)) 
	{
		FindKeyWord(pStr+nLeng,"EndSendContentKey=""",pPackRuleFile->EndSengContentKey,nLeng);
	}	
	FindKeyWord(pStr+nLeng,"CodeType=""",pPackRuleFile->CodeType,nLeng);
	return true;
}

bool CGlobalData::FindRuleFileInfo(char *pStr,PACKRULEFILE *pPackRuleFile)
{
	int nLeng = 0;
	if(!FindKeyWord(pStr+nLeng,"包头=""",pPackRuleFile->PackHead,nLeng))	            return false;
	if(!FindKeyWord(pStr+nLeng,"域名=""",pPackRuleFile->Host,nLeng))	                return false;
	if(!FindKeyWord(pStr+nLeng,"用户名开始=""",pPackRuleFile->UserNameKey,nLeng))	    return false;
	if(!FindKeyWord(pStr+nLeng,"用户名结束=""",pPackRuleFile->UserEndNameKey,nLeng))	return false;
	if(FindKeyWord(pStr+nLeng,"用户密码开始=""",pPackRuleFile->PassWordKey,nLeng))
	{
		FindKeyWord(pStr+nLeng,"用户密码结束=""",pPackRuleFile->EndPassWordKey,nLeng);
	}	
	if(FindKeyWord(pStr+nLeng,"用户发送开始=""",pPackRuleFile->SendContentKey,nLeng)) 
	{
		FindKeyWord(pStr+nLeng,"用户发送结束=""",pPackRuleFile->EndSengContentKey,nLeng);
	}	
	FindKeyWord(pStr+nLeng,"包编码信息=""",pPackRuleFile->CodeType,nLeng);
	FindKeyWord(pStr+nLeng,"Referer=""",pPackRuleFile->Referer,nLeng);
	return true;
}

bool CGlobalData::WritePackRuleFile(PACKRULEFILE *pPackRuleFile,FILE *pFile)
{
	if(m_strHttpFilePath)
	{
		if(!fseek(pFile,0,SEEK_END))		
		{
			if(0>fprintf(pFile,"ItemName=\"%s\"; PackHeadName=\"%s\"; HostName=\"%s\"; UserNameKeyName=\"%s\"; UserEndNameKeyName=\"%s\"; PassWordKeyName=\"%s\"; EndPassWordKey=\"%s\"; SendContentKeyName=\"%s\"; EndsengContentKey=\"%s\"; CodeType=\"%s\"; \r\n",\
						pPackRuleFile->Item,pPackRuleFile->PackHead,pPackRuleFile->Host,pPackRuleFile->UserNameKey,pPackRuleFile->UserEndNameKey,\
						pPackRuleFile->PassWordKey,pPackRuleFile->EndPassWordKey,pPackRuleFile->SendContentKey,pPackRuleFile->EndSengContentKey,pPackRuleFile->CodeType))
			{
				sprintf_s(m_strError,sizeof(m_strError),"写入\"%s\"包规则文件项时错误",pPackRuleFile->Item);
				return false;
			}	
		}
		else
		{
			sprintf_s(m_strError,sizeof(m_strError),"\"%s\"路径下读取包规则文件记录错误!",m_strPackRulePath);
			return false;
		}
	}
	return true;
}

bool CGlobalData::FindKeyWord(char *pStrData,const char *pStrKey,char *pStrSeve ,int &nLeng)
{
 	char *pItem = NULL;
 	char *pEnd =  NULL;
 	int nLength = 0;
	if(pItem = StrStrA(pStrData,pStrKey))			 
	{	
		if(pEnd = StrStrA(pItem,"""; "))
		{
			nLength = strlen(pStrKey)+1;
			nLeng += pEnd-pItem;
			if(0>=(pEnd-pItem-nLength-1))	
				return false;		
			memcpy(pStrSeve,pItem+nLength,(pEnd-pItem-nLength-1));
			pItem = NULL;
			nLength = 0;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool CGlobalData::FindInterceptedFile(char *pStr,RADE_FILE *RadeFileInfo)
{
	int nSz = 0;
	char pOperateItem[10] = {0};
	if(!FindKeyWord(pStr,"项目名=""",RadeFileInfo->Item,nSz))	    	         return false;
	if(!FindKeyWord(pStr+nSz,"用户名=""",RadeFileInfo->UserName,nSz))            return false;
	FindKeyWord(pStr+nSz,"密码=""",RadeFileInfo->Password,nSz);
	FindKeyWord(pStr+nSz,"收发内容=""",RadeFileInfo->SendContent,nSz);
	if(!FindKeyWord(pStr+nSz,"原IP地址=""",RadeFileInfo->ip_src,nSz))		     return false;
	if(!FindKeyWord(pStr+nSz,"目的IP地址=""",RadeFileInfo->ip_dst,nSz))	         return false;
	if(!FindKeyWord(pStr+nSz,"原端口号=""",  RadeFileInfo->SrcPort,nSz))         return false;
	if(!FindKeyWord(pStr+nSz,"目的端口号=""",RadeFileInfo->DestPort,nSz))	     return false;
	if(!FindKeyWord(pStr+nSz,"方向=""",RadeFileInfo->PackDirection,nSz))	     return false;
	if(!FindKeyWord(pStr+nSz,"原MAC地址=""",RadeFileInfo->SrcMac,nSz))			 return false;
	if(!FindKeyWord(pStr+nSz,"目的MAC地址 =""",RadeFileInfo->DestMac,nSz))	     return false;
	
	return true;
}

void CGlobalData::DeleteInterCeptedFile()
{
	if(remove(CGlobalData::m_strInterceptedInfoPath))
	{
		sprintf_s(m_strError,sizeof(m_strError),"%s \r\路径下文件移除失败!",CGlobalData::m_strInterceptedInfoPath);
	}
}

bool  CGlobalData::printPcapMSG(pack_info *pi,FILE *pFile)
{
	struct tm *ltime = localtime(&(pi->PackTime));
	char strTime[128] = {0};
	strftime(strTime,sizeof(strTime),"%c",ltime); 
	char pSrc[16] = {0};
	char pDst[16] = {0}; 
	memcpy(pSrc,inet_ntoa(pi->ip_src),sizeof(pSrc));
	memcpy(pDst,inet_ntoa(pi->ip_dst),sizeof(pDst));
	if(0 >= fprintf(pFile,"项目名=\"%s\"; 用户名=\"%s\"; 密码=\"%s\"; 收发内容=\"%s\"; 原IP地址=\"%s\"; 目的IP地址=\"%s\"; 原端口号=\"%d\"; 目的端口号=\"%d\"; 方向=\"%s\"; 原MAC地址=\"%02X-%02X-%02X-%02X-%02X-%02X\"; 目的MAC地址 =\"%02X-%02X-%02X-%02X-%02X-%02X\"; 时间=\"%s\"; \r\n",\
		            pi->interceptedInfo.Item,pi->interceptedInfo.UserName,pi->interceptedInfo.Password,pi->interceptedInfo.SendContent,pSrc,pDst,pi->SrcPort,pi->DestPort,pi->PackDirection,\
		            pi->SrcMac[0],pi->SrcMac[1],pi->SrcMac[2],pi->SrcMac[3],pi->SrcMac[4],pi->SrcMac[5],\
		            pi->DestMac[0],pi->DestMac[1],pi->DestMac[2],pi->DestMac[3],pi->DestMac[4],pi->DestMac[5],\
		            strTime))
	{
		sprintf_s(CGlobalData::m_strError,sizeof(CGlobalData::m_strError),"<%s> 捕获内容文件写入错误!",pi->interceptedInfo.Item);
		return false;
	}
	return true;
}

bool  CGlobalData::printPackRule(PACKRULEFILE *pPackRule,FILE *pFile)
{
	if(0 >= fprintf(pFile,"项目名=\"%s\"; 包头=\"%s\"; 域名=\"%s\"; 用户名开始=\"%s\"; 用户名结束=\"%s\"; 用户密码开始=\"%s\"; 用户密码结束=\"%s\"; 用户发送开始=\"%s\"; 用户发送结束=\"%s\"; 包编码信息=\"%s\"; Referer=\"%s\"; \r\n",
		                  pPackRule->Item,pPackRule->PackHead,pPackRule->Host,pPackRule->UserNameKey,pPackRule->UserEndNameKey,pPackRule->PassWordKey,\
						  pPackRule->EndPassWordKey,pPackRule->SendContentKey,pPackRule->EndSengContentKey,pPackRule->CodeType,pPackRule->Referer))
	{
		sprintf_s(CGlobalData::m_strError,sizeof(CGlobalData::m_strError),"<%s> 捕获包规则文件写入错误!",pPackRule->Item);
		return false;
	}
	return true;
}

char *CGlobalData::PrintLogInfo(char *pThreadName, char *pErrorInfo,char cType /*='E'*/,int iErrorID /*=0*/)
{
	time_t t = time(NULL);
	char strTime[128] = {0};
	strftime(strTime,sizeof(strTime),"%c",localtime(&t)); 

	FILE*  file= fopen(CGlobalData::m_strPrintLogPath, "a+");
	if (file)
	{
		::EnterCriticalSection(&m_writeErrorFileLock);
		if(cType == 'O')
		{
			sprintf_s(CGlobalData::m_strError,sizeof(CGlobalData::m_strError),"%s   错误代码:%d",pErrorInfo,iErrorID);
			fprintf(file,"[MSG]  	[%s]	 [%s]  [%s]	 [%d] ; \r\n",strTime,pThreadName,pErrorInfo,iErrorID);
		}
		else if(cType == 'E')
		{
			sprintf_s(CGlobalData::m_strError,sizeof(CGlobalData::m_strError),"%s   错误代码:%d",pErrorInfo,iErrorID);
			fprintf(file,"[ERROR]	[%s]	 [%s]  [%s]	 [%d] ; \r\n",strTime,pThreadName,pErrorInfo,iErrorID);
		}
		::LeaveCriticalSection(&m_writeErrorFileLock);
		fclose(file);
	}

#ifdef _DEBUG
	::OutputDebugStringA(CGlobalData::m_strError);
#endif 
	return m_strError;
} 

bool CGlobalData::SaveFindInf(FindInfo &findInfo)
{
	FindInfo	MultiCodeFindInfo;
	CGlobalData::m_vtFindInfo.push_back(findInfo);

	if('\0' != *findInfo.userName)     CChangeCode::AntiFindCode("U-U",findInfo.userName,MultiCodeFindInfo.userName);
	if('\0' != *findInfo.Password)     CChangeCode::AntiFindCode("U-U",findInfo.Password,MultiCodeFindInfo.Password);
	if('\0' != *findInfo.SendContent)  CChangeCode::AntiFindCode("U-U",findInfo.SendContent,MultiCodeFindInfo.SendContent);
	memcpy(MultiCodeFindInfo.Type,"U-U",sizeof("U-U"));
	CGlobalData::m_vtFindInfo.push_back(MultiCodeFindInfo);
	memset(&MultiCodeFindInfo,0,sizeof(MultiCodeFindInfo));
	
	if('\0' != *findInfo.userName)     CChangeCode::AntiFindCode("U-G",findInfo.userName,MultiCodeFindInfo.userName);
	if('\0' != *findInfo.Password)     CChangeCode::AntiFindCode("U-G",findInfo.Password,MultiCodeFindInfo.Password);
	if('\0' != *findInfo.SendContent)  CChangeCode::AntiFindCode("U-G",findInfo.SendContent,MultiCodeFindInfo.SendContent);
	memcpy(MultiCodeFindInfo.Type,"U-G",sizeof("U-G"));
	CGlobalData::m_vtFindInfo.push_back(MultiCodeFindInfo);
	memset(&MultiCodeFindInfo,0,sizeof(MultiCodeFindInfo));

	if('\0' != *findInfo.userName)     CChangeCode::AntiFindCode("U-",findInfo.userName,MultiCodeFindInfo.userName);
	if('\0' != *findInfo.Password)     CChangeCode::AntiFindCode("U-",findInfo.Password,MultiCodeFindInfo.Password);
	if('\0' != *findInfo.SendContent)  CChangeCode::AntiFindCode("U-",findInfo.SendContent,MultiCodeFindInfo.SendContent);
	memcpy(MultiCodeFindInfo.Type,"U-",sizeof("U-"));
	CGlobalData::m_vtFindInfo.push_back(MultiCodeFindInfo);
	memset(&MultiCodeFindInfo,0,sizeof(MultiCodeFindInfo));

	if('\0' != *findInfo.userName)     CChangeCode::AntiFindCode("U-G-U",findInfo.userName,MultiCodeFindInfo.userName);
	if('\0' != *findInfo.Password)     CChangeCode::AntiFindCode("U-G-U",findInfo.Password,MultiCodeFindInfo.Password);
	if('\0' != *findInfo.SendContent)  CChangeCode::AntiFindCode("U-G-U",findInfo.SendContent,MultiCodeFindInfo.SendContent);
	memcpy(MultiCodeFindInfo.Type,"U-G-U",sizeof("U-G-U"));
	CGlobalData::m_vtFindInfo.push_back(MultiCodeFindInfo);
	memset(&MultiCodeFindInfo,0,sizeof(MultiCodeFindInfo));

	return true;
}