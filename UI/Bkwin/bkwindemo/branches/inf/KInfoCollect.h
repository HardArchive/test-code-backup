
#pragma once
#include "../KFunction.h"

#define		DEFFILEPOSTFIX					TEXT(".inf")
#define		DEF_STR_INFOC_URL			    _T("http://info.duba.net/pagetracer4Unencrypt/duba/__utm.gif?")
#define		KAV_PROCESS_ERROR(Condition)    if (!(Condition))   goto Exit0;
#define		KAV_DELETE_ARRAY(pArray)		if (pArray) { delete [](pArray); (pArray) = NULL; }
#define		KAV_DELETE(p)					if (p) { delete (p); (p) = NULL; }
#define		BASE64_MAXLINE								76
#define		BASE64_EOL									"\r\n"

class KInfoCollect
{
public:
	static void InfocCollect_Initialize();
	static void InfocCollect_Uninitialize();
	/*
		duba_install
		point 安装步骤int 0，安装初始界面；1，安装完成
		installtype 安装类型 int 0,首次安装；1，覆盖安装
		time 时间 int 安装过程所有时间，单位秒
	*/
	static void InfocCollectInstaller_Active(IN int nstep, IN int nInstallType, IN int time, IN BOOL bIsNeedCreateThread = TRUE);
	static void InfocCollectUnInstaller(IN int ndays, CString strchoice, IN BOOL bIsNeedCreateThread);
	static void InfocCollectFeedback(CString strword, CString strqq,  int nforumclickIN, BOOL bIsNeedCreateThread);
	//统一的添加
	static void KInfoCollect::AddData(std::wstring sActionName,IN std::wstring sData);
};

class InfocWriteData
{
public:
	BOOL WriteStringToFile(IN std::wstring strActionName, IN std::wstring strFileData);
	void EncyptData(IN std::wstring strInData, OUT std::wstring& strOutData);

private:
	BOOL _Insert( IN const std::wstring& strActionName, IN const std::wstring& strAllData );
	void _GetTimeForInfoc( OUT DWORD& dwTime );
	void _GetTimeForInfoc( OUT std::wstring& strTime );
	void _GetRandData( OUT std::wstring& strRand );



	int Base64EnCode(IN const std::wstring& strSource, OUT std::wstring& strDes );
	int KA2W(IN LPCSTR pszAString, IN int nszAStringSize, OUT LPWSTR pszWString, IN int nszWStringSize);
	int KW2A(IN LPCWSTR pszWString, IN int nszWStringSize, OUT LPSTR pszAString, IN int nszAStringSize);
	int KW2A(IN const std::wstring & strWstring, OUT std::string & strAstring);
	int KA2W(IN const std::string & strAstring, OUT std::wstring& strWstring);

	int _FormatByteToStr(BYTE byData, char szStr[2]);
	int InfocUrlCode( IN std::wstring strSource, OUT std::wstring& strDes );
	std::string _EscapeEncoding(int nDataSize, BYTE *pbyDataBuff);
		
};
