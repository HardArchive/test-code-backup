/*******************************************************************************
 * Copyright (C), 2011-2020, 李  照
 * FileName: HTTP.inl
 * Author: 李  照    Version: 1.0    Date: 2011-08-30
 * Description: HTTP 协议分析共用头
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. Decode(tagDecodeInput &di, CCapture* cap) 解析数据
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2011-08-30   1.0    build this moudle  
 ******************************************************************************/
#pragma once

#ifndef COMMON_H_
#define COMMON_H_

// 从p1中复制数据到p2
// nMaxLen=复制的最大长度；chEnd=复制结束字符，如果遇到此字符，则不再复制；
// bNoToMax=如果未长到结束字符，则复制最大个数的长度；bAppendZero=复制结束是否添加“\0”字符
inline int CopyTo( char*p2, const char* p1, int nMaxLen, const char chEnd, 
				  const bool bNoToMax, const bool bAppendZero )
{
	int nLen = -1;

	// 出错
	if (p1 == NULL || p2 == NULL) return -1;
	if (nMaxLen < 1) return -1;

	char* p = (char*)p1;
	for (int i=0;i<nMaxLen;i++)
	{
		if (*p == chEnd)
		{
			nLen = i;
			if (nLen > 0) memcpy(p2, p1, nLen);
			break;
		}
		p++;
	}
	if (nLen == -1)
	{
		if (bNoToMax)
		{
			memcpy(p2, p1, nMaxLen);
			nLen = nMaxLen;
		}
		else nLen = 0;
	}
	if (bAppendZero) p2[nLen] = 0;

	return nLen;
}

// 从p1中复制数据到p2
// nMaxLen=复制的最大长度；chEnd=复制结束字符，如果遇到此字符，则不再复制；
// bNoToMax=如果未长到结束字符，则复制最大个数的长度；bAppendZero=复制结束是否添加“\0”字符
inline int CopyTo( char*p2, const char* p1, int nMaxLen, const char* szEnd, 
				  const bool bNoToMax, const bool bAppendZero )
{
	int nLen = -1;

	// 出错
	if (p1 == NULL || p2 == NULL || szEnd == NULL) return -1;
	if (nMaxLen < 1) return -1;
	int nEndLen = strlen(szEnd);

	char* p = (char*)p1;
	for (int i=0;i<nMaxLen;i++)
	{
		if (0 == strncmp(p, szEnd, nEndLen))
		{
			nLen = i;
			if (nLen > 0) memcpy(p2, p1, nLen);
			break;
		}
		p++;
	}
	if (nLen == -1)
	{
		if (bNoToMax)
		{
			memcpy(p2, p1, nMaxLen);
			nLen = nMaxLen;
		}
		else nLen = 0;
	}
	if (bAppendZero) p2[nLen] = 0;

	return nLen;
}

// 汉字取子串，最大长度为指定的长度，超过最大长度时，半个汉字的要去掉
inline int GBSubString(const char* p, char* pOut, const int nMaxLen)
{
	int nLen = strlen(p);
	if (nLen > nMaxLen) nLen = nMaxLen;
	// 这里不判断最后一个字是否为汉字，这些工作由上层处理
	for (int i=nLen-1; i>-1; i--)
	{
		if (0 == (p[i] & 0x80))
		{
			nLen = (nLen-i) % 2 == 0 ? nMaxLen-1 : nMaxLen;
			break;
		}
	}
	
	// 处理数据
	if (p == pOut)
	{
		pOut[nLen] = '\0';
	}
	else
	{
		memcpy(pOut, p, nLen);
	}

	return nLen;
}

// 汉字取子串，最大长度为指定的长度，超过最大长度时，半个汉字的要去掉
inline int GBSubStringLen(const char* p, const int nMaxLen)
{
	int nLen = strlen(p);
	
	// 这里不判断最后一个字是否为汉字，这些工作由上层处理
	for (int i=nMaxLen-1; i>-1; i--)
	{
		if (0 == (p[i] & 0x80))
		{
			nLen = (nMaxLen-i) % 2 == 0 ? nMaxLen-1 : nMaxLen;
			break;
		}
	}

	return nLen;
}

