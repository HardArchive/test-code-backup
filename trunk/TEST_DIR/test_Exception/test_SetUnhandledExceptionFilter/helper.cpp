#include "stdafx.h"
#include "helper.h"
#include <Windows.h>
//��鵱ǰĿ¼��Ҫ���ļ����Ƿ����,��������ڷ���false�����ڷ���true
inline bool ChickDirExist(const TCHAR* ptInPath)
{
	if (!ptInPath || _tcsclen(ptInPath)<2)	return false;

	//����·���Ƿ����
	if (GetFileAttributes(ptInPath) != FILE_ATTRIBUTE_DIRECTORY)	return false;
	return true;
}
//��ȡ��Ŀ¼·��
inline bool GetParentPath(const TCHAR* ptInPath, TCHAR* POutParentPath)
{
	int iLen = 0;    
	TCHAR* ptTemp = NULL;		
	TCHAR tszPath[MAX_PATH] = {0};        //��ǰ·�� ��Ŀ¼
	_tcscpy_s(tszPath, ptInPath);         //���Ҫ������Ŀ¼�ַ���

	//�������Ƿ���ȷ
	if (!tszPath || _tcsclen(tszPath)<4)
		return false;

	//������ȥ��β��Ϊ'\\'���ַ�
	if (_T('\\') == tszPath[_tcsclen(tszPath)-1])
	{
		tszPath[_tcsclen(tszPath)-1] = 0;
	}

	ptTemp = _tcsrchr(tszPath, _T('\\'));  //��β�������ַ� ȡ����Ŀ¼��(������·��)
	iLen = _tcsclen(ptTemp);
	_tcsncpy_s(POutParentPath, MAX_PATH, tszPath, _tcsclen(tszPath)-iLen); //�õ���Ŀ¼·��

	return true;
}

//��ȡ��ǰ��������Ŀ¼ �ɹ�����true��ʧ�ܷ���false
bool GetExePath(TCHAR* ptInPath)
{
	TCHAR* ptTem = NULL;
	TCHAR tszTemp[MAX_PATH] = {0};
	//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
	if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
		return false; 

	ptTem = _tcsrchr(tszTemp, _T('\\'));
	memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
	return true;
}

//�����༶Ŀ¼���ɹ�����true�� ʧ�ܷ���false
//const TCHAR* ptInPath ·����, bool bIsFileName true �����ļ�����·���� false �������ļ�����·����
bool CreateMultipleDirectory(const TCHAR* ptInPath, bool bIsFileName)
{
	int iLen = 0;    
	TCHAR* ptTemp = NULL;		
	TCHAR tszPath[MAX_PATH] = {0};        //��ǰ·�� ��Ŀ¼
	TCHAR tszTemPath[MAX_PATH] = {0};     //��Ŀ¼
	_tcscpy_s(tszPath, ptInPath);         //���Ҫ������Ŀ¼�ַ���

	//ȡ�ò������ļ���������·��
	if (bIsFileName)
	{
		if (!GetParentPath(ptInPath, tszPath)) return false;
	}
	else
		_tcscpy_s(tszPath, ptInPath);

	//ȡ�ø�·��
	if (!GetParentPath(tszPath, tszTemPath)) return false;

	//���鸸��·���Ƿ����  		
	if (ChickDirExist(tszTemPath))
	{   //�������������Ŀ¼
		if (ChickDirExist(tszPath)) return true;
		if (!CreateDirectory(tszPath, NULL))
		{	//�����ļ���ʧ��
			::MessageBox(NULL, tszPath, _T("�����ļ���ʧ��!!!!!"), MB_OK);
			return false;
		}

		SetFileAttributes(tszPath, FILE_ATTRIBUTE_NORMAL);			
	}	
	else
	{	//�����������ݹ鸸��·��
		if (!CreateMultipleDirectory(tszTemPath, false))	return false;

		//��Ŀ¼������������������Ŀ¼
		if (!CreateMultipleDirectory(tszPath, false))	return false;						
	}
	return true;
}