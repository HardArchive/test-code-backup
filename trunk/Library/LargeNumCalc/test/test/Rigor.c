/***********************************************
** RigorCalc.c: implementation
************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Rigor.h"

void InitWTNumberToZero(WTNumber *pNum)
{
	memset(pNum,0,sizeof(WTNumber));
}
void AdjustBits(WTNumber *pNum)
{
	while(pNum->intbits>1&&pNum->intpart[pNum->intbits-1]==0) pNum->intbits--;
	while(pNum->floatbits&&pNum->floatpart[pNum->floatbits-1]==0) pNum->floatbits--;
}
void MoveFloatPoint(WTNumber *pNum,int deta)
{
	/* deta<0�������ƶ�С����,deta>0�������ƶ�С���� */
	if(deta)
	{
		WTNumber n=*pNum;
		InitWTNumberToZero(pNum);
		pNum->sign=n.sign;
		if(deta<0)
		{
			int i;
			deta=-deta;
			for(i=deta;i<n.intbits;i++)
			{
				pNum->intpart[pNum->intbits++]=n.intpart[i];
			}
			for(i=deta-1;i>=0;i--)
			{
				pNum->floatpart[pNum->floatbits++]=n.intpart[i];
			}
			for(i=0;i<n.floatbits;i++)
			{
				pNum->floatpart[pNum->floatbits++]=n.floatpart[i];
			}
		}
		else
		{
			int i;
			for(i=deta;i<n.floatbits;i++)	/* ����С������*/
			{
				pNum->floatpart[pNum->floatbits++]=n.floatpart[i];
			}
			for(i=deta-1;i>=0;i--)	/* С���������Ĳ���*/
			{
				pNum->intpart[pNum->intbits++]=n.floatpart[i];
			}
			for(i=0;i<n.intbits;i++)	/* ��������*/
			{
				pNum->intpart[pNum->intbits++]=n.intpart[i];
			}
		}
	}
	AdjustBits(pNum);
}

int CharArrLenByWTNumber(CONST WTNumber* pNum)
{
	int len = pNum->floatbits+pNum->intbits+1;
	if(pNum->intbits==0) len++;	/* .1 --> 0.1*/
	if(pNum->floatbits) len++;	/* '.'*/
	if(pNum->sign) len++;		/* '-'*/
	if(pNum->infinite) return 11;    /* #INFINITE */
	return len;
}
void CharArrToWTNumber(CONST char *arr,WTNumber *pNum)
{
	char *point;
	InitWTNumberToZero(pNum);
	if(*arr=='-')	/* ����Ǹ���*/
	{
		arr++;
		pNum->sign=1;
	}
	point=strchr(arr,'.');
	if(point)	/* �ҵ�С���� */
	{
		int n=pNum->intbits=point-arr;	/* �����������λ */
		while(n)	/* ������λ��==0��ѭ�� */
		{
			pNum->intpart[pNum->intbits-n]=arr[n-1]-'0';	/* �����ֵ�λ���ڵ��±�Ԫ��*/
			n--;
		}
		while(*++point)
		{
			pNum->floatpart[pNum->floatbits]=*point-'0';
			pNum->floatbits++;
		}
	}
	else	/* ˵��ûдС����,ȫ������.*/
	{
		int n=pNum->intbits=strlen(arr);
		while(n)
		{
			pNum->intpart[pNum->intbits-n]=arr[n-1]-'0';
			n--;
		}
	}
	AdjustBits(pNum);
	/* ����-0 ��0�����*/
	if(pNum->floatbits==0)
	{
		if(pNum->intbits==0 || pNum->intbits==1&&pNum->intpart[0]==0)
			pNum->sign=0;
	}
}

void WTNumberToCharArr(char *szBuf,CONST WTNumber* pNum)
{
	int n=pNum->intbits,c;
	memset(szBuf,0,CharArrLenByWTNumber(pNum));
	if(pNum->sign)	/* ����Ǹ���*/
	{
		*szBuf++='-';
	}
	if(pNum->infinite) 
	{
		strcat(szBuf,"#INFINITE");
		return;
	}
	while(n)
	{
		szBuf[pNum->intbits-n]=pNum->intpart[n-1]+'0';
		n--;
	}
	c=0;	/* �Ƿ����0*/
	if(pNum->intbits==0) {
		strcat(szBuf,"0");
		c=1;
	}
	if(pNum->floatbits) strcat(szBuf,".");
	n=0;
	while(n<pNum->floatbits)
	{
		szBuf[pNum->intbits+1+n+c]=pNum->floatpart[n]+'0';
		n++;
	}
}

