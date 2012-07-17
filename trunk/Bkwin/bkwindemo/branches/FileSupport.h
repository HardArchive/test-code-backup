/********************************************************************
	created:	2008/09/24
	filename: 	FileSupport.h
	file base:	FileSupport
	file ext:	h
	author:		huchao
	
	purpose:	Windows �ļ������ķ�װ��ʹ�����ļ���·��������
*********************************************************************/
#pragma once
#include <Windows.h>
#include <atlstr.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Version.lib")

#ifndef UNICODE
#define UNICODE
#endif

#pragma warning (disable: 4996)
#pragma warning (disable: 4102)

class CFileSupport
{
private:
	CFileSupport(void);
	~CFileSupport(void);

public:	// �����ļ�����

	// ���ܣ�ɾ��ָ��Ŀ¼���ļ����ļ��У�ֻ���ļ�Ҳ��ɾ����
	// ����˵��
	// lpszFullPath��ָ��Ҫɾ�����ļ����ļ���ȫ·��
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ����ļ�������Ҳ���سɹ���
	static BOOL DeleteFileEx(
		IN LPCWSTR lpszFullPath);

	// ���ܣ����ļ������ʶ���ȵ��´�����ʱ��ɾ��
	// ����˵��
	// lpszFullPath��ָ��Ҫɾ���ļ���ȫ·�����������ļ��У�
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ����ļ�������Ҳ���سɹ���
	static BOOL DeleteFileUntilReboot(
		IN LPCWSTR lpszFullPath);

	// ���ܣ����ļ������ʶ���ȵ��´�����ʱ���滻
	// ����˵��
	// lpszExistingFileName����Ҫ�滻���ļ�
	// lpNewFileName�����ļ�
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ����ļ�������Ҳ����TRUE��
	static BOOL MoveFileUntilReboot(
		IN LPCWSTR lpszExistingFileName, 
		IN LPCWSTR lpNewFileName);


public:	// ��ȡ�ļ���Ϣ����

	// ���ܣ����ָ��Ŀ¼���ļ����ļ��еĴ�С����λ��byte��
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// lpFileSizeHigh���ļ���˫�ֵĴ�С
	// ����ֵ���ļ���С��byte��
	static DWORD GetFileSize(
		IN LPCWSTR lpszFullPath, 
		OUT LPDWORD lpFileSizeHigh = NULL);

	// ���ܣ����ָ���ļ��İ汾��
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// pwVer1���汾�ŵĵ� 1 ��ֵ
	// pwVer2���汾�ŵĵ� 2 ��ֵ
	// pwVer3���汾�ŵĵ� 3 ��ֵ
	// pwVer4���汾�ŵĵ� 4 ��ֵ
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL GetFileVersion(
		IN LPCWSTR lpszFullPath, 
		OUT WORD* pwVer1, 
		OUT WORD* pwVer2, 
		OUT WORD* pwVer3, 
		OUT WORD* pwVer4);

	// ���ܣ����ָ���ļ��İ汾���ַ���
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// lpszVersion���ļ��İ汾�ţ������ڣ�2008.9.24.625��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL GetFileVersionString(
		IN LPCWSTR lpszFullPath, 
		OUT LPWSTR lpszVersion);

	// ���ܣ����ָ���ļ��İ汾���ַ���
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// ����ֵ���ļ��İ汾�ţ������ڣ�2008.9.24.625��
	static CString GetFileVersionString(
		IN LPCWSTR lpszFullPath);

	// ���ܣ����ָ���ļ��Ĵ���ʱ��
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// lpTime���ļ��Ĵ���ʱ��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL GetFileCreationTime(
		IN LPCWSTR lpszFullPath, 
		OUT LPSYSTEMTIME lpTime);

	// ���ܣ����ָ���ļ�����ʱ��
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// lpTime���ļ��ķ���ʱ��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL GetFileLastAccessTime(
		IN LPCWSTR lpszFullPath, 
		OUT LPSYSTEMTIME lpTime);

	// ���ܣ����ָ���ļ��޸�ʱ��
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// lpTime���ļ����޸�ʱ��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL GetFileLastWrite(
		IN LPCWSTR lpszFullPath, 
		OUT LPSYSTEMTIME lpTime);


public:	// �ļ�У�����

	// ���ܣ����ָ���ļ��� CRC ֵ
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// ����ֵ���ļ���CRC��0--ʧ��
	static UINT GetFileCRC(
		IN LPCWSTR lpszFullPath);


public:	// Ŀ¼����

	// ���ܣ������༶Ŀ¼��ֱ������㣨���������ļ���ͬ�����ļ�����ɾ��ͬ�����ļ���
	// ����˵��
	// lpszFullPath��ָ�����ļ���ȫ·��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL CreateDirectoryEx(
		IN LPCWSTR lpszFullPath, 
		IN BOOL bDelSameNameFile = FALSE);


