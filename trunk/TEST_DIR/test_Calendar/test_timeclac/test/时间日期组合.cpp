#include "stdafx.h"
#include<iostream>
#include<stdio.h>
#include<windows.h>
using namespace std;
int i=0,j=0;
char months[][12]={"none","January","February","March","April","May","June","July","August","September","October","November","December"};
char days[][15]={"none","星期一","星期二","星期三","星期四","星期五","星期六","星期日"};
class date
{
public:
	bool isleapyear();
	void addday(int days);
	void minusday(int day);
	int week();
	void print();
	void renew(int newy,int newm,int newd);
	int compare(date &p);
	friend class datetime;
	int returnday()
	{return day;}
private:	
	int year,month,day;
};





bool date::isleapyear()
{
	if((year%4==0)&&(year%100!=0)||(year%400==0))
		return true;
	return false;
}


void date::print()
{
	cout<<year<<"-"<<month<<"-"<<day<<"--"<<months[month]<<"--"<<days[week()]<<endl;

}

void date::minusday(int days)
{
	day-=days+j;
	while(day<=0)
	{
		switch(month-1)
		{
		//case 0:
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 12:
		case 10:{month--;day+=31;break;}
		case 4:
		case 6:
		case 9:
		case 11:{month--;day+=30;break;}
		case 2:
			{
				if(isleapyear())
				{month--;day+=29;break;}
				else
				{month--;day+=28;break;}
			}
		}
		if(month<1)
		{year--;month+=12;}
	}
}

void date::addday(int days)
{
	day+=days+i;
	while(day>=365)
	{
		if(isleapyear()&&day>365)
		{
			year++;
			day-=366;
		}
		else
		{
			year++;
			day-=365;
		}
	}
	while(day>31)
	{
		switch(month)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12: {month++; day-=31;break;}
		case 4:
		case 6:
		case 9:

		case 11:{month++;day-=30;break;}
		case 2:
			{
				if(isleapyear())
				{month++;day-=29;break;}
				else
				{month++;day-=28;break;}
			}
		}
		if(month>12)
		{year++;month-=12;}
	}
	i=0;
}

int date::week()
{

	long sum=0,i;
	for (i=1;i<year;i++)
		if (i%4==0&&i%100!=0 || i%400==0)
			sum+=366;
		else 
			sum+=365;

	switch (month)
	{
case 12 : sum+=30;
case 11 : sum+=31;
case 10 : sum+=30;
case 9 : sum+=31;
case 8 : sum+=31;
case 7 : sum+=30;
case 6 : sum+=31;
case 5 : sum+=30;
case 4 : sum+=31;
case 3 : if (year%4==0&&year%100!=0||year%400==0) 
			 sum+=29;
		 else sum+=28;
case 2 : sum+=31;
default : sum+=0;
	}
	sum+=day-1;

	switch (sum%7) 
	{                                         /*公元元年一月一号星期一*/
	case 0 : return(1);break;
	case 1 : return(2);break;
	case 2 : return(3);break;
	case 3 : return(4);break;
	case 4 : return(5);break;
	case 5 : return(6);break;
	case 6 : return(7);break;
	}
}

void date::renew(int newy,int newm,int newd)
{
	year=newy;
	month=newm;
	day=newd;
	if(year<0||year>100000||day<0||day>31||month<0||month>12)
	{
		cout<<"Error Format!!!"<<endl;
		exit(1);
	}
}
int date::compare(date &p)
{
	cout<<"日期大的:";
	if(year>p.year)
	{print();return 1;}
	else
		if(year==p.year&&(month>p.month||(month==p.month&&day>p.day)))
		{
			print();return 1;
		}
		else if(year==p.year&&month>p.month&&day>p.day)
		{cout<<"一样大"; 
	return 0;
	}
		else
		{
			p.print();
			return 2;
		}
}

class timer
{
public:

	void print();
	void renew(int newh,int newm,int news);
	void add(timer &p);
	void minus(timer &p);
	void compare(timer &p);
	friend class datetime;

private:
	int hour,minute,second;
};



void timer::renew(int newh,int newm,int news)
{
	hour=newh;minute=newm;second=news;
	if(hour<0||minute<0||second<0||hour>24||minute>60||second>60)
	{
		cout<<"Error Format!!!"<<endl;

	}
}

void timer::print()
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

