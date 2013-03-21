// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <string>
using namespace std;

//�ַ���ǰ��ȥ��
void StringFrontMinusZero(string& strIN)
{
	if ('0' != strIN.at(0)) return;
	strIN.assign(strIN, 1, strIN.length());
	StringFrontMinusZero(strIN);
}
//IP�ַ���ȥ0
void MinusZeroIP(string strInIP, string& strOutIP)
{
	if (!strInIP.length()) return;
	string strFront;
	string strBack = strInIP;
	size_t sizetPos = strBack.find('.');
	//���û�ҵ�
	if (string::npos == sizetPos)
	{
		StringFrontMinusZero(strBack);
		strOutIP.append(strBack, 0, strBack.length());
		return;
	}
	strFront.append(strBack, 0, sizetPos+1);
	StringFrontMinusZero(strFront);
	strBack.erase(0, sizetPos+1);
	
	MinusZeroIP(strBack, strFront);	

	strOutIP.append(strFront, 0, strFront.length());
}

//����ַ�������С��3��ǰ����� ������λ
void RuleString(string& strInIP)
{
	size_t sizetLen = strInIP.length();
	if (sizetLen<3)
	{
		strInIP.insert(0, 3-sizetLen, '0');
	}
}
//IP�ַ�����0
void AddZeroIP(string strInIP, string& strOutIP)
{
	if (!strInIP.length()) return;
	string strFront;
	string strBack = strInIP;
	size_t sizetPos = strBack.find('.');
	//���û�ҵ�
	if (string::npos == sizetPos)
	{
		RuleString(strBack);
		strOutIP.append(strBack, 0, strBack.length());
		return;
	}
	strFront.append(strBack, 0, sizetPos);
	RuleString(strFront);
	strFront.append(1, '.');
	strBack.erase(0, sizetPos+1);

	AddZeroIP(strBack, strFront);	

	strOutIP.append(strFront, 0, strFront.length());

}

int _tmain(int argc, _TCHAR* argv[])
{
	string strIP1("092.108.030.007");
	string strIP2;
	MinusZeroIP(strIP1, strIP2);
	strIP1.clear();
	AddZeroIP(strIP2, strIP1);
	return 0;
}

