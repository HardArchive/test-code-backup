#include "stdafx.h"

#include <iostream>

using namespace std;

struct LunarDate
{
	int year;
	int month;
	int day;
	bool isLeap;
	int yearCyl;//cyclic天干地支
	int monCyl;
	int dayCyl;
};


int lunarInfo[]=
{
	// 1899年:0x0AB50  
	0x04BD8, 0x04AE0, 0x0A570, 0x054D5, 0x0D260, 0x0D950, 0x16554, 0x056A0, 0x09AD0, 0x055D2,   // 1900-1909   
	0x04AE0, 0x0A5B6, 0x0A4D0, 0x0D250, 0x1D255, 0x0B540, 0x0D6A0/*0x1D295,0x0B550,0x056A0*/, 0x0ADA2, 0x095B0, 0x14977,   // 1910-1919  
	0x04970, 0x0A4B0, 0x0B4B5, 0x06A50, 0x06D40, 0x1AB54, 0x02B60, 0x09570, 0x052F2, 0x04970,   // 1920-1929  
	0x06566, 0x0D4A0, 0x0EA50, 0x06E95/*0x16A95*/, 0x05AD0, 0x02B60, 0x186E3, 0x092E0, 0x1C8D7, 0x0C950,   // 1930-1939  
	0x0D4A0, 0x1D8A6, 0x0B550, 0x056A0, 0x1A5B4, 0x025D0, 0x092D0, 0x0D2B2, 0x0A950, 0x0B557,   // 1940-1949  
	0x06CA0, 0x0B550, 0x15355, 0x04DA0, 0x0A5B0, 0x14573, 0x052B0, 0x0A9A8, 0x0E950, 0x06AA0,   // 1950-1959  
	0x0AEA6, 0x0AB50, 0x04B60, 0x0AAE4, 0x0A570, 0x05260, 0x0F263, 0x0D950, 0x05B57, 0x056A0,   // 1960-1969  
	0x096D0, 0x04DD5, 0x04AD0, 0x0A4D0, 0x0D4D4, 0x0D250, 0x0D558, 0x0B540, 0x0B6A0, 0x195A6,   // 1970-1979  
	0x095B0, 0x049B0, 0x0A974, 0x0A4B0, 0x0B27A, 0x06A50, 0x06D40, 0x0AF46, 0x0AB60, 0x09570,   // 1980-1989  
	0x04AF5, 0x04970, 0x064B0, 0x074A3, 0x0EA50, 0x06B58, 0x055C0/*0x05AC0*/, 0x0AB60, 0x096D5, 0x092E0,   // 1990-1999  
	0x0C960, 0x0D954, 0x0D4A0, 0x0DA50, 0x07552, 0x056A0, 0x0ABB7, 0x025D0, 0x092D0, 0x0CAB5,   // 2000-2009  
	0x0A950, 0x0B4A0, 0x0BAA4, 0x0AD50, 0x055D9, 0x04BA0, 0x0A5B0, 0x15176, 0x052B0, 0x0A930,   // 2010-2019
	0x07954, 0x06AA0, 0x0AD50, 0x05B52, 0x04B60, 0x0A6E6, 0x0A4E0, 0x0D260, 0x0EA65, 0x0D530,   // 2020-2029  
	0x05AA0, 0x076A3, 0x096D0, 0x04BD7/*0x04AFB*/, 0x04AD0, 0x0A4D0, 0x1D0B6, 0x0D250, 0x0D520, 0x0DD45,   // 2030-2039  
	0x0B5A0, 0x056D0, 0x055B2, 0x049B0, 0x0A577, 0x0A4B0, 0x0AA50, 0x1B255, 0x06D20, 0x0ADA0,   // 2040-2049  
	// 增加2050-2100年数据  
	0x14B63, 0x09370, 0x049F8, 0x04970, 0x064B0, 0x168A6, 0x0EA50, 0x06B20, 0x1A6C4, 0x0AAE0,   // 2050-2059  
	0x092E0, 0x0D2E3, 0x0C960, 0x0D557, 0x0D4A0, 0x0DA50, 0x05D55, 0x056A0, 0x0A6D0, 0x055D4,   // 2060-2069  
	0x052D0, 0x0A9B8, 0x0A950, 0x0B4A0, 0x0B6A6, 0x0AD50, 0x055A0, 0x0ABA4, 0x0A5B0, 0x052B0,   // 2070-2079  
	0x0B273, 0x06930, 0x07337, 0x06AA0, 0x0AD50, 0x14B55, 0x04B60, 0x0A570, 0x054E4, 0x0D260,   // 2080-2089  
	0x0E968, 0x0D520, 0x0DAA0, 0x16AA6, 0x056D0, 0x04AE0, 0x0A9D4, 0x0A4D0, 0x0D150, 0x0F252,   // 2090-2099  
	0x0D520,                                                                                    // 2100  
};  

