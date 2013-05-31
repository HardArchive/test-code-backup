#include "Encrypt.h"

CEncrypt::CEncrypt(void)
{
}

CEncrypt::~CEncrypt(void)
{
}

bool CEncrypt::SetKey(const char* szKey)
{
	if((int)strlen(szKey) >= 8)
	{
		return false;
	}

	sprintf_safe(m_szKey, 8, "%s", szKey);
	return true;
}

bool CEncrypt::DoEncrypt(bool blFlag, const char* szSrc, int nSrcLen, char* szDes, int nDesLen)
{
	if((int)strlen(m_szKey) == 0)
	{
		return false;
	}

	int nEncryptState = 0;
	if(true == blFlag)
	{
		nEncryptState = 0;  //º”√‹
	}
	else
	{
		nEncryptState = 1; //Ω‚√‹
	}

	return m_3DES.DoDES3(nEncryptState, (unsigned char *)szSrc, nSrcLen, (unsigned char *)szDes, (unsigned char *)m_szKey);
	
}
