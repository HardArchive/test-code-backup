//#pragma once
#ifndef __QUEUE_HEADER_
#define __QUEUE_HEADER_

#include <set>
#include <vector>
#include <string>
using namespace std;
namespace QNA
{	

class CQueue
{
public:
	static CQueue* GetInstance()
	{
		return &m_instance;
	}

	// 数据入队
	int Push(const char* szFile)
	{
		EnterCriticalSection(&m_lock);
		string sFile(szFile);
		m_list.push_back(sFile);
		if (0==m_setList.count(sFile))
			m_setList.insert(sFile);
		LeaveCriticalSection(&m_lock);
		return 0;
	}
	// 数据出队，没有数据时返回NULL
	int Pop(string & sFile)
	{
		EnterCriticalSection(&m_lock);
		// 没有数据，返回NULL
		if (m_list.empty())
		{
			static DWORD dwTick = GetTickCount();
			if (GetTickCount() - dwTick > 10000)
			{
				// 李照修改 2011-03-08 增加了显示 set list 文件个数
				TracePrint(LOG_DEBUG, "RSCloudSave 删除队列为空！ files set has: %d\r\n", m_setList.size());
				dwTick = GetTickCount();
			}
			LeaveCriticalSection(&m_lock);
			return -1;
		}

		sFile = m_list.front();
		m_list.erase(m_list.begin());
		if (1==m_setList.count(sFile))
			m_setList.erase(sFile);

		LeaveCriticalSection(&m_lock);
		return 0;
	}

	// 是否存在文件
	bool Exists(const char* szFile)
	{
		string sFile(szFile);
		EnterCriticalSection(&m_lock);
		bool bIsExists = m_setList.count(sFile) == 1;
		LeaveCriticalSection(&m_lock);
		return bIsExists;
	}
	// 是否存在文件
	bool Exists(string &sFile)
	{
		EnterCriticalSection(&m_lock);
		bool bIsExists = m_setList.count(sFile) == 1;
		LeaveCriticalSection(&m_lock);
		return bIsExists;
	}

	// 清除所有数据
	void Clear()
	{
		EnterCriticalSection(&m_lock);
		// 没有数据，返回NULL
		if(!m_list.empty())
		{
			m_list.clear();
		}
		LeaveCriticalSection(&m_lock);
	}

	// 删除所有文件
	void DeleteAll(const bool &bQuit)
	{
		EnterCriticalSection(&m_lock);
		// 没有数据，返回NULL
		if (m_list.empty())
		{
			static DWORD dwTick = GetTickCount();
			if (GetTickCount() - dwTick > 10000)
			{
				TracePrint(LOG_DEBUG, "RSCloudSave 删除队列为空！\r\n");
				dwTick = GetTickCount();
			}
			LeaveCriticalSection(&m_lock);
			return;
		}

		vector<string> vec(m_list);
		m_list.clear();
		// 李照修改 2011-03-06 删除数据列表 修改为一次清除
		m_setList.clear();

		for (vector<string>::iterator it=vec.begin();it!=vec.end();++it)
		{
			// 如果文件不存在了，则直接跳过
			if (!PathFileExists(it->c_str()))
			{
				continue;
			}

			//TracePrint(LOG_DEBUG, "RSCloudSave delete file %s.\r\n", it->c_str());
			// 李照修改 2010-11-30 测试不删除生成的文件，需要注释下面的删除语句
			if (FALSE == DeleteFile(it->c_str()))
			{
				DWORD dwError = GetLastError();
				TracePrint(LOG_DEBUG, "RSCloudSave delete file failed: %d.\r\n", dwError);

				// 如果删除错误，可能需要重新入队
				if (dwError != ERROR_FILE_NOT_FOUND)
				{
					m_list.push_back(*it);
					// 李照修改 2011-03-06 删除数据列表 修改为一次清除 重新入队
					if (0== m_setList.count(*it))
						m_setList.insert(*it);
				}
			}

			TracePrint(LOG_INFO, "FileDispatcher 删除队列出队%s\r\n", it->c_str());
		}


		LeaveCriticalSection(&m_lock);
	}
	////// 李照修改 2011-03-08 删除空的输出目录和错误目录，如果文件大小为0也一起处理
	////void DeleteDir( const char* szSourceDir, const char* szOutDir, const char* szErrorDir);

private:
	// 文件列表项
	vector<string> m_list;
	set<string> m_setList;
	// 同步变更
	CRITICAL_SECTION m_lock;
	// 实例对象
	static CQueue m_instance;

	// 构造函数
	CQueue()
	{
		InitializeCriticalSection(&m_lock);
	}
	// 析构函数
	~CQueue()
	{
		Clear();
		DeleteCriticalSection(&m_lock);
	}
};
	//CQueue CQueue::m_instance;
}
#endif