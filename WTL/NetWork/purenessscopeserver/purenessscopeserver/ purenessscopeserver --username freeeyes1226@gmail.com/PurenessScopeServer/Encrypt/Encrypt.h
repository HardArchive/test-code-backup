#ifndef _ENCRYPT_H
#define _ENCRYPT_H

#include "define.h"
#include "3Des.h"

class CEncrypt
{
public:
	CEncrypt(void);
	~CEncrypt(void);

	bool SetKey(const char* szKey);

	bool DoEncrypt(bool blFlag, const char* szSrc, int nSrcLen, char* szDes, int nDesLen);

private:
	C3DES m_3DES;
	char  m_szKey[8];

};
#endif
