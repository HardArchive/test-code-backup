#ifndef _BIGNUM_H_ 
#define _BIGNUM_H_ 

#define SIZE			128  //һ���������ø��ֽڱ��棬����ʾλ������
#define SIZE_10			2 * SIZE

typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;

UCHAR atox(char ch);  //��һ��ʮ�����Ƶ��ַ�(4λ)תλ���֣�ת��ʧ�ܷ���xff

typedef struct BigNum  //�������ṹ
{
	UCHAR data[SIZE];  //�ռ�Ϊ(SIZE * sizeof(UCHAR))������SIZE���ֽ�
}BigNum;

BigNum Init(char* str);  //��ʼ����������strΪʮ�������ַ���

int GetByte(BigNum bignum);  //�ж��ж��ٸ��ֽڲ�Ϊ

BigNum MovByteLetf(BigNum bignum, int n);  //����n���ֽ�

BigNum MovByteRight(BigNum bignum, int n);  //����n���ֽ�

int Cmp(BigNum bignum_a, BigNum bignum_b);  //�������Ƚϴ�С��>���أ�<����-1��==����

BigNum Add(BigNum bignum_a, BigNum bignum_b);  //�������ӷ�

BigNum Sub(BigNum bignum_a, BigNum bignum_b);  //����������

//BigNum Mul(BigNum bignum_a, UCHAR uchar);  //�������˷�UCHAR

BigNum Mul(BigNum bignum_a, BigNum bignum_b);  //�������˷�

BigNum Div(BigNum bignum_a, BigNum bignum_b);  //����������

BigNum Mod(BigNum bignum_a, BigNum bignum_b);  //������ģ����

BigNum Pow(BigNum bignum, int n);  //�������˷�����

void Print_16(BigNum bignum);  //��ӡʮ�����ƴ�����

void Print_10(BigNum bignum);  //��ӡʮ���ƴ�����

#endif
