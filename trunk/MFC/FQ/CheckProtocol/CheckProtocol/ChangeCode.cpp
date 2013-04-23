#include "stdafx.h"
#include "ChangeCode.h"

CChangeCode *CChangeCode::m_pThis = NULL;

CChangeCode::CChangeCode(void)
{
	m_pThis = this;
}


CChangeCode::~CChangeCode(void)
{
	m_pThis = NULL;
}

void CChangeCode::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return;
}
void CChangeCode::UTF_8ToUnicode(WCHAR* pOut,char *pText)
{
	char* uchar = (char *)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	return;
}

void CChangeCode::UnicodeToUTF_8(char* pOut,WCHAR* pText)
{
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}
void CChangeCode::UnicodeToGB2312(char* pOut,WCHAR uData)
{
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	return;
}

//Xt: 做为解Url使用
char CChangeCode:: CharToInt(char ch)
{
	if(ch>='0' && ch<='9')return (char)(ch-'0');
	if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
	return -1;
}
char CChangeCode::StrToBin(char *str)
{
	char tempWord[2];
	char chn;

	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0  -- 00000000

	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

	return chn;
}


//Xt: UTF_8 转gb2312
void CChangeCode::UTF_8ToGB2312(char *pText,char *pDest,int pLen)
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
	memcpy(pDest,rst,strlen(rst)); 
	delete []rst;
}

//Xt: GB2312 转为 UTF-8
void CChangeCode::GB2312ToUTF_8(string& pOut,char *pText, int pLen)
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
//Xt:把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节  如%3D%AE%88
string CChangeCode::UrlGB2312(char * str)
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
//Xt:把str编码为网页中的 UTF-8 url encode ,英文不变，汉字三字节  如%3D%AE%88
string CChangeCode::UrlUTF8(char * str)
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
			sprintf_s(tempbuff,sizeof(tempbuff),"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);
			dd.append(tempbuff);
		}

	}
	return dd;
}
//Xt:把url GB2312解码
void CChangeCode::UrlGB2312Decode(string strSrc,char *pStrDest)
{
	string output="";
	char tmp[2];
	int  ndx = 0,
		 i = 0,
		 idx = 0,
		 len = strSrc.length();

	while(i<len){
		if(strSrc[i] == '%')
		{
			tmp[0] = strSrc[i+1];
			tmp[1] = strSrc[i+2];
			output += StrToBin(tmp);
			i=i+3;
		}
		else if(strSrc[i] == '+')
		{
			output += ' ';
			i++;
		}
		else{
			output += strSrc[i];
			i++;
		}
	}
	memcpy(pStrDest,output.data(),output.length());
	
}
//Xt:把url utf8解码
void CChangeCode::UrlUTF8Decode(string strSrc,char *strDest)
{
	char strDestToSrc[1024] = {0};                             //Xt: ????????????超出界限问题
	UrlGB2312Decode(strSrc,strDestToSrc);
    UTF_8ToGB2312(strDestToSrc,strDest,strlen(strDestToSrc));
}

bool CChangeCode::IsUTF8(char* str,unsigned long length)
{ 
	int i;
	unsigned long nBytes = 0;                  //UFT8可用1-6个字节编码,ASCII用一个字节
	unsigned char chr;
	bool bAllAscii = true;                     //如果全部都是ASCII, 说明不是UTF-8
	for(i=0;i<length;i++)
	{
		chr = *(str+i);
		if( (chr&0x80) != 0 )                  // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii= false;
		if(nBytes == 0)                        //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr>=0x80)
			{
				if(chr >= 0xFC&&chr <= 0xFD)
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
					return false;
				}
				nBytes--;
			}
		}
		else                                   //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return false;
			}
			nBytes--;
		}
	}
	if( nBytes > 0 )                           //违返规则
	{
		return false;
	}
	if( bAllAscii )                            //如果全部都是ASCII, 说明不是UTF-8
	{
		return false;
	}
	return true;
}

/* void CChangeCode::UTF_8ToANSI(char *strUTF8,string &strANSI)
{  
	int nLen = ::MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,strUTF8,-1,NULL,0); 
	//返回需要的unicode长度   
	WCHAR * wszANSI = new WCHAR[nLen+1];   
	memset(wszANSI, 0, nLen * 2 + 2);   
	nLen = MultiByteToWideChar(CP_UTF8, 0,strUTF8, -1, wszANSI, nLen);              //Xt: 把utf8转成unicode  
	nLen = WideCharToMultiByte(CP_ACP, 0, wszANSI, -1, NULL, 0, NULL, NULL);        //Xt: 得到要的ansi长度   
	char *szANSI=new char[nLen + 1];   
	memset(szANSI, 0, nLen + 1);   
	WideCharToMultiByte (CP_ACP, 0, wszANSI, -1, szANSI, nLen, NULL,NULL);          //Xt: 把unicode转成ansi   
	strANSI = szANSI;   
	delete [] wszANSI;   
	delete [] szANSI;   
}*/



