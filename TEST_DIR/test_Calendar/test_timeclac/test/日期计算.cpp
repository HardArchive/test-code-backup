#include "stdafx.h"
#include<iostream>
using namespace std;
char months[][12]={"none","january","february","march","april","may","june","july","august","september","october","november","december"};
char days[][15]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
class date
{
public:


	date(int newy,int newm,int newd);
	bool isleapyear();
	void addday(int days);
	void minusday(int day);
	int week();
	void print();
	void renew(int newy,int newm,int newd);
	void compare(date &p);
private:	
	int year,month,day;
};


date::date(int newy,int newm,int newd)
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
	day-=days;
	while(day<=0)
	{
		switch(month-1)
		{
		case 0:
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
	day+=days;
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
}

int date::week()
{

	long sum=0,i;
	for (i=1;i<year;i++)
		if (i%4==0&&i%100!=0 || i%400==0)
			sum+=366;
		else 
			sum+=365;

	switch (month) {
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
void date::compare(date &p)
{
	cout<<"日期大的:";
	if(year>p.year)
		print();
	else
		if(year==p.year&&(month>p.month||(month==p.month&&day>p.day)))
			print();
		else 
			p.print();

}

void menu2()
{
	cout<<"***************日期计算*****************************\n"<<endl;

	cout<<"****               1:增加天数   o(∩_∩)o...    ****  "<<endl;
	cout<<"****               2:减去天数   o(∩_∩)o...    ****  "<<endl;
	cout<<"****               3:显示日期    o(∩_∩)o...   ****  "<<endl;
	cout<<"****               4:判断闰年     o(∩_∩)o...  ****  "<<endl;
	cout<<"****               5:从新输入年份  o(∩_∩)o... ****  "<<endl;
	cout<<"****               6:退出          o(∩_∩)o... ****  "<<endl;
	cout<<"****               0:比较日期大小   o(∩_∩)o...****  "<<endl;
	cout<<"****************************************************"<<endl;

}


void main2()
{
	int year,month,day,i,j;

	cout<<"请首先输入日期";
	cin>>year>>month>>day;
	date a(year,month,day),b(year,month,day);

	cout<<"现在日期：";
	a.print();
	menu2();
	while(1)
	{

		cin>>i;
		switch(i)
		{
			system("cls");
		case 0 : 
			cout<<"当前日期";
			a.print();
			cout<<"请输入待比较日期:";
			cin>>year>>month>>day;	
			b.renew(year,month,day);
			a.compare(b);
			menu2();
			break;
		case 1 : 
			{
				cout<<"输入要增加的天数"<<endl;
				cin>>j;a.addday(j);
				a.print();
				break;
			}
		case 2 : 	
			{cout<<"输入要减去的天数"<<endl;
			cin>>j;a.minusday(j);
			a.print();break;
			}
		case 3 :{ a.print();break;}
		case 4 :
			{if(a.isleapyear())
				cout<<"闰年";
			else
				cout<<"平年";break;}
		case 5 : 
			{
				system("cls");

				cout<<"请输入新日期";
				cin>>year>>month>>day;
				a.renew(year,month,day);
				cout<<"您输入的日期为"<<endl;
				a.print();
				menu2();
				break;
			}
		case 6:
			{
				cout<<"再见谢谢使用本系统(*^__^*) 嘻嘻……"<<endl;
				exit(0);
			}

			cout<<"按键返回";
			system("Pause");
		}

	}
}