// 汉字取子串，最大长度为指定的长度，超过最大长度时，半个汉字的要去掉
inline int GBSubStringLenSafe(const char* p, const int nMaxLen)
{
	if (p == NULL) return 0;
	int nLen = strlen(p);
	if (nLen > nMaxLen)
	{
		if (p[nMaxLen-1] < 0)
		{
			for (int i=nMaxLen-1; i>-1; i--)
			{
				if (0 == (p[i] & 0x80))
				{
					nLen = (nMaxLen-i) % 2 == 0 ? nMaxLen-1 : nMaxLen;
					break;
				}
			}

		}
		else
		{
			nLen = nMaxLen;
		}
	}
	return nLen;
}

inline int StrReplace(char* pDest, const char* pFind, const char* pReplace)
{
	if (pFind == NULL || pReplace == NULL) return -1;
	int nFind = strlen(pFind);
	int nReplace = strlen(pReplace);
	int nLen = strlen(pDest);
	if (nFind == 0) return -2;

	char* p = pDest;
	for (int i=0;i<nLen-nFind;i++)
	{
		if (0 == strncmp(p+i, pFind, nFind))
		{
			strcpy(p+i+nReplace, p+i+nFind);
			memcpy(p+i, pReplace, nReplace);
			break;
		}
	}

	return nLen-nFind+nReplace;
}

// 从p1中复制数据到p2
// nMaxLen=复制的最大长度；chEnd=复制结束字符，如果遇到此字符，则不再复制；
// bNoToMax=如果未长到结束字符，则复制最大个数的长度；bAppendZero=复制结束是否添加“\0”字符
inline int StrToLower( char *p, int nMaxLen = 0)
{
	int nLen = -1;
	// 出错
	if (p == NULL) return -1;
	if (nMaxLen < 1) return -1;
	for (int i=0;i<nMaxLen;i++)
	{
		if ('A' <= *p && *p <= 'Z')
		{
			*p += 0x20;
		}
		p++;
	}
	return 0;
}

// 从p1中复制数据到p2
// nMaxLen=复制的最大长度；chEnd=复制结束字符，如果遇到此字符，则不再复制；
// bNoToMax=如果未长到结束字符，则复制最大个数的长度；bAppendZero=复制结束是否添加“\0”字符
inline int StrToUpper( char *p, int nMaxLen = 0)
{
	int nLen = -1;
	// 出错
	if (p == NULL) return -1;
	if (nMaxLen < 1) return -1;
	for (int i=0;i<nMaxLen;i++)
	{
		if ('a' <= *p && *p <= 'z')
		{
			*p -= 0x20;
		}
		p++;
	}
	return 0;
}

// 递归创建目录
inline bool RCreateDirectory(const char* path, int nMaxLen=0)
{
	if (path == NULL) return false;
	char* p = (char*)path;
	if (nMaxLen == 0) nMaxLen = strlen(path);
	if (nMaxLen ==0) return false;

	for (int i=0;i<nMaxLen; i++)
	{
		if (*p == '\\' && *(p-1) != ':')
		{
			*p = 0;
			if (!PathFileExists(path)) CreateDirectory(path, NULL);
			*p = '\\';
		}
		p++;
	}
	if (!PathFileExists(path)) CreateDirectory(path, NULL);
	return true;
}

// 字符串服务类型转换为整型
inline int ServiceTypeToInt(const char* sServiceType, int nMaxLen)
{
	char *p = (char*)sServiceType;
	for (int i=0;i<nMaxLen;i++)
	{
		if (*p != '0') break;
		p++;
	}

	int nSserviceType = atoi(p);
	return nSserviceType;
}

// 查找第一个出现的字符
inline char* FindFirstChar(char* p, const char ch, const char chEndChar='\0' )
{
	char *pFind = NULL;
	while(*p != chEndChar)
	{
		if (*p == ch)
		{
			pFind = p;
			break;
		}
	}

	return pFind;
}

