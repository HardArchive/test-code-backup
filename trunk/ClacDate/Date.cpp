// ����һ������,��ѯ����֮�������
//����
//��ǰ������ 2008 11 10 ��5050����� 2022 9 8

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
 20        cout<<"�����뵱ǰ����"<<endl;
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
 36                days[i-1] = isLeapYear() ? 29:28; //����2������
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
114    cout <<"����������,�Բ�ѯ����֮�������"<<endl;
115    cin >> increase;
116
117    test.calculate(increase);
118    cout <<"\n"<<increase<<"���Ժ���:"<<endl;
119    test.show();
120
121    return 0;
122}


//����������֮����
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


�����������ڼ������
���������е���������������鿴����������[code]//�������ĳ�������빫ԪԪ��Ԫ���ļ�������ĺ���
int today(Date d)
{
        
        int count = 0;                                //�����������ı���
        Date initday = {1,1,1};                  //���岢��ʼ��Ԫ��
        if (d.year==1&&d.month==1&&d.day==1)
        {
                count = 0;
                return 0;
        }
        else
        {
                Date tmp;                        //�����·ݵ����������Ƿ��������ͬ����˶�����ʱ���ڱ�����ѭ�������·�����
                int a = d.year - 1;        //���ں������year֮ǰ�ж��������������
                int b = a/400 + a/4 - a/100;        //����ĸ���
                int c = 0;                        //��ʼ�����������ܹ�������
                int day = d.day - 1;//�������ڵ���ͷ����
                //for ѭ����������ڵ�����������������ĺ�
                for (int i=1; i<d.month; i++)
                {
                        tmp.month=i;
                        c += maxday(tmp);
                }
                count = a*365 + b + c + day; //�����������Ϸ������������ϵ�����������������ͷ�������Ǽ��������
                //count = b*366 + (a-b)*365 + c + day;
                return count;
        }
}[/code]�������ô˺��������������������ڼ�����������ֱ�Ӽ��������ڵļ������Ч�ʸ��ߣ� 



