// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//////�� URL ר�ø�ʽ�ַ�����ԭ����ͨ�ַ���
////
////#include <iconv.h>
////#include <iostream>
////
////using namespace std;
////
////char Char2Int(char ch)
////{
////	if(ch>='0' && ch<='9')return (char)(ch-'0');
////	if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
////	if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
////	return -1;
////}
////
////char Str2Bin(char *str)
////{
////	char tempWord[2];
////	char chn;
////
////	tempWord[0] = Char2Int(str[0]);                                //make the B to 11 -- 00001011
////	tempWord[1] = Char2Int(str[1]);                                //make the 0 to 0  -- 00000000
////
////	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000
////
////	return chn;
////}
////
////string UrlDecode(string str)
////{
////	string output="";
////	char tmp[2];
////	int i=0,idx=0,ndx,len=str.length();
////
////	while(i<len)
////	{
////		if(str[i]=='%')
////		{
////			tmp[0]=str[i+1];
////			tmp[1]=str[i+2];
////			output+=Str2Bin(tmp);
////			i=i+3;
////		}
////		else if(str[i]=='+')
////		{
////			output+=' ';
////			i++;
////		}
////		else
////		{
////			output+=str[i];
////			i++;
////		}
////	}
////
////	return output;
////}
////
////// ����ת�������� ���ڽ�utf-8 ��ʽת�� gb2312
////class CodeConverter
////{
////private:
////	iconv_t cd;
////public:
////	CodeConverter(const char *from_charset,const char *to_charset)
////	{
////		// ����
////		cd = iconv_open(to_charset,from_charset);
////	}
////
////	~CodeConverter()
////	{
////		// ����
////		iconv_close(cd);
////	}
////
////	int convert(char *inbuf,int inlen,char *outbuf,int outlen)
////	{
////		// ת�����
////		char **pin = &inbuf;
////		char **pout = &outbuf;
////
////		memset(outbuf,0,outlen);
////		return iconv(cd, pin, (size_t *)&inlen,pout, (size_t *)&outlen);
////	}
////};
////
//////����url_Utf-8 ,��� gb2312
////string Url2Str_Utf8(string instr)
////{
////	string input;
////	input = UrlDecode(instr);
////
////	const int outlen=instr.length();
////	char output[outlen];
////
////	CodeConverter cc = CodeConverter("utf-8","gb2312");
////	cc.convert((char *)input.c_str(), strlen(input.c_str()), output, outlen);
////
////	return output;
////}
////
//////����url_gb2312 ,��� gb2312 ʵ������ֱ�ӵ��� UrlDecode()
////string Url2Str_gb2312(string str)
////{
////	return UrlDecode(str);
////}
////
////
//////ʾ������
////int main()
////{
////	//char out2[OUTLEN];
////
////	//+�й����������ڵ���^_^
////	cout<<"Url2String_gb2312:"<<Url2Str_gb2312("%2B%D6%D0%B9%FA%B9%FE%B9%FE%B9%FE%D6%D5%D3%DA%B5%C3%C1%CB%5E_%5E")<<endl;
////	cout<<"Url2String_Utf8:"<<Url2Str_Utf8("%2B%E4%B8%AD%E5%9B%BD%E5%93%88%E5%93%88%E5%93%88%E7%BB%88%E4%BA%8E%E5%BE%97%E4%BA%86%5E_%5E")<<endl;
////
////	// utf-8-->gb2312        
////
////	//cout << "utf-8-->gb2312 in=" << out1 << ",out=" << out2 << endl;
////	//cout<<Url2String_Utf8(out1);
////
////	return 0;
////}

#include <string>
using namespace std;
//#include "IsUTF_8.h"
//#include "Url.h"

int CheckSimplified() 
{ 
	char szText[] = "����CIW.��"; 
	char szChinese[3] = {0}; 

	int i = 0, nLen = strlen(szText); 
	for(; i < nLen; i++) 
	{ 
		if( szText[i] >= 0 && szText[i] <= 127 ) //����ȫ���ַ��� 0-0x7F
		{
			printf("������:%c\n", szText[i]); 
		}
		else //��ȫ���ַ� >0x7F
		{
			szChinese[0] = szText[i];
			szChinese[1] = szText[i + 1];
			printf("����:%s\n", szChinese), i++; //������2���ֽ�,����i++ 
		}

	} 
	return 0; 
}

