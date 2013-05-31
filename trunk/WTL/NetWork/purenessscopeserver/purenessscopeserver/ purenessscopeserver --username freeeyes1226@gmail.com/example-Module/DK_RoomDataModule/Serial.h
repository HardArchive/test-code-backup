#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

//�������л��������
class CSerial
{
public:
	CSerial(void);
	~CSerial(void);

	void Close();
	bool Init(char* pData, int nCellSize, int nCount);

	char* GetData(int nIndex);

	char* GetBase() const;

public:
	char* m_pData;               //�����ڴ�ͷ����һ����ַ��ͷָ���ַ
	int   m_nSize;               //��ǰ�����ڴ�ͷ����Ĵ�С
	int   m_nCellSize;           //��ǰͷ�����ݿ�Ĵ�С
	int   m_nCount;              //��ǰͷ�����ݿ�ĸ���

};
#endif
