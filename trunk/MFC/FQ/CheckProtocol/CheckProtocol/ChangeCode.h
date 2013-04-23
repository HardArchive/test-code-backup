#pragma once
//Xt:改进运行应用String，否则定义数字超界问题
class CChangeCode
{
public:
	CChangeCode(void);
	~CChangeCode(void);
	void   UTF_8ToGB2312(char *pSrc,char *pDest,int pLen);				         //Xt: UTF_8  To  GB2312         
	void   GB2312ToUTF_8(string& pOut,char *pText, int pLen);                    //Xt: GB2312 To  UTF_8
	string UrlGB2312(char * str);                                                //Xt: URL GB23122编码
	string UrlUTF8(char * str);                                                  //Xt: URL UTF8编码
	void   UrlUTF8Decode(string strSrc,char *strDest);                           //Xt: URL UTF8解码
	void   UrlGB2312Decode(string strSrc,char *pStrDest);                        //Xt: URL GB2312解码
 
	void   URLGB2312ToURLUTF8(char *strSrc,char *pStrDest);                      //Xt: 带%25编码
    void   URLGB2312ToURLUTF8Decode(string strSrc,char *pStrDest);               //Xt: 带%25解码
   
public:
	static bool   IsUTF8(char* str,unsigned long length);                               
	static char  *UnicodeToGB2312Code(char *pcstr,const wchar_t *pwstr);               
	static WCHAR *Gb2312ToUnicodeCode(WCHAR* pWout,char *gbBuffer);
	static void   FindCode(char *pStrCodeType,char *strSrc,char *pStrDest);
	static void   AntiFindCode(char *pStrCodeType,char *strSrc,char *pStrDest);

private:
	void   Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	void   UTF_8ToUnicode(WCHAR* pOut,char *pText);
	void   UnicodeToUTF_8(char* pOut,WCHAR* pText);
	char   CharToInt(char ch);
	char   StrToBin(char *str);
	void   UnicodeToGB2312(char* pOut,WCHAR uData);

public:
	static CChangeCode *m_pThis;
};
