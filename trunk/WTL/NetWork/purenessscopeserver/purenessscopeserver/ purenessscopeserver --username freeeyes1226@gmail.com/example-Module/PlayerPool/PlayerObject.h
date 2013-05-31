#ifndef _PLAYEROBJECT_H
#define _PLAYEROBJECT_H

#include <stdio.h>
#include <time.h>

#include "define.h"

//���ݽṹ��Ļ���
class CObject
{
public:
	CObject() { m_blWrite = false; m_ttUpdateTime = time(NULL); };
	virtual ~CObject() {};

	void EnterWrite() { m_blWrite = true;}
	void LeavelWrite() { m_ttUpdateTime = time(NULL); m_blWrite = false;}

	bool GetWriteSate() { return m_blWrite; }

#define ENTERWRITE() EnterWrite();    //����д��ĺ�
#define LEAVELWEITE() LeavelWrite();  //����д��ĺ�

private:
	bool   m_blWrite;      //д���

public:
	time_t m_ttUpdateTime; //���ݸ���ʱ�䣬DS��������������ʱ���������Ƿ���¡�
};

//���ݽṹ���̳���CDataBase������ʵ�����ݵ���ϸ�ڲ��ṹ
class CPlayerData : public CObject
{
public:
	CPlayerData() 
	{ 
		m_blOnline  = false;
		m_nPlayerID = 0;
		m_nLevel    = 1;
	};

	~CPlayerData() {}

	void Create(const char* pPlayerName)
	{
		ENTERWRITE();  //���д���

		sprintf_safe(m_szPlayerName, 50, "%s", pPlayerName);
		m_nPlayerID = 0;
		m_nLevel    = 1;

		LEAVELWEITE();  //�ͷ�д���
	};

	const char* GetPlayerName()
	{
		return m_szPlayerName;
	};

	void SetPlayerName(const char* szPlayerName)
	{
		sprintf_safe(m_szPlayerName, 50, "%s", szPlayerName);
	}

	int GetLevel()
	{
		return m_nLevel;
	};

	void SetPlayerLevel(int nLevel)
	{
		m_nLevel = nLevel;
	}

	int GetPlayerID()
	{
		return m_nPlayerID;
	};

	void SetPlayerID(int nID)
	{
		m_nPlayerID = nID;
	}

	const char* GetKey()
	{
		return m_szPlayerName;
	};

	const time_t GetTimeStamp()
	{
		return m_ttUpdateTime;
	};

	void SetTimeStamp()
	{
		m_ttUpdateTime = time(NULL);
	}

	void SetOnline(bool blOnline)
	{
		m_blOnline = blOnline;
		m_ttUpdateTime = time(NULL);
	};

	bool GetOnline()
	{
		return m_blOnline;
	};

private:
	char m_szPlayerName[50];     //��ҵ�����
	int  m_nPlayerID;            //��ҵ�ID
	int  m_nLevel;               //��ҵĵȼ�
	bool m_blOnline;             //����Ƿ�����
};

#endif
