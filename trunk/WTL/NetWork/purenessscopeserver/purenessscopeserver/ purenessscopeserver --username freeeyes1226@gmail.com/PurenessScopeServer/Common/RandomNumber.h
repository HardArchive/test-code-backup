#ifndef _RANDOMNUMBER_H
#define _RANDOMNUMBER_H

//����������ĺ���
//����ָ����Χ���������������

#include <stdlib.h>
#include <time.h>

class CRandomNumber
{
public:
	CRandomNumber(void);
	~CRandomNumber(void);

	void SetRange(int nMinNumber, int nMaxNumber);
	int GetRandom();

private:
	int m_nMinNumber;
	int m_nMaxNumber;
};
#endif
