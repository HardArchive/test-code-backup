#ifndef _SMPOOL_H
#define _SMPOOL_H

#include "SMAccessObject.h"
#include "Serial.h"

#include <map>

using namespace std;

template<typename T>
class CSMPool
{
private:
	//��¼ÿ�����е���������
	struct _SMBlock
	{
		unsigned int	m_offT;         //���ݶ���ƫ�ƣ����ﲻ����T*����Ϊ�����ڴ��в�Ӧ�ô���ָ��ֵ��Ӧ�ô���ƫ����
		int				m_nID;          //���ݵ�ǰ���
		bool			m_blUse;        //�Ƿ���ʹ��true������ʹ�ã�false��û��ʹ��
		time_t			m_ttUpdateTime; //DS������������ɺ��д����Ϣʱ�䡣

		_SMBlock()
		{
			m_offT  = 0;
			m_nID   = 0;
			m_blUse = false;
		}
	};

public:
	CSMPool()
	{
		m_pSMAccessObject = NULL;
	};

	~CSMPool()
	{
		if(m_pSMAccessObject != NULL)
		{
			delete m_pSMAccessObject;
			m_pSMAccessObject = NULL;
		}
	};

	bool Init(SMKey key, int nMaxCount)
	{
		bool blRet = false;

		m_pSMAccessObject = new CSMAccessObject();
		if(NULL == m_pSMAccessObject)
		{
			return false;
		}

		//���ȳ��Դ򿪣������Ƿ�������У���������ڣ��򴴽��µġ�
		blRet = m_pSMAccessObject->Open(key, nMaxCount*sizeof(T), nMaxCount*sizeof(_SMBlock));
		if(false == blRet)
		{
			printf("[Init]Create.\n");
			//���û�У����½���
			blRet = m_pSMAccessObject->Create(key, nMaxCount*sizeof(T), nMaxCount*sizeof(_SMBlock));
			if(false == blRet)
			{
				return false;
			}

			//��ʼ��ʼ������ͷ��
			m_Serial.Init(m_pSMAccessObject->GetHeadData(), sizeof(_SMBlock), nMaxCount);

			//��ʼ���ֹ����ڴ�ռ�
			m_mapSMBlock.clear();
			m_mapFreeSMBlock.clear();
			m_mapUsedSMBlock.clear();
			for(int i = 0; i < nMaxCount; i++)
			{
				_SMBlock* pSMBlock = (_SMBlock* )m_Serial.GetData(i);

				//������Ϊ_SMBlock���µģ����������ʼ������ṹ
				new (pSMBlock)_SMBlock();
				if(pSMBlock == NULL)
				{
					return false;
				}

				pSMBlock->m_nID = i;
				pSMBlock->m_offT = i * sizeof(T);
				T* pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(pSMBlock->m_offT));
				new (pT)T();
				if(NULL == pT)
				{
					return false;
				}
				m_mapSMBlock.insert(typename mapSMBlock::value_type(i, pSMBlock));
				m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(i, pSMBlock));
			}

