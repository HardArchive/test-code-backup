#pragma once
#include "PathUtil.h"
#include "DirectoryTraversalUtil.h"
#include "ShareMemory.h"
#include "comm_protocol.h"
using namespace RG;
#include <string>
using namespace std;
class CDirTraversal : public CDirectoryTraversalUtil
{
	typedef struct MEM_SHARE_MANAGE_INFO_TAG
	{
		string strPath;
		RG::CShareMemory* pclsShareMemory;

		inline void Reset()
		{
			strPath.clear();
			pclsShareMemory = NULL;
		}

		inline void Release()
		{
			if (pclsShareMemory)
			{
				pclsShareMemory->Close();
				delete pclsShareMemory;
			}
			Reset();
		}
	}MEMSHAREMANAGEINFO, *PMEMSHAREMANAGEINFO;
public:
	CDirTraversal(void)
	{
		Reset();
	}

	~CDirTraversal(void)
	{
		Reset();
	}

public:
	void Start(const PTCHAR ptInPath)
	{
		m_bExit = true;
		m_strPath = string(ptInPath);
		Init(ptInPath);

		QueueUserWorkItem((LPTHREAD_START_ROUTINE)TraversalProc, (LPVOID)this, WT_EXECUTELONGFUNCTION);
	}
	void stop()
	{
		m_bExit = false;
		StopTraversal();
		Reset();
	}
	
	//ȡ������ȡ���ļ���Ϣ
	bool GetMemShareInfo(MEMSHAREINFO* pstuMemShareInfo)
	{
		if (!pstuMemShareInfo) return false;
		if (m_bExit)
		{
			m_queueMemShareInfo.Lock();
			if (!m_queueMemShareInfo.empty())
			{
				*pstuMemShareInfo = m_queueMemShareInfo.front();
				m_queueMemShareInfo.pop();
				m_queueMemShareInfo.Unlock();	
				TRACE(_T("Client - GetMemShareInfoȡ�ù����ļ���Ϣ�ɹ�������\r\n"));
				return true;
			}	
			m_queueMemShareInfo.Unlock();	
		}
		return false;;
	}

	void Reset()
	{
		m_strPath.clear();
		//��ն���
		//PMEMSHAREINFO pstuClientInfo = NULL;
		m_queueMemShareInfo.Lock();
		while (!m_queueMemShareInfo.empty())
		{
/*			pstuClientInfo = m_queueMemShareInfo.front();
			delete pstuClientInfo;		*/	
			m_queueMemShareInfo.pop();
			//pstuClientInfo = NULL;
		}
		m_queueMemShareInfo.Unlock();


		//���map
		m_mapMemShare.Lock();
		if (m_mapMemShare.size())
		{
			for (map<string, MEMSHAREMANAGEINFO>::iterator it=m_mapMemShare.begin(); it!=m_mapMemShare.end(); ++it)
			{
				it->second.Release();
			}
		}
		m_mapMemShare.clear();
		m_mapMemShare.Unlock();
		m_dwFileNum = 1000;
	}

	//�ر��ڴ�ӳ��
	void CloseMemShare(string& strPath)
	{
		RG::TRACE(_T("�ر��ڴ�ӳ��:%s"), strPath.c_str());	
		m_mapMemShare.Lock();
		if (!m_mapMemShare.size())
		{
			m_mapMemShare.Unlock();
			return;
		}
		for (map<string, MEMSHAREMANAGEINFO>::iterator it=m_mapMemShare.begin(); it!=m_mapMemShare.end(); ++it)
		{
			if (!strPath.compare(it->first))
			{
				it->second.Release();
				m_mapMemShare.erase(it);
				break;
			}	
		}
		m_mapMemShare.Unlock();
	}
private:
	static UINT WINAPI TraversalProc( LPVOID lpThreadParameter)
	{
		CDirTraversal* pclsHandleFile = (CDirTraversal*)lpThreadParameter;
		return pclsHandleFile->StartTraversal();
	}

