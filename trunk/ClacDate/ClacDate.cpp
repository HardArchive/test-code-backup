#include "stdafx.h"

typedef struct _DATA 
{
	int iYear;
	int iMonth;
	int iDate;
}DATA, PDATA;

//判断是否为闰年
bool IsLeap(int year)
{	
	if(year % 400 == 0)
		return true;	
	if(year % 100 == 0)
		return false;	
	if(year % 4 == 0)
		return true;
	return false;
}

//计算两日期间的时间差
int Diffdate(DATA stuBegin, DATA stuEnd)
{
	int days = 0;
	int months = 0,years = 0; 
	int tmpyear = 0,tmpmonth = 0,leapyear = 0; 
	short monthday[2][12] = 
	{
		{31,28,31,30,31,30,31,30,31,30,31},
		{31,29,31,30,31,30,31,30,31,30,31}
	}; 

	if(IsLeap(stuEnd.iYear))
	{ 
		leapyear =1; 
	}
	else
	{
		leapyear = 0; 
	} 
	years = stuEnd.iYear - stuBegin.iYear; 
	months = stuEnd.iMonth - stuBegin.iMonth; 
	days = stuEnd.iDate - stuBegin.iDate; 
	tmpyear = stuBegin.iYear;

	while(years>0)
	{
		if(IsLeap(tmpyear))
			days += 366; 
		else
			days += 365; 
		tmpyear++; 
		years--; 
	} 

	if(months > 0)
	{
		while(months-- > 0)
		{
			days += monthday[leapyear][stuBegin.iMonth++-1]; 
		} 
	}
	else
	{
		while(months++ < 0)
		{
			stuBegin.iMonth--; 
			days -= monthday[leapyear][stuBegin.iMonth-1]; 
		} 
	} 	
	return days; 
}

//判断剩余的可借阅天数
int IsOverdut(char* pszCurrentDate, char* pszSignDate, int iReadDays)
{
	int iRet = 0;
	DATA stuBegin = {0};
	DATA stuEnd = {0};
	char szYear[5] = {0};
	char szMonth[3] = {0};
	char szDay[3] = {0};

	strncpy(szYear, pszSignDate, 4);
	strncpy(szMonth, pszSignDate+5, 2);
	strncpy(szDay, pszSignDate+8, 2);
	stuBegin.iYear = atoi(szYear);
	stuBegin.iMonth = atoi(szMonth);
	stuBegin.iDate = atoi(szDay);

	strncpy(szYear, pszCurrentDate, 4);
	strncpy(szMonth, pszCurrentDate+5, 2);
	strncpy(szDay, pszCurrentDate+8, 2);

	stuEnd.iYear = atoi(szYear);
	stuEnd.iMonth = atoi(szMonth);
	stuEnd.iDate = atoi(szDay);

	iRet = Diffdate(stuBegin, stuEnd);
	iRet = iReadDays - iRet;
	return iRet;
}

void main()
{
	int iTemp = 0;
	char szTemp[MAX_PATH] = {0};
	char* pszCurenttime = "2012-12-31";
	char* pszSignDate = "2013-01-05";

	iTemp = IsOverdut(pszCurenttime, pszSignDate, 3);

	iTemp>0?sprintf(szTemp, "%d天", iTemp):(iTemp==0?sprintf(szTemp, "即将过期"):sprintf(szTemp, "已过期%d天", -iTemp));
	printf(szTemp);

}