void CChangeCode::URLGB2312ToURLUTF8(char *strSrc,char *pStrDest)
{
	string str;
	char   strDestToSrc1[1024] = "";
	char   strDestToSrc2[1024] = "";

	str = UrlUTF8(strSrc);
	memcpy(strDestToSrc1,str.c_str(),str.length());
	str.clear();

	str = UrlGB2312(strDestToSrc1);
	memcpy(strDestToSrc2,str.c_str(),str.length());
	str.clear();

	str = UrlGB2312(strDestToSrc2);
	memcpy(pStrDest,str.c_str(),str.length());
	str.clear();

}
void CChangeCode::URLGB2312ToURLUTF8Decode(string strSrc,char *pStrDest)
{
	char   strDestToSrc1[1024] = "";					         //Xt:?????????????
	char   strDestToSrc2[1024] = "";							 //Xt:?????????????
	UrlGB2312Decode(strSrc,strDestToSrc1);
	UrlGB2312Decode(strDestToSrc1,strDestToSrc2);
	UrlUTF8Decode(strDestToSrc2,pStrDest);
}

// void CChangeCode::UnicodeToGB2312Code(char* pOut,WCHAR* uData)
// {
// 	WideCharToMultiByte(CP_ACP,NULL,uData,1,pOut,wcslen(uData),NULL,NULL);
// 	return;
// }

char *CChangeCode::UnicodeToGB2312Code(char *pcstr,const wchar_t *pwstr)//, size_t len)
{
	int nlength=wcslen(pwstr);

	//获取转换后的长度
	int nbytes = WideCharToMultiByte( 0,0,pwstr,nlength,NULL,0,NULL,NULL );

	//if(nbytes>len)   nbytes=len;
	WideCharToMultiByte( 0,0,pwstr,nlength,pcstr,nbytes,NULL,NULL );
	return pcstr ;
}

WCHAR *CChangeCode::Gb2312ToUnicodeCode(WCHAR* pWout,char *gbBuffer)
{
	int nLen = strlen(gbBuffer);
	if(gbBuffer)
	{
		int nWLen = MultiByteToWideChar(CP_ACP,0,(const char *)gbBuffer,nLen,NULL,0);

		::MultiByteToWideChar(CP_ACP,0,(const char *)gbBuffer,nLen,pWout,nWLen);    
	}
	return pWout;
}

void  CChangeCode::FindCode(char *pStrCodeType,char *strSrc,char *pStrDest)
{
	if(!memcmp(pStrCodeType,"U-U",sizeof("U-U")))
	{
		m_pThis->UrlUTF8Decode(strSrc,pStrDest);
		return;
	}
	if(!memcmp(pStrCodeType,"U-G",sizeof("U-G")))
	{
		m_pThis->UrlGB2312Decode(strSrc,pStrDest);
		return;
	}
	if(!memcmp(pStrCodeType,"U-",sizeof("U-")))
	{
		m_pThis->UTF_8ToGB2312(strSrc,pStrDest,strlen(strSrc));	
		return;
	}
	if(!memcmp(pStrCodeType,"U-G-U",sizeof("U-G-U")))
	{
		m_pThis->URLGB2312ToURLUTF8Decode(strSrc,pStrDest);
		return;
	}
	if('\0' == *pStrCodeType  )
	{
		memcpy(pStrDest,strSrc,strlen(strSrc));
		return;
	}
}

void  CChangeCode::AntiFindCode(char *pStrCodeType,char *strSrc,char *pStrDest)
{
	string str;
	if(!memcmp(pStrCodeType,"U-U",sizeof("U-U")))
	{
		str = m_pThis->UrlUTF8(strSrc);
		memcpy(pStrDest,str.c_str(),str.length());
		return;
	}
	str.clear();

	if(!memcmp(pStrCodeType,"U-G",sizeof("U-G")))
	{
		str = m_pThis->UrlGB2312(strSrc);
		memcpy(pStrDest,str.c_str(),str.length());	
		return;
	}
	str.clear();

	if(!memcmp(pStrCodeType,"U-",sizeof("U-")))
	{
		m_pThis->GB2312ToUTF_8(str,strSrc,strlen(strSrc));
		memcpy(pStrDest,str.c_str(),str.length());
		return;
	}
	str.clear();

	if(!memcmp(pStrCodeType,"U-G-U",sizeof("U-G-U")))
	{
		m_pThis->URLGB2312ToURLUTF8(strSrc,pStrDest);
		return;
	}
}