//
//万年历程序原代码
// 山芋
//C语言编的程序打印的万年历和程序原代码
//The calendar of the year 2001.
//Januray 1                         February 2
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//       1   2   3   4   5   6                         1   2   3
//   7   8   9  10  11  12  13         4   5   6   7   8   9  10
//  14  15  16  17  18  19  20        11  12  13  14  15  16  17
//  21  22  23  24  25  26  27        18  19  20  21  22  23  24
//  28  29  30  31                    25  26  27  28
//===========================       ===========================
//March 3                           April 4
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//                   1   2   3         1   2   3   4   5   6   7
//   4   5   6   7   8   9  10         8   9  10  11  12  13  14
//  11  12  13  14  15  16  17        15  16  17  18  19  20  21
//  18  19  20  21  22  23  24        22  23  24  25  26  27  28
//  25  26  27  28  29  30  31        29  30
//===========================       ===========================
//May 5                             June 6
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//           1   2   3   4   5                             1   2
//   6   7   8   9  10  11  12         3   4   5   6   7   8   9
//  13  14  15  16  17  18  19        10  11  12  13  14  15  16
//  20  21  22  23  24  25  26        17  18  19  20  21  22  23
//  27  28  29  30  31                24  25  26  27  28  29  30
//===========================       ===========================
//July 7                            August 8
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//   1   2   3   4   5   6   7                     1   2   3   4
//   8   9  10  11  12  13  14         5   6   7   8   9  10  11
//  15  16  17  18  19  20  21        12  13  14  15  16  17  18
//  22  23  24  25  26  27  28        19  20  21  22  23  24  25
//  29  30  31                        26  27  28  29  30  31
//===========================       ===========================
//September 9                       October 10
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//                           1             1   2   3   4   5   6
//   2   3   4   5   6   7   8         7   8   9  10  11  12  13
//   9  10  11  12  13  14  15        14  15  16  17  18  19  20
//  16  17  18  19  20  21  22        21  22  23  24  25  26  27
//  23  24  25  26  27  28  29        28  29  30  31
//  30
//===========================       ===========================
//Nevember 11                       December 12
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//                   1   2   3                                 1
//   4   5   6   7   8   9  10         2   3   4   5   6   7   8
//  11  12  13  14  15  16  17         9  10  11  12  13  14  15
//  18  19  20  21  22  23  24        16  17  18  19  20  21  22
//  25  26  27  28  29  30            23  24  25  26  27  28  29
//                                    30  31
//===========================       ===========================
//The calendar of the year 2002.
//Januray 1                         February 2
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//           1   2   3   4   5                             1   2
//   6   7   8   9  10  11  12         3   4   5   6   7   8   9
//  13  14  15  16  17  18  19        10  11  12  13  14  15  16
//  20  21  22  23  24  25  26        17  18  19  20  21  22  23
//  27  28  29  30  31                24  25  26  27  28
//===========================       ===========================
//March 3                           April 4
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//                       1   2             1   2   3   4   5   6
//   3   4   5   6   7   8   9         7   8   9  10  11  12  13
//  10  11  12  13  14  15  16        14  15  16  17  18  19  20
//  17  18  19  20  21  22  23        21  22  23  24  25  26  27
//  24  25  26  27  28  29  30        28  29  30
//  31
//===========================       ===========================
//May 5                             June 6
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//               1   2   3   4                                 1
//   5   6   7   8   9  10  11         2   3   4   5   6   7   8
//  12  13  14  15  16  17  18         9  10  11  12  13  14  15
//  19  20  21  22  23  24  25        16  17  18  19  20  21  22
//  26  27  28  29  30  31            23  24  25  26  27  28  29
//                                    30
//===========================       ===========================
//July 7                            August 8
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//       1   2   3   4   5   6                         1   2   3
//   7   8   9  10  11  12  13         4   5   6   7   8   9  10
//  14  15  16  17  18  19  20        11  12  13  14  15  16  17
//  21  22  23  24  25  26  27        18  19  20  21  22  23  24
//  28  29  30  31                    25  26  27  28  29  30  31
//===========================       ===========================
//September 9                       October 10
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//   1   2   3   4   5   6   7                 1   2   3   4   5
//   8   9  10  11  12  13  14         6   7   8   9  10  11  12
//  15  16  17  18  19  20  21        13  14  15  16  17  18  19
//  22  23  24  25  26  27  28        20  21  22  23  24  25  26
//  29  30                            27  28  29  30  31
//===========================       ===========================
//Nevember 11                       December 12
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//                       1   2         1   2   3   4   5   6   7
//   3   4   5   6   7   8   9         8   9  10  11  12  13  14
//  10  11  12  13  14  15  16        15  16  17  18  19  20  21
//  17  18  19  20  21  22  23        22  23  24  25  26  27  28
//  24  25  26  27  28  29  30        29  30  31
//===========================       ===========================
//The calendar of the year 2003.
//Januray 1                         February 2
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//               1   2   3   4                                 1
//   5   6   7   8   9  10  11         2   3   4   5   6   7   8
//  12  13  14  15  16  17  18         9  10  11  12  13  14  15
//  19  20  21  22  23  24  25        16  17  18  19  20  21  22
//  26  27  28  29  30  31            23  24  25  26  27  28
//===========================       ===========================
//March 3                           April 4
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//                           1                 1   2   3   4   5
//   2   3   4   5   6   7   8         6   7   8   9  10  11  12
//   9  10  11  12  13  14  15        13  14  15  16  17  18  19
//  16  17  18  19  20  21  22        20  21  22  23  24  25  26
//  23  24  25  26  27  28  29        27  28  29  30
//  30  31
//===========================       ===========================
//May 5                             June 6
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//                   1   2   3         1   2   3   4   5   6   7
//   4   5   6   7   8   9  10         8   9  10  11  12  13  14
//  11  12  13  14  15  16  17        15  16  17  18  19  20  21
//  18  19  20  21  22  23  24        22  23  24  25  26  27  28
//  25  26  27  28  29  30  31        29  30
//===========================       ===========================
//July 7                            August 8
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//           1   2   3   4   5                             1   2
//   6   7   8   9  10  11  12         3   4   5   6   7   8   9
//  13  14  15  16  17  18  19        10  11  12  13  14  15  16
//  20  21  22  23  24  25  26        17  18  19  20  21  22  23
//  27  28  29  30  31                24  25  26  27  28  29  30
//                                    31
//===========================       ===========================
//September 9                       October 10
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//       1   2   3   4   5   6                     1   2   3   4
//   7   8   9  10  11  12  13         5   6   7   8   9  10  11
//  14  15  16  17  18  19  20        12  13  14  15  16  17  18
//  21  22  23  24  25  26  27        19  20  21  22  23  24  25
//  28  29  30                        26  27  28  29  30  31
//===========================       ===========================
//Nevember 11                       December 12
//Sun Mon Tue Wed Thu Fri Sat       Sun Mon Tue Wed Thu Fri Sat
//                           1             1   2   3   4   5   6
//   2   3   4   5   6   7   8         7   8   9  10  11  12  13
//   9  10  11  12  13  14  15        14  15  16  17  18  19  20
//  16  17  18  19  20  21  22        21  22  23  24  25  26  27
//  23  24  25  26  27  28  29        28  29  30  31
//  30
//===========================       ===========================
void print(int n)
{
    int i;
    for(i = 0; i < n; i++) printf(" ");
}
int day(int year)
{
    long a, b;
    if(year <= 2000)
    {
        a = 2000 - year;
        b = 6 - (a + a / 4 - a / 100 + a / 400) % 7;
        return b;
    }
    else
    {
        a = year - 2000;
        b = (a + 1 + (a - 1) / 4 - (a - 1) / 100 + (a - 1) / 400) % 7 + 6;
        return b % 7;
    }
}
#define X " Sun Mon Tue Wed Thu Fri Sat"
#define P printf
#define B break
#define FP fprintf
#include <stdio.h>
void main()
{
    int i, j, k, m, n, f1, f2, year, d;
    int a[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    char fname[12];
    FILE *fw;
    clrscr();
    P(" Input the year:");
    scanf("%d", &year);
    上
    P(" Input the file name:");
    scanf("%s", fname);
    P("\n");
    fw = fopen(fname, "w+");
    if(fw == NULL)
    {
        P(" can not open file.");
        return;
    }
    P(" The calendar of the year %d.\n", year);
    FP(fw, " The calendar of the year %d.\n", year);
    d = day(year);
    if(year % 4 == 0 && year % 100 || year % 400 == 0) a[2]++;
    for(i = 1; i <= 12; i += 2)
    {
        m = 0;
        n = 0;
        f1 = 0;
        f2 = 0;
        switch(i)
        {
        case 1:
            P(" Januray 1   ");
            FP(fw, " Januray 1   ");
            B;
        case 3:
            P(" March 3     ");
            FP(fw, " March 3     ");
            B;
        case 5:
            P(" May 5       ");
            FP(fw, " May 5       ");
            B;
        case 7:
            P(" July 7     ");
            FP(fw, " July 7      ");
            B;
        case 9:
            P(" September 9 ");
            FP(fw, " September 9 ");
            B;
        case 11:
            P(" Nevember 11 ");
            FP(fw, " Nevember 11 ");
            B;
        }
        print(21);
        for(k = 0; k < 21; k++) fprintf(fw, " ");
        switch(i + 1)
        {
        case 2:
            P(" February 2  ");
            FP(fw, " February 2  ");
            B;
        case 4:
            P(" April 4    ");
            FP(fw, " April 4     ");
            B;
        case 6:
            P(" June 6      ");
            FP(fw, " June 6       ");
            B;
        case 8:
            P(" August 8   ");
            FP(fw, " August 8   ");
            B;
        case 10:
            P(" October 10  ");
            FP(fw, " October 10  ");
            B;
        case 12:
            P(" December 12");
            FP(fw, " December 12");
            B;
        }
        P("\n");
        fprintf(fw, "\n");
        P(X);
        fprintf(fw, X);
        print(6);
        fprintf(fw, "      ");
        P(X);
        fprintf(fw, X);
        fprintf(fw, "\n");
        P("\n");
        for(j = 0; j < 6; j++)
        {
            if(j == 0)
            {
                print(d * 4);
                for(k = 0; k < d * 4; k++) fprintf(fw, " ");
                for(k = 0; k < 7 - d; k++)
                {
                    P("%4d", ++m);
                    fprintf(fw, "%4d", m);
                }
                print(6);
                fprintf(fw, "      ");
                d += a[i] % 7;
                d %= 7;
                print(d * 4);
                for(k = 0; k < d * 4; k++) fprintf(fw, " ");
                for(k = 0; k < 7 - d; k++)
                {
                    P("%4d", ++n);
                    fprintf(fw, "%4d", n);
                }
                P("\n");
                fprintf(fw, "\n");
            }
            else
            {
                for(k = 0; k < 7; k++)
                {
                    if(m < a[i])
                    {
                        P("%4d", ++m);
                        fprintf(fw, "%4d", m);
                    }
                    else
                    {
                        print(4);
                        fprintf(fw, "    ");
                    }
                    if(m == a[i]) f1 = 1;
                }
                print(6);
                fprintf(fw, "      ");
                for(k = 0; k < 7; k++)
                {
                    if(n < a[i + 1])
                    {
                        P("%4d", ++n);
                        fprintf(fw, "%4d", n);
                    }
                    else
                    {
                        print(4);
                        fprintf(fw, "    ");
                    }
                    if(n == a[i + 1]) f2 = 1;
                }
                P("\n");
                fprintf(fw, "\n");
                if(f1 && f2) B;
            }
        }
        d += a[i + 1] % 7;
        d %= 7;
        P(" ");
        fprintf(fw, " ");
        for(k = 0; k < 27; k++)
        {
            P("=");
            fprintf(fw, "=");
        }
        print(6);
        fprintf(fw, "      ");
        P(" ");
        fprintf(fw, " ");
        for(k = 0; k < 27; k++)
        {
            P("=");
            fprintf(fw, "=");
        }
        P("\n");
        fprintf(fw, "\n");
        if(i == 5)
        {
            getch();
            clrscr();
        }
    }
    getch();
}



== == == == == == == == == == == == == == == == == == == == == == == == == == == =

    //新编万年历星期速算法C语言的实现(hotpower)
#include <stdio.h>

    code unsigned char WeekTab[] =  //闰年月星期表
{
    (3 << 5) + 31,//1月
    (6 << 5) + 29,//2月
    (0 << 5) + 31,//3月
    (3 << 5) + 30,//4月
    (5 << 5) + 31,//5月
    (1 << 5) + 30,//6月
    (3 << 5) + 31,//7月
    (6 << 5) + 31,//8月
    (1 << 5) + 30,//9月
    (4 << 5) + 31,//10月
    (0 << 5) + 30,//11月
    (2 << 5) + 31 //12月
};

/*------------------------------------------------------------------------------
----------
                                2000年~2099年星期算法
--------------------------------------------------------------------------------
---------*/
unsigned char WeekDay20(unsigned char y, unsigned char m, unsigned char d)
{
    unsigned char week, day;
    day = WeekTab[m - 1];//月表
    week = day >> 5;//月星期数
    day &= 0x1f;//月天数
    if ((m < 3) && (y & 0x03)) //平年
    {
        if (m == 2) day--;//平年月天数
        week++;//平年月表+1
    }
    y = y + (y >> 2);//年+年/4
    week = (week +  y + d + 2) % 7;//(星期=年+年/4+月表+2日)%7
    return (week << 5) | day;//返回星期和月天数
}

/*------------------------------------------------------------------------------
----------
                                0000年~9999年星期算法
--------------------------------------------------------------------------------
---------*/
unsigned char WeekDay(unsigned char c, unsigned char y, unsigned char m,
                      unsigned char d)
{
    unsigned char week, day;
    c &= 0x03;//百年%4
    c = c | (c << 2);//百年%4*5
    day = WeekTab[m - 1];//月表
    week = day >> 5;//月星期数
    day &= 0x1f;//月天数
    if ((m < 3) && !((c == 0) | (y & ((y & 0x03) == 0)))) //平年
    {
        if (m == 2) day--;//平年月天数
        week++;//平年月表+1
    }
    y = y + (y >> 2);//年+年/4
    week = (week + c + y + d + 2) % 7;//(星期=百年%4*5+年+年/4+月表+日+2)%7
    return (week << 5) | day;//返回星期和月天数
}

unsigned char BcdToBin(unsigned char val)
{
    val = (val >> 4) * 10 + (val & 0x0f);//将BCD码转换为10进制数
    return val;//返回10进制数
}

void main(void)
{
    unsigned char c, y, m, d;
    unsigned char cx, yx, mx, dx;
    unsigned char WDay, Week, Day;
    /*---------------------------------------------------------
         0001年1月1日 星期天
    ---------------------------------------------------------*/
    c    = 0x00;
    y    = 0x01;
    m    = 0x01;
    d    = 0x01;
    cx   = BcdToBin(c);//百年
    yx   = BcdToBin(y);//年
    mx   = BcdToBin(m);//月
    dx   = BcdToBin(d);//日
    WDay = WeekDay(cx, yx, mx, dx);//取星期和月天数
    Week = WDay >> 5;//得到星期
    Day  = WDay & 0x1f;//得到最大月天数
    /*---------------------------------------------------------
         1918年1月21日 星期一
    ---------------------------------------------------------*/
    c    = 0x19;
    y    = 0x18;
    m    = 0x01;
    d    = 0x21;
    cx   = BcdToBin(c);//百年
    yx   = BcdToBin(y);//年
    mx   = BcdToBin(m);//月
    dx   = BcdToBin(d);//日
    WDay = WeekDay(cx, yx, mx, dx);//取星期和月天数
    Week = WDay >> 5;//得到星期
    Day  = WDay & 0x1f;//得到最大月天数
    /*---------------------------------------------------------
         9999年12月31日 星期天
    ---------------------------------------------------------*/
    c    = 0x00;
    y    = 0x01;
    m    = 0x01;
    d    = 0x01;
    cx   = BcdToBin(c);//百年
    yx   = BcdToBin(y);//年
    mx   = BcdToBin(m);//月
    dx   = BcdToBin(d);//日
    WDay = WeekDay(cx, yx, mx, dx);//取星期和月天数
    Week = WDay >> 5;//得到星期
    Day  = WDay & 0x1f;//得到最大月天数
    /*---------------------------------------------------------
         2004年6月18日 星期五
    ---------------------------------------------------------*/
    c    = 0x20;
    y    = 0x04;
    m    = 0x06;
    d    = 0x18;
    cx   = BcdToBin(c);//百年
    yx   = BcdToBin(y);//年
    mx   = BcdToBin(m);//月
    dx   = BcdToBin(d);//日
    WDay = WeekDay20(yx, mx, dx);//取星期和月天数
    Week = WDay >> 5;//得到星期
    Day  = WDay & 0x1f;//得到最大月天数
    /*---------------------------------------------------------
         2018年3月8日 星期四
    ---------------------------------------------------------*/
    c    = 0x20;
    y    = 0x18;
    m    = 0x03;
    d    = 0x08;
    cx   = BcdToBin(c);//百年
    yx   = BcdToBin(y);//年
    mx   = BcdToBin(m);//月
    dx   = BcdToBin(d);//日
    WDay = WeekDay20(yx, mx, dx);//取星期和月天数
    Week = WDay >> 5;//得到星期
    Day  = WDay & 0x1f;//得到最大月天数
    while(1);
}
