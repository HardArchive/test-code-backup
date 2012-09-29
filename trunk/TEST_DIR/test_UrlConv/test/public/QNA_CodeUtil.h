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

namespace QNA
{
	//做为解Url使用
	//将字符所代表的数字转化为数值
	char CharToInt(char ch)
	{
		if(ch>='0' && ch<='9') return (char)(ch-'0');
		if(ch>='a' && ch<='f') return (char)(ch-'a'+10);
		if(ch>='A' && ch<='F') return (char)(ch-'A'+10);
		return -1;
	}
	//将字符串所代码16进制数字，转化为数值存放于一个char里
	char StrToBin(char* str)
	{
		char chn;
		char sztempWord[2] = {0};			

		sztempWord[0] = CharToInt(str[0]);              //make the B to 11 -- 00001011
		sztempWord[1] = CharToInt(str[1]);              //make the 0 to 0 -- 00000000
		chn = (sztempWord[0] << 4) | sztempWord[1];     //to change the BO to 10110000

		return chn;
	}

	//去掉RUl编码
	string UrlDecode(string strInUrl)
	{
		int iNum = 0;
		int iLen = 0;
		char sztmp[3] = {0};
		char szTem2[3] = {0};
		string strRet = "";

		iLen = strInUrl.length();
		while(iNum < iLen)
		{
			if('%' == strInUrl[iNum])
			{
				if ('u' == strInUrl[iNum+1])
				{
					sztmp[0] = strInUrl[iNum+4];
					sztmp[1] = strInUrl[iNum+5];
					szTem2[0] = StrToBin(sztmp);

					sztmp[0] = strInUrl[iNum+2];
					sztmp[1] = strInUrl[iNum+3];
					szTem2[1] = StrToBin(sztmp);
					WideCharToMultiByte(CP_ACP,NULL, (WCHAR*)szTem2, 1, sztmp, 2, NULL, NULL);
					strRet += sztmp;
					iNum = iNum + 6;

				}
				else
				{//%C9%FA
					sztmp[0] = strInUrl[iNum+1];
					sztmp[1] = strInUrl[iNum+2];
					strRet += StrToBin(sztmp);
					iNum = iNum + 3;
				}
			}
			else if('+' == strInUrl[iNum])
			{
				strRet += ' ';
				iNum++;
			}
			else
			{
				strRet += strInUrl[iNum];
				iNum++;
			}
		}

		return strRet;
	}


	//UTF8是以8bits即1Bytes为编码的最基本单位，当然也可以有基于16bits和32bits的形式，分别称为UTF16和UTF32，但目前用得不多，
	// 而UTF8则被广泛应用在文件储存和网络传输中。
	//编码原理
	//先看这个模板：
	//UCS-4 range (hex.)	UTF-8 octet sequence (binary)
	//0000 0000-0000 007F	0xxxxxxx
	//0000 0080-0000 07FF	110xxxxx 10xxxxxx
	//0000 0800-0000 FFFF	1110xxxx 10xxxxxx 10xxxxxx
	//0001 0000-001F FFFF	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	//0020 0000-03FF FFFF	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	//0400 0000-7FFF FFFF	1111110x 10xxxxxx ... 10xxxxxx
	//编码步骤：
	//1) 首先确定需要多少个8bits(octets)
	//2) 按照上述模板填充每个octets的高位bits
	//3) 把字符的bits填充至x中，字符顺序：低位→高位，UTF8顺序：最后一个octet的最末位x→第一个octet最高位x
	//根据UTF8编码,最多可由6个字节组成,所以UTF8是1-6字节编码组成


	int IsTextUTF8(char* str, ULONGLONG length)
	{
		int i = 0;
		DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
		UCHAR chr = 0;
		BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8
		for(i=0; i<length; i++)
		{
			chr= *(str+i);
			// 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			if((chr&0x80) != 0) 
				bAllAscii= FALSE;
			//如果不是ASCII码,应该是多字节符,计算字节数
			if(nBytes==0)
			{
				if(chr>=0x80)
				{
					if(chr>=0xFC&&chr<=0xFD)
						nBytes=6;
					else if(chr>=0xF8)
						nBytes=5;
					else if(chr>=0xF0)
						nBytes=4;
					else if(chr>=0xE0)
						nBytes=3;
					else if(chr>=0xC0)
						nBytes=2;
					else
					{
						return FALSE;
					}
					nBytes--;
				}
			}
			else //多字节符的非首字节,应为 10xxxxxx
			{
				if( (chr&0xC0) != 0x80 )
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		if( nBytes > 0 ) //违返规则
		{
			return FALSE;
		}
		if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
		{
			return FALSE;
		}
		return TRUE;
	} 

	void UTF_8ToUnicode(WCHAR* pOut,char *pText)
	{
		char* uchar = (char *)pOut;
		uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
		uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	}

	void UnicodeToGB2312(char* pOut,WCHAR uData)
	{
		WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	}

	//UTF_8 转gb2312
	void UTF_8ToGB2312(string &pOut, char *pText, int pLen)
	{
		char buf[4];
		char* rst = new char[pLen + (pLen >> 2) + 2];
		memset(buf,0,4);
		memset(rst,0,pLen + (pLen >> 2) + 2);

		int i =0;
		int j = 0;

		while(i < pLen)
		{
			if(*(pText + i) >= 0)
			{
				rst[j++] = pText[i++];
			}
			else                
			{
				WCHAR Wtemp;
				UTF_8ToUnicode(&Wtemp,pText + i);
				UnicodeToGB2312(buf,Wtemp);

				unsigned short int tmp = 0;
				tmp = rst[j] = buf[0];
				tmp = rst[j+1] = buf[1];
				tmp = rst[j+2] = buf[2];
				i += 3;   
				j += 2;  
			}
		}
		rst[j]='\0';
		pOut = rst;
		delete []rst;
	}


	//Url编码转化为标准Gb2312可显示编码
	string UrlToGb2312(string strInUrl)
	{
		int iLen = 0;
		char szTem[MAX_PATH] = {0};
		char szTem2[MAX_PATH] = {0};
		string strRet = "";
		strRet = UrlDecode(strInUrl);
		iLen = strRet.length();
		strncpy(szTem, strRet.c_str(), iLen+1);
		if (IsTextUTF8(szTem, iLen))
		{
			UTF_8ToGB2312(strRet, szTem, iLen);
		}

		return strRet;
	}
}
#endif
