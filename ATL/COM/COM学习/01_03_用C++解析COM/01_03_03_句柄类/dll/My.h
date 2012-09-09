#pragma once

class  CMy
{
	int m_iResult;
public:
	CMy(void);
	~CMy(void);
	void Add(int x1,int x2);
	int Get()
	{
		return m_iResult;
	}
};