	//�����ļ�
	bool TraversalFileDoWork(const PTCHAR ptInFilePath)
	{
		TCHAR tszPath[MAX_PATH] = {0};
//_asm int 3;
		//ȡ�ó���Ŀ¼����֮���·�� 
		_tcsncpy_s(tszPath, ptInFilePath+m_strPath.length(), strlen(ptInFilePath)-m_strPath.length());
		return OpenMemShare(ptInFilePath, tszPath);
	}

	//�����ļ���
	bool TraversalFolderDoWork(const PTCHAR ptInPath)
	{
		MEMSHAREINFO stuMemShareInfo;
		stuMemShareInfo.Reset();	
		stuMemShareInfo.stuFileInfo.bIsFile = false;
		//ȡ��Ŀ¼����
		stuMemShareInfo.stuFileInfo.dwFileAttributes = GetFileAttributes(ptInPath);

		//ȡ�ó���Ŀ¼����֮���·�� 
		_tcsncpy_s(stuMemShareInfo.stuFileInfo.tszFilePath, ptInPath+m_strPath.length(), strlen(ptInPath)-m_strPath.length());

		m_queueMemShareInfo.Lock();
		m_queueMemShareInfo.push(stuMemShareInfo);
		m_queueMemShareInfo.Unlock();
		return true;
	}

	//���ļ�ȡ���ļ����ݲ����ͳ�ȥ
	//ptInFilePath�������ļ�·�� ��ptInPath����Ŀ¼����ļ�·��
	bool OpenMemShare(const PTCHAR ptInFilePath, const PTCHAR ptInPath)
	{
		MEMSHAREINFO stuMemShareInfo;
		MEMSHAREMANAGEINFO stuMemShareManageInfo;
		stuMemShareInfo.Reset();
		stuMemShareManageInfo.Reset();


		RG::CShareMemory* pclsShareMemory = new RG::CShareMemory;
		_stprintf_s(stuMemShareInfo.tszFileMapName, _T("Global%d"), m_dwFileNum++);
		if (!pclsShareMemory->Create(stuMemShareInfo.tszFileMapName, 0, ptInFilePath))
		{
			::MessageBox(NULL, "��ӳ���ļ�ʧ�ܣ�����", "SendFile", MB_OK);
			return false;
		}

		stuMemShareInfo.stuFileInfo.bIsFile = true;
		stuMemShareInfo.stuFileInfo.dwAddrLen = pclsShareMemory->GetFileMapLen();
		//ȡ��Ŀ¼����
		stuMemShareInfo.stuFileInfo.dwFileAttributes = GetFileAttributes(ptInFilePath);
		//ȡ�ó���Ŀ¼����֮���·�� 
		_tcscpy_s(stuMemShareInfo.stuFileInfo.tszFilePath, MAX_PATH, ptInPath);

		m_queueMemShareInfo.Lock();
		m_queueMemShareInfo.push(stuMemShareInfo);
		m_queueMemShareInfo.Unlock();


		m_mapMemShare.Lock();
		if (0 == m_mapMemShare.count(string(ptInPath)) )
		{
			stuMemShareManageInfo.pclsShareMemory = pclsShareMemory;
			stuMemShareManageInfo.strPath = string(ptInPath);
			m_mapMemShare[string(ptInPath)] = stuMemShareManageInfo;
		}
		m_mapMemShare.Unlock();

		RG::TRACE(_T("�ļ��Ѿ���ȡ���"));	
		//clsShareMemory.Close();
		return true;
	}



private:
	bool m_bExit;              //�˳���־  trueΪ��ʼ  false ֹͣ
	DWORD m_dwFileNum;                                                     //�ļ�����
	string m_strPath;	                                                   //Ҫ������Ŀ¼
	RG::SAFESTL::CMap<string, MEMSHAREMANAGEINFO> m_mapMemShare;
	RG::SAFESTL::CQueue<MEMSHAREINFO> m_queueMemShareInfo;                 //��Ҫ���͵�����
};
