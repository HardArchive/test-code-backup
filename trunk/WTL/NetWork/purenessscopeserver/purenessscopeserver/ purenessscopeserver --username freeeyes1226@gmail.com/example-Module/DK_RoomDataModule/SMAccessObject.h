#ifndef _SMACCESSOBJECT_H
#define _SMACCESSOBJECT_H

#include "ShareMemoryAPI.h"

class CSMAccessObject
{
public:
	CSMAccessObject(void);
	~CSMAccessObject(void);

	//�������󣬴������ڴ�Ϊʵ���ڴ�+ͷ����
	bool Create(SMKey key, int nSize, int nHeadSize);

	//�������Ƿ���ڣ�����������ٴ�����ֱ�Ӵ�
	bool Open(SMKey key, int nSize, int nHeadSize);

	//ɾ������
	void Destory();

	//�õ�ָ������ƫ�Ƶ�ָ��λ��
	char* GetData(int nSize);

	//�õ�ͷ������λ��
	char* GetHeadData();

	//�õ��˶�����ܳ���
	int GetDataSize();

	//�õ��˶����ͷ�ܳ���
	int GetHeadDataSize();

private:
	SMHandle m_SMHandle;
	SMKey    m_Key;
	int      m_nSize;
	int      m_nHeadSize;
	char*    m_pHeadData;
	char*    m_pData;
};
#endif
