// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

#include < iostream > 
#include < iomanip > 
using   namespace  std;
int  FistDayofYear( int  y);
int  DaysofMonth( int  m);
void  PrintMonth( int  m);
void  PrintHead( int  m);
bool  LeapYear( int  y);
int  WeekDay,year;

void  main1()
{
INPUT:
	cerr << " ���������(>1):  " ;
	cin >> year;
	WeekDay = FistDayofYear(year);
	cout << " \n\n                   " << year << " ��\n " ;
	cout << " ========================================================== " ;
	for ( int  a = 1 ;a < 13 ;a ++ )
		PrintMonth(a);
	cout << endl;
	int  r = 0 ,u = 0 ;
	cout << " ������1���˳���0  :  " ;
	cin >> r;
	if (r > u)
		goto  INPUT;
	else 
		goto  END;
END:;
} 
void  PrintMonth( int  m)
{
	PrintHead(m);
	int  day = DaysofMonth(m);
	for ( int  i = 1 ;i <= day;i ++ )
	{
		cout << setw( 5 ) << i;
		WeekDay = (WeekDay + 1 ) % 7 ;
		if (WeekDay == 0 )
		{
			cout << endl;
			cout << setw( 5 ) << "               " ;
		} 
	} 
} 
void  PrintHead( int  m)
{
	cout << " \n\n " << setw( 5 ) << m;
	cout << " ��          ��   һ   ��   ��   ��   ��   ��   \n " ;
	cout << setw( 5 ) << "               " ;
	for ( int  i = 0 ;i < WeekDay;i ++ )
		cout << setw( 5 ) << "      " ;
} 

//ȡ��һ�����ܹ��м���
int  DaysofMonth( int  m)
{
	switch (m)
	{
	case   1 :
	case   3 :
	case   5 :
	case   7 :
	case   8 :
	case   10 :
	case   12 : return   31 ;
	case   4 :
	case   6 :
	case   9 :
	case   11 : return   30 ;
	case   2 : if (LeapYear(year)) return   29 ;
			   else   return   28 ;
	} 
	return   0 ;
} 
//�ж��Ƿ�Ϊ����
bool  LeapYear( int  y)
{
	return (y % 4 == 0 && y % 100 != 0 || y % 400 == 0 );
} 

//W = [Y-1] + [(Y-1)/4] - [(Y-1)/100] + [(Y-1)/400] + D 

//Y���������D����һ������һ���е��ۻ�������Ҳ������һ������һ�����ǵڼ��졣 
// y��1��1��
//����һ��ĵ�һ�������ڼ�
// windows�����Ǵ�1980-2099��
int  FistDayofYear( int  y)
{
	long  m;
	m = y * 365 ;  //��0�꿪ʼ�����ڵ�����
	for ( int  i = 1 ;i < y;i ++ )
		m += LeapYear(i);//�ټ����������������������һ��
	return  m %= 7 ;
} 


