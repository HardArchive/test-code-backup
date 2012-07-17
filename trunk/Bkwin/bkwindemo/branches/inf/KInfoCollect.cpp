#include "StdAfx.h"
#include "KInfoCollect.h"
#include "KRequestHttpFile.h"
#include "KStreamWrapper.h"
#include <process.h>
#include "CKavData.h"
// 基本字段
std::wstring			g_strInfoC;
std::wstring			g_strInfocParam;
BOOL					g_HttpInited = FALSE;
int						g_nNetworkErrCode = 1;
LONG					g_lInfoCThreadNumber = 0;// InfoC线程数。
BOOL					g_bRunOnce			= FALSE;

struct stInfocParamElem 
{
	TCHAR szActionName[32];
	TCHAR szParamStr[MAX_PATH*2];
};

/*

  函数名:	Base64Encode
  功能：	完成Base64的编码功能
  参数:		pcszIn		---		需要编码的字符串
			nInSize		---		需要编码字符串长度
			pszOut		---		接收编码之后的字符串指针，
								不能为空，大小可以通过调
								用EstimateEncodeBufferSize
								函数获得
			nOutSizeMax ---		参数pszOut的Buffer的大小
			pnReturnSize---		返回实际的编码之后文本的大小
			nNeedCRLF	---		是否需要回车换行，正常情况下面为TRUE
*/

