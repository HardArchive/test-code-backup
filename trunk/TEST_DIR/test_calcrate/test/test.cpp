// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "RateCalc.h"
//                  3��     6��     12��    24��     36��    60��
//����	  ��������	������	����	    һ��	    ����	    ����	    ����
//���л�׼	0.35 	2.60 	2.80 	3.00 	3.75 	4.25 	4.75 
//��������	0.39 	2.86 	3.08 	3.30 	3.75 	4.25 	4.75 
//��������	0.35 	2.85 	3.05 	3.25 	3.75 	4.25 	4.75 
//�ַ�����	0.39 	2.86 	3.08 	3.30 	3.75 	4.25 	4.75 

BANKREGULARRATES g_szBankRegularRates[5] = {
	{_T("���л�׼"), {{{0,1},0.35},{{1,3},2.60},{{1,6},2.80},{{0,1},3.00},{{0,2},3.75},{{0,3},4.25},{{0,5},4.75}}},
	{_T("��������"), {{{0,1},0.39},{{1,3},2.86},{{1,6},3.08},{{0,1},3.30},{{0,2},3.75},{{0,3},4.25},{{0,5},4.75}}},
	{_T("��������"), {{{0,1},0.35},{{1,3},2.85},{{1,6},3.05},{{0,1},3.25},{{0,2},3.75},{{0,3},4.25},{{0,5},4.75}}},
	{_T("�ַ�����"), {{{0,1},0.39},{{1,3},2.86},{{1,6},3.08},{{0,1},3.30},{{0,2},3.75},{{0,3},4.25},{{0,5},4.75}}},
	{_T("���һ���"), {{{0,1},5.00},{{0,1},5.00},{{0,1},5.00},{{0,1},5.00},{{0,1},5.00},{{0,1},5.00},{{0,1},5.00}}},
};
//10K �� 3���� ��18�� 20544.676316626468
int _tmain(int argc, _TCHAR* argv[])
{
	PREGULARRATES pstuRegularates = &g_szBankRegularRates[1].stuRegularRates[1];
	PREGULARRATES pstuRegularates2 = &g_szBankRegularRates[1].stuRegularRates[2];
	PREGULARRATES pstuRegularates3 = &g_szBankRegularRates[1].stuRegularRates[3];
	PREGULARRATES pstuRegularates4 = &g_szBankRegularRates[1].stuRegularRates[4];
	PREGULARRATES pstuRegularates5 = &g_szBankRegularRates[1].stuRegularRates[5];
	PREGULARRATES pstuRegularates6 = &g_szBankRegularRates[1].stuRegularRates[6];
	STORAGETIME stuInStorageTime = {0, 18};
	RG::CRateCalc clsRateCalc;
	double dPrincipalInterest = clsRateCalc.GetPrincipalInterest(10000, pstuRegularates, stuInStorageTime);
	double dPrincipalInterest2 = clsRateCalc.GetPrincipalInterest(10000, pstuRegularates2, stuInStorageTime);
	double dPrincipalInterest3 = clsRateCalc.GetPrincipalInterest(10000, pstuRegularates3, stuInStorageTime);
	double dPrincipalInterest4 = clsRateCalc.GetPrincipalInterest(10000, pstuRegularates4, stuInStorageTime);
	double dPrincipalInterest5 = clsRateCalc.GetPrincipalInterest(10000, pstuRegularates5, stuInStorageTime);
	double dPrincipalInterest6 = clsRateCalc.GetPrincipalInterest(10000, pstuRegularates6, stuInStorageTime);

	STORAGETIME stuInIntervalTime = {0, 1};
	PREGULARRATES pstuRegularates7 = &g_szBankRegularRates[1].stuRegularRates[6];
	double dPrincipalInterest7 = clsRateCalc.GetRegular(10000, pstuRegularates7, stuInIntervalTime, stuInStorageTime);




	return 0;
}

