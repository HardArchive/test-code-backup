
#include "StdAfx.h"
#include "CodeUtil.h"
//这是个类CCodeUtil (CCodeUtil.cpp文件)

CCodeUtil::CCodeUtil(void)
{
}

CCodeUtil::~CCodeUtil(void)
{
}

void CCodeUtil::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return;
}

void CCodeUtil::UTF_8ToUnicode(WCHAR* pOut,char *pText)
{
	char* uchar = (char *)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	return;
}

void CCodeUtil::UnicodeToUTF_8(char* pOut,WCHAR* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}

void CCodeUtil::UnicodeToGB2312(char* pOut,WCHAR uData)
{
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	return;
}

//UTF_8 转gb2312
void CCodeUtil::UTF_8ToGB2312(string &pOut, char *pText, int pLen)
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

			//newBuf[j] = Ctemp[0];
			//newBuf[j + 1] = Ctemp[1];

			i += 3;   
			j += 2;  
		}

	}
	rst[j]='\0';
	pOut = rst;
	delete []rst;
}

//GB2312 转为 UTF-8
void CCodeUtil::GB2312ToUTF_8(string& pOut,char *pText, int pLen)
{
	char buf[4];
	memset(buf,0,4);

	pOut.clear();

	int i = 0;
	while(i < pLen)
	{
		//如果是英文直接复制就可以
		if( pText[i] >= 0)
		{
			char asciistr[2]={0};
			asciistr[0] = (pText[i++]);
			pOut.append(asciistr);
		}
		else
		{
			WCHAR pbuffer;
			Gb2312ToUnicode(&pbuffer,pText+i);

			UnicodeToUTF_8(buf,&pbuffer);

			pOut.append(buf);

			i += 2;
		}
	}

	return;
}

//把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节 如%3D%AE%88
string CCodeUtil::UrlGB2312(char * str)
{
	string dd;
	size_t len = strlen(str);
	for (size_t i=0;i<len;i++)
	{
		if(isalnum((BYTE)str[i]))
		{
			char tempbuff[2];
			sprintf(tempbuff,"%c",str[i]);
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)str[i]))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff,"%%%X%X",((BYTE*)str)[i] >>4,((BYTE*)str)[i] %16);
			dd.append(tempbuff);
		}

	}
	return dd;
}

//把str编码为网页中的 UTF-8 url encode ,英文不变，汉字三字节 如%3D%AE%88

string CCodeUtil::UrlUTF8(char * str)
{
	string tt;
	string dd;
	GB2312ToUTF_8(tt,str,(int)strlen(str));

	size_t len=tt.length();
	for (size_t i=0;i<len;i++)
	{
		if(isalnum((BYTE)tt.at(i)))
		{
			char tempbuff[2]={0};
			sprintf(tempbuff,"%c",(BYTE)tt.at(i));
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)tt.at(i)))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf(tempbuff,"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);
			dd.append(tempbuff);
		}

	}
	return dd;
}
//把url GB2312解码
string CCodeUtil::UrlGB2312Decode(string str)
{
	string output="";
	char tmp[2];
	int i=0,idx=0,ndx,len=str.length();

	while(i<len)
	{
		if(str[i]=='%')
		{
			tmp[0]=str[i+1];
			tmp[1]=str[i+2];
			output += StrToBin(tmp);
			i=i+3;
		}
		else if(str[i]=='+')
		{
			output+=' ';
			i++;
		}
		else
		{
			output+=str[i];
			i++;
		}
	}

	return output;
}

//把url utf8解码
string CCodeUtil::UrlUTF8Decode(string str)
{
	string output="";

	string temp =UrlGB2312Decode(str);//

	UTF_8ToGB2312(output,(char *)temp.data(),strlen(temp.data()));

	return output;

}

//做为解Url使用
//将字符所代表的数字转化为数值
char CCodeUtil::CharToInt(char ch)
{
	if(ch>='0' && ch<='9') return (char)(ch-'0');
	if(ch>='a' && ch<='f') return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F') return (char)(ch-'A'+10);
	return -1;
}
//将字符串所代码16进制数字，转化为数值存放于一个char里
char CCodeUtil::StrToBin(char* str)
{
	char chn;
	char sztempWord[2] = {0};			

	sztempWord[0] = CharToInt(str[0]);              //make the B to 11 -- 00001011
	sztempWord[1] = CharToInt(str[1]);              //make the 0 to 0 -- 00000000
	chn = (sztempWord[0] << 4) | sztempWord[1];     //to change the BO to 10110000

	return chn;
}

string CCodeUtil::UrlDecode(string strInUrl)
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

string CCodeUtil::UrlToGb2312(string strInUrl)
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


int CCodeUtil::IsTextUTF8(char* str, ULONGLONG length)
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



