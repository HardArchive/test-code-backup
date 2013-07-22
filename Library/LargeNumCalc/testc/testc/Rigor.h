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
	int intbits;                     /* 整数数位*/
	int floatbits;                   /* 小数有效数位*/
	char infinite;                   /* 无穷大*/
	char sign;                       /* 符号*/
	char intpart[INT_BIT_MAX];       /* 整数部分*/
	char floatpart[FLOAT_BIT_MAX];   /* 小数部分*/
} WTNumber;
/* 算术函数指针类型.*/
typedef void (*PFNCALC)(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);

/* 初始化WTNumber为0.*/
void InitWTNumberToZero(WTNumber *pNum);
/* 判断需要多少个字符空间存储WTNumber转换的字符串.*/
int CharArrLenByWTNumber(CONST WTNumber* pNum);
/* 从字符串转换到WTNumber.*/
void CharArrToWTNumber(CONST char *arr,WTNumber *pNum);
/* 从WTNumber转换到字符串.*/
void WTNumberToCharArr(char *szBuf,CONST WTNumber *pNum);
/* 调节数位,删除最高整数位是0的和最低小数位是0的数位.*/
void AdjustBits(WTNumber *pNum);
/* 移动小数点,deta=0不移动,deta<0往左移动,deta>0往右移动.*/
void MoveFloatPoint(WTNumber *pNum,int deta);
/* 使无穷大 */
void MakeInfinite(WTNumber *pNum);
/* 根据算术函数返回结果 */
char *Result(CONST char *val1,CONST char *val2,PFNCALC pfnCalc);
/* 比较2个数大小 */
int WTCompare(CONST WTNumber* pn1,CONST WTNumber* pn2);
/* 判断是否为0 */
int IsZero(CONST WTNumber *pNum);
/* 加法*/
void WTAdd(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);
/* 乘法*/
void WTMultiply(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);
/* 减法*/
void WTSubtract(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);
/* 除法*/
void WTDivide(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes);

#endif /* AFX_RIGORCALC_H__FC9C727A_5204_4798_9686_0CC00C25729C__INCLUDED_ */
