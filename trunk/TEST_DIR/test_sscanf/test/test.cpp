// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

// C 运行时头文件

#include <ATLComTime.h>

DATE string2date(char* ptInTime)
{
	DATE dtRet = 0;
	if (!ptInTime) return dtRet;

	int iYear = 0, iMonth = 0, iDay = 0, iHour = 0, iMin = 0, iSec = 0;
	sscanf_s("1970-02-04 08:57:32", "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);

	COleDateTime dtCurrent(iYear, iMonth, iDay, iHour, iMin, iSec);

	dtRet = dtCurrent.m_dt;
	return dtRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//1. 常见用法。
	char buf[512] ;
	sscanf("123456 ", "%s", buf);//此处buf是数组名，它的意思是将123456以%s的形式存入buf中！
	printf("%s\n", buf);
	//结果为：123456

	//2. 取指定长度的字符串。如在下例中，取最大长度为4字节的字符串。
	sscanf("123456 ", "%4s", buf);
	printf("%s\n", buf);
	//结果为：1234

	//3. 取到指定字符为止的字符串。如在下例中，取遇到空格为止字符串。
	sscanf("123456 abcdedf", "%[^ ]", buf);
	printf("%s\n", buf);
	//结果为：123456

	//4. 取仅包含指定字符集的字符串。如在下例中，取仅包含1到9和小写字母的字符串。
	sscanf("123456abcdedfBCDEF", "%[1-9a-z]", buf);
	printf("%s\n", buf);
	//结果为：123456abcdedf	
	//当输入：
	sscanf("123456abcdedfBCDEF","%[1-9A-Z]",buf);
	printf("%s\n",buf);
	//结果为：123456

	//5. 取到指定字符集为止的字符串。如在下例中，取遇到大写字母为止的字符串。
	sscanf("123456abcdedfBCDEF", "%[^A-Z]", buf);
	printf("%s\n", buf);
	//结果为：123456abcdedf

	//6、给定一个字符串iios/12DDWDFF@122，获取 / 和 @ 之间的字符串，先将 "iios/"过滤掉，再将非'@'的一串内容送到buf中
	sscanf("iios/12DDWDFF@122", "%*[^/]/%[^@]", buf);
	printf("%s\n", buf);
	//结果为：12DDWDFF

	//7、给定一个字符串"hello, world"，仅保留world。（注意："，"之后有一空格，%s遇空格停止，加*则是忽略第一个读到的字符串）
	sscanf("hello, world", "%*s%s", buf);
	printf("%s\n", buf);
	//结果为：world
	//%*s表示第一个匹配到的%s被过滤掉，即hello被过滤了
	//如果没有空格则结果为NULL。
	//sscanf的功能很类似于正则表达式, 但却没有正则表达式强大,所以如果对于比较复杂的字符串处理,建议使用正则表达式.


	//-------------------------------------------------------
	//用它来分隔类似这样的字符串2006:03:18:
	int a, b, c;
	/*sscanf("2006:03:18", "%d:%d:%d", a, b, c); */ /*错误方法, 要在变量a,b,c前加上取地址符, modified by huanmie_09*/
	sscanf("2006:03:18", "%d:%d:%d", &a, &b, &c);
	//以及2006:03:18 - 2006:04:18:
	char sztime1[16] = "", sztime2[16] = "";
	sscanf("2006:03:18 - 2006:04:18", "%s - %s", sztime1, sztime2);
	//但是后来，我需要处理2006:03:18-2006:04:18
	//仅仅是取消了‘-’两边的空格，却打破了%s对字符串的界定。
	//我需要重新设计一个函数来处理这样的情况？这并不复杂，但是，为了使所有的代码都有统一的风格，我需要改动很多地方，把已有的sscanf替换成我自己的分割函数。
	//我以为我肯定需要这样做，并伴随着对sscanf的强烈不满而入睡；一觉醒来，发现其实不必。
	//format-type中有%[]这样的type field。如果读取的字符串，不是以空格来分隔的话，就可以使用%[]。
	//%[]类似于一个正则表达式。[a-z]表示读取a-z的所有字符，[^a-z]表示读取除a-z以外的所有字符。所以那个问题也就迎刃而解了:
	sscanf("2006:03:18 - 2006:04:18", "%[0-9,:] - %[0-9,:]", sztime1, sztime2);
	//-------非本段作者---------------------------------------------------------------------------------------------------
	sscanf("2006:03:18-2006:04:18","%[^-]-%s",sztime1,sztime2);
	//描述：%[^-]匹配到‘-’默认为存到sztime1的字符串“2006:03:18”加空格符，所以%s会默认输入“-2006:04:18”到sztime2这也就是%s前面加‘-’的原因。输出跟作者一样，但更易懂、易读。
	//----------------------------------------------------------------------------------------------------end-----------------
	

	//1970-01-01 08:00:00
	int iYear = 0, iMonth = 0, iDay = 0, iHour = 0, iMin = 0, iSec = 0;
	//sscanf("1970-02-04 08:57:32", "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);
	sscanf_s("1970-02-04 08:57:32", "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);

	char szTem[MAX_PATH] = {0};
	sprintf_s(szTem, MAX_PATH, "%04d%02d%02d%02d%02d%02d00", iYear, iMonth, iDay, iHour, iMin, iSec);

	DATE dtTime = string2date("1970-02-04 08:57:32");

	
	memset(szTem, 0, MAX_PATH);
	strcpy_s(szTem, MAX_PATH, "310107100063290000Z");
	char* pTem = szTem+strlen(szTem)-5;


	int iTem = atoi(pTem);


	return 0;
}

