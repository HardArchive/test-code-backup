/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: HTTP.inl
 * Author: ��  ��    Version: 1.0    Date: 2011-08-30
 * Description: HTTP Э���������ͷ
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. Decode(tagDecodeInput &di, CCapture* cap) ��������
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2011-08-30   1.0    build this moudle  
 ******************************************************************************/
#pragma once

#ifndef COMMON_H_
#define COMMON_H_

// ��p1�и������ݵ�p2
// nMaxLen=���Ƶ���󳤶ȣ�chEnd=���ƽ����ַ�������������ַ������ٸ��ƣ�
// bNoToMax=���δ���������ַ��������������ĳ��ȣ�bAppendZero=���ƽ����Ƿ���ӡ�\0���ַ�
inline int CopyTo( char*p2, const char* p1, int nMaxLen, const char chEnd, 
				  const bool bNoToMax, const bool bAppendZero )
{
	int nLen = -1;

	// ����
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

// ��p1�и������ݵ�p2
// nMaxLen=���Ƶ���󳤶ȣ�chEnd=���ƽ����ַ�������������ַ������ٸ��ƣ�
// bNoToMax=���δ���������ַ��������������ĳ��ȣ�bAppendZero=���ƽ����Ƿ���ӡ�\0���ַ�
inline int CopyTo( char*p2, const char* p1, int nMaxLen, const char* szEnd, 
				  const bool bNoToMax, const bool bAppendZero )
{
	int nLen = -1;

	// ����
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

// ����ȡ�Ӵ�����󳤶�Ϊָ���ĳ��ȣ�������󳤶�ʱ��������ֵ�Ҫȥ��
inline int GBSubString(const char* p, char* pOut, const int nMaxLen)
{
	int nLen = strlen(p);
	if (nLen > nMaxLen) nLen = nMaxLen;
	// ���ﲻ�ж����һ�����Ƿ�Ϊ���֣���Щ�������ϲ㴦��
	for (int i=nLen-1; i>-1; i--)
	{
		if (0 == (p[i] & 0x80))
		{
			nLen = (nLen-i) % 2 == 0 ? nMaxLen-1 : nMaxLen;
			break;
		}
	}
	
	// ��������
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

// ����ȡ�Ӵ�����󳤶�Ϊָ���ĳ��ȣ�������󳤶�ʱ��������ֵ�Ҫȥ��
inline int GBSubStringLen(const char* p, const int nMaxLen)
{
	int nLen = strlen(p);
	
	// ���ﲻ�ж����һ�����Ƿ�Ϊ���֣���Щ�������ϲ㴦��
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

// ����ȡ�Ӵ�����󳤶�Ϊָ���ĳ��ȣ�������󳤶�ʱ��������ֵ�Ҫȥ��
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

// ��p1�и������ݵ�p2
// nMaxLen=���Ƶ���󳤶ȣ�chEnd=���ƽ����ַ�������������ַ������ٸ��ƣ�
// bNoToMax=���δ���������ַ��������������ĳ��ȣ�bAppendZero=���ƽ����Ƿ���ӡ�\0���ַ�
inline int StrToLower( char *p, int nMaxLen = 0)
{
	int nLen = -1;
	// ����
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

// ��p1�и������ݵ�p2
// nMaxLen=���Ƶ���󳤶ȣ�chEnd=���ƽ����ַ�������������ַ������ٸ��ƣ�
// bNoToMax=���δ���������ַ��������������ĳ��ȣ�bAppendZero=���ƽ����Ƿ���ӡ�\0���ַ�
inline int StrToUpper( char *p, int nMaxLen = 0)
{
	int nLen = -1;
	// ����
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

// �ݹ鴴��Ŀ¼
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

// �ַ�����������ת��Ϊ����
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

// ���ҵ�һ�����ֵ��ַ�
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

// ���ҵ�һ�����ֵ��ַ�
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

// ����ָ���ַ����ֵĵ�һ��λ�õ�ƫ����
// p1=��ǰָ��λ�ã�nMaxLen=�����ҵĳ��ȣ�chEnd=�����ַ���bNoToEnd=δ�ҵ�ʱ���Ƿ�ȡ���ֵ
// ���أ�-1=��������=�ַ����ȡ�
inline int FirstIndexOfChar( const char* p1, int nMaxLen=0, const char chEnd='\r', const bool bNoToEnd=false )
{
	int nLen = -1;

	// ����
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

// HTTP URL ����
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
		// ���ȼ�1
		nCharCount ++;
	}while(*tmp && tmp < pSrc+nMaxLen);

	return nCharCount;
}

// ��ȡGUID�ַ���
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

// ��ȡGUID�ַ���
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