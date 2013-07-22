// BigNum.c : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <string.h>
#include "BigNum.h"

UCHAR atox(char ch)  //将一个十六进制的字符(4位)转位数字，转换失败返回xff
{
	UCHAR res;
	if (ch >= '0' && ch <= '9')
	{
		res = ch - '0';
	}
	else if (ch >= 'a' && ch <= 'f')
	{
		res = ch - 'a' + 10;
	}
	else if (ch >= 'A' && ch <= 'F')
	{
		res = ch - 'A' + 10;
	}
	else
	{
		printf("change error!\n");
		return 0xff;
	}
	return res;
}

int GetByte(BigNum bignum)  //判断有多少个字节不为
{
	int ByteOfBigNum = SIZE;
	while ((bignum.data[ByteOfBigNum - 1] == 0) && (ByteOfBigNum > 0))
	{
		ByteOfBigNum--;
	}
	return ByteOfBigNum;
}

BigNum MovByteLetf(BigNum bignum, int n)  //左移n个字节
{
	int i;
	int bignum_len = GetByte(bignum);
	for (i = bignum_len - 1; i >= 0; i--)
	{
		if (i + n >= SIZE)
		{
			continue;
		}
		bignum.data[i + n] = bignum.data[i];
	}
	for (i = n - 1; i >= 0; i--)
	{
		bignum.data[i] = 0;
	}
	return bignum;
}

BigNum MovByteRight(BigNum bignum, int n)  //右移n个字节
{
	int i;
	int bignum_len = GetByte(bignum);
	for (i = 0; i < bignum_len; i++)
	{
		if (i + n >= SIZE)
		{
			bignum.data[i] = 0;
			continue;
		}
		bignum.data[i] = bignum.data[i + n];
	}
	return bignum;
}

//初始化大整数，str为十六进制字符串
BigNum Init(char* str)  
{
	BigNum bignum;
	int len = (int)strlen(str);
	int i;
	int j = 0;
	if (len > 2 * SIZE)
	{
		len = 2 * SIZE;
	}
	for (i = len - 1; i > 0; i -= 2)
	{
		bignum.data[j] = atox(str[i]) + atox(str[i - 1]) * 16;
		j++;
	}
	if (i == 0)
	{
		bignum.data[j] = atox(str[i]);
		j++;
	}
	while (j < SIZE)
	{
		bignum.data[j] = 0;
		j++;
	}
	return bignum;
}

int Cmp(BigNum bignum_a, BigNum bignum_b)  //大整数比较大小，>返回，<返回-1，==返回
{
	int bignum_a_len = GetByte(bignum_a);
	int bignum_b_len = GetByte(bignum_b);
	int max = bignum_a_len > bignum_b_len ? bignum_a_len : bignum_b_len;
	int i;
	for (i = max - 1; i >= 0; i--)
	{
		if (bignum_a.data[i] > bignum_b.data[i])
		{
			return 1;
		}
		if (bignum_a.data[i] < bignum_b.data[i])
		{
			return -1;
		}
	}
	return 0;
}

BigNum Add(BigNum bignum_a, BigNum bignum_b)  //大整数加法
{
	BigNum bignum_c;
	USHORT temp;
	UCHAR carry = 0;
	int i;
	for (i = 0; i < SIZE; i++)
	{
		temp = bignum_a.data[i] + bignum_b.data[i] + carry;
		bignum_c.data[i] = temp & 0x00ff;
		carry = (temp >> 8) & 0xff;
	}
	return bignum_c;
}

BigNum Sub(BigNum bignum_a, BigNum bignum_b)  //大整数减法
{
	BigNum bignum_c;
	USHORT temp;
	UCHAR carry = 0;
	int i;
	for (i = 0; i < SIZE; i++)
	{
		temp = bignum_a.data[i] - bignum_b.data[i] - carry;
		bignum_c.data[i] = temp & 0x00ff;
		carry = (temp >> 15) & 0x01;
	}
	return bignum_c;
}

BigNum Mul(BigNum bignum_a, BigNum bignum_b)  //大整数乘法
{
	BigNum bignum_c = Init("0");
	USHORT temp;
	UCHAR carry;
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		carry = 0;
		for (j = 0; j < SIZE; j++)
		{
			temp = bignum_a.data[i] * bignum_b.data[j] + bignum_c.data[j + i] + carry;
			bignum_c.data[j + i] = temp & 0x00ff;
			carry = (temp >> 8) & 0xff;
		}
	}
	return bignum_c;
}

BigNum Div(BigNum bignum_a, BigNum bignum_b)  //大整数除法
{
	BigNum bignum_c = Init("0");
	BigNum B;
	int bignum_a_len;
	int bignum_b_len;
	int bignum_c_len;
	if (Cmp(bignum_b, bignum_c) == 0)
	{
		printf("错误！除数为\n");
		return bignum_c;
	}
	bignum_a_len = GetByte(bignum_a);
	bignum_b_len = GetByte(bignum_b);
	bignum_c_len = bignum_a_len - bignum_b_len;
	while (bignum_c_len >= 0)
	{
		B = MovByteLetf(bignum_b, bignum_c_len);
		while (Cmp(bignum_a, B) != -1)
		{
			bignum_a = Sub(bignum_a, B);
			bignum_c.data[bignum_c_len]++;
		}
		bignum_c_len--;
	}
	return bignum_c;
}

BigNum Mod(BigNum bignum_a, BigNum bignum_b)  //大整数模运算
{
	BigNum bignum_c = Init("0");
	BigNum B;
	int bignum_a_len;
	int bignum_b_len;
	int bignum_c_len;
	if (Cmp(bignum_b, bignum_c) == 0)
	{
		printf("错误！除数为\n");
		return bignum_c;
	}
	bignum_a_len = GetByte(bignum_a);
	bignum_b_len = GetByte(bignum_b);
	bignum_c_len = bignum_a_len - bignum_b_len;
	while (bignum_c_len >= 0)
	{
		B = MovByteLetf(bignum_b, bignum_c_len);
		while (Cmp(bignum_a, B) != -1)
		{
			bignum_a = Sub(bignum_a, B);
		}
		bignum_c_len--;
	}
	return bignum_a;
}

BigNum Pow(BigNum bignum, int n)  //大整数乘方运算
{
	int i;
	BigNum bignum_res = Init("1");
	for (i = 0; i < n; i++)
	{
		bignum_res = Mul(bignum_res, bignum);
	}
	return bignum_res;
}

void Print_16(BigNum bignum)  //打印十六进制大整数
{
	int i;
	int nFlag = 0;
	for (i = SIZE - 1; i >= 0; i--)
	{
		if (nFlag == 0 && bignum.data[i] == 0)  //前面是的不打印
		{
			continue;
		}
		else
		{
			nFlag++;
			if (nFlag == 1)  //首位的不打印，如x01，只打印
			{
				printf("%x", bignum.data[i]);
			}
			else  //后面的要打印
			{
				printf("%02x", bignum.data[i]);
			}
		}
	}
	if (nFlag == 0)
	{
		printf("0");
	}
	printf("\n");
}

void Print_10(BigNum bignum)  //打印十进制大整数
{
	int data_10[SIZE_10];
	int i = 0;
	int j;
	while (Cmp(bignum, Init("0")) == 1)
	{
		data_10[i] = (Mod(bignum, Init("a"))).data[0];
		bignum = (Div(bignum, Init("a")));
		i++;
	}
	for (j = i - 1; j >= 0; j--)
	{
		printf("%d", data_10[j]);
	}
	printf("\n");
}
