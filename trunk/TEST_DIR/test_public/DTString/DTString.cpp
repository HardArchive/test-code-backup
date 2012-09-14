//////////////////////////////////////////////////////////////////////////
// Project:			DTString
// FileNme:			DTString.cpp
// Description:		DTString is a String using the standard C++ coding of the class,
//				in order to be compatible with the MFC code
// Change Time:		2012-2-9 14:13:38
// Code by ccnyou
//
// Change Time:		2012-2-11
// Change Log :		for add some method by ccnyou such as operator +(string)
// Change Time:		2012-2-24 0:49:38
// Change Log :		for add method ReverseFind and explain by ccnyou
//
// Change Log:      2012-2-28
//                  change macro _MACOS => __APPLE_CC__
//////////////////////////////////////////////////////////////////////////


#include "DTString.h"

const DTString &DTString::operator=(const char *pStr)
{
	m_string = pStr;
	return *this;
}

const DTString &DTString::operator+=(const char *pStr)
{
	m_string += pStr;
	return *this;
}

DTString operator+(const DTString &str1,const char *pStr)
{
	DTString strRet = str1;
	strRet.m_string += pStr;
	return strRet;
}

const DTString &DTString::operator = (const DTString &Str)
{
    m_string = Str.operator LPCSTR();
    return *this;
}

const DTString &DTString::operator+=(const DTString &Str)
{
	m_string += Str;
	return *this;
}

DTString operator+(const DTString &Str1,const DTString &Str2)
{
	DTString strRet = Str1;
	strRet.m_string += Str2.m_string;
	return strRet;
}


const DTString &DTString::operator = (const string &Str)
{
	m_string = Str;
	return *this;
}

const DTString &DTString::operator += (const string &Str)
{
	m_string += Str;
	return *this;
}

DTString operator+(const DTString &str1,const string &Str)
{
	DTString strRet = str1;
	strRet.m_string += Str;
	return strRet;
}

DTString::operator LPCSTR() const
{
	return this->m_string.c_str();
}



ostream &operator <<(ostream &os,DTString &str)
{
	os<<str.m_string;
	return os;
}

istream &operator >>(istream &is,DTString &str)
{
	is>>str.m_string;
	return is;
}





DTString::DTString(const char *pStr)
{
	m_string = pStr;
}


DTString::DTString()
{
	m_string = "";
}

DTString::~DTString()
{

}

int DTString::GetLength()
{
	return m_string.length();
}

DTString DTString::Left( int nCount ) const
{
	char *pBuf = (char *)malloc(nCount+1);
	strncpy(pBuf,*this,nCount);
	pBuf[nCount] = '\0';
	DTString strTmp(pBuf);
	free(pBuf);
	return strTmp;
}

DTString DTString::Right( int nCount ) const
{
	DTString strTmp;
	strTmp = m_string.c_str() + m_string.length() - nCount;
	return strTmp;
}


int DTString::Find( char ch, int nStart ) const
{
	const char *pStr = m_string.c_str() + nStart;
	const char *pFindRet = strchr(pStr,ch);
	return pFindRet - pStr + nStart;
}

int DTString::Find(char const *pStrToFind,int nStartPos) const
{
	const char *pStr = m_string.c_str() + nStartPos;
	const char *pFindRet = strstr(pStr,pStrToFind);
	return pFindRet - pStr + nStartPos;
}

int DTString::ReverseFind(char ch) const
{
	int iRet = -1;
	const char *pStr = m_string.c_str();
	const char *pFind = strrchr(pStr,ch);
	if (pFind != NULL)
	{
		iRet = pFind-pStr;
	}
	return iRet;
}





#ifdef __APPLE_CC__
#pragma mark -
#pragma mark MACOS

DTString::operator PCNSSTRING() const
{
    const char *pStr = m_string.c_str();
    NSString *str = [NSString stringWithCString:pStr encoding:NSUTF8StringEncoding];
    return str;
}

DTString::DTString(NSString *Str)
{
    LoadNSString(Str);
}

NSString *DTString::ns_str() const
{
    const char *pStr = m_string.c_str();
    NSString *str = [NSString stringWithCString:pStr encoding:NSUTF8StringEncoding];
    return str;
}

DTString &DTString::LoadNSString(NSString *str)
{
    const char *pStr = [str UTF8String];
    m_string = pStr;
    return *this;
}

DTString &DTString::operator = (NSString *str)
{
    const char *pStr = [str UTF8String];
    m_string = pStr;
    return *this;
}

#endif

void DTString::Format(LPCTSTR lpszFormat, ...)
{
	char		*pBuffer = NULL;
	va_list      arg_ptr;
	pBuffer = (char	*)malloc(MAX_FORMAT_BUFFER);
	if (pBuffer == NULL)
	{
		cerr<<"Out of memory!"<<endl;
		abort();
	}
	pBuffer[MAX_FORMAT_BUFFER-1] = '\0';

	va_start(arg_ptr,lpszFormat);
	vsnprintf(pBuffer,MAX_FORMAT_BUFFER,lpszFormat,arg_ptr);
	if (pBuffer[MAX_FORMAT_BUFFER-1] != '\0')
	{
		cout<<"Warning:Data too long!"<<endl;
		FILE *fpTempFile = tmpfile();
		if (fpTempFile == NULL)
		{
			goto END_FORMAT;
		}
		vfprintf(fpTempFile,lpszFormat,arg_ptr);
		fseek(fpTempFile,0,SEEK_END);
		int iFileLength = ftell(fpTempFile);
		rewind(fpTempFile);

		free(pBuffer);
		pBuffer = (char	*)malloc(iFileLength + 1);
		if (pBuffer == NULL)
		{
			cerr<<"Out of memory!"<<endl;
			abort();
		}
		fread(pBuffer,iFileLength,1,fpTempFile);
		pBuffer[iFileLength] = '\0';

		fclose(fpTempFile);
	}
	va_end(arg_ptr);

	m_string = pBuffer;

END_FORMAT:
	free(pBuffer);
}
