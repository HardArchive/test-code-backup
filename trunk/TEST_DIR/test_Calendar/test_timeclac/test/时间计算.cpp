#include "stdafx.h"

#include<stdlib.h>
#include<windows.h>
#include<iostream>
using namespace std;

class time
{
public:
	time(int newh,int newm,int news);
	void print();
	void renew(int newh,int newm,int news);
	void add(time &p);
	void minus(time &p);
	void compare(time &p);

private:
	int hour,minute,second;
};


time::time(int newh,int newm,int news)
{
	hour=newh;minute=newm;second=news;
}

void time::renew(int newh,int newm,int news)
{
	hour=newh;minute=newm;second=news;
	if(hour<0||minute<0||second<0||hour>24||minute>60||second>60)
	{
		cout<<"Error Format!!!"<<endl;

	}
}

void time::print()
{
	int i;
	cout<<hour<<":"<<minute<<":"<<second<<endl;
	if(hour>12)
	{i=hour-12;
	cout<<i<<":"<<minute<<":"<<second<<"Pm"<<endl;
	} 
	else
	{
		i=hour;
		cout<<i<<":"<<minute<<":"<<second<<"Am"<<endl;
	}
}

void time::add(time &p)
{
	hour+=p.hour;minute+=p.minute;second+=p.second;
	while(second>60)
	{
		second-=60;
		minute++;
	}
	while(minute>60)
	{
		minute-=60;
		hour++;
	}
	while(hour>24)
		hour-=24;
}

void time::minus(time &p)
{
	hour-=p.hour;minute-=p.minute;second-=p.second;
	while(second<0)
	{
		second+=60;
		minute--;
	}
	while(minute<0)
	{
		minute+=60;
		hour--;
	}
	while(hour<=0)
		hour+=24;
}

void time::compare(time &p)
{
	if(hour>p.hour)
		print();
	else if(hour==p.hour&&(minute>p.minute||(minute==p.minute&&second>p.second)))
		print();
	else 
		p.print();

}

void menu3()
{
	cout<<"***************���ڼ���*****************************\n"<<endl;

	cout<<"****               1:����ʱ��   o(��_��)o...    ****  "<<endl;
	cout<<"****               2:��ȥʱ��   o(��_��)o...    ****  "<<endl;
	cout<<"****               3:��ʾʱ��    o(��_��)o...   ****  "<<endl;
	cout<<"****               4:��������ʱ��  o(��_��)o... ****  "<<endl;
	cout<<"****               5:��ǰϵͳʱ��  o(��_��)o... ****  "<<endl;
	cout<<"****               6:�Ƚ�ʱ���С   o(��_��)o...****  "<<endl;
	cout<<"****               0:�˳�           o(��_��)o...****  "<<endl;
	cout<<"****************************************************"<<endl;

}

void main3()
{
	int hour,minute,second,i;
	time a(13,20,30),b(0,0,0);
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	cout<<"����������ʱ��:"<<endl;


	cin>>hour>>minute>>second;
	a.renew(hour,minute,second);
	menu3();
	while(1)
	{
		cin>>i;

		switch(i)
		{
		case 0:
			{
				cout<<"�ټ�ллʹ�ñ�ϵͳ(*^__^*) ��������"<<endl;
				exit(0);
			}
		case 6 : 
			{
				cout<<"��ǰʱ��";
				a.print();
				cout<<"��������Ƚ�ʱ��:";

				cin>>hour>>minute>>second;
				b.renew(hour,minute,second);
				a.compare(b);
				break;}
		case 1 : 
			{
				cout<<"����Ҫ���ӵ�ʱ��"<<endl;
				cin>>hour>>minute>>second;
				b.renew(hour,minute,second);
				a.add(b);
				a.print();
				break;
			}
		case 2 : 	
			{cout<<"����Ҫ��ȥ��ʱ��"<<endl;
			cin>>hour>>minute>>second;
			b.renew(hour,minute,second);
			a.minus(b);
			a.print();break;
			}
		case 3 :{ 

			a.print();break;}
		case 4 :
			{
				system("cls");

				cout<<"������ʱ��"<<endl;
				cin>>hour>>minute>>second;
				a.renew(hour,minute,second);
				cout<<"�������ʱ��Ϊ"<<endl;
				a.print();
				menu3();

				break;
			}
		case 5:
			{a.renew(sys.wHour,sys.wMinute,sys.wSecond);
			cout<<"ϵͳ��ǰ����ʱ��Ϊ"<<endl;
			a.print();
			menu3();
			break;
			}
		}
	}
}