public:	// �ļ�·������

	// ���ܣ���õ�ǰ�ļ�������·��
	// ����˵��
	// lpszFullPath�������ļ���·��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL GetCurFilePath(
		OUT LPWSTR lpszFullPath);

	// ���ܣ���õ�ǰ�ļ�������·��
	// ����ֵ���ļ���·��
	static CString GetCurFilePath(void);

	// ���ܣ���õ�ǰ����exe�ļ�������
	// ����˵��
	// lpszFullPath�������ļ�������
	// FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL GetCurFileName(
		OUT LPWSTR lpszFullPath);

	// ���ܣ���õ�ǰ����exe�ļ�������
	// ����ֵ���ļ�������
	static CString GetCurFileName(void);

	// ���ܣ���·�������� '\\'
	// ����˵��
	// lpszFullPath����Ҫ��ӵ�·��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL PathAddBackslash(
		IN OUT LPWSTR lpszFullPath);

	// ���ܣ���·�������� '\\'
	// ����˵��
	// strFullPath����Ҫ��ӵ�·��
	// ����ֵ����� '\\' ���·��
	static CString PathAddBackslashStr(
		IN const CString& strFullPath);

	// ���ܣ�ȥ��·������'\\'
	// ����˵��
	// lpszFullPath����Ҫȥ����·��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL PathRemoveBackslash(
		IN OUT LPWSTR lpszFullPath);

	// ���ܣ�ȥ��·������'\\'
	// ����˵��
	// strFullPath����Ҫȥ����·��
	// ����ֵ��ȥ�����·��
	static CString PathRemoveBackslashStr(
		IN const CString& strFullPath);

	// ���ܣ���·��������'/'
	// ����˵��
	// lpszFullPath����Ҫ��ӵ�·��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL PathAddSlash(
		IN OUT LPWSTR lpszFullPath);

	// ���ܣ���·��������'/'
	// ����˵��
	// strFullPath����Ҫ��ӵ�·��
	// ����ֵ����� '/' ���·��
	static CString PathAddSlashStr(
		IN const CString& strFullPath);

	// ���ܣ�ȥ��·������'/'
	// ����˵��
	// lpszFullPath����Ҫȥ����·��
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL PathRemoveSlash(
		IN OUT LPWSTR lpszFullPath);

	// ���ܣ�ȥ��·������'/'
	// ����˵��
	// strFullPath����Ҫȥ����·��
	// ����ֵ��ȥ�����·��
	static CString PathRemoveSlashStr(
		IN const CString& strFullPath);

	// ���ܣ����ַ��� lpszFullPath �� cOrg �ַ�����Ϊ cDest �ַ�
	// ����˵��
	// lpszFullPath����Ҫת�����ַ���
	// cDest��Ŀ���ַ�
	// cOrg��ԭ�ַ�
	// ����ֵ��FALSE--ʧ�ܣ�TRUE--�ɹ�
	static BOOL RelpacePathChar(
		IN OUT LPWSTR lpszFullPath, 
		IN const WCHAR cDest, 
		IN const WCHAR cOrg);

	// ���ܣ����ַ��� strFullPtah �� cOrg �ַ�����Ϊ cDest �ַ�
	// ����˵��
	// strFullPtah����Ҫת�����ַ���
	// cDest��Ŀ���ַ�
	// cOrg��ԭ�ַ�
	// ����ֵ��ת������ַ���
	static CString RelpacePathCharStr(
		IN const CString& strFullPtah, 
		IN const WCHAR cDest, 
		IN const WCHAR cOrg);

	// ���ܣ���� Windows Temp Ŀ¼��Ψһ���ļ���
	// ����˵��
	// lpszTempFile��Windows Temp ·����Ψһ���ļ����ַ���
	static BOOL GetTempFileName(
		OUT LPWSTR lpszTempFile);

	// ���ܣ���� Windows Temp Ŀ¼��Ψһ���ļ���
	// ����ֵ��Windows Temp ·����Ψһ���ļ����ַ���
	static CString GetTempFileName();

private:
	static BOOL _GetFileVersion(LPCWSTR lpszFullPath, WORD* pwVer1, WORD* pwVer2, WORD* pwVer3, WORD* pwVer4);

	static BOOL _GetFileTime(LPCWSTR lpszFullPath,
		LPFILETIME lpCreationTime, 
		LPFILETIME lpLastAccessTime,
		LPFILETIME lpLastWriteTime);

	static BOOL _CreateDirectory(LPCWSTR lpszDir, BOOL bDelSameNameFile);

	static CString _ConfirmPathString(const CString& strPath);

	static BOOL _DeleteFileEx(LPCWSTR lpszFullPath);

	static BOOL _GetCurFilePath(LPWSTR lpszFullPath);

	static BOOL _GetCurFileName(LPWSTR lpszFullPath);
};
