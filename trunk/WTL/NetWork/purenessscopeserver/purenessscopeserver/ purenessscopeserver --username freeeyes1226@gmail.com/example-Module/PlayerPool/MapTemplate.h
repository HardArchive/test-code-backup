#ifndef _MAPTEMPLATE_H
#define _MAPTEMPLATE_H

#include <map>
#include "ListTemplate.h"

using namespace std;

//����ģ�廯map����
template <class Key, class T>
class CMapTemplate
{
private:
	typedef map<Key, T*>          mapData;          //����map�ṹ
	typedef map<CListNode*, Key> mapNode2Key;      //���������Key�Ķ�Ӧ��ϵ
	typedef map<Key, CListNode*> mapKey2Node;      //���������Key�Ķ�Ӧ��ϵ
	CLinkList                     m_LinkList;      //����ʹ�õ�����
	CLinkList                     m_FreeLinkList;  //���е�����
	mapData                       m_mapData;
	mapNode2Key                   m_mapNode2Key;
	mapKey2Node                   m_mapKey2Node;
	int                           m_nMaxCount;

public:
	CMapTemplate(int nCount)
	{
		m_nMaxCount = nCount;
		for(int i = 0; i < nCount; i++)
		{
			CListNode* pListNode = new CListNode();
			m_FreeLinkList.Add(pListNode, NULL);
		}
	};

	~CMapTemplate() 
	{
		//Clear();
	};

	//����Ƿ�����Ҫ��̭�����ݣ�����з���Ҫ��̭���ݵ�ָ��
	T* GetDeleteKey()
	{
		if((int)m_mapData.size() > m_nMaxCount - 1)
		{
			//���������㷨
			CListNode* pListNode = m_LinkList.GetLast();
			if(NULL == pListNode)
			{
				return false;
			}

			Key* pKey = FindNode2Key(pListNode);

			typename mapData::iterator f = m_mapData.find(*pKey);

			if(f != m_mapData.end())
			{
				T* pT = (T*)f->second;
				//��������
				CListNode* pListNode = FindKey2Node(*pKey);
				if(NULL == pListNode)
				{
					return false;
				}

				DelKey2Node(*pKey);
				DelNode2Key(pListNode);

				m_LinkList.DelNode(pListNode, false);
				m_FreeLinkList.Add(pListNode, NULL);

				//��������map
				m_mapData.erase(f);

				return pT;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	};


	bool DelLastMapData(bool blDelete = true)
	{
		CListNode* pListNode = m_LinkList.GetLast();
		if(NULL == pListNode)
		{
			return false;
		}

		Key* pKey = FindNode2Key(pListNode);

		return DelMapData(*pKey, blDelete);
	};

	//�������
	bool AddMapData(Key mapkey, T* pData, bool blDelete = true)
	{
		if((int)m_mapData.size() >= m_nMaxCount)
		{
			//���������㷨
			DelLastMapData(blDelete);
		}

		//��Ӷ�Ӧ��ϵ������
		CListNode* pListNode = m_FreeLinkList.GetLastNode();
		if(NULL == pListNode)
		{
			return false;
		}

		m_LinkList.Add(pListNode, NULL);
		if(AddNode2Key(pListNode, mapkey) == false)
		{
			return false;
		}

		if(AddKey2Node(pListNode, mapkey) == false)
		{
			return false;
		}
		m_LinkList.MoveTop(pListNode);

		typename mapData::iterator f = m_mapData.find(mapkey);
		if(f != m_mapData.end())
		{
			return false;
		}

		m_mapData.insert(typename mapData::value_type(mapkey, pData));
		return true;
	}

	//ɾ�����ݣ�Ĭ����ɾ��T����
	bool DelMapData(Key mapkey, bool blDelete = true)
	{
		CListNode* pListNode = FindKey2Node(mapkey);
		if(NULL == pListNode)
		{
			return false;
		}

		DelKey2Node(mapkey);
		DelNode2Key(pListNode);

		m_LinkList.DelNode(pListNode, false);

		m_FreeLinkList.Add(pListNode, NULL);

		typename mapData::iterator f = m_mapData.find(mapkey);

		if(f != m_mapData.end())
		{
			T* pData = (T*)f->second;
			if(pData != NULL && blDelete == true)
			{
				delete pData;
				pData = NULL;
			}
			m_mapData.erase(f);

			return true;
		}
		else
		{
			return false;
		}
	}

	//���map�ڵ���������
	void Clear()
	{
		typename mapData::iterator b = m_mapData.begin();
		typename mapData::iterator e = m_mapData.end();

		for(b; b != e; b++)
		{
			T* pData = (T*)b->second;
			if(pData != NULL)
			{
				delete pData;
			}
		}

		m_mapData.clear();
	}

	//��������
	T* SearchMapData(Key mapkey)
	{
		typename mapData::iterator f = m_mapData.find(mapkey);

		if(f != m_mapData.end())
		{
			CListNode* pListNode = FindKey2Node(mapkey);
			if(NULL == pListNode)
			{
				return false;
			}
			m_LinkList.MoveTop(pListNode);

			T* pData = (T*)f->second;
			return pData;
		}
		else
		{
			return NULL;
		}
	}

	//�õ���ǰ��map�ĸ���
	int GetSize()
	{
		return (int)m_mapData.size();
	}

private:
	bool AddNode2Key(CListNode* pListNode, Key mapkey)
	{
		typename mapNode2Key::iterator fNode = m_mapNode2Key.find(pListNode);
		if(fNode != m_mapNode2Key.end())
		{
			return false;
		}
		m_mapNode2Key.insert(typename mapNode2Key::value_type(pListNode, mapkey));
		return true;
	}

	bool AddKey2Node(CListNode* pListNode, Key mapkey)
	{
		typename mapKey2Node::iterator fNode = m_mapKey2Node.find(mapkey);
		if(fNode != m_mapKey2Node.end())
		{
			return false;
		}
		m_mapKey2Node.insert(typename mapKey2Node::value_type(mapkey, pListNode));
		return true;
	}

	bool DelNode2Key(CListNode* pListNode)
	{
		typename mapNode2Key::iterator fNode = m_mapNode2Key.find(pListNode);
		if(fNode != m_mapNode2Key.end())
		{
			m_mapNode2Key.erase(fNode);
			return true;
		}

		return false;
	}

	bool DelKey2Node(Key mapKey)
	{
		typename mapKey2Node::iterator fNode = m_mapKey2Node.find(mapKey);
		if(fNode != m_mapKey2Node.end())
		{
			m_mapKey2Node.erase(fNode);
			return true;
		}
		return false;
	}

	CListNode* FindKey2Node(Key mapKey)
	{
		typename mapKey2Node::iterator fNode = m_mapKey2Node.find(mapKey);
		if(fNode != m_mapKey2Node.end())
		{
			return (CListNode* )fNode->second;
		}
		return NULL;
	}

	Key* FindNode2Key(CListNode* pListNode)
	{
		typename mapNode2Key::iterator fNode = m_mapNode2Key.find(pListNode);
		if(fNode != m_mapNode2Key.end())
		{
			return(Key* )&fNode->second;
		}

		return NULL;
	}
};
#endif
