#pragma once

#include "GeneralHelper.h"

#include <tlhelp32.h>
#include <malloc.h>

#define PATH_SEPARATOR				_T("\\")
#define PATH_SEPARATOR_CHAR			_T('\\')
#define FILE_EXTEND_SEPARATOR		_T(".")
#define FILE_EXTEND_SEPARATOR_CHAR	_T('.')
#define DISK_SYMBLE					_T(":")
#define DISK_SYMBLE_CHAR			_T(':')
#define EXE_FILE_EXTEND_NAME		_T(".exe")

enum EnCodePage
{
	XCP_ACP		= CP_ACP,
	XCP_GB2312	= 936,
#ifdef _WIN32_WCE
	XCP_BIG5	= CP_ACP,
#else
	XCP_BIG5	= 950,
#endif
	XCP_GBK		= XCP_GB2312,
	XCP_UTF7	= CP_UTF7,
	XCP_UTF8	= CP_UTF8,
};

/************************************* �ַ����� *************************************/

//���ַ�ת�������ʾ��ֵ: ���ַ� 'F' ת���� ��ֵ 15
#define CHARTOVALUE(c)				(c <= '9' ? c - '0' : (c <= 'F' ? c - 'A' + 0x0A : c - 'a' + 0X0A))
//��˫�ַ�ת�������ʾ��ֵ: ���ַ� '1F' ת���� ��ֵ 31
#define DOUBLECHARTOVALUE(pc)		(((CHARTOVALUE(*(pc))) << 4) | (CHARTOVALUE(*(pc + 1))))

//����ֵת������ʮ�������ַ�: ����ֵ 15 ת���� �ַ� 'F'
#define VALUETOCHAR(n)				(n <= 9 ? n + '0' : (n <= 'F' ? n + 'A' - 0X0A : n + 'a' - 0X0A))
//����ֵ�ַ�ת����������ʮ�������ַ�: ����ֵ 11 ת���� �ַ� '0B'
#define VALUETODOUBLECHAR(pc, n)	{*(pc) = VALUETOCHAR((n >> 4)); *((pc) + 1) = VALUETOCHAR((n & 0X0F));}

//��˫�ַ�ת�������ʾ��ֵ: ���ַ� '1F' ת���� ��ֵ 31
BYTE DoubleCharToByte(LPCTSTR psValue);
//����ֵ�ַ�ת����������ʮ�������ַ�: ����ֵ 11 ת���� �ַ� '0B'
LPTSTR ByteToDoubleChar(BYTE b, LPTSTR des);
//��ʮ������ת�������ʾ��ֵ: ���ַ� '601F' ת���� ��ֵ 24607
UINT HexStrToInt(LPCTSTR pHexText, int len = -1);
//��ʮ����ת�������ʾ��ֵ: ���ַ� '0000024607' ת���� ��ֵ 24607
UINT DecStrToInt(LPCTSTR pDecText, int len = -1);
//����ֵ������ʮ�������ַ�����ʾ��ֵ: ����ֵ 24607 ת���� �ַ� '601F'
CString& IntToHexStr(CString& dest, UINT v, int len = -1);
//����ֵ������ʮ�����ַ�����ʾ��ֵ: ����ֵ 24607 ת���� �ַ� '24607'
CString& IntToDecStr(CString& dest, UINT v, int len = -1);
//��ʮ�����Ʊ�ʾ�ĵ�ַ��Ϊʮ���Ʊ�ʾ�ĵ�ַ: �� "000C35CE" ת���� '0000800236'
CString& HexAddrToDecAddr(CString& dest, LPCTSTR src, int destlen = 10, int srclen = -1);
//��ʮ���Ʊ�ʾ�ĵ�ַ��Ϊʮ�����Ʊ�ʾ�ĵ�ַ: �� "0000800236" ת���� '000C35CE'
CString& DecAddrToHexAddr(CString& dest, LPCTSTR src, int destlen = 8, int srclen = -1);

//Code Page Name -> Code Page Value
EnCodePage GetCodePageByName(LPCTSTR lpszCodePageName);

//MBCS -> UNICODE
BOOL MbcsToUnicode(const char* pszInString, WCHAR** ptrOutWStr, int& nSizeCount);
//UNICODE -> MBCS
BOOL UnicodeToMbcs(const WCHAR* pwzInString, char** ptrOutStr, int& nSizeCount);
//UTF8 -> UNICODE
BOOL Utf8ToUnicode(const char* pszInString, WCHAR** ptrOutWStr, int& nSizeCount);
//UNICODE -> UTF8
BOOL UnicodeToUtf8(const WCHAR* pwzInString, char** ptrOutStr, int& nSizeCount);
//CP_XXX -> UNICODE
BOOL CPToUni(const char* pszInString, WCHAR** ptrOutWStr, unsigned int nCodePage, int& nSizeCount);
//UNICODE -> CP_XXX
BOOL UniToCP(const WCHAR* pwzInString, char** ptrOutStr, unsigned int nCodePage, int& nSizeCount);

