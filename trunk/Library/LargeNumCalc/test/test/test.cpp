// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>


////////#define SIZE			128  //һ���������ø��ֽڱ��棬����ʾλ������
//////////ֻ֧��ʮ��������
//////////�������ṹ
////////typedef struct BIG_NUM  
////////{
////////	BYTE byData[SIZE];  //�ռ�Ϊ(SIZE * 1)������SIZE���ֽ�  ÿһ��byte���һλ���ֵ��ַ���ʽ
////////
////////	BIG_NUM()
////////	{
////////		Init();
////////	}
////////
////////	void Init(char chIn = '0')
////////	{
////////		memset(byData, 0, SIZE);
////////		memset(byData, chIn, SIZE);
////////	}
////////
////////	//���صȺ�
////////	BIG_NUM& operator=(BIG_NUM& value)
////////	{
////////		Init();
////////		memcpy(byData, value.byData, SIZE);
////////		return *this;
////////	}
////////
////////	BIG_NUM& operator=(unsigned value)
////////	{
////////		Init();
////////		Add(value);
////////		return *this;
////////	}
////////
////////	//BIG_NUM& operator=(unsigned long value)
////////	//{
////////	//	//memset(byData, 0, SIZE);
////////	//	//memcpy(byData, value.byData, SIZE);
////////	//	*byData = value;
////////	//	return *this;
////////	//}
////////	//BIG_NUM& operator+(BIG_NUM& value)
////////	//{
////////	//	Dollar += value.Dollar;
////////	//	Cents += value.Cents;
////////	//	return *this;
////////	//}
////////
////////	BIG_NUM& Add(unsigned value)
////////	{
////////		BIGNUM bgTem;
////////		unsigned uiNum = value;
////////		int iCount = 0;
////////		while (uiNum>10)
////////		{
////////			iCount++;
////////			uiNum = uiNum/10;
////////		}
////////		Add(uiNum, iCount+1);
////////		uiNum = value%GetTenNum(iCount);
////////		if (uiNum>0)
////////		{
////////			Add(uiNum);
////////		}
////////		return *this;
////////	}
////////
////////	int GetTenNum(int iCount)
////////	{
////////		int iRet = 1;
////////		for (int i=iCount; iCount>0;iCount--)
////////		{
////////			iRet = iRet*10;
////////		}
////////		return iRet;
////////	}
////////	
////////
////////	void Add(unsigned uiIput, int iPosition)
////////	{
////////		unsigned uiTem = byData[SIZE-iPosition]-'0'+uiIput;
////////		if (uiTem>10)
////////		{
////////			Add(1, iPosition-1);
////////			byData[SIZE-iPosition] = uiTem-10+'0';
////////		}
////////		else
////////		{
////////			byData[SIZE-iPosition] += uiIput;
////////		}
////////	}
////////
////////	//���ش������ṹ�ĵ�ʵ�����ָ���
////////	int GetLen()
////////	{
////////		int iLen = 0;
////////		for (iLen=0; iLen<SIZE; iLen++)
////////		{
////////			if ('0' != byData[iLen])
////////			{
////////				break;
////////			}
////////		}
////////		return SIZE-iLen;
////////	}
////////
////////	void Print_10()  //��ӡʮ���ƴ�����
////////	{
////////		int iCount = 0;
////////		unsigned uiTem = 0;
////////		for (int i = SIZE-GetLen(); i < SIZE; i++)
////////		{
////////			printf("%c", byData[i]);
////////		}
////////		printf("\n");
////////	}
////////
////////
////////}BIGNUM, * PBIGNUM;
//////////
//////////
//////////BYTE atox(char ch)  //��һ��ʮ�����Ƶ��ַ�(4λ)תλ���֣�ת��ʧ�ܷ���xff
//////////{
//////////	BYTE res;
//////////	if (ch >= '0' && ch <= '9')
//////////	{
//////////		res = ch - '0';
//////////	}
//////////	else if (ch >= 'a' && ch <= 'f')
//////////	{
//////////		res = ch - 'a' + 10;
//////////	}
//////////	else if (ch >= 'A' && ch <= 'F')
//////////	{
//////////		res = ch - 'A' + 10;
//////////	}
//////////	else
//////////	{
//////////		printf("change error!\n");
//////////		return 0xff;
//////////	}
//////////	return res;
//////////}
//////////
////////////��ʼ����������strΪʮ�������ַ���
//////////BIGNUM Init(char* str)  
//////////{
//////////	BIGNUM bignum;
//////////	int len = (int)strlen(str);
//////////	int i;
//////////	int j = 0;
//////////	if (len > 2 * SIZE)
//////////	{
//////////		len = 2 * SIZE;
//////////	}
//////////	for (i = len - 1; i > 0; i -= 2)
//////////	{
//////////		bignum.byData[j] = atox(str[i]) + atox(str[i - 1]) * 16;
//////////		j++;
//////////	}
//////////	if (i == 0)
//////////	{
//////////		bignum.byData[j] = atox(str[i]);
//////////		j++;
//////////	}
//////////	while (j < SIZE)
//////////	{
//////////		bignum.byData[j] = 0;
//////////		j++;
//////////	}
//////////	return bignum;
//////////}
//////////BIGNUM Mul(BIGNUM bignum_a, BIGNUM bignum_b)  //�������˷�
//////////{
//////////	BIGNUM bignum_c = Init("0");
//////////	BYTE temp;
//////////	BYTE carry;
//////////	int i, j;
//////////	for (i = 0; i < SIZE; i++)
//////////	{
//////////		carry = 0;
//////////		for (j = 0; j < SIZE; j++)
//////////		{
//////////			temp = bignum_a.byData[i] * bignum_b.byData[j] + bignum_c.byData[j + i] + carry;
//////////			bignum_c.byData[j + i] = temp & 0x00ff;
//////////			carry = (temp >> 8) & 0xff;
//////////		}
//////////	}
//////////	return bignum_c;
//////////}
//////////
////////////����׳�  
//////////BIGNUM Factorial(int iInput)
//////////{
//////////	BIGNUM bnReturn = {0};
//////////	int iTem = iInput;
//////////	//if (iTem) bnReturn = 1;
//////////	while(iTem)
//////////	{
//////////		//bnReturn = bnReturn*(iTem--);
//////////		//bnReturn = Mul(bnReturn, iTem--);
//////////	}
//////////	return bnReturn;
//////////}
//////////#include "BigNum.h"

#include <stdio.h>
//#include "Rigor.h"
#include "Rigor.h"
typedef char CHARARR[256];
typedef unsigned char       BYTE;
int main()
//int _tmain(int argc, _TCHAR* argv[])
{
	////////int iTem = 2;
	////////int* pTem = &iTem;

	////////BIGNUM bgTem;

	////////bgTem = 200;
	////////bgTem.Print_10();

	////////iTem = 20000;
	////////pTem = &iTem;
	////////bgTem = 2000;
	////////bgTem.Print_10();

	////////bgTem = 20000;
	////////bgTem.Print_10();

	////////bgTem = 2014569;
	////////bgTem.Print_10();

	CHARARR num1;
	CHARARR num2;
	puts("Input two numbers,please:");
	while(scanf("%s%s",num1,num2)!=-1) {
		printf("The result of addition:\n%s\n",Result(num1,num2, WTAdd));
		printf("The result of subtration:\n%s\n",Result(num1,num2, WTSubtract));
		printf("The result of multiplication:\n%s\n",Result(num1,num2, WTMultiply));
		printf("The result of divide:\n%s\n",Result(num1,num2, WTDivide));
		puts("Input two numbers,please:");
	}

	return 0;
}

