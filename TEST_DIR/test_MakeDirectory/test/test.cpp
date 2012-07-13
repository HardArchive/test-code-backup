// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "stdafx.h"
#include <Windows.h>
#include <string>
//#include <iostream>
//#include <algorithm>
#include <vector>
//#include <fstream>
using namespace std;
//跟据ptInDir创建多级目录
bool MakeDirectory(const TCHAR* ptInDir)
{
	int iNum = 0;
	TCHAR* ptDir = NULL;
	TCHAR tszParentDir[MAX_PATH] = {0};
	TCHAR tszTempDir[MAX_PATH] = {0};
	//_asm int 3;
	//检验路径是否存在
	if (GetFileAttributes(ptInDir) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return TRUE;
	}

	
	ptDir = (TCHAR*)_tcsrchr(ptInDir, L'\\');  //从尾部查找字符
	if (!ptDir)
	{
		return false;
	}
	iNum = _tcslen(ptDir);
	if (1 == iNum)
	{
		_tcsnccpy(tszTempDir, ptInDir, _tcslen(ptInDir)-1);
		ptDir = (TCHAR*)_tcsrchr(tszTempDir, L'\\');
		//ptDir += 1;
		iNum += _tcslen(ptDir);
	}

	_tcsnccpy(tszParentDir, ptInDir, (_tcslen(ptInDir)-iNum));

	//检验父级路径是否存在  如果存在则创建子目录
	if (GetFileAttributes(tszParentDir) == FILE_ATTRIBUTE_DIRECTORY)
	{		
		if (!CreateDirectory(ptInDir, NULL))
		{
			//创建文件夹失败
			//FindClose(hParentFile);
			::MessageBox(NULL, ptInDir, TEXT("创建文件夹失败"), MB_OK);
			return false;
		}
		else
		{
			SetFileAttributes(ptInDir, FILE_ATTRIBUTE_NORMAL);	
			return true;
		}
	}
	else
	{
		//如果不存在则递归
		if (!MakeDirectory(tszParentDir))
		{
			return false;
		}
		else
		{//父目录创建完毕则创建子目录
			if (!MakeDirectory(ptInDir))
			{
				return false;
			}
			return true;
		}
	}

	return true;
}


/************************************************************************/
/* 函数名：CreatedMultipleDirectorye( char * )
/* 功能：创建多级目录
/* 参数: char * //路径字符串
/* 函数返回类型：bool                                                   */
/************************************************************************/

bool CreatedMultipleDirectory( char* direct)
{
	string Directoryname = direct;

	if (  Directoryname[Directoryname.length() - 1] !=  '//' )
	{
		//Directoryname +=
		Directoryname.append( 1, '//');

	}
	std::vector<string> vpath;
	string strtemp;
	bool  bSuccess = false;
	for ( int i = 0; i < Directoryname.length(); i++ )
	{
		if ( Directoryname[i] != '//')
		{
			strtemp.append( 1,Directoryname[i] );    
		}
		else
		{
			vpath.push_back( strtemp );
			strtemp.append( 1, '//' );
		}
	}
	std::vector<string>:: const_iterator vIter;
	for ( vIter = vpath.begin();vIter != vpath.end(); vIter++ )
	{
		bSuccess = CreateDirectory( vIter->c_str(), NULL ) ? true :false;
	}


	return bSuccess; 
}

int _tmain(int argc, _TCHAR* argv[])
{
	//MakeDirectory(_T("E:\\数据银行\\My DBank"));

	CreatedMultipleDirectory("E:\\数据银行\\My DBank\\1");
	return 0;
}

