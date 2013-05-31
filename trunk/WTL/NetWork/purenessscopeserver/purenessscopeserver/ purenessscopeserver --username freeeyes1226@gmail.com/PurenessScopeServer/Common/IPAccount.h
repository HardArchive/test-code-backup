#ifndef _IPACCOUNT_H
#define _IPACCOUNT_H

//��ӶԿͻ������ӵ�ͳ����Ϣ
//add by freeeyes
//2011-10-08

#include <string>
#include "MapTemplate.h"
#include "ace/Date_Time.h"

struct _IPAccount
{
	string         m_strIP;              //��ǰ���ӵ�ַ
	int            m_nCount;             //��ǰ���Ӵ���
	int            m_nAllCount;          //ָ��IP���Ӵ����ܺ� 
	int            m_nMinute;            //��ǰ������
	ACE_Date_Time  m_dtLastTime;         //�������ʱ��

	_IPAccount()
	{
		m_strIP      = "";
		m_nCount     = 0;
		m_nAllCount  = 0;
		m_dtLastTime.update();
		m_nMinute    = (int)m_dtLastTime.minute();
	}

	void Add()
	{
		m_dtLastTime.update();
		if(m_dtLastTime.minute() != m_nMinute)
		{
			m_nMinute  = (int)m_dtLastTime.minute();
			m_nCount   = 1;
			m_nAllCount++;
		}
		else
		{
			m_nCount++;
			m_nAllCount++;
		}
	}
};

typedef vector<_IPAccount> vecIPAccount;

class CIPAccount
{
public:
	CIPAccount() 
  { 
    m_nNeedCheck       = 0;
    m_nMaxConnectCount = 100;  //Ĭ��ÿ�����100�� 
  };

	~CIPAccount() 
	{
		OUR_DEBUG((LM_INFO, "[CIPAccount::~CIPAccount].\n"));
		Close(); 
		OUR_DEBUG((LM_INFO, "[CIPAccount::~CIPAccount]End.\n"));
	};

	void Close()
	{
		m_mapIPAccount.Clear();
	}

	void Init(int nNeedCheck, int nMaxConnectCount)
	{
    m_nNeedCheck       = nNeedCheck;
		m_nMaxConnectCount = nMaxConnectCount;
	};

	bool AddIP(string strIP)
	{
    //������Ҫ����Ҫ�ж��������Ҫ�������IPͳ��
    if(m_nNeedCheck == 0)
    {
		  _IPAccount* pIPAccount = m_mapIPAccount.SearchMapData(strIP);
		  if(NULL == pIPAccount)
		  {
			  //û���ҵ������
			  pIPAccount = new _IPAccount();
			  if(NULL == pIPAccount)
			  {
				  return true;
			  }
			  else
			  {
				  pIPAccount->m_strIP     = strIP;
				  pIPAccount->Add();
				  m_mapIPAccount.AddMapData(strIP, pIPAccount);
			  }
		  }
		  else
		  {
			  pIPAccount->Add();
			  if(pIPAccount->m_nCount >= m_nMaxConnectCount)
			  {
				  return false;
			  }
		  }

      return true;
    }
    else
    {
      return true;
    }
	};

	int GetCount()
	{
		return m_mapIPAccount.GetSize();
	}

	void GetInfo(vecIPAccount& VecIPAccount)
	{
		for(int i = 0; i < m_mapIPAccount.GetSize(); i++)
		{
			_IPAccount* pIPAccount = m_mapIPAccount.GetMapData(i);
			if(NULL != pIPAccount)
			{
				VecIPAccount.push_back(*pIPAccount);
			}
		}
	}

private:
	CMapTemplate<string, _IPAccount> m_mapIPAccount;
	int            m_nMaxConnectCount;
  int            m_nNeedCheck;        //�Ƿ���Ҫ��֤��0Ϊ��Ҫ��1Ϊ����Ҫ
};

typedef ACE_Singleton<CIPAccount, ACE_Recursive_Thread_Mutex> App_IPAccount;

#endif
