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

	// �������
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
	// ���ݳ��ӣ�û������ʱ����NULL
	int Pop(string & sFile)
	{
		EnterCriticalSection(&m_lock);
		// û�����ݣ�����NULL
		if (m_list.empty())
		{
			static DWORD dwTick = GetTickCount();
			if (GetTickCount() - dwTick > 10000)
			{
				// �����޸� 2011-03-08 ��������ʾ set list �ļ�����
				TracePrint(LOG_DEBUG, "RSCloudSave ɾ������Ϊ�գ� files set has: %d\r\n", m_setList.size());
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

	// �Ƿ�����ļ�
	bool Exists(const char* szFile)
	{
		string sFile(szFile);
		EnterCriticalSection(&m_lock);
		bool bIsExists = m_setList.count(sFile) == 1;
		LeaveCriticalSection(&m_lock);
		return bIsExists;
	}
	// �Ƿ�����ļ�
	bool Exists(string &sFile)
	{
		EnterCriticalSection(&m_lock);
		bool bIsExists = m_setList.count(sFile) == 1;
		LeaveCriticalSection(&m_lock);
		return bIsExists;
	}

	// �����������
	void Clear()
	{
		EnterCriticalSection(&m_lock);
		// û�����ݣ�����NULL
		if(!m_list.empty())
		{
			m_list.clear();
		}
		LeaveCriticalSection(&m_lock);
	}

	// ɾ�������ļ�
	void DeleteAll(const bool &bQuit)
	{
		EnterCriticalSection(&m_lock);
		// û�����ݣ�����NULL
		if (m_list.empty())
		{
			static DWORD dwTick = GetTickCount();
			if (GetTickCount() - dwTick > 10000)
			{
				TracePrint(LOG_DEBUG, "RSCloudSave ɾ������Ϊ�գ�\r\n");
				dwTick = GetTickCount();
			}
			LeaveCriticalSection(&m_lock);
			return;
		}

		vector<string> vec(m_list);
		m_list.clear();
		// �����޸� 2011-03-06 ɾ�������б� �޸�Ϊһ�����
		m_setList.clear();

		for (vector<string>::iterator it=vec.begin();it!=vec.end();++it)
		{
			// ����ļ��������ˣ���ֱ������
			if (!PathFileExists(it->c_str()))
			{
				continue;
			}

			//TracePrint(LOG_DEBUG, "RSCloudSave delete file %s.\r\n", it->c_str());
			// �����޸� 2010-11-30 ���Բ�ɾ�����ɵ��ļ�����Ҫע�������ɾ�����
			if (FALSE == DeleteFile(it->c_str()))
			{
				DWORD dwError = GetLastError();
				TracePrint(LOG_DEBUG, "RSCloudSave delete file failed: %d.\r\n", dwError);

				// ���ɾ�����󣬿�����Ҫ�������
				if (dwError != ERROR_FILE_NOT_FOUND)
				{
					m_list.push_back(*it);
					// �����޸� 2011-03-06 ɾ�������б� �޸�Ϊһ����� �������
					if (0== m_setList.count(*it))
						m_setList.insert(*it);
				}
			}

			TracePrint(LOG_INFO, "FileDispatcher ɾ�����г���%s\r\n", it->c_str());
		}


		LeaveCriticalSection(&m_lock);
	}
	////// �����޸� 2011-03-08 ɾ���յ����Ŀ¼�ʹ���Ŀ¼������ļ���СΪ0Ҳһ����
	////void DeleteDir( const char* szSourceDir, const char* szOutDir, const char* szErrorDir);

private:
	// �ļ��б���
	vector<string> m_list;
	set<string> m_setList;
	// ͬ�����
	CRITICAL_SECTION m_lock;
	// ʵ������
	static CQueue m_instance;

	// ���캯��
	CQueue()
	{
		InitializeCriticalSection(&m_lock);
	}
	// ��������
	~CQueue()
	{
		Clear();
		DeleteCriticalSection(&m_lock);
	}
};
	//CQueue CQueue::m_instance;
}
#endif