//int UrlDecode(char* pszInUrl, char* pszOutDecode)
//{
//	int iCount = 0;
//	char szTmp[MAX_PATH] = {0};
//	int iLen = strlen(pszInUrl);
//
//	if (!(pszInUrl && pszOutDecode))
//	{
//		return false;
//	}
//
//	for (int i=0; i<iLen+1; i++)
//	{
//		if ('%' != pszInUrl[i])
//		{
//			pszOutDecode[iCount++] = pszInUrl[i];
//		}
//	}
//
//	return iCount+1;
//}

//���ּ��ϵͳ������Ĺ��ܾͱ����ܶԸ���HTTP����������н��룬��UTF��
//HEX���롣������ҵ����ѵ�IDS������������Щ�����������Ƿ��ǹ����ַ�����
//
//�����ı����˵�URL������UTF (%xx%xx) ���������ĵ�HEX����(%xx)�������XX
//��ʮ������ֵ��������һ�ֲ�ͬ���͵ı�����%u���룬����������Unicode/wide
//�����ַ�������%u���벻�Ǳ�׼�ı��룬IDSϵͳ���ܽ���%u�����Կ����ƹ�
//IDS�ļ��ϵͳ��һ����ʵ�����Ӿ���Codered��档


//
//string UrlDecode2(string str)
//{
//	
//	int iLen = 0;
//	char szTem[MAX_PATH] = {0};
//	string strRet;
//	strCoding clsStrCoding;
//
//
//	strRet = clsStrCoding.UrlGB2312Decode(str);
//	iLen = strRet.length();
//	strncpy(szTem, strRet.c_str(), iLen+1);
//
//	if (IsTextUTF8(szTem, iLen))
//	{
//		clsStrCoding.UTF_8ToGB2312(strRet, szTem, iLen);
//	}
//
//
//	if (IsUTF8_sina(szTem))
//	{
//		clsStrCoding.UTF_8ToGB2312(strRet, szTem, iLen);
//	}
//
//
//	if (IsUTF8_Notepad2(szTem, iLen))
//	{
//		clsStrCoding.UTF_8ToGB2312(strRet, szTem, iLen);
//	}
//
//	return strRet;
//}
#include <Windows.h>
//#include "public/CodeUtil.h"
#include "public/QNA_CodeUtil.h"

int _tmain(int argc, _TCHAR* argv[])
{

	UINT iRet = GetACP(); //��ȡ��ǰϵͳ�ı��뷽ʽ

	//CCodeUtil clsStrCoding;
	string str1 = "abcd%E6%9C%AA%E6%84%88%E4%B9%A6%E7%94%9F";
	string str2 = "123%ce%b4%d3%fa%ca%e9%c9%fa";
	string str3 = "aaa%E9%A5%B6%E5%88%9A";
	string str5 = "rain%u5566%u5566%u9ed1%u8863%u4eba";
	char szTem2[MAX_PATH] = {"RAIN%C0%B2%C0%B2%BA%DA%D2%C2%C8%CB"};
	string str6 = szTem2;

	//string str11 = clsStrCoding.UrlToGb2312(str1);
	//printf("str11:%s\r\n", str11.c_str());

	//string str21 = clsStrCoding.UrlToGb2312(str2);
	//printf("str21:%s\r\n", str21.c_str());

	//string str31 = clsStrCoding.UrlToGb2312(str3);
	//printf("str31:%s\r\n", str31.c_str());

	//string str51 = clsStrCoding.UrlToGb2312(str5);
	//printf("str51:%s\r\n", str51.c_str());

	//string str61 = clsStrCoding.UrlToGb2312(str6);
	//printf("str61:%s\r\n", str61.c_str());

	string str11 = QNA::UrlToGb2312(str1);
	printf("str11:%s\r\n", str11.c_str());

	string str21 = QNA::UrlToGb2312(str2);
	printf("str21:%s\r\n", str21.c_str());

	string str31 = QNA::UrlToGb2312(str3);
	printf("str31:%s\r\n", str31.c_str());

	string str51 = QNA::UrlToGb2312(str5);
	printf("str51:%s\r\n", str51.c_str());

	string str61 = QNA::UrlToGb2312(str6);
	printf("str61:%s\r\n", str61.c_str());

	getchar();
	//clsStrCoding.UrlUTF8Decode()
	return 0;
}