char *Result(CONST char *val1,CONST char *val2,PFNCALC pfnCalc)
{
	static char *s_szRes=NULL;
	WTNumber n1,n2,res;
	if(s_szRes) {
		/*delete [] s_szRes;*/
		free(s_szRes);
		s_szRes=NULL;
	}
	CharArrToWTNumber(val1,&n1);
	CharArrToWTNumber(val2,&n2);
	pfnCalc(&n1,&n2,&res);
/*	s_szRes = new char[CharArrLenByWTNumber(&res)];*/
	s_szRes=(char*)malloc(CharArrLenByWTNumber(&res)*sizeof(char));
	WTNumberToCharArr(s_szRes,&res);
	return s_szRes;
}

int WTCompare(CONST WTNumber* pn1,CONST WTNumber* pn2)
{
	/* �����ǱȽϷ���*/
	if(pn1->sign==0&&pn2->sign!=0)	/* pn1������,pn2�Ǹ���*/
		return 1;	/* >*/
	else if(pn1->sign!=0&&pn2->sign==0)	/* pn1�Ǹ���,pn2������*/
		return -1;	/* <*/
	else	/* ͬ��״̬*/
	{
		/* �Ƚ���������*/
		if(pn1->intbits>pn2->intbits)	/* pn1������λ��*/
			return pn1->sign?-1:1;
		else if(pn1->intbits<pn2->intbits)
			return pn1->sign?1:-1;
		else	/* ������λ��ͬ*/
		{
			int i=pn1->intbits-1;	/*ָ�����λ*/
			while(i>=0)
			{
				if(pn1->intpart[i]>pn2->intpart[i])
					return pn1->sign?-1:1;
				else if(pn1->intpart[i]<pn2->intpart[i])
					return pn1->sign?1:-1;
				else i--;
			}
			/* ����������ͬ,�Ƚ�С������*/
			for(i=0;i<pn1->floatbits&&i<pn2->floatbits;)
			{
				if(pn1->floatpart[i]>pn2->floatpart[i])
					return pn1->sign?-1:1;
				else if(pn1->floatpart[i]<pn2->floatpart[i])
					return pn1->sign?1:-1;
				else i++;
			}
			if(i<pn1->floatbits) return pn1->sign?-1:1;
			if(i<pn2->floatbits) return pn1->sign?1:-1;
			return 0;	/* ���*/
		}
	}
}
void MakeInfinite(WTNumber *pNum)
{
	pNum->infinite=1;
}
int IsZero(CONST WTNumber* pNum)
{
	if(pNum->floatbits==0&&pNum->intbits==0) return 1;
	if(pNum->floatbits==0&&pNum->intbits==1&&pNum->intpart[0]==0) return 1;
	return 0;
}

void WTAdd(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes)
{
	InitWTNumberToZero(pRes);
	if(pn1->sign^pn2->sign)	/*���*/
	{
		WTNumber n2=*pn2;
		n2.sign=pn1->sign;
		WTSubtract(pn1,&n2,pRes);
	}
	else	/*ͬ��*/
	{
		int maxfloatbits=pn1->floatbits>pn2->floatbits?pn1->floatbits:pn2->floatbits;
		int addbit=0;	/* ��λֵ*/
		int i,j;
		for(i=maxfloatbits-1;i>=0;i--)
		{
			int value=pn1->floatpart[i]+pn2->floatpart[i]+addbit;
			addbit=value/10;	/* �����Ƿ񳬹�10. ���ý�λֵ*/
			pRes->floatpart[i]=value%10;
		}
		pRes->floatbits=maxfloatbits;
		/* ����,С�����ּ������.*/
		for(j=0;j<pn1->intbits||j<pn2->intbits;j++)
		{
			int value=pn1->intpart[j]+pn2->intpart[j]+addbit;
			addbit=value/10;
			pRes->intpart[j]=value%10;
			pRes->intbits++;
		}
		if(addbit>0)
		{
			pRes->intpart[j]=addbit;
			pRes->intbits++;
		}
		pRes->sign=pn1->sign;	/*��������*/
		AdjustBits(pRes);
	}
}

void WTMultiply(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes)
{
	WTNumber z1=*pn1,z2=*pn2;
	WTNumber sum={0};
	int i,j;
	InitWTNumberToZero(pRes);
	MoveFloatPoint(&z1,z1.floatbits);
	MoveFloatPoint(&z2,z2.floatbits);
	/* ����z1*z2 */
	for(i=0;i<z2.intbits;i++)
	{
		WTNumber tmp={0};	/* �����ʱ�˻�*/
		int addbit=0;
		tmp.intbits=z1.intbits+i;
		for(j=0;j<z1.intbits;j++)
		{
			int value = z2.intpart[i]*z1.intpart[j]+addbit;
			addbit=value/10;
			tmp.intpart[j+i]=value%10;
		}
		if(addbit)
		{
			tmp.intpart[j+i]=addbit;
			tmp.intbits++;
		}
		WTAdd(&sum,&tmp,pRes);
		sum=*pRes;
	}
	*pRes=sum;
	MoveFloatPoint(pRes,-(pn1->floatbits+pn2->floatbits));
	/* �жϷ���,���Ϊ��*/
	if(pn1->sign^pn2->sign) pRes->sign=1;
}

