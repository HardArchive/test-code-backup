/**************************************************************
** RigorCalc.h: .
**
***************************************************************/

#ifndef AFX_RIGORCALC_H__FC9C727A_5204_4798_9686_0CC00C25729C__INCLUDED_
#define AFX_RIGORCALC_H__FC9C727A_5204_4798_9686_0CC00C25729C__INCLUDED_

#define INT_BIT_MAX 255
#define FLOAT_BIT_MAX 255

#ifndef CONST
	#ifdef __cplusplus
		#define CONST const
	#else
		#define CONST
	#endif
#endif

typedef struct tagWTNumber {
	int intbits;                     /* ������λ*/
	int floatbits;                   /* С����Ч��λ*/
	char infinite;                   /* �����*/
	char sign;                       /* ����*/
	char intpart[INT_BIT_MAX];       /* ��������*/
	char floatpart[FLOAT_BIT_MAX];   /* С������*/
} WTNumber;
/* ��������ָ������.*/
typedef void (*PFNCALC)(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);

/* ��ʼ��WTNumberΪ0.*/
void InitWTNumberToZero(WTNumber *pNum);
/* �ж���Ҫ���ٸ��ַ��ռ�洢WTNumberת�����ַ���.*/
int CharArrLenByWTNumber(CONST WTNumber* pNum);
/* ���ַ���ת����WTNumber.*/
void CharArrToWTNumber(CONST char *arr,WTNumber *pNum);
/* ��WTNumberת�����ַ���.*/
void WTNumberToCharArr(char *szBuf,CONST WTNumber *pNum);
/* ������λ,ɾ���������λ��0�ĺ����С��λ��0����λ.*/
void AdjustBits(WTNumber *pNum);
/* �ƶ�С����,deta=0���ƶ�,deta<0�����ƶ�,deta>0�����ƶ�.*/
void MoveFloatPoint(WTNumber *pNum,int deta);
/* ʹ����� */
void MakeInfinite(WTNumber *pNum);
/* ���������������ؽ�� */
char *Result(CONST char *val1,CONST char *val2,PFNCALC pfnCalc);
/* �Ƚ�2������С */
int WTCompare(CONST WTNumber* pn1,CONST WTNumber* pn2);
/* �ж��Ƿ�Ϊ0 */
int IsZero(CONST WTNumber *pNum);
/* �ӷ�*/
void WTAdd(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);
/* �˷�*/
void WTMultiply(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);
/* ����*/
void WTSubtract(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);
/* ����*/
void WTDivide(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);

#endif /* AFX_RIGORCALC_H__FC9C727A_5204_4798_9686_0CC00C25729C__INCLUDED_ */
