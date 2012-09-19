// 给定一个天数,查询该天之后的日期
//例如
//当前日期是 2008 11 10 则5050天后是 2022 9 8

#include <iostream>

using namespace std;
  8
  9class DAY
 10{
 11private:
 12    int y;
 13    int m;
 14    int d;
 15    int days[12];
 16
 17public:
 18    DAY()
 19    {
 20        cout<<"请输入当前日期"<<endl;
 21        cout <<"year"<<endl;
 22        cin >> y;
 23        cout << "mouth"<<endl;
 24        cin >>m;
 25        cout << "day"<<endl;
 26        cin >> d;
 27
 28        int i;
 29
 30        for( i = 1 ; i <= 12 ; i++)
 31        {
 32            switch(i)
 33            {
 34
 35            case 2:
 36                days[i-1] = isLeapYear() ? 29:28; //设置2月天数
 37                break;
 38
 39            case 4:
 40            case 6:
 41            case 9:
 42            case 11:
 43                days[i-1] = 30;
 44                break;
 45
 46            default :
 47                days[i-1] = 31;
 48            }
 49        }
 50    }
 51
 52    void show()
 53    {
 54        cout <<"year:"<<y<<endl;
 55        cout <<"mouth:"<<m<<endl;
 56        cout <<"day:"<<d<<endl;
 57    }
 58
 59    int judge_day(int mouth)
 60    {
 61        return days[mouth-1];
 62    }
 63
 64    void calculate(int increase)
 65    {
 66        if(d + increase <= days[m-1])
 67            d += increase;
 68        else
 69        {
 70            increase -= days[m-1] - d;
 71            m++;
 72
 73            if(m>12)  
 74            {
 75                //if m is greater than 12 , and increase is less than days[m-1]
 76                //,then the following while statement will not be executed,
 77                //so must reset m and year++
 78
 79                m = 1;
 80                y++;
 81            }
 82
 83            while(increase -days[m-1] >0)
 84            {
 85                increase -= days[m-1];
 86                m++;
 87
 88                if(m > 12)
 89                {
 90                    y++;
 91                    m = 1;
 92
 93                    days[1] = isLeapYear() ? 29 : 28;
 94                }
 95            }
 96
 97            d = increase;
 98        }
 99    }
100
101    bool isLeapYear()
102    {
103        return ( y%4 == 0 && y%100 != 0 ) || (y%400 == 0);
104    }
105};
106
107int main()
108{
109    DAY test;
110    int increase;
111
112    test.show();
113
114    cout <<"请输入天数,以查询该天之后的日期"<<endl;
115    cin >> increase;
116
117    test.calculate(increase);
118    cout <<"\n"<<increase<<"天以后是:"<<endl;
119    test.show();
120
121    return 0;
122}


//计算两日期之间间隔
int daysFrom11(int year,int month,int day)
{
	int days[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	int i,sum=0;
	for(i=0;i<month;i++)
		sum+=days[i];
	if(month>2)
		if((year%4==0)&&(year%100!=0)||(year%400==0))
			sum+=1;
	sum+=day;
	return sum;
}

void swap(int& x1,int& x2){
	int tmp=x1;
	x1=x2;x2=tmp;
}

long difs2Date(int y1,int m1,int d1,int y2,int m2,int d2){
	int from11yr1,from11yr2;
	long totalDays=0;
	if(y1>y2){
		swap(y1,y2);
		swap(m1,m2);
		swap(d1,d2);
	}
	if(y1==y2){
		from11yr1=daysFrom11(y1,m1,d1);
		from11yr2=daysFrom11(y2,m2,d2);
		totalDays=abs(from11yr1-from11yr2)+1;
	}
	else{
		from11yr1=daysFrom11(y1,m1,d1);
		int total_day1=365-from11yr1+1;
		if(m1<=2)
			if(y1%4==0&&(y1%100!=0)||(y1%400==0))
				total_day1+=1;
		totalDays+=total_day1;
		int tmpYear=y1;
		while(++tmpYear<y2){
			int tmpDays=365;
			if((tmpYear%4==0)&&(tmpYear%100!=0)||(tmpYear%400==0))
				tmpDays+=1;
			totalDays+=tmpDays;
		}
		from11yr2=daysFrom11(y2,m2,d2);
		totalDays+=from11yr2;
	}
	return totalDays-1;
}


time_t SystemTimeToTimet(SYSTEMTIME st)

{

time_t SystemTimeToTimet(SYSTEMTIME st)

{

    FILETIME ft;

    SystemTimeToFileTime( &st, &ft );


    LONGLONG nLL;

    ULARGE_INTEGER ui;

    ui.LowPart = ft.dwLowDateTime;

    ui.HighPart = ft.dwHighDateTime;

    nLL = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;

    time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);

    return pt;

}


：计算两日期间隔天数
函数中如有调用其他函数，请查看其他函数。[code]//定义计算某个日期与公元元年元旦的间隔天数的函数
int today(Date d)
{
        
        int count = 0;                                //保存间隔天数的变量
        Date initday = {1,1,1};                  //定义并初始化元年
        if (d.year==1&&d.month==1&&d.day==1)
        {
                count = 0;
                return 0;
        }
        else
        {
                Date tmp;                        //由于月份的天数由于是否闰年而不同，因此定义临时日期变量来循环计算月份天数
                int a = d.year - 1;        //便于后面计算year之前有多少闰年与非闰年
                int b = a/400 + a/4 - a/100;        //闰年的个数
                int c = 0;                        //初始化当年整月总共的天数
                int day = d.day - 1;//计算日期的零头天数
                //for 循环计算出日期当年的整月所在天数的和
                for (int i=1; i<d.month; i++)
                {
                        tmp.month=i;
                        c += maxday(tmp);
                }
                count = a*365 + b + c + day; //闰年天数加上非闰年天数加上当年整月天数加上零头天数既是间隔总天数
                //count = b*366 + (a-b)*365 + c + day;
                return count;
        }
}[/code]两个调用此函数作差即可求得任意两日期间隔天数，这比直接计算两日期的间隔天数效率更高！ 