int Base64Encode(const char *pcszIn, int nInSize, char *pszOut, int nOutSizeMax, int *pnReturnSize, int nNeedCRLF)
{
	int nResult = false;
	char szBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int nInPos  = 0;
	int nOutPos = 0;
	int nLineLen = 0;
	int i = 0;

	if (pcszIn == NULL || pszOut == NULL)
		goto Exit0;

	if (nOutSizeMax < (nInSize + 2) / 3 * 4)
		goto Exit0;

	/* Get three characters at a time from the input buffer and encode them */
	for (i = 0; i < nInSize / 3; i++) 
	{
		/* Get the next 2 characters */
		int c1 = pcszIn[nInPos++] & 0xFF;
		int c2 = pcszIn[nInPos++] & 0xFF;
		int c3 = pcszIn[nInPos++] & 0xFF;

		/* Encode into the 4 6 bit characters */
		pszOut[nOutPos++] = szBase64Tab[(c1 & 0xFC) >> 2];
		pszOut[nOutPos++] = szBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
		pszOut[nOutPos++] = szBase64Tab[((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
		pszOut[nOutPos++] = szBase64Tab[c3 & 0x3F];
		nLineLen += 4;

		if (nNeedCRLF)
		{
			/* Handle the case where we have gone over the max line boundary */
			if (nLineLen >= BASE64_MAXLINE - 3)
			{
				char *cp = BASE64_EOL;

				pszOut[nOutPos++] = *cp++;

				if (*cp)
				{
					pszOut[nOutPos++] = *cp;
				}

				nLineLen = 0;
			}
		}
	}

	/* Encode the remaining one or two characters in the input buffer */
	switch (nInSize % 3) 
	{
	case 0:
		{
			if (nNeedCRLF)
			{
				char *cp = BASE64_EOL;
				pszOut[nOutPos++] = *cp++;

				if (*cp)
				{
					pszOut[nOutPos++] = *cp;
				}
			}

			break;
		}
	case 1:
		{
			int c1 = pcszIn[nInPos] & 0xFF;

			pszOut[nOutPos++] = szBase64Tab[(c1 & 0xFC) >> 2];
			pszOut[nOutPos++] = szBase64Tab[((c1 & 0x03) << 4)];
			pszOut[nOutPos++] = '=';
			pszOut[nOutPos++] = '=';

			if (nNeedCRLF)
			{
				char *cp = BASE64_EOL;
				pszOut[nOutPos++] = *cp++;

				if (*cp) 
				{
					pszOut[nOutPos++] = *cp;
				}
			}

			break;
		}
	case 2:
		{
			int c1 = pcszIn[nInPos++] & 0xFF;
			int c2 = pcszIn[nInPos] & 0xFF;

			pszOut[nOutPos++] = szBase64Tab[(c1 & 0xFC) >> 2];
			pszOut[nOutPos++] = szBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
			pszOut[nOutPos++] = szBase64Tab[((c2 & 0x0F) << 2)];
			pszOut[nOutPos++] = '=';

			if (nNeedCRLF)
			{
				char *cp = BASE64_EOL;
				pszOut[nOutPos++] = *cp++;

				if (*cp)
				{
					pszOut[nOutPos++] = *cp;
				}
			}

			break;
		}

	default: 
		{
			//assert(false);
			break;
		}
	}

	*pnReturnSize = nOutPos;

	ASSERT(nOutPos < nOutSizeMax);

	if (nOutPos < nOutSizeMax)
		*(pszOut + nOutPos) = '\0';

	nResult = true;

Exit0:

	return nResult;
}


class KInfoCThreadGuard
{
public:
	KInfoCThreadGuard()
	{
		::InterlockedIncrement(&g_lInfoCThreadNumber);
	};
	~KInfoCThreadGuard()
	{
		::InterlockedDecrement(&g_lInfoCThreadNumber);
	}
};

BOOL InitInfoC()
{
	if (!g_HttpInited)
	{
		if (g_strInfoC.empty())
		{
			std::wstring  strInfocUrl;
			CString strUUidex;

			strUUidex = CKavData::Instance()->GetUUIDEx();
			strInfocUrl = DEF_STR_INFOC_URL;

			g_strInfoC += strInfocUrl;
			g_strInfocParam.clear();

			g_strInfocParam = _T("passport=");
			g_strInfocParam.append(_T("&uuidex="));
			g_strInfocParam.append(strUUidex);
			g_strInfocParam.append( _T("&vip="));
			g_strInfocParam.append(_T("&pid="));
			g_strInfocParam.append(CKavData::Instance()->GetPid());
			g_strInfocParam.append(_T("&tid1="));
			g_strInfocParam.append(CKavData::Instance()->Gettid1());
			g_strInfocParam.append(_T("&tid2="));
			g_strInfocParam.append(CKavData::Instance()->Gettid2());
			g_strInfocParam.append(_T("&tod1="));
			g_strInfocParam.append(CKavData::Instance()->Gettod1());
			g_strInfocParam.append(_T("&tod2="));
			g_strInfocParam.append(CKavData::Instance()->Gettod2());
			g_strInfocParam.append(_T("&type=6"));
			g_strInfocParam.append(_T("&tryno="));
			g_strInfocParam.append( CKavData::Instance()->GetTryOut());
			g_strInfocParam.append(_T("&lastver="));
			g_strInfocParam.append(CKavData::Instance()->GetAppVer());

			g_strInfoC += g_strInfocParam;

			g_HttpInited = TRUE;
		}
	}

	return g_HttpInited;
}

u_int WINAPI InfocCollectThread(LPVOID pParam)
{
	KInfoCThreadGuard _InfoCGuard;
	stInfocParamElem* _pszAddData = (stInfocParamElem*)pParam;
	std::wstring	 str,strAllDataTemp;

	if (_pszAddData != NULL && InitInfoC())
	{
		std::wstring strInfoC(g_strInfoC);
		strInfoC.append(_T("&"));
		strInfoC.append(_pszAddData->szParamStr);
		if (!g_bRunOnce)
		{
			str = g_strInfocParam  ;
			str.append(_pszAddData->szParamStr);
			InfocWriteData infoc;
			infoc.EncyptData(str ,strAllDataTemp);

			strInfoC = DEF_STR_INFOC_URL;
			strInfoC.append(_T("01_________"));
			strInfoC.append(strAllDataTemp);

			g_nNetworkErrCode = WinHttp_RequestHttpFile(strInfoC.c_str());
		}
		if ((!g_bRunOnce && g_nNetworkErrCode != 0) ||(g_nNetworkErrCode == 0 && g_bRunOnce))
		{
			std::wstring strFileData(g_strInfocParam);
			strFileData.append(_T("&"));
			strFileData.append(_pszAddData->szParamStr);

			std::wstring strFileName;
			strFileName = _T("common_");
			strFileName += _pszAddData->szActionName;

			InfocWriteData infoc;
			infoc.WriteStringToFile(strFileName, strFileData);
		}
		
		delete _pszAddData;
		_pszAddData = NULL;
	}

	return 0;
}

void KInfoCollect::InfocCollect_Initialize()
{
	InitInfoC();
}
void KInfoCollect::InfocCollect_Uninitialize()
{
	int uCount = 10;				

	do
	{
		if (0 >= g_lInfoCThreadNumber)
		{
			break;
		}

		if (0 >= --uCount)
		{
			break;
		}

		::Sleep(500);

	}while (true);

}

void KInfoCollect::InfocCollectInstaller_Active(IN int nstep, IN int nInstallType, IN int time, IN BOOL bIsNeedCreateThread)
{	
	u_int uTid = 0;

	stInfocParamElem* pElem = new stInfocParamElem;
	if (pElem)
	{
		_stprintf_s(pElem->szParamStr, MAX_PATH, _T("actionname=duba_install&point=%d&installtype=%d&time=%d"), nstep, nInstallType, time);
		
		_tcscpy_s(pElem->szActionName, _T("duba_install"));

		if (bIsNeedCreateThread)
			_beginthreadex(NULL, 0, InfocCollectThread, (LPVOID)pElem, 0, &uTid);
		else
			InfocCollectThread((LPVOID)pElem);
	}
}

void KInfoCollect::InfocCollectUnInstaller(IN int ndays, CString strchoice, IN BOOL bIsNeedCreateThread)
{	
	u_int uTid = 0;

	stInfocParamElem* pElem = new stInfocParamElem;
	if (pElem)
	{
		SYSTEMTIME st;
		CString strDate;
		if (ndays < 0)
			ndays = 0;
		
		GetLocalTime(&st);
		strDate.Format(_T("%4d-%d-%d-%d-%d"),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute);
		
		_stprintf_s(pElem->szParamStr, MAX_PATH, _T("actionname=duba_uninstall&days=%d&choice=%s&collecttime=%s"),  ndays, strchoice, strDate);

		_tcscpy_s(pElem->szActionName, _T("duba_uninstall"));

		if (bIsNeedCreateThread)
			_beginthreadex(NULL, 0, InfocCollectThread, (LPVOID)pElem, 0, &uTid);
		else
			InfocCollectThread((LPVOID)pElem);
	}
}

void KInfoCollect::InfocCollectFeedback(CString strword, CString strqq,  int nforumclickIN, BOOL bIsNeedCreateThread)
{
	u_int uTid = 0;

	stInfocParamElem* pElem = new stInfocParamElem;
	if (pElem)
	{
		SYSTEMTIME st;
		CString strDate;

		_stprintf_s(pElem->szParamStr, MAX_PATH * 2, _T("actionname=duba_uninword&word=%s&qq=%s&forumclick=%d"),  strword, strqq, nforumclickIN);

		_tcscpy_s(pElem->szActionName, _T("duba_uninword"));

		if (bIsNeedCreateThread)
			_beginthreadex(NULL, 0, InfocCollectThread, (LPVOID)pElem, 0, &uTid);
		else
			InfocCollectThread((LPVOID)pElem);
	}
}
void KInfoCollect::AddData(std::wstring sActionName,IN std::wstring sData)
{
	u_int uTid = 0;

	stInfocParamElem* pElem = new stInfocParamElem;
	if (pElem)
	{
		_tcscpy_s(pElem->szParamStr, MAX_PATH, sData.c_str());
		_tcscpy_s(pElem->szActionName, sActionName.c_str());
		_beginthreadex(NULL, 0, InfocCollectThread, (LPVOID)pElem, 0, &uTid);
	}

}

void InfocWriteData::_GetTimeForInfoc( OUT DWORD& dwTime )
{
	//dwTime = _time32(NULL);
	dwTime = ::GetTickCount();
}

void InfocWriteData::_GetTimeForInfoc( OUT std::wstring& strTime )
{
	strTime.clear();
	DWORD dwTime;
	_GetTimeForInfoc(dwTime);

	WCHAR szTime[32] = { 0 };
	int nRet = ::_ltow_s(dwTime, szTime, 32, 10);
	if (nRet == 0)
		strTime = szTime;
}

void InfocWriteData::_GetRandData( OUT std::wstring& strRand )
{
	int nData = rand() % 10000 + 1;
	nData += rand() % 10000 + 1;
	nData /= 2;

	WCHAR szTime[32] = { 0 };
	int nRet = ::_ltow_s(nData, szTime, 32, 10);
	if (nRet == 0)
		strRand = szTime;
}

BOOL InfocWriteData::_Insert( IN const std::wstring& strActionName, IN const std::wstring& strAllData )
{
	//得到一个文件路径：（文件路径组成：表名+时间+10000之间的随机数
	std::wstring strCurFilePath ;
	int nRetCode = KFunction::GetKavAppDataPath(strCurFilePath);
	strCurFilePath += _T("KCLT");

	KFunction::CreateDeepDirectory(strCurFilePath.c_str());
	
	strCurFilePath += _T("\\");
	strCurFilePath += strActionName;
	std::wstring strCutTime;
	_GetTimeForInfoc(strCutTime);
	strCurFilePath += strCutTime;
	
	std::wstring strRandData;
	std::wstring strTempFilePath = strCurFilePath;
	srand(NULL);
	std::wstring strAllDataTemp ;
	EncyptData(strAllData, strAllDataTemp);

	do 
	{
		strTempFilePath = strCurFilePath;
		_GetRandData(strRandData);
		strTempFilePath += strRandData;
		strTempFilePath += DEFFILEPOSTFIX;

		if (!PathFileExists(strTempFilePath.c_str()))
		{
			CFileOutStream file(strTempFilePath.c_str());
			u_int uRet = 0;
			file.Write((byte *)(strAllDataTemp.c_str()), strAllDataTemp.size() * 2, &uRet);
			break;
		}

	} while(TRUE);


	return TRUE;
}

int InfocWriteData::KA2W(IN LPCSTR pszAString, IN int nszAStringSize, OUT LPWSTR pszWString, IN int nszWStringSize)
{
	int nResult = false;
	int nRetCode = false;

	nRetCode = MultiByteToWideChar(CP_UTF8, 0, pszAString, nszAStringSize, (LPWSTR)pszWString, nszWStringSize);
	KAV_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	return nResult;
}

int InfocWriteData::KW2A(IN LPCWSTR pszWString, IN int nszWStringSize, OUT LPSTR pszAString, IN int nszAStringSize)
{
	int nResult = false;
	int nRetCode = false;

	nRetCode = WideCharToMultiByte (CP_OEMCP, NULL, pszWString, nszWStringSize, pszAString, nszAStringSize, NULL, FALSE);
	KAV_PROCESS_ERROR(nRetCode);

	nResult = true;
Exit0:
	return nResult;

}

int InfocWriteData::KW2A(IN const std::wstring & strWstring, OUT std::string & strAstring)
{
	int nResult = false;
	int nRetCode = false;

	char* pOutString = NULL;

	int nOutSize = (strWstring.size() + 1) * 2;
	pOutString = new char[nOutSize];
	KAV_PROCESS_ERROR(pOutString);

	memset(pOutString, 0, nOutSize);

	nRetCode = KW2A(strWstring.c_str(), (int)strWstring.size(), pOutString, (int)nOutSize);
	KAV_PROCESS_ERROR(nRetCode);

	strAstring = pOutString;

	nResult = true;
Exit0:
	KAV_DELETE_ARRAY(pOutString);
	return nResult;
}

int InfocWriteData::KA2W(IN const std::string & strAstring, OUT std::wstring& strWstring)
{
	int nResult = false;
	int nRetCode = false;

	WCHAR* pOutString = NULL;

	pOutString = new WCHAR[strAstring.size() + 1];
	KAV_PROCESS_ERROR(pOutString);

	wmemset(pOutString, 0, strAstring.size() + 1);

	nRetCode = KA2W(strAstring.c_str(), (int)strAstring.size(), pOutString, (int)strAstring.size());
	KAV_PROCESS_ERROR(nRetCode);

	strWstring = pOutString;

	nResult = true;
Exit0:
	KAV_DELETE_ARRAY(pOutString);
	return nResult;

}
int InfocWriteData::Base64EnCode(IN const std::wstring& strSource, OUT std::wstring& strDes )
{
	int nResult = false;
	int nRetCode = false;

	char* pszOut = NULL;
	int nRetEncodeSize = 0;

	int nszSize = 0;//(strSource.size() + 1) * 2;
	int nszSizeRet = 0;//nszSize * 4 / 3 + 1;

	std::string strIn;
	std::string strOut;

	nRetCode = KW2A(strSource, strIn);
	KAV_PROCESS_ERROR(nRetCode);

	nszSize = strIn.size();
	nszSizeRet = nszSize * 2;
	pszOut = new char[nszSizeRet];
	::ZeroMemory(pszOut, nszSizeRet);

	nRetCode = Base64Encode(strIn.c_str(), nszSize, pszOut, nszSizeRet, &nRetEncodeSize, false);
	KAV_PROCESS_ERROR(nRetCode);

	strOut = pszOut;
	nRetCode = KA2W(strOut, strDes);
	KAV_PROCESS_ERROR(nRetCode);


	nResult = true;

Exit0:
	KAV_DELETE(pszOut);

	return nResult;
}

int InfocWriteData::_FormatByteToStr(BYTE byData, char szStr[2])
{
	BYTE byHighOfData = 0;
	BYTE byLowOfData = 0;

	memset(szStr, 0, 2 * sizeof(char));

	byHighOfData = byData / 16;
	byLowOfData = byData % 16;

	if (byHighOfData < 10)
		szStr[0] = ('0') + byHighOfData;
	else
		szStr[0] = ('A') + byHighOfData - 10;

	if (byLowOfData < 10)
		szStr[1] = ('0') + byLowOfData;
	else
		szStr[1] = ('A') + byLowOfData - 10;

	return true;
}

std::string InfocWriteData::_EscapeEncoding(int nDataSize, BYTE *pbyDataBuff)
{
	std::string sReturnStr;

	char *pbySearchPoint = NULL;
	char szBinaryNum[3] = { 0 };

	for (int i = 0; i < nDataSize; i++)
	{
		if ( (pbyDataBuff[i] < ('0')) ||
			((pbyDataBuff[i] > ('9')) && pbyDataBuff[i] < ('A')) ||
			((pbyDataBuff[i] > ('Z')) && pbyDataBuff[i] < ('a')) ||
			(pbyDataBuff[i] > ('z')))
		{
			_FormatByteToStr(pbyDataBuff[i], szBinaryNum);

			sReturnStr += "%";
			sReturnStr += szBinaryNum;
		}
		else
			sReturnStr += pbyDataBuff[i];
	}

	return sReturnStr;
}

int InfocWriteData::InfocUrlCode( IN std::wstring strSource, OUT std::wstring& strDes )
{
	int nRetturn = false;

	std::string strSourceTemp;
	std::string strUrlEncode;
	int nRetCode = KW2A(strSource, strSourceTemp);
	KAV_PROCESS_ERROR(nRetCode);

	strUrlEncode = _EscapeEncoding(strSourceTemp.size(), (BYTE *)(strSourceTemp.c_str()));

	nRetCode = KA2W(strUrlEncode, strDes);
	KAV_PROCESS_ERROR(nRetCode);

	nRetturn = true;

Exit0:
	return nRetturn;
}

void InfocWriteData::EncyptData( IN std::wstring strInData, OUT std::wstring& strOutData )
{
	
	std::wstring strOutDataTemp;
	if (true != Base64EnCode(strInData, strOutDataTemp))
	{
		//__MsgLogEx(((L"infoc_base64 encode fail! the string is %s"), strInData));
		return ;
	}

	if (true != InfocUrlCode(strOutDataTemp, strOutData))
	{
		//__MsgLogEx(((L"infoc_base64 encode fail! the string is %s"), strInData));
		//return;	
	}
}

BOOL InfocWriteData::WriteStringToFile( IN std::wstring strActionName, IN std::wstring strFileData )
{
	return _Insert(strActionName, strFileData);
}

