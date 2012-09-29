//file CodeUtil.h
/*************************************************************************************************** 
* 1�� Class      �� CCodeUtil
* 2�� Version    �� 1.1
* 3�� Description�� ����ת��������֧��Url Unicode��Gb2312��UTF_8
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-7-18 10:30:07
* 6�� History    �� 
* 7�� Remark     �� ֧�ֿ��ֽںͶ��ֽ�
****************************************************************************************************/  
#ifndef __CODE_UNTIL_H__
#define __CODE_UNTIL_H__

#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

namespace QNA
{
	//��Ϊ��Urlʹ��
	//���ַ������������ת��Ϊ��ֵ
	char CharToInt(char ch)
	{
		if(ch>='0' && ch<='9') return (char)(ch-'0');
		if(ch>='a' && ch<='f') return (char)(ch-'a'+10);
		if(ch>='A' && ch<='F') return (char)(ch-'A'+10);
		return -1;
	}
	//���ַ���������16�������֣�ת��Ϊ��ֵ�����һ��char��
	char StrToBin(char* str)
	{
		char chn;
		char sztempWord[2] = {0};			

		sztempWord[0] = CharToInt(str[0]);              //make the B to 11 -- 00001011
		sztempWord[1] = CharToInt(str[1]);              //make the 0 to 0 -- 00000000
		chn = (sztempWord[0] << 4) | sztempWord[1];     //to change the BO to 10110000

		return chn;
	}

	//ȥ��RUl����
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


	//UTF8����8bits��1BytesΪ������������λ����ȻҲ�����л���16bits��32bits����ʽ���ֱ��ΪUTF16��UTF32����Ŀǰ�õò��࣬
	// ��UTF8�򱻹㷺Ӧ�����ļ���������紫���С�
	//����ԭ��
	//�ȿ����ģ�壺
	//UCS-4 range (hex.)	UTF-8 octet sequence (binary)
	//0000 0000-0000 007F	0xxxxxxx
	//0000 0080-0000 07FF	110xxxxx 10xxxxxx
	//0000 0800-0000 FFFF	1110xxxx 10xxxxxx 10xxxxxx
	//0001 0000-001F FFFF	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	//0020 0000-03FF FFFF	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	//0400 0000-7FFF FFFF	1111110x 10xxxxxx ... 10xxxxxx
	//���벽�裺
	//1) ����ȷ����Ҫ���ٸ�8bits(octets)
	//2) ��������ģ�����ÿ��octets�ĸ�λbits
	//3) ���ַ���bits�����x�У��ַ�˳�򣺵�λ����λ��UTF8˳�����һ��octet����ĩλx����һ��octet���λx
	//����UTF8����,������6���ֽ����,����UTF8��1-6�ֽڱ������


	int IsTextUTF8(char* str, ULONGLONG length)
	{
		int i = 0;
		DWORD nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
		UCHAR chr = 0;
		BOOL bAllAscii = TRUE; //���ȫ������ASCII, ˵������UTF-8
		for(i=0; i<length; i++)
		{
			chr= *(str+i);
			// �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx
			if((chr&0x80) != 0) 
				bAllAscii= FALSE;
			//�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
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
			else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
			{
				if( (chr&0xC0) != 0x80 )
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		if( nBytes > 0 ) //Υ������
		{
			return FALSE;
		}
		if( bAllAscii ) //���ȫ������ASCII, ˵������UTF-8
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

	//UTF_8 תgb2312
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


	//Url����ת��Ϊ��׼Gb2312����ʾ����
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