//==== 传回农历 y年闰哪个月 1-12 , 没闰传回 0
int GetLunarLeapMonth(int y) 
{ 
	return(lunarInfo[y-1900] & 0xf);
}

//==== 传回农历 y年闰月的天数
int GetLunarLeapDays(int y) 
{
	if(GetLunarLeapMonth(y))  return((lunarInfo[y-1900] & 0x10000)? 30: 29);
	else return(0);
}

//==== 传回农历 y年的总天数
int GetLunarYearDays(int y) 
{
	int i, sum = 348;
	for(i=0x8000; i>0x8; i>>=1) sum += (lunarInfo[y-1900] & i)? 1: 0;
	return sum+GetLunarLeapDays(y);
}

//====================================== 传回农历 y年m月的总天数
int GetLunarMonthDays(int y,int m) 
{ 
	if ( y >= 1900 )   
		return(lunarInfo[y-1900] & (0x10000>>m))? 30: 29 ;
	else if ( (y == 1899) && (m == 12) )   
		return 30;   
	else   
		return 0; 
}

int GetSolarYearDays(int year) 
{ 
	if(year%4==0&&year%100!=0||year%400==0) 
		return 366; 
	else 
		return 365; 
}
int GetSolarMonthDays(int year,int month) 
{ 
	if(month==1||month==3||month==5||month==7||month==8||month==10||month==12) 
		return 31; 
	if(month==4||month==6||month==9||month==11) 
		return 30; 
	if(month==2&&GetSolarYearDays(year)==366) 
		return 29; 
	else 
		return 28; 
} 

int GetDaysFrom19000131(int solarYear, int solarMonth,int solarDay)
{ 
	int     nRecYear;   
	int     nDays;    
	// 1900年不是闰年，2000年是闰年，所以从1901年起每4年必有一个闰年   
	if ( solarYear >= 1901 )   
	{   
		nRecYear = solarYear - 1901;   
		nDays = (nRecYear * 365) + (nRecYear / 4);   
		nDays += 365;   // 1900年是365天   
	}   
	else   
	{ 
		nDays = 0;   
	}   

	for (int i = 1; i < solarMonth; i++)
		nDays += GetSolarMonthDays(solarYear,i);

	nDays += (solarDay - 31);

	return  nDays;
}
//==== 算出农历, 传入日期物件, 传回农历日期物件
//     该物件属性有 .year .month .day .isLeap .yearCyl .dayCyl .monCyl
struct LunarDate GetLunar(int solaryear, int solarmonth,int day) 
{
	struct LunarDate lunar;
	int i, leap=0, temp=0;
	int offset   = GetDaysFrom19000131(solaryear,solarmonth,day);
	lunar.dayCyl = offset + 40;
	lunar.monCyl = 14;  

	for(i=1900; i< 2100 && offset>0; i++) 
	{ 
		temp=GetLunarYearDays(i); 
		if (offset < temp)
			break;
		offset-=temp; 
	}

	lunar.year = i;
	lunar.yearCyl = i-1864;
	leap = GetLunarLeapMonth(i); //闰哪个月
	lunar.isLeap = false;

	for(i=1; i<13; i++) 
	{
		//闰月
		if(leap>0 && i==(leap+1) && !lunar.isLeap)
		{ 
			--i; 
			lunar.isLeap = true; 
			temp = GetLunarLeapDays(lunar.year); 
		}
		else
		{ 
			temp = GetLunarMonthDays(lunar.year, i);
		}
		//解除闰月
		if(lunar.isLeap && i==(leap+1)) 
		{
			lunar.isLeap = false;
		}
		if (offset < temp)
			break;
		offset -= temp;
	}

	lunar.month = i;
	lunar.day = offset + 1;
	return lunar;
}

int main(int argc,char** args)
{
	int iYear=0,iMonth=0,iDay=0;
	cerr << " 请输入年份(>1):  " ;
	cin >> iYear;
	cerr << " 请输入月份(>1):  " ;
	cin >> iMonth;
	cerr << " 请输入日份(>1):  " ;
	cin >> iDay;
	struct LunarDate lunar = GetLunar(iYear,iMonth,iDay);
	cout<<lunar.year<<"-"<<lunar.month<<"-"<<lunar.day<<endl;
	return 0;
}