void timer::add(timer &p)
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
	i++;
}

void timer::minus(timer &p)
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
	j--;
}

void timer::compare(timer &p)
{
	if(hour>p.hour)
		print();
	else if(hour==p.hour&&(minute>p.minute||(minute==p.minute&&second>p.second)))
		print();
	else 
		p.print();

}


class datetime
{
public:
	datetime(int newy,int newm,int newd,int newh,int newmm,int news);
	void renew(int newy,int newm,int newd,int newh,int newmm,int news);
	void add(int newy,int newm,int newd,int newh,int newmm,int news);
	void compare(datetime &p);


	date a;
	timer b;

};


datetime::datetime(int newy,int newm,int newd,int newh,int newmm,int news)
{
	a.renew(newy,newm,newd);
	b.renew(newh,newmm,news);

}


void datetime::renew(int newy=0,int newm=0,int newd=0,int newh=0,int newmm=0,int news=0)
{
	a.renew(newy,newm,newd);
	b.renew(newh,newmm,news);
}

void datetime::compare(datetime &p)
{
	if(a.compare(p.a)==1)
	{
		cout<<"日期时间比较大的：";
		a.print();b.print();
	}
	else if(a.compare(p.a)==0)
		cout<<"日期时间一样大：";
	else if(a.compare(p.a)==2)
	{
		cout<<"日期时间比较大的：";
		p.a.print();p.b.print();
	}

}

void menu3()
{
	cout<<"*********************日期计算*****************************\n"<<endl;
	cout<<"****               1:增加天数与时间   o(∩_∩)o...    ****  "<<endl;
	cout<<"****               2:减去天数与时间   o(∩_∩)o...    ****  "<<endl;
	cout<<"****               3:显示日期时间     o(∩_∩)o...    ****  "<<endl;
	cout<<"****               4:比较日期时间大小 o(∩_∩)o...    ****  "<<endl;
	cout<<"****               5:重新输入年份     o(∩_∩)o...    ****  "<<endl;
	cout<<"****               6:退出             o(∩_∩)o...    ****  "<<endl;
	cout<<"****               0:初始化为系统时间   o(∩_∩)o...  ****  "<<endl;
	cout<<"**********************************************************"<<endl;

}

//这个日期有问  2月分会算出30天来 待改
void main1()
{  
	datetime a(0,0,0,0,0,0),b(0,0,0,0,0,0); 
	int year,month,day,hour,minute,second,i;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	cout<<"请首先输入初始日期时间："<<endl;
	cin>>year>>month>>day>>hour>>minute>>second;
	a.renew(year,month,day,hour,minute,second);
	cout<<"您输入日期时间为"<<endl;
	a.a.print();a.b.print();
	menu3();
	while(1)
	{

		cin>>i;
		switch(i)
		{
		case 0:

			a.renew(sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
			cout<<"系统当前日期时间为"<<endl;
			a.a.print();a.b.print();
			menu3();
			break;

		case 1:
			{
				cout<<"请输入要增加的天数与时间"<<endl;
				cin>>day>>hour>>minute>>second;
				b.renew(0,0,day,hour,minute,second);
				a.b.add(b.b);
				a.a.addday(b.a.returnday());
				a.a.print();a.b.print();
				break;

			}
		case 2:
			{
				cout<<"请输入要减去的天数与时间:"<<endl;
				cin>>day>>hour>>minute>>second;
				b.renew(0,0,day,hour,minute,second);
				a.b.minus(b.b);
				a.a.minusday(b.a.returnday());
				a.a.print();a.b.print();
				break;




			}
		case 3:

			{
				a.a.print();a.b.print();break;
			}
		case 4:
			{
				cout<<"请输入要比较日期时间"<<endl;
				cin>>year>>month>>day>>hour>>minute>>second;
				b.renew(year,month,day,hour,minute,second);
				a.compare(b);break;
			}
		case 5:
			{
				system("cls");
				cout<<"请重新输入日期时间"<<endl;
				cin>>year>>month>>day>>hour>>minute>>second;
				a.renew(year,month,day,hour,minute,second);
				cout<<"您输入日期时间为"<<endl;
				a.a.print();a.b.print();
				menu3();
				break;
			}
		case 6:
			cout<<"再见谢谢使用本系统(*^__^*) 嘻嘻……"<<endl;
			exit(0);
		}  


	}  





}

