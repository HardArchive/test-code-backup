#include "stdafx.h"
#include "helper.h"
#include <Windows.h>
//检查当前目录需要的文件夹是否存在,如果不存在返回false，存在返回true
inline bool ChickDirExist(const TCHAR* ptInPath)
{
	if (!ptInPath || _tcsclen(ptInPath)<2)	return false;

	//检验路径是否存在
	if (GetFileAttributes(ptInPath) != FILE_ATTRIBUTE_DIRECTORY)	return false;
	return true;
}
//获取父目录路径
inline bool GetParentPath(const TCHAR* ptInPath, TCHAR* POutParentPath)
{
	int iLen = 0;    
	TCHAR* ptTemp = NULL;		
	TCHAR tszPath[MAX_PATH] = {0};        //当前路径 子目录
	_tcscpy_s(tszPath, ptInPath);         //存放要创建的目录字符串

	//检查参数是否正确
	if (!tszPath || _tcsclen(tszPath)<4)
		return false;

	//在这里去掉尾部为'\\'的字符
	if (_T('\\') == tszPath[_tcsclen(tszPath)-1])
	{
		tszPath[_tcsclen(tszPath)-1] = 0;
	}

	ptTemp = _tcsrchr(tszPath, _T('\\'));  //从尾部查找字符 取得子目录名(不包括路径)
	iLen = _tcsclen(ptTemp);
	_tcsncpy_s(POutParentPath, MAX_PATH, tszPath, _tcsclen(tszPath)-iLen); //得到父目录路径

	return true;
}

//获取当前程序所在目录 成功返回true，失败返回false
bool GetExePath(TCHAR* ptInPath)
{
	TCHAR* ptTem = NULL;
	TCHAR tszTemp[MAX_PATH] = {0};
	//获取当前目录  //这里是获取当前进程文件的完整路径 
	if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
		return false; 

	ptTem = _tcsrchr(tszTemp, _T('\\'));
	memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
	return true;
}

//创建多级目录，成功返回true， 失败返回false
//const TCHAR* ptInPath 路径名, bool bIsFileName true 包括文件名的路径名 false 不包括文件名的路径名
bool CreateMultipleDirectory(const TCHAR* ptInPath, bool bIsFileName)
{
	int iLen = 0;    
	TCHAR* ptTemp = NULL;		
	TCHAR tszPath[MAX_PATH] = {0};        //当前路径 子目录
	TCHAR tszTemPath[MAX_PATH] = {0};     //父目录
	_tcscpy_s(tszPath, ptInPath);         //存放要创建的目录字符串

	//取得不包括文件名的完整路径
	if (bIsFileName)
	{
		if (!GetParentPath(ptInPath, tszPath)) return false;
	}
	else
		_tcscpy_s(tszPath, ptInPath);

	//取得父路径
	if (!GetParentPath(tszPath, tszTemPath)) return false;

	//检验父级路径是否存在  		
	if (ChickDirExist(tszTemPath))
	{   //如果存在则检查子目录
		if (ChickDirExist(tszPath)) return true;
		if (!CreateDirectory(tszPath, NULL))
		{	//创建文件夹失败
			::MessageBox(NULL, tszPath, _T("创建文件夹失败!!!!!"), MB_OK);
			return false;
		}

		SetFileAttributes(tszPath, FILE_ATTRIBUTE_NORMAL);			
	}	
	else
	{	//如果不存在则递归父级路径
		if (!CreateMultipleDirectory(tszTemPath, false))	return false;

		//父目录创建完毕则继续创建子目录
		if (!CreateMultipleDirectory(tszPath, false))	return false;						
	}
	return true;
}