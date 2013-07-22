#ifndef _BIGNUM_H_ 
#define _BIGNUM_H_ 

#define SIZE			128  //一个大整数用个字节保存，最多表示位大整数
#define SIZE_10			2 * SIZE

typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;

UCHAR atox(char ch);  //将一个十六进制的字符(4位)转位数字，转换失败返回xff

typedef struct BigNum  //大整数结构
{
	UCHAR data[SIZE];  //空间为(SIZE * sizeof(UCHAR))，就是SIZE个字节
}BigNum;

BigNum Init(char* str);  //初始化大整数，str为十六进制字符串

int GetByte(BigNum bignum);  //判断有多少个字节不为

BigNum MovByteLetf(BigNum bignum, int n);  //左移n个字节

BigNum MovByteRight(BigNum bignum, int n);  //右移n个字节

int Cmp(BigNum bignum_a, BigNum bignum_b);  //大整数比较大小，>返回，<返回-1，==返回

BigNum Add(BigNum bignum_a, BigNum bignum_b);  //大整数加法

BigNum Sub(BigNum bignum_a, BigNum bignum_b);  //大整数减法

//BigNum Mul(BigNum bignum_a, UCHAR uchar);  //大整数乘法UCHAR

BigNum Mul(BigNum bignum_a, BigNum bignum_b);  //大整数乘法

BigNum Div(BigNum bignum_a, BigNum bignum_b);  //大整数除法

BigNum Mod(BigNum bignum_a, BigNum bignum_b);  //大整数模运算

BigNum Pow(BigNum bignum, int n);  //大整数乘方运算

void Print_16(BigNum bignum);  //打印十六进制大整数

void Print_10(BigNum bignum);  //打印十进制大整数

#endif
