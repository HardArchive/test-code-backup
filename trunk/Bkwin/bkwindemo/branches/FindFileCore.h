#ifndef __KFindFileCore_h__
#define __KFindFileCore_h__

#include <tchar.h>

/*
@ FuncName	: PFNFindCallBack
@ Param		: strPathName �ļ���·������:c:\data\a.dat
@ Param		: nFile ��ʾstrPathName���ļ�����·��,�μ�enumFileType
@ Return	: ����TRUE��ʾҪ�������ң�FALSE��ʾ�����ٲ�����
*/
typedef BOOL (* PFNFindCallBack)(const CString& strPathName, int nFile, LPVOID param);

enum enumFileType
{
	enumFindFileAndDir = 0,		//�����ļ����ļ���
	enumFindFile,			//��Ҫ�ļ�
	enumFindDir,			//��Ҫ�ļ���
};

class KFindFileCore
{
public:
	KFindFileCore();
	~KFindFileCore();

public:
	void Stop(void);
	
	void SetCallback(PFNFindCallBack callback, LPVOID param);
	/*
	@ FuncName	: Find
	@ Param		: strPath Ҫ������·��
	@ Param		: strExpression �����ı��ʽ���磺a?b*.dat
	@ Param		: nFile 0��ʾ�����ļ����ļ��У�1��ʾֻҪ�ļ���2��ʾֻҪ�ļ���
	@ Param		: nDepth ����Ŀ¼��ȡ�0��ʾֻ������ǰĿ¼��1ָ��һ��Ŀ¼��2������-1��ʾ����Ŀ¼
	*/
	void Find(const TCHAR* szPath, const TCHAR* szExpression = _T("*"), int nFile = enumFindFile, int nDepth = -1);

protected:
	/*
	@ FuncName	: MakePath
	@ Brief		: ȷ��strPath��'\'����'/'��β
	*/
	void MakePath(CString& strPath);
	
	/*
	@ FuncName	: Find
	@ Param		: strPath Ҫ������·��
	@ Param		: strExpression �����ı��ʽ���磺a?b*.dat
	@ Param		: nFile 0��ʾ�����ļ����ļ��У�1��ʾֻҪ�ļ���2��ʾֻҪ�ļ���
	@ Param		: nDepth ����Ŀ¼��ȡ�0��ʾֻ������ǰĿ¼��1ָ��һ��Ŀ¼��2������-1��ʾ����Ŀ¼
	@ Return	: ����m_callback�ķ���ֵ����ʾ�Ƿ�Ҫ��������
	*/
	BOOL __Find(CString strPath, CString& strExpression, int nFile, int nDepth);

	
private:
	BOOL			m_bStop;
	PFNFindCallBack m_callback;
	LPVOID			m_param;
};

#endif