BOOL CCodeUtil::IsUnicode(const char* pBuffer,int cb/*,LPBOOL lpbBOM,LPBOOL lpbReverse*/)
{
	int i = 0xFFFF;

	BOOL bIsTextUnicode;

	BOOL bHasBOM;
	BOOL bHasRBOM;

	if (!pBuffer || cb < 2)
		return FALSE;
	//DWORD IsTextUnicode(CONST PVOID pvBuffer, int cb,PINT pResult);
	//文本文件存在的问题是，它们的内容没有严格和明确的规则，因此很难确定该文件是包含ANSI字符还是Unicode字符。
	// IsTextUnicode使用一系列统计方法和定性方法，以便猜测缓存的内容。由于这不是一种确切的科学方法，因此IsTextUnicode有可能返回不正确的结果。
	//第一个参数pvBuffer用于标识要测试的缓存的地址。该数据是个无效指针，因为你不知道你拥有的是ANSI字符数组还是Unicode字符数组。
	//第二个参数cb用于设定pvBuffer指向的字节数。同样，由于你不知道缓存中放的是什么，因此cb是个字节数，而不是字符数。
	//	请注意，不必设定缓存的整个长度。当然，IsTextUnicode能够测试的字节越多，得到的结果越准确。
	//第三个参数pResult是个整数的地址，必须在调用IsTextUnicode之前对它进行初始化。
	//	对该整数进行初始化后，就可以指明你要IsTextUnicode执行哪些测试。也可以为该参数传递NULL，
	//	在这种情况下，IsTextUnicode将执行它能够进行的所有测试（详细说明请参见PlatformSDK文档）。
	//如果IsTextUnicode认为缓存包含Unicode文本，便返回TRUE，否则返回FALSE。
	//	确实是这样，尽管Microsoft将该函数的原型规定为返回DWORD，但是它实际上返回一个布尔值。
	//	如果在pResult参数指向的整数中必须进行特定的测试，该函数就会在返回之前设定整数中的信息位，以反映每个测试的结果。

	bIsTextUnicode = IsTextUnicode(pBuffer, cb, &i);

	bHasBOM  = (*((UNALIGNED PWCHAR)pBuffer) == 0xFEFF);
	bHasRBOM = (*((UNALIGNED PWCHAR)pBuffer) == 0xFFFE);

	if (i == 0xFFFF) // i doesn't seem to have been modified ...
		i = 0;

	if (bIsTextUnicode || bHasBOM || bHasRBOM ||
		((i & (IS_TEXT_UNICODE_UNICODE_MASK | IS_TEXT_UNICODE_REVERSE_MASK)) &&
		!((i & IS_TEXT_UNICODE_UNICODE_MASK) && (i & IS_TEXT_UNICODE_REVERSE_MASK)) &&
		!(i & IS_TEXT_UNICODE_ODD_LENGTH) &&
		!(i & IS_TEXT_UNICODE_ILLEGAL_CHARS && !(i & IS_TEXT_UNICODE_REVERSE_SIGNATURE)) &&
		!((i & IS_TEXT_UNICODE_REVERSE_MASK) == IS_TEXT_UNICODE_REVERSE_STATISTICS))) 
	{

		//if (lpbBOM)
		//	*lpbBOM = (bHasBOM || bHasRBOM ||
		//	(i & (IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_REVERSE_SIGNATURE)))
		//	? TRUE : FALSE;

		//if (lpbReverse)
		//	*lpbReverse = (bHasRBOM || (i & IS_TEXT_UNICODE_REVERSE_MASK)) ? TRUE : FALSE;

		return TRUE;
	}
	else
		return FALSE;
}


//16进制字符串转换成ASCII数据(HEX)
PBYTE CCodeUtil::StringToHex(char* pInString, const int nLen, const PBYTE pbtOutput, const int nMaxLen)
{
	int iLen = 0;
	char szTem[2] = {0};
	while(iLen<nLen)
	{
		szTem[0] = pInString[iLen];
		szTem[1] = pInString[iLen+1];

		pbtOutput[iLen/2] = StrToBin(szTem);
		iLen += 2;
	}

	return pbtOutput;
}

//ASCII数据转换成16进制字符串
char* CCodeUtil::HexToString(char* pszOut, const int nMaxLen, const PBYTE pbtInput, const int nLen)
{
	const char* chHexList = "0123456789ABCDEF";
	int nIndex = 0;
	for (int i=0, j=0;i<nLen;i++, j+=2)
	{
		nIndex = (pbtInput[i] & 0xf);
		pszOut[i*2+1] = chHexList[nIndex];
		nIndex = ((pbtInput[i]>>4) & 0xf);
		pszOut[i*2] = chHexList[nIndex];
	}
	return pszOut;
}