			m_nMaxCount = nMaxCount;
		}
		else
		{
			printf("[Init]Open.\n");

			//��ʼ��ʼ������ͷ��
			m_Serial.Init(m_pSMAccessObject->GetHeadData(), sizeof(_SMBlock), nMaxCount);

			//��ԭ��map����
			m_mapSMBlock.clear();
			m_mapFreeSMBlock.clear();
			m_mapUsedSMBlock.clear();
			for(int i = 0; i < nMaxCount; i++)
			{
				_SMBlock* pSMBlock = (_SMBlock* )m_Serial.GetData(i);
				if(pSMBlock == NULL)
				{
					return false;
				}

				pSMBlock->m_offT = i * sizeof(T);
				T* pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(pSMBlock->m_offT));
				m_mapSMBlock.insert(typename mapSMBlock::value_type(pSMBlock->m_nID, pSMBlock));

				//����
				if(pSMBlock->m_blUse == true)
				{
					//��������ʹ�õ��б�
					m_mapUsedSMBlock.insert(typename mapUsedSMBlock::value_type(pT, pSMBlock));
				}
				else
				{
					//����û��ʹ�õ��б�
					m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(i, pSMBlock));
				}
			}

			m_nMaxCount = nMaxCount;
		}


		return true;
	};

	void Close()
	{
		typename mapSMBlock::iterator b = m_mapSMBlock.begin();
		typename mapSMBlock::iterator e = m_mapSMBlock.end();
		for(b; b != e; b++)
		{
			_SMBlock* pSMBlock = (_SMBlock* )b->second;
			if(pSMBlock != NULL)
			{
				delete pSMBlock;
			}
		}
		m_mapSMBlock.clear();

		m_pSMAccessObject->Destory();
	}

	T* NewObject()
	{
		//��m_mapSMBlock�в���һ������Ķ���
		if(m_mapFreeSMBlock.size() > 0)
		{
			typename mapFreeSMBlock::iterator b = m_mapFreeSMBlock.begin();

			if(b == m_mapFreeSMBlock.end())
			{
				return NULL;
			}

			_SMBlock* pSMBlock = (_SMBlock* )b->second;
			if(NULL == pSMBlock)
			{
				return NULL;
			}
			else
			{
				m_mapFreeSMBlock.erase(b);
				pSMBlock->m_blUse        = true;
				pSMBlock->m_ttUpdateTime = time(NULL);
				T* pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(pSMBlock->m_offT));
				m_mapUsedSMBlock.insert(typename mapUsedSMBlock::value_type(pT, pSMBlock));

				return pT;
			}
		}
		else
		{
			return NULL;
		}
	};

	int GetDataPos(T* pT)
	{
		//���ݵ�ַƫ�ƻ�õ�ǰ���ݵ����λ�� 
		char* pData = m_pSMAccessObject->GetHeadData() + m_pSMAccessObject->GetHeadDataSize();
		int nPos = (int)(((char* )pT - pData)/sizeof(T));
		if(nPos < m_nMaxCount && nPos >= 0)
		{
			return nPos;
		}
		else
		{
			return -1;
		}
	}

	T* GetPosToData(int nPos)
	{
		//����һ��ƫ�Ƶ�ַ���һ��ָ����T*
		if(nPos < m_nMaxCount && nPos >= 0)
		{
			T* pT = (T*)(m_pSMAccessObject->GetHeadData() + m_pSMAccessObject->GetHeadDataSize() + nPos*sizeof(T)); 
			return pT;
		}
		else
		{
			return NULL;
		}
	}

	bool DeleteObject(T* pData)
	{
		typename mapUsedSMBlock::iterator f = m_mapUsedSMBlock.find(pData);
		if(f == m_mapUsedSMBlock.end())
		{
			return false;
		}
		else
		{
			_SMBlock* pSMBlock = (_SMBlock* )f->second;
			m_mapUsedSMBlock.erase(f);
			pSMBlock->m_blUse = false;
			m_mapFreeSMBlock.insert(typename mapFreeSMBlock::value_type(pSMBlock->m_nID, pSMBlock));

			return true;
		}
	};

	int GetFreeObjectCount()
	{
		return (int)m_mapFreeSMBlock.size();
	}

	int GetUsedObjectCount()
	{
		return (int)m_mapUsedSMBlock.size();
	};

	T* GetUsedObject(int nIndex)
	{
		if(nIndex >= (int)m_mapUsedSMBlock.size())
		{
			return NULL;
		}

		typename mapUsedSMBlock::iterator b = m_mapUsedSMBlock.begin();
		typename mapUsedSMBlock::iterator e = m_mapUsedSMBlock.end();

		int nPos = 0;
		for(b; b != e; b++)
		{
			if(nPos == nIndex)
			{
				_SMBlock* pSMBlock = (_SMBlock* )b->second;
				if(NULL != pSMBlock)
				{
					T* pT  = reinterpret_cast<T*>(m_pSMAccessObject->GetData(pSMBlock->m_offT));
					return pT;
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				if(nPos > nIndex)
				{
					return NULL;
				}
				nPos++;
			}
		}

		return NULL;
	};

	const time_t GetObjectHeadTimeStamp(T* pData)
	{
		typename mapUsedSMBlock::iterator f = m_mapUsedSMBlock.find(pData);
		if(f == m_mapUsedSMBlock.end())
		{
			return NULL;
		}
		else
		{
			_SMBlock* pSMBlock = (_SMBlock* )f->second;
			return (const time_t)pSMBlock->m_ttUpdateTime;
		}
	};

	bool SetObjectHeadTimeStamp(T* pData)
	{
		typename mapUsedSMBlock::iterator f = m_mapUsedSMBlock.find(pData);
		if(f == m_mapUsedSMBlock.end())
		{
			return false;
		}
		else
		{
			_SMBlock* pSMBlock = (_SMBlock* )f->second;
			pSMBlock->m_ttUpdateTime = time(NULL);
			return true;
		}
	}

private:
	typedef map<int, _SMBlock*> mapSMBlock;
	typedef map<T*, _SMBlock*>  mapUsedSMBlock;
	typedef map<int, _SMBlock*> mapFreeSMBlock;

private:
	CSMAccessObject* m_pSMAccessObject; //���öԹ����ڴ�Ľӿ�����
	mapSMBlock       m_mapSMBlock;      //��ǰ�Ѵ��ڵ��ڴ�鵥Ԫ
	mapUsedSMBlock   m_mapUsedSMBlock;  //����ʹ�õ��ڴ�鵥Ԫ
	mapFreeSMBlock   m_mapFreeSMBlock;  //Ŀǰ��δʹ�õ��ڴ�鵥Ԫ
	CSerial          m_Serial;          //���л����࣬���ڴ�������ڴ��ĵ�ַ�����������Ľṹ���ڹ����ڴ���
	int              m_nMaxCount;       //��ǰ��������ڴ������
};

#endif
