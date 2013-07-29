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
	
	//取出所获取的文件信息
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
				TRACE(_T("Client - GetMemShareInfo取得共享文件信息成功！！！\r\n"));
				return true;
			}	
			m_queueMemShareInfo.Unlock();	
		}
		return false;;
	}

	void Reset()
	{
		m_strPath.clear();
		//清空队列
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


		//清空map
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

	//关闭内存映射
	void CloseMemShare(string& strPath)
	{
		RG::TRACE(_T("关闭内存映射:%s"), strPath.c_str());	
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

	//处理文件
	bool TraversalFileDoWork(const PTCHAR ptInFilePath)
	{
		TCHAR tszPath[MAX_PATH] = {0};
//_asm int 3;
		//取得除根目录部分之外的路径 
		_tcsncpy_s(tszPath, ptInFilePath+m_strPath.length(), strlen(ptInFilePath)-m_strPath.length());
		return OpenMemShare(ptInFilePath, tszPath);
	}

	//处理文件夹
	bool TraversalFolderDoWork(const PTCHAR ptInPath)
	{
		MEMSHAREINFO stuMemShareInfo;
		stuMemShareInfo.Reset();	
		stuMemShareInfo.stuFileInfo.bIsFile = false;
		//取得目录属性
		stuMemShareInfo.stuFileInfo.dwFileAttributes = GetFileAttributes(ptInPath);

		//取得除根目录部分之外的路径 
		_tcsncpy_s(stuMemShareInfo.stuFileInfo.tszFilePath, ptInPath+m_strPath.length(), strlen(ptInPath)-m_strPath.length());

		m_queueMemShareInfo.Lock();
		m_queueMemShareInfo.push(stuMemShareInfo);
		m_queueMemShareInfo.Unlock();
		return true;
	}

	//打开文件取出文件内容并发送出去
	//ptInFilePath完整的文件路径 ，ptInPath除根目录外的文件路径
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
			::MessageBox(NULL, "打开映射文件失败！！！", "SendFile", MB_OK);
			return false;
		}

		stuMemShareInfo.stuFileInfo.bIsFile = true;
		stuMemShareInfo.stuFileInfo.dwAddrLen = pclsShareMemory->GetFileMapLen();
		//取得目录属性
		stuMemShareInfo.stuFileInfo.dwFileAttributes = GetFileAttributes(ptInFilePath);
		//取得除根目录部分之外的路径 
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

		RG::TRACE(_T("文件已经读取完毕"));	
		//clsShareMemory.Close();
		return true;
	}



private:
	bool m_bExit;              //退出标志  true为开始  false 停止
	DWORD m_dwFileNum;                                                     //文件数量
	string m_strPath;	                                                   //要遍历的目录
	RG::SAFESTL::CMap<string, MEMSHAREMANAGEINFO> m_mapMemShare;
	RG::SAFESTL::CQueue<MEMSHAREINFO> m_queueMemShareInfo;                 //需要发送的内容
};
