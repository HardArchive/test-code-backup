//file CodeUtil.h
/*************************************************************************************************** 
* 1、 Class      ： CCodeUtil
* 2、 Version    ： 1.1
* 3、 Description： 编码转换辅助类支持Url Unicode、Gb2312、UTF_8
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-7-18 10:30:07
* 6、 History    ： 
* 7、 Remark     ： 支持宽字节和多字节
****************************************************************************************************/  
#ifndef __CODE_UNTIL_H__
#define __CODE_UNTIL_H__

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

class CCodeUtil
{
public:
	CCodeUtil(void);
	~CCodeUtil(void);

	void UTF_8ToGB2312(string &pOut, char *pText, int pLen);//utf_8转为gb2312
	void GB2312ToUTF_8(string& pOut,char *pText, int pLen); //gb2312 转utf_8
	string UrlGB2312(char * str);                           //urlgb2312编码
	string UrlUTF8(char * str);                             //urlutf8 编码
	string UrlUTF8Decode(string str);                       //urlutf8解码
	string UrlGB2312Decode(string str);                     //urlgb2312解码
	void UnicodeToUTF_8(char* pOut,WCHAR* pText);

	int IsTextUTF8(char* str, ULONGLONG length);
	BOOL IsUnicode(const char* pBuffer,int cb/*,LPBOOL lpbBOM,LPBOOL lpbReverse*/);
	string UrlToGb2312(string strInUrl);                   //Url编码转化为标准Gb2312可显示编码

	PBYTE StringToHex(char* pInString, const int nLen, const PBYTE pbtOutput, const int nMaxLen);
	char *HexToString(char* pszOut, const int nMaxLen, const PBYTE pbtInput, const int nLen);

private:
	void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	void UTF_8ToUnicode(WCHAR* pOut,char *pText);
	//void UnicodeToUTF_8(char* pOut,WCHAR* pText);
	void UnicodeToGB2312(char* pOut,WCHAR uData);

	char CharToInt(char ch);
	char StrToBin(char *str);
	string UrlDecode(string strInUrl);
};

#endif