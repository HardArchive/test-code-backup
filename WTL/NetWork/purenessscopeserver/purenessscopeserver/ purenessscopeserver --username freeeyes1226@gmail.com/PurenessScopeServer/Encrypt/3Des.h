#if !defined(_CRYPT3DES_H)
#define _CRYPT3DES_H

#if !defined(ED_FLAG)
#define ED_FLAG

#define encrypt 0
#define decrypt 1
#endif

#ifdef WIN32
#include "windows.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

//////////////////////////////////////////////////////////////////////////
/*
unsigned char key[8] = {"doks"};
unsigned char buff[8] = {"abcdef"};

C3DES des;
des.DoDES3(0,buff,key);
des.DoDES3(1,buff,key);
*/

/* 3DES Class. */
class C3DES  
{
public:
	bool DoDES3(int nWay,unsigned char* pSrc,int nSrcSize,unsigned char* pDes,unsigned char pKey[8]);
private:
	bool DoDES3(
		unsigned char EDFlag,  //EDFlag是加\脱密标志,0表示加密，1表示脱密
		unsigned char databuf[8], //DataBuf将被处理的明文或密文的缓冲区,并兼作输出缓冲区
		unsigned char keybuf[8]  //8byte的密钥缓冲区
	);
	inline void pro_key(void);
	inline void first_p(void);
	inline void expand_x(void);
	inline void fction(void);
	inline void make(bool FirstFlag);

	static const unsigned long sp0[64],sp1[64],sp2[64],sp3[64],sp4[64],sp5[64],sp6[64],sp7[64];
	static const unsigned char pc0[64],pc0_v[64],pc1[56],pc2[48],rotate[16],bytebit[8];
	static const unsigned long bigbyte[24],bigbyte1[32];
	unsigned char key[8],m[8],c[8],k[16][8],s_in[8],pc[64];
	unsigned char ed_flag;
	unsigned long x[2];
};


#endif


