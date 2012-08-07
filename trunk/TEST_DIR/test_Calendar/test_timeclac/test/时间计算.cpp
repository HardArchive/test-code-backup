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
	cout<<"***************日期计算*****************************\n"<<endl;

	cout<<"****               1:增加时间   o(∩_∩)o...    ****  "<<endl;
	cout<<"****               2:减去时间   o(∩_∩)o...    ****  "<<endl;
	cout<<"****               3:显示时间    o(∩_∩)o...   ****  "<<endl;
	cout<<"****               4:从新输入时间  o(∩_∩)o... ****  "<<endl;
	cout<<"****               5:当前系统时间  o(∩_∩)o... ****  "<<endl;
	cout<<"****               6:比较时间大小   o(∩_∩)o...****  "<<endl;
	cout<<"****               0:退出           o(∩_∩)o...****  "<<endl;
	cout<<"****************************************************"<<endl;

}

void main3()
{
	int hour,minute,second,i;
	time a(13,20,30),b(0,0,0);
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	cout<<"请首先输入时间:"<<endl;


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
				cout<<"再见谢谢使用本系统(*^__^*) 嘻嘻……"<<endl;
				exit(0);
			}
		case 6 : 
			{
				cout<<"当前时间";
				a.print();
				cout<<"请输入待比较时间:";

				cin>>hour>>minute>>second;
				b.renew(hour,minute,second);
				a.compare(b);
				break;}
		case 1 : 
			{
				cout<<"输入要增加的时间"<<endl;
				cin>>hour>>minute>>second;
				b.renew(hour,minute,second);
				a.add(b);
				a.print();
				break;
			}
		case 2 : 	
			{cout<<"输入要减去的时间"<<endl;
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

				cout<<"请输入时间"<<endl;
				cin>>hour>>minute>>second;
				a.renew(hour,minute,second);
				cout<<"您输入的时间为"<<endl;
				a.print();
				menu3();

				break;
			}
		case 5:
			{a.renew(sys.wHour,sys.wMinute,sys.wSecond);
			cout<<"系统当前日期时间为"<<endl;
			a.print();
			menu3();
			break;
			}
		}
	}
}