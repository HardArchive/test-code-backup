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

#include "Url.h"

int _tmain(int argc, _TCHAR* argv[])
{
	string output="/proxy.html";
	string output1="//proxy.html";
	string output2="\\proxy.html";
	strCoding clsStrCoding;
	string str1 = "%E6%9C%AA%E6%84%88%E4%B9%A6%E";
	char szTem[MAX_PATH] = {"%B0%A1"};
	string str2 = clsStrCoding.UrlUTF8Decode(str1);
	//clsStrCoding.UrlUTF8Decode()
	return 0;
}

