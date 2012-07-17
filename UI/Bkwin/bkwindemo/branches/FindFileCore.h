#ifndef __KFindFileCore_h__
#define __KFindFileCore_h__

#include <tchar.h>

/*
@ FuncName	: PFNFindCallBack
@ Param		: strPathName 文件的路径名如:c:\data\a.dat
@ Param		: nFile 表示strPathName是文件还是路径,参见enumFileType
@ Return	: 返回TRUE表示要继续查找，FALSE表示不用再查找了
*/
typedef BOOL (* PFNFindCallBack)(const CString& strPathName, int nFile, LPVOID param);

enum enumFileType
{
	enumFindFileAndDir = 0,		//所有文件和文件夹
	enumFindFile,			//仅要文件
	enumFindDir,			//仅要文件夹
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
	@ Param		: strPath 要搜索的路径
	@ Param		: strExpression 搜索的表达式，如：a?b*.dat
	@ Param		: nFile 0表示所有文件和文件夹；1表示只要文件；2表示只要文件夹
	@ Param		: nDepth 搜索目录深度。0表示只搜索当前目录，1指深一级目录，2……；-1表示所有目录
	*/
	void Find(const TCHAR* szPath, const TCHAR* szExpression = _T("*"), int nFile = enumFindFile, int nDepth = -1);

protected:
	/*
	@ FuncName	: MakePath
	@ Brief		: 确保strPath以'\'或者'/'结尾
	*/
	void MakePath(CString& strPath);
	
	/*
	@ FuncName	: Find
	@ Param		: strPath 要搜索的路径
	@ Param		: strExpression 搜索的表达式，如：a?b*.dat
	@ Param		: nFile 0表示所有文件和文件夹；1表示只要文件；2表示只要文件夹
	@ Param		: nDepth 搜索目录深度。0表示只搜索当前目录，1指深一级目录，2……；-1表示所有目录
	@ Return	: 返回m_callback的返回值，表示是否要继续查找
	*/
	BOOL __Find(CString strPath, CString& strExpression, int nFile, int nDepth);

	
private:
	BOOL			m_bStop;
	PFNFindCallBack m_callback;
	LPVOID			m_param;
};

#endif