//���ֽ�����ת����16�����ַ���
int BytesToHex(const BYTE* pBytes, int nLength, LPTSTR* lpszDest);
//��16�����ַ���ת�����ֽ�����
int HexToBytes(LPCTSTR lpszHex, BYTE** ppBytes, int* pnLength);

//����ͨ�ַ���ת����16�����ַ���
CString& StrToHex(const TCHAR* src, CString& des);
//��16�����ַ���ת������ͨ�ַ���
CString& HexToStr(const TCHAR* src, CString& des);
//����ͨ�ַ���ת����UTF8�ַ���, Ȼ���ٰѸ�UTF8�ַ���ת����16�����ַ���
CString& StrToUtf8Hex(const TCHAR* src, CString& strDec);
//��16�����ַ���ת����UTF8�ַ���, Ȼ���ٰѸ�UTF8�ַ���ת������ͨ�ַ���
CString& HexUtf8ToStr(const TCHAR* src, CString& strDec);

//ȡ��ϵͳ��������������Ϣ
CString GetSystemErrorDesc(DWORD dwCode);

//�ָ��ַ���
BOOL SplitStr(LPCTSTR pszSrc, vector<CString>& vtItem, LPCTSTR pszSepectors = NULL, LPCTSTR pszQMarks = NULL);

//��ȡ�ļ�����
CString ExtractFileName(LPCTSTR lpszFullFileName);
//��ȡ�ļ�·��
CString ExtractPath(LPCTSTR lpszFullFileName);
//��ȡ��ǰģ��·��
CString ExtractModulePath(HMODULE hModule = NULL);

//��������
BOOL RunProcess(LPCTSTR sFileName, LPCTSTR cmdline = NULL, BOOL bHide = TRUE, LPCTSTR dir = NULL, BOOL bWait = TRUE, DWORD dwWaitTime = INFINITE);
BOOL ShellRunExe(LPCTSTR lpszPath, LPCTSTR lpszParams = NULL, int iShow = SW_SHOWNORMAL, HANDLE* phProcess = NULL, BOOL bWait = FALSE, DWORD dwWaitTime = INFINITE);

//���ҽ���
BOOL FindProcess(LPCTSTR pszProcessName);

//���ҽ��̾��
HANDLE FindProcessHandle(LPCTSTR pszProcessName, DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION, BOOL bInheritHandle = FALSE);

typedef map<DWORD, PROCESSENTRY32*>									ProcessInfos;
typedef MapWrapper<ProcessInfos, PtrMap_Cleaner<ProcessInfos>>		ProcessInfoMap;

//���ҵ�ǰ���е���Ϣ
BOOL FindRunningProcessesInfo(ProcessInfoMap& infoMap);

//���ҽ��̵�������
HWND FindProcessMainWindow(HANDLE hProcess, LPCTSTR lpszClassName = NULL);

BOOL TerminateProcessFairily(HANDLE hProcess, DWORD dwWait = INFINITE);

//�򵥼�¼��־
void WriteLog(LPCTSTR pszLogFileName, LPCTSTR pszLog);

#ifndef _WIN32_WCE
	//���õ�ǰ·��
	BOOL SetCurrentPathToModulePath(HMODULE hModule = NULL);
	//���ҽ���
	BOOL FindProcessEx(LPCTSTR pszProcessName);
	//��ȡ INI ����
	CString GetIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpFileName, DWORD dwSize);
#endif

	//��ȡ����ϵͳ�汾
	DWORD	GetOSVersion();
	//��ȡ CPU ����
	DWORD	GetCpuCount();

#ifdef _AFX
	//��ת����ʱ���ַ���
	CString SecondToTimeStr(DWORD dwSeconds, BOOL bDayOnly = FALSE);
#endif

//����ע�����
BOOL GetRegistryValue(HKEY hRoot, LPCTSTR wcSubKey, LPCTSTR wcName, LPBYTE pValue, DWORD* pdwSize, DWORD* pdwType);
//��ȡע�����
BOOL SetRegistryValue(HKEY hRoot, LPCTSTR wcSubKey, LPCTSTR wcName, LPBYTE pValue, DWORD dwSize, DWORD dwType);

//�ַ���ʱ��תΪDATE
DATE string2date(const char* ptInTime);
/////////////////IP������///////////////////
//IPֵתIP
string ValueToIP(const int& nValue);
//IPתIPֵ
int IPToValue(const string& strIP);
//IP�ַ�����0  eg:192.168.30.107->192.168.030.107
void AddZeroIP(string strInIP, string& strOutIP);
//MAC��ַȥ�ֺ� eg:00:1F:D0:C1:EF:75->001FD0C1EF75
string GetMAC(char* pInMac);   
//IP�ַ���ȥ0
void MinusZeroIP(string strInIP, string& strOutIP);
//IPֵת��IP����0
string ValueToIPZero(const int& nValue);
//��ȡ��ǰ��������Ŀ¼ �ɹ�����true��ʧ�ܷ���false
bool GetExePath(TCHAR* ptInPath);
//�����༶Ŀ¼���ɹ�����true�� ʧ�ܷ���false
bool CreateMultipleDirectory(const TCHAR* ptInPath, bool bIsFileName);