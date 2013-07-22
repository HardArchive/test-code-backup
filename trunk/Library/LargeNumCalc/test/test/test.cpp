// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>


////////#define SIZE			128  //一个大整数用个字节保存，最多表示位大整数
//////////只支持十进制运算
//////////大整数结构
////////typedef struct BIG_NUM  
////////{
////////	BYTE byData[SIZE];  //空间为(SIZE * 1)，就是SIZE个字节  每一个byte存放一位数字的字符形式
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
////////	//重载等号
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
////////	//返回大整数结构的的实际数字个数
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
////////	void Print_10()  //打印十进制大整数
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
//////////BYTE atox(char ch)  //将一个十六进制的字符(4位)转位数字，转换失败返回xff
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
////////////初始化大整数，str为十六进制字符串
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
//////////BIGNUM Mul(BIGNUM bignum_a, BIGNUM bignum_b)  //大整数乘法
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
////////////计算阶乘  
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

