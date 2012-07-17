/********************************************************************
created:	2008/10/17
filename: 	FileSearch.h
file base:	FileSearch
file ext:	h
author:		huchao

purpose:	Windows �ļ����������ķ�װ
*********************************************************************/
#pragma once
#include <Windows.h>
#include <atlstr.h>
#include <vector>
#include <cassert>

// �����ļ�ȫ·���Ľṹ��
typedef struct tagSearchFileInfo
{
	CString strFullPath;	// �ļ���ȫ·��
}SearchFileInfo, *pSearchFileInfo;

typedef std::vector<pSearchFileInfo> vecFileInfo;	// �����ļ�ȫ·���� Vector

class CFileSearch
{
public:
	CFileSearch(void);
	~CFileSearch(void);

public:
	// ���ܣ���ʼ���ļ�����
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ�
	BOOL InitSearch();

	// ���ܣ�����ʼ���ļ�����
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ�
	BOOL UnInitSearch();

	// ���ܣ������Ƿ���Ҫ�������ļ���
	// ����˵��
	// bIsSearchChild��TRUE--��Ҫ������FALSE--����Ҫ����
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ�
	BOOL SetSearchChildDirectory(IN BOOL bIsSearchChild);

	// ���ܣ����һ���ض����ļ������յ������б�
	// ����˵��
	// strFileFullPath���ļ��ľ���·��
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ�
	BOOL AddFileToSearchList(IN const CString& strFileFullPath);

	// ���ܣ������Ҫ�������ļ���
	// ����˵��
	// strFullPath���ļ��еľ���·��
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ�
	BOOL AddSearchDiretory(IN const CString& strFullPath);

	// ���ܣ������ļ�������
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ�
	BOOL DoSearch();

	// ���ܣ���ȡ�����ļ���������б�
	// ����˵��
	// vecFiles�������ļ��б��Vector�����ڴ˴��������ã�ע���ֹ�౻������
	// ����ֵ��FALSE--ʧ�ܣ�����ɹ�
	BOOL GetSearchResultList(OUT vecFileInfo& vecFiles);

protected:
	typedef enum enumSearchStatus
	{	// �����ļ���״̬
		enumStatus_Stoped = 0,	// ��δ��ʼ����
		enumStatus_Inited,		// ��ʼ�����
		enumStatus_Search,		// ����
	}SearchStatus;

	BOOL FreeItemMemory();

private:
	BOOL _FindFiles();
	BOOL _AddFindFilesToList(const CString& strDirectory);

	SearchStatus m_CurSearchStatus;			// ��ǰ������״̬
	vecFileInfo m_vecFileInfo;				// ���������������ļ��б�
	BOOL m_bIsSearchChildDiretory;			// �Ƿ��������ļ���
	std::vector<CString> m_vecSearchDir;	// ��Ҫ�������ļ����б�
};