void WTSubtract(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes)
{
	InitWTNumberToZero(pRes);
	if(pn1->sign^pn2->sign)	/* ������*/
	{
		WTNumber n2=*pn2;
		n2.sign=pn1->sign;
		WTAdd(pn1,&n2,pRes);
	}
	else	/* ͬ�����*/
	{
		int cmp=WTCompare(pn1,pn2);
		int swapflag,i,maxfloatbits,subtractbit;
		if(cmp==0) return;	/* ��Ⱦ�û��Ҫ�ټ���.*/
		swapflag=pn1->sign==0?cmp==-1:cmp==1;
		if(swapflag)
		{
			CONST WTNumber* p=pn1;
			pn1=pn2;
			pn2=p;
		}
		maxfloatbits=pn1->floatbits>pn2->floatbits?pn1->floatbits:pn2->floatbits;
		subtractbit=0;	/* ��λֵ*/
		/* �ȼ���С������*/
		for(i=maxfloatbits-1;i>=0;i--)
		{
			if(pn1->floatpart[i]-subtractbit<pn2->floatpart[i])
			{
				int value=pn1->floatpart[i]-pn2->floatpart[i]-subtractbit+10;
				subtractbit=1;
				pRes->floatpart[i]=value;
			}
			else
			{
				int value=pn1->floatpart[i]-pn2->floatpart[i]-subtractbit;
				subtractbit=0;
				pRes->floatpart[i]=value;
			}
		}
		pRes->floatbits=maxfloatbits;
		/* ����С�����ּ������.*/
		for(i=0;i<pn1->intbits||i<pn2->intbits;i++)
		{
			if(pn1->intpart[i]-subtractbit<pn2->intpart[i])
			{
				int value=pn1->intpart[i]-pn2->intpart[i]-subtractbit+10;
				subtractbit=1;
				pRes->intpart[i]=value;
			}
			else
			{
				int value=pn1->intpart[i]-pn2->intpart[i]-subtractbit;
				subtractbit=0;
				pRes->intpart[i]=value;
			}
			pRes->intbits++;
		}
		pRes->sign=swapflag?!pn1->sign:pn1->sign;	/*��������*/
		AdjustBits(pRes);
	}
}

void WTDivide(CONST WTNumber* pn1,CONST WTNumber* pn2,WTNumber* pRes)
{
	WTNumber z1=*pn1,z2=*pn2;
	int deta=z2.floatbits-z1.floatbits;
	MoveFloatPoint(&z1,z1.floatbits);
	MoveFloatPoint(&z2,z2.floatbits);
	InitWTNumberToZero(pRes);
	if(IsZero(pn1)) return ;
	if(IsZero(pn2)) {
		pRes->sign=pn1->sign;
		MakeInfinite(pRes);
		return ;
	}
	z1.sign=z2.sign=0;                /*ͳһ����,���ڱȽϴ�С*/ 
	while(z1.intbits!=z2.intbits) {    /*ȷ����λ���,�ⲽ�ķѺܶ�ʱ��*/
		if(z1.intbits<z2.intbits) {
			MoveFloatPoint(&z1,1);
			deta--;
		} else {
			MoveFloatPoint(&z2,1);
			deta++;
		}
	}
	while(pRes->floatbits<(INT_BIT_MAX/2)) {
		int cmp=WTCompare(&z1,&z2);
		int n=10;
		WTNumber mulres={0},subres={0};
		if(cmp==-1) {      /*z1<z2*/
			MoveFloatPoint(&z1,1);
			pRes->floatpart[pRes->floatbits++]=0;
			continue;
		} else if(cmp==0) { /*z1==z2*/
			pRes->floatpart[pRes->floatbits++]=1;
			break;
		}
		do {	/*����*/
			WTNumber tmp={0};
			tmp.intpart[0]=--n;
			tmp.intbits=1;
			WTMultiply(&z2,&tmp,&mulres);
		} while((cmp=WTCompare(&mulres,&z1))==1);
		pRes->floatpart[pRes->floatbits++]=n;
		if(cmp==0) break;
		WTSubtract(&z1,&mulres,&subres);
		MoveFloatPoint(&subres,1);
		z1=subres;
	}
	MoveFloatPoint(pRes,1);
	MoveFloatPoint(pRes,deta);
	/* �жϷ���,���Ϊ��*/
	if(pn1->sign^pn2->sign) pRes->sign=1;
}