// 查找第一个出现的字符
inline char* FindLineEnd( char* p, const char chEndChar='\0' )
{
	char *pFind = NULL;
	while(*p != chEndChar)
	{
		if (*p == '\r' && *(p+1)=='\n')
		{
			pFind = p;
			break;
		}
		p++;
	}

	return pFind;
}

// 查找指定字符出现的第一个位置的偏移量
// p1=当前指针位置；nMaxLen=最大查找的长度；chEnd=查找字符；bNoToEnd=未找到时，是否取最大值
// 返回：-1=错误；其他=字符长度。
inline int FirstIndexOfChar( const char* p1, int nMaxLen=0, const char chEnd='\r', const bool bNoToEnd=false )
{
	int nLen = -1;

	// 出错
	if (p1 == NULL) return -1;
	if (nMaxLen == 0) nMaxLen = strlen(p1);
	if (nMaxLen == 0) return 0;

	char* p = (char*)p1;
	for (int i=0;i<nMaxLen;i++)
	{
		if (*p == chEnd)
		{
			nLen = i;
			break;
		}
		p++;
	}
	if (bNoToEnd)
	{
		if (nLen == -1) nLen = nMaxLen;
	}
	else
	{
		if (nLen == -1) nLen = 0;
	}


	return nLen;
}

// HTTP URL 解码
inline int HttpUrlDecode( char* pSrc, char* pDst, int nMaxLen=0 )
{
	if (pSrc == NULL || pDst == NULL)
	{
		TracePrint(LOG_WARNING, "RSRcvSave HttpUrlDecode input parameter error!\r\n");
		return 0;
	}
	if (nMaxLen == 0) nMaxLen = strlen(pSrc);
	if (nMaxLen == 0) return 0;

	const char *tmp = pSrc;
	char *dst = pDst;

	int nCharCount = 0;
	byte btmp1=0,btmp2=0,bret=0;
	do{
		if (*tmp != '%'){
			if (*tmp == 0x2b)
				*dst++ = ' ';
			else
				*dst++ = *tmp;
			tmp++;
		}
		else{
			tmp++;
			if (isalpha(*tmp))
				btmp1 = *tmp-65+10;
			else
				btmp1 = *tmp-48;
			tmp++;
			if (isalpha(*tmp))
				btmp2 = *tmp-65+10;
			else
				btmp2 = *tmp-48;
			bret = (btmp1<<4) + btmp2;
			tmp++;
			*dst++ = (char)bret;
		}
		// 长度加1
		nCharCount ++;
	}while(*tmp && tmp < pSrc+nMaxLen);

	return nCharCount;
}

// 获取GUID字符串
inline string NewGuid()
{
	int nLen = 0;
	GUID m_guid; 
	char pOut[40]={0};
	if(S_OK ==::CoCreateGuid(&m_guid))   
	{         
		nLen = ::sprintf(pOut, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X" ,
			m_guid.Data1,  m_guid.Data2,   m_guid.Data3 ,
			m_guid.Data4[0],   m_guid.Data4[1],
			m_guid.Data4[2],   m_guid.Data4[3],
			m_guid.Data4[4],   m_guid.Data4[5],
			m_guid.Data4[6],   m_guid.Data4[7] );   
	}
	return nLen == 0 ? string("") : string(pOut);
}

// 获取GUID字符串
inline int NewGuid( char* pOut )
{
	int nLen = 0;

	GUID m_guid; 
	if(S_OK ==::CoCreateGuid(&m_guid))   
	{         
		nLen = ::sprintf(pOut, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X" ,
			m_guid.Data1,  m_guid.Data2,   m_guid.Data3 ,
			m_guid.Data4[0],   m_guid.Data4[1],
			m_guid.Data4[2],   m_guid.Data4[3],
			m_guid.Data4[4],   m_guid.Data4[5],
			m_guid.Data4[6],   m_guid.Data4[7] );   
	}   

	return nLen;
}
#endif