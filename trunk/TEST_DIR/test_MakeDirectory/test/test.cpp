// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
//����ptInDir�����༶Ŀ¼
bool MakeDirectory(const TCHAR* ptInDir)
{
	int iNum = 0;
	TCHAR* ptDir = NULL;
	TCHAR tszParentDir[MAX_PATH] = {0};
	TCHAR tszTempDir[MAX_PATH] = {0};
	//_asm int 3;
	//����·���Ƿ����
	if (GetFileAttributes(ptInDir) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return TRUE;
	}

	
	ptDir = (TCHAR*)_tcsrchr(ptInDir, L'\\');  //��β�������ַ�
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

	//���鸸��·���Ƿ����  ��������򴴽���Ŀ¼
	if (GetFileAttributes(tszParentDir) == FILE_ATTRIBUTE_DIRECTORY)
	{		
		if (!CreateDirectory(ptInDir, NULL))
		{
			//�����ļ���ʧ��
			//FindClose(hParentFile);
			::MessageBox(NULL, ptInDir, TEXT("�����ļ���ʧ��"), MB_OK);
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
		//�����������ݹ�
		if (!MakeDirectory(tszParentDir))
		{
			return false;
		}
		else
		{//��Ŀ¼��������򴴽���Ŀ¼
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
/* ��������CreatedMultipleDirectorye( char * )
/* ���ܣ������༶Ŀ¼
/* ����: char * //·���ַ���
/* �����������ͣ�bool                                                   */
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
	//MakeDirectory(_T("E:\\��������\\My DBank"));

	CreatedMultipleDirectory("E:\\��������\\My DBank\\1");
	return 0;
}

