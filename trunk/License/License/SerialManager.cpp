#include "StdAfx.h"
#include "SerialManager.h"
#include "HDSerial.h"


CSerialManager::CSerialManager(void)
{
	MakeMachineCode();    //创建机器码
	GetFinalData(m_strMachineCode,m_strSerial);
}

CSerialManager::~CSerialManager(void)
{
}
BOOL CSerialManager::ValidateSerial(CString& strSerial)
{
	if (strSerial.CompareNoCase(m_strSerial)==0)
	{
		return TRUE;
	}
	return FALSE;
}
CString& CSerialManager::MakeMachineCode()
{
	unsigned long s1,s2;     
	TCHAR sel;   
	sel='1';   
	CString MyCpuID,CPUID1,CPUID2;   
	__asm{   
		mov eax,01h   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,eax   
	}   
	CPUID1.Format(_T("%08X%08X"),s1,s2);   
	__asm{   
		mov eax,03h   
			xor ecx,ecx   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,ecx   
	}   
	CPUID2.Format(_T("%08X%08X"),s1,s2);   

	MyCpuID=CPUID1+CPUID2;  //BFEB FBFF 0001 067A 0000000000000000
	OutputDebugString("CPU序列号:");
	OutputDebugString(MyCpuID);

	//DWORD ser;
	//TCHAR namebuf[128];
	//TCHAR filebuf[128];
	//::GetVolumeInformation(_T("c:\\"),namebuf,128,&ser,0,0,filebuf,128);
	CString strdisk;
	//strdisk.Format(_T("%08X"),ser);

	CGetHDSerial HDSerial;  // 创建实例 
	strdisk.Format("%s", HDSerial.GetHDSerial()); // 得到硬盘序列号
	OutputDebugString("硬盘序列号:");

	OutputDebugString(strdisk);
	CString strmachine;
	strmachine=MyCpuID.Mid(3,5); //从MyCpuID的第3位开始取5个
	strmachine+=strdisk.Mid(3,5);//从strdisk的第3位开始取5个,合并生成机器码

	CString code[16]={_T("ad"),_T("eh"),_T("im"),_T("np"),_T("ru"),_T("vy"),_T("zc"),_T("gk"),
		_T("pt"),_T("xb"),_T("fj"),_T("ox"),_T("wa"),_T("ei"),_T("nr"),_T("qu")};
	CString reg,stred;
	int num;

	stred = strmachine ;
	stred.MakeLower();
	for(int i=0;i<10;i++)
	{
		char p=stred.GetAt(i);
		if(p>='a'&&p<='f')
			num=p-'a'+10;
		else
			num=p-'0';
		CString tmp=code[num];
		reg+=tmp;
	}
	reg.MakeUpper();
	m_strMachineCode = reg;
	return m_strMachineCode;

}
void CSerialManager::rc5_destroy(rc5_ctx *c)
{
	int i;
	for(i=0;i<(c->nr)*2+2;i++)
		c->xk[i]=0;
	free(c->xk);
}

void CSerialManager::rc5_init(rc5_ctx *c, int rounds)
{
	c->nr=rounds;
	c->xk=(DWORD *)malloc(4*(rounds*2+2));
}

BOOL CSerialManager::rc5_key(rc5_ctx *c, BYTE *key, int keylen)
{
	DWORD pk[10],A,B;
	int xk_len,pk_len,i,num_steps,rc;
	DWORD *cp;
	xk_len=c->nr*2+2;
	pk_len=keylen/4;
	if((keylen%4)!=0) 
		pk_len+=1;
	//pk=(DWORD *)malloc(pk_len * 4);
	//if(pk==NULL)
	//	return(false);
	for(i=0;i<pk_len;i++)
		pk[i]=0;
	cp=&pk[0];
	for(i=0;i<keylen;i++)
		cp[i]=key[i];
	c->xk[0]=0xb7e15163;
	for(i=1;i<xk_len;i++)
		c->xk[i]=c->xk[i-1]+0x9e3779b9;
	A=B=0;
	for(i=0;i<xk_len;i++)
	{
		A=A + c->xk[i];
		B=B ^ c->xk[i];
	};
	if(pk_len>xk_len) 
		num_steps=3*pk_len;
	else
		num_steps=3*xk_len;
	A=B=0;
	for(i=0;i<num_steps;i++)
	{
		A=c->xk[i % xk_len]=ROTL32(c->xk[i % xk_len] + A + B,3);
		rc=(A+B) & 31;
		B=pk[i % pk_len]=ROTL32(pk[i % pk_len]+ A + B,rc);
	};
	for(i=0;i<pk_len;i++)
		pk[i]=0;
	//delete pk;
	//	free(pk);
	return(true);
}

void CSerialManager::rc5_encrypt(rc5_ctx *c, DWORD *data, int blocks)
{
	DWORD *d,*sk;
	int h,i,rc;
	d=data;
	sk=(c->xk)+2;
	for(h=0;h<blocks;h++)
	{
		d[0]+=c->xk[0];
		d[1]+=c->xk[1];
		for(i=0;i<c->nr*2;i+=2)
		{
			d[0]^=d[1];
			rc=d[1] & 31;
			d[0]=ROTL32(d[1],rc);
			d[0]+=sk[i];
			d[1]^=d[0];
			rc=d[0] & 31;
			d[1]=ROTL32(d[1],rc);
			d[1]+=sk[i+1];
		};
		d+=2;
	}
}

int CSerialManager::seal_g(BYTE *in, int i,DWORD *h)
{
	DWORD h0;
	DWORD h1;
	DWORD h2;
	DWORD h3;
	DWORD h4;
	DWORD a;
	DWORD b;
	DWORD c;
	DWORD d;
	DWORD e;
	BYTE *kp;
	DWORD w[80];
	DWORD temp;
	kp=in;
	h0=WORD(kp);
	kp+=4;
	h1=WORD(kp);
	kp+=4;
	h2=WORD(kp);
	kp+=4;
	h3=WORD(kp);
	kp+=4;
	h4=WORD(kp);
	kp+=4;
	w[0]=i;
	for(i=1;i<16;i++)
		w[i]=0;
	for(i=16;i<80;i++)
		w[i]=w[i-3]^w[i-8]^w[i-14]^w[i-16];
	a=h0;
	b=h1;
	c=h2;
	d=h3;
	e=h4;
	for(i=0;i<20;i++)
	{
		temp=ROT27(a) + F1(b,c,d) + e + w[i] + 0x5a827999;
		e=d;
		d=c;
		c=ROT2(b);
		b=a;
		a=temp;
	};
	for(i=20;i<40;i++)
	{
		temp=ROT27(a)+F2(b,c,d)+e+w[i]+0x6ed9eba1;
		e=d;
		d=c;
		c=ROT2(b);
		b=a;
		a=temp;
	};
	for(i=40;i<60;i++)
	{
		temp=ROT27(a)+F3(b,c,d)+e+w[i]+0x8f1bbcdc;
		d=c;
		c=ROT2(b);
		b=a;
		a=temp;
	};
	for(i=60;i<80;i++)
	{
		temp=ROT27(a)+F4(b,c,d)+e+w[i]+0xca62c1d6;
		e=d;
		d=c;
		c=ROT2(b);
		b=a;
		a=temp;
	};
	h[0]=h0+a;
	h[1]=h1+b;
	h[2]=h2+c;
	h[3]=h3+d;
	h[4]=h4+e;
	return(ALG_OK);
}

DWORD CSerialManager::seal_gamma(BYTE *a, int i)
{
	DWORD h[5];
	(void) seal_g(a,i/5,h);
	return h[i % 5];
}

int CSerialManager::seal_init(seal_ctx *result, BYTE *key)
{
	int i;
	DWORD h[5];
	for(i=0;i<510;i+=5)
		seal_g(key,i/5,&(result->t[i]));
	seal_g(key,510/5,h);
	for(i=510;i<512;i++)
		result->t[i]=h[i-510];
	seal_g(key,(-1+0x1000)/5,h);
	for(i=0;i<4;i++)
		result->s[i]=h[i+1];
	for(i=4;i<254;i+=5)
		seal_g(key,(i+0x1000)/5,&(result->s[i]));
	seal_g(key,(254+0x1000)/5,h);
	for(i=254;i<256;i++)
		result->s[i]=h[i-254];
	seal_g(key,(-2+0x2000)/5,h);
	for(i=0;i<3;i++)
		result->r[i]=h[i+2];
	for(i=3;i<13;i+=5)
		seal_g(key,(i+0x2000)/5,&(result->r[i]));
	seal_g(key,(13+0x2000)/5,h);
	for(i=13;i<16;i++)
		result->r[i]=h[i-13];
	return(ALG_OK);
}

int CSerialManager::seal(seal_ctx *key, DWORD in, DWORD *out)
{
	int i;
	int j;
	int l;
	DWORD a;
	DWORD b;
	DWORD c;
	DWORD d;
	WORD p;
	WORD q;
	DWORD n1;
	DWORD n2;
	DWORD n3;
	DWORD n4;
	DWORD *wp;
	wp=out;
	for(l=0;l<4;l++)
	{
		a=in^key->r[4*l];
		b=ROT8(in)^key->r[4*l+1];
		c=ROT16(in)^key->r[4*l+2];
		d=ROT24(in)^key->r[4*l+3];
		for(j=0;j<2;j++)
		{
			p=(WORD)(a & 0x000007fc);
			b+=key->t[p/4];
			a=ROT9(a);

			p=(WORD)(b & 0x000007fc);
			c+=key->t[p/4];
			b=ROT9(b);

			p=(WORD)(c & 0x000007fc);
			d+=key->t[p/4];
			c=ROT9(c);

			p=(WORD)(d & 0x000007fc);
			a+=key->t[p/4];
			d=ROT9(d);
		};
		n1=d;
		n2=b;
		n3=a;
		n4=c;

		p=(WORD)(a & 0x000007fc);
		b+=key->t[p/4];
		a=ROT9(a);

		p=(WORD)(b & 0x000007fc);
		c+=key->t[p/4];
		b=ROT9(b);

		p=(WORD)(c & 0x000007fc);
		d+=key->t[p/4];
		c=ROT9(c);

		p=(WORD)(d & 0x000007fc);
		a+=key->t[p/4];
		d=ROT9(d);

		for(i=0;i<64;i++)
		{
			p=(WORD)(a & 0x000007fc);
			b+=key->t[p/4] ;
			a=ROT9(a);
			b^=a;

			q=(WORD)(b & 0x000007fc);
			c^=key->t[q/4] ;
			b=ROT9(b);
			c+=b;

			p=(WORD)((p+c) & 0x000007fc);
			d+=key->t[p/4] ;
			c=ROT9(c);
			d^=c;

			q=(WORD)((q+d) & 0x000007fc);
			a^=key->t[q/4] ;
			d=ROT9(d);
			a+=d;

			p=(WORD)((p+a) & 0x000007fc);
			b^=key->t[q/4] ;
			a=ROT9(a);

			q=(WORD)((q+b) & 0x000007fc);
			c+=key->t[p/4] ;
			b=ROT9(b);

			p=(WORD)((p+c) & 0x000007fc);
			d^=key->t[q/4] ;
			c=ROT9(c);

			q=(WORD)((q+b) & 0x000007fc);
			a+=key->t[p/4] ;
			d=ROT9(d);

			*wp=b+key->s[4*i];
			wp++;
			*wp=c^key->s[4*i+1];
			wp++;
			*wp=d+key->s[4*i+2];
			wp++;
			*wp=a^key->s[4*i+3];
			wp++;

			if(i & 1)
			{
				a+=n3;
				c+=n4;
			}
			else
			{
				a+=n1;
				c+=n2;
			}
		}
	}
	return(ALG_OK);			
}

void CSerialManager::seal_refill_buffer(seal_ctx *c)
{
	seal(c,c->counter,c->ks_buf);
	c->counter++;
	c->ks_pos=0;
}

void CSerialManager::seal_key(seal_ctx *c, BYTE *key)
{
	seal_init(c,key);
	c->counter=0;
	c->ks_pos=WORDS_PER_SEAL_CALL;
}

void CSerialManager::seal_encrypt(seal_ctx *c, DWORD *data_ptr, int w)
{
	int i;
	for(i=0;i<w;i++)
	{
		if(c->ks_pos>WORDS_PER_SEAL_CALL) seal_refill_buffer(c);
		data_ptr[i]^=c->ks_buf[c->ks_pos];
		c->ks_pos++;
	}
}

void CSerialManager::seal_decrypt(seal_ctx *c, DWORD *data_ptr, int w)
{
	seal_encrypt(c,data_ptr,w);
}

void CSerialManager::seal_resynch(seal_ctx *c, DWORD synch_word)
{
	c->counter=synch_word;
	c->ks_pos=WORDS_PER_SEAL_CALL;
}


int CSerialManager::GetFinalData(BYTE cardnum[], BYTE finaldata[],int narcardSize,int narfinalSize)
{
	des_ctx* dc=new des_ctx;
	BYTE *pdata;
	BYTE key[8]={0x84,0x79,0x06,0x91,0x26,0x8c,0x1a,0x3f};
	BYTE *pkey;
	BYTE data[24];
	DWORD *pldata,ldata[12];
	int nSize=24;
	int len,i;
	len=narcardSize;
	if(len>24) return(-1);
	if(narfinalSize!=20) return(-2);
	///////////////////////des///////////////////////////
	CardNumToData(cardnum,data,narcardSize);
	pdata=&data[0];
	pkey=&key[0];
	DesEnc(dc,pkey,pdata,nSize/8);
	delete dc;
	i=1;
	int j;
	j=nSize/2;
	while(i<(nSize/2))
	{
		BYTE temp;
		temp=data[i];
		data[i]=data[j];
		data[j]=temp;
		i+=2;
		j+=2;
	};
	pdata=&data[0];
	BYTE rckey[5]={0x49,0x3c,0x91,0xa5,0xf0};
	BYTE *prckey;
	int rcSize,size;
	prckey=&rckey[0];
	rc5_ctx *c=new rc5_ctx;
	CharToLong(data,24,ldata,rcSize);
	pldata=&ldata[0];
	Rc5Enc(c,15,prckey,5,pldata, rcSize/2);
	delete c;
	/////////////////////////////////////////seal///////////////////////
	seal_ctx* sc=new seal_ctx;
	BYTE sealkey[20]={
		0xa5,0x61,0x8f,0x0d,0x63,0x9e,0xfa,0x22,0x1c,0x48,
		0xe3,0x57,0x04,0x73,0xde,0x8b,0xf4,0x92,0x6a,0x9c
	};
	BYTE *psealkey;
	nSize=12;
	j=nSize/2;
	while(i<(nSize/2))
	{
		BYTE temp;
		temp=data[i];
		data[i]=data[j];
		data[j]=temp;
		i+=2;
		j+=2;
	};
	pldata=&ldata[0];
	psealkey=&sealkey[0];
	SealEnc(sc,sealkey,pldata,nSize);
	delete sc;
	DWORD fdata[10];
	i=0;
	for(i=0;i<10;i++)
		fdata[i]=ldata[i];
	fdata[2]=ldata[2] ^ ldata[7];
	fdata[4]=ldata[4] ^ ldata[9];
	fdata[7]=ldata[8];
	fdata[8]=ldata[10];
	fdata[9]=ldata[11];
	BYTE sdata[20];
	LongToChar(fdata,10,sdata,size);
	DataToString(sdata,finaldata,size,narfinalSize);
	return(1);
}

int CSerialManager::GetFinalData(CString cardnum, CString &finaldata)
{
	des_ctx* dc=new des_ctx;
	BYTE *pdata;
	BYTE key[8]={0x84,0x79,0x06,0x91,0x26,0x8c,0x1a,0x3f};
	BYTE *pkey;
	BYTE data[24];
	DWORD *pldata,ldata[12];
	int nSize=24;
	int len,i;
	len=cardnum.GetLength();
	if(len>24) return(-1);
	///////////////////////des///////////////////////////
	CardNumToData(cardnum,data);
	pdata=&data[0];
	pkey=&key[0];
	DesEnc(dc,pkey,pdata,nSize/8);
	delete dc;
	i=1;
	int j;
	j=nSize/2;
	while(i<(nSize/2))
	{
		BYTE temp;
		temp=data[i];
		data[i]=data[j];
		data[j]=temp;
		i+=2;
		j+=2;
	};
	pdata=&data[0];
	BYTE rckey[5]={0x49,0x3c,0x91,0xa5,0xf0};
	BYTE *prckey;
	int rcSize,size;
	prckey=&rckey[0];
	rc5_ctx *c=new rc5_ctx;
	CharToLong(data,24,ldata,rcSize);
	pldata=&ldata[0];
	Rc5Enc(c,15,prckey,5,pldata, rcSize/2);
	delete c;
	/////////////////////////////////////////seal///////////////////////
	seal_ctx* sc=new seal_ctx;
	BYTE sealkey[20]={
		0xa5,0x61,0x8f,0x0d,0x63,0x9e,0xfa,0x22,0x1c,0x48,
		0xe3,0x57,0x04,0x73,0xde,0x8b,0xf4,0x92,0x6a,0x9c
	};
	BYTE *psealkey;
	nSize=12;
	j=nSize/2;
	while(i<(nSize/2))
	{
		BYTE temp;
		temp=data[i];
		data[i]=data[j];
		data[j]=temp;
		i+=2;
		j+=2;
	};
	pldata=&ldata[0];
	psealkey=&sealkey[0];
	SealEnc(sc,sealkey,pldata,nSize); 
	delete sc;
	DWORD fdata[10];
	i=0;
	for(i=0;i<10;i++)
		fdata[i]=ldata[i];
	fdata[2]=ldata[2] ^ ldata[7];
	fdata[4]=ldata[4] ^ ldata[9];
	fdata[7]=ldata[8];
	fdata[8]=ldata[10];
	fdata[9]=ldata[11];
	BYTE sdata[20];
	LongToChar(fdata,10,sdata,size);
	finaldata.Empty();
	DataToString(sdata,finaldata,size);
	return(1);
}


void CSerialManager::CharToLong(BYTE olddata[],int oldSize,DWORD newdata[],int &newSize)
{
	int i;
	DWORD temp,data1,data2;
	for(i=0;i<oldSize;i+=2)
	{
		temp=0;
		data1=0;
		data2=0;
		data1=olddata[i+1];
		data2=((DWORD)((DWORD)olddata[i] & 0x0000ffffL))  <<  16;
		data2=data2  &  0xffff0000L;
		temp=data1  |  data2;
		newdata[i/2]=temp;
	};
	newSize=oldSize/2;
}

void CSerialManager::LongToChar(DWORD olddata[], int oldSize, BYTE newdata[], int &newSize)
{
	int i;
	DWORD temp,data1,data2,tempdata;
	for(i=0;i<oldSize;i++)
	{
		temp=0;
		data1=0;
		data2=0;
		temp=olddata[i];
		data2=temp  &  0x0000ffffL;
		tempdata=temp  >>  16;
		data1=tempdata  &  0x0000ffffL;
		newdata[i*2]=(BYTE)data1;
		newdata[i*2+1]=(BYTE)data2;
	}
	newSize=oldSize*2;
}


void CSerialManager::DataToString(BYTE data[], BYTE finaldata[], int dataSize, int finalSize)
{
	BYTE stringdata[36]={
		0x41, 0x59, 0x34, 0x43, 0x57, 0x36, 0x45, 0x55, 0x33, 0x47, 
		0x53, 0x37, 0x49, 0x51, 0x32, 0x4b, 0x4f, 0x4d,	0x38, 0x4e,
		0x4c, 0x50, 0x31, 0x4a, 0x52, 0x48, 0x39, 0x54, 0x46, 0x56,
		0x44, 0x35, 0x58, 0x42, 0x30, 0x5a
	};
	BYTE chardataindex[24];
	//BYTE chardata[24];
	//int charSize;
	FLOAT numsum,charsum;
	numsum=256.0;
	charsum=36.0;
	//LongToChar(data,nSize,chardata,charSize);
	//	BYTE n=(BYTE)((charsum * 255)/numsum);
	int i;
	for(i=0;i<dataSize;i++)
		chardataindex[i]=(BYTE)((charsum * data[i])/numsum);
	for(i=0;i<dataSize;i++)
		finaldata[i]=stringdata[chardataindex[i]];
}

void CSerialManager::DataToString(BYTE data[], CString &finaldata,int nSize)
{
	BYTE stringdata[36]={
		0x41, 0x59, 0x34, 0x43, 0x57, 0x36, 0x45, 0x55, 0x33, 0x47, 
		0x53, 0x37, 0x49, 0x51, 0x32, 0x4b, 0x4f, 0x4d,	0x38, 0x4e,
		0x4c, 0x50, 0x31, 0x4a, 0x52, 0x48, 0x39, 0x54, 0x46, 0x56,
		0x44, 0x35, 0x58, 0x42, 0x30, 0x5a
	};
	BYTE chardataindex[24];
	//BYTE chardata[24];
	//int charSize;
	FLOAT numsum,charsum;
	numsum=256.0;
	charsum=36.0;
	//LongToChar(data,nSize,chardata,charSize);
	//	BYTE n=(BYTE)((charsum * 255)/numsum);
	int i;
	for(i=0;i<nSize;i++)
		chardataindex[i]=(BYTE)((charsum * data[i])/numsum);
	for(i=0;i<nSize;i++)
		finaldata.Insert(i+1,stringdata[chardataindex[i]]);
}


void CSerialManager::CardNumToData(BYTE cardnum[], BYTE data[], int narcardSize, int ndataSize)
{
	int len,i,j;
	len=narcardSize;
	for(i=0;i<len;i++)
		data[i]=cardnum[i];
	i=len;
	j=0;
	while(i<ndataSize)
	{
		data[i]=data[j] ^ data[j+1];
		i++;
		j++;
	};
	i=1;
	j=ndataSize/2;
	while(i<(ndataSize/2))
	{
		BYTE temp;
		temp=data[i];
		data[i]=data[j];
		data[j]=temp;
		i+=2;
		j+=2;
	};
}

void CSerialManager::CardNumToData(CString cardnum, BYTE data[], int nSize)
{
	int len,i,j;
	len=cardnum.GetLength();
	for(i=0;i<len;i++)
		data[i]=cardnum.GetAt(i);
	i=len;
	j=0;
	while(i<nSize)
	{
		data[i]=data[j] ^ data[j+1];
		i++;
		j++;
	};
	i=1;
	j=nSize/2;
	while(i<(nSize/2))
	{
		BYTE temp;
		temp=data[i];
		data[i]=data[j];
		data[j]=temp;
		i+=2;
		j+=2;
	};
}

void CSerialManager::des_deskey(BYTE *key, WORD edf)
{
	register int i, j, l, m, n;
	BYTE pclm[56], pcr[56];
	DWORD kn[32];
	for( j = 0; j < 56; j++ ) 
	{
		l= pc1[j];
		m=l & 0x07;
		pclm[j]= (key[l >> 3] & bytebit[m]) ? 1 : 0;
	}
	for( i = 0; i < 16; i++ ) 
	{
		if( edf== DE1 ) 
			m= (15 - i) << 1;
		else 
			m= i << 1;
		n = m + 1;
		kn[m] = kn[n] = 0L;
		for( j = 0; j < 28; j++ ) 
		{
			l=  j + totrot[i];                                 
			if( l < 28 ) 
				pcr[j] = pclm[l];                         
			else 
				pcr[j] = pclm[l - 28];
		}
		for( j = 28; j < 56; j++ ) 
		{
			l=  j + totrot[i];
			if( l < 56 ) 
				pcr[j] = pclm[l];
			else 
				pcr[j] = pclm[l - 28];
		}
		for( j = 0; j < 24; j++ )
		{
			if( pcr[pc2[j]] ) 
				kn[m] |= bigbyte[j];
			if( pcr[pc2[j+24]] ) 
				kn[n] |= bigbyte[j];
		}
	}
	des_cookey(kn);
	return;
}

void CSerialManager::des_cookey(DWORD *raw1)
{
	register DWORD *cook, *raw0;
	DWORD dough[32];
	register int i;
	cook=dough;
	for(i=0; i < 16;i++)  
	{
		raw0 = raw1;
		raw1++;
		*cook    = (*raw0 & 0x00fc0000L) <<  6;
		*cook   |= (*raw0 & 0x00000fc0L) << 10;
		*cook   |= (*raw1 & 0x00fc0000L) >> 10;
		*cook++ |= (*raw1 & 0x00000fc0L) >>  6;
		*cook    = (*raw0 & 0x0003f000L) << 12;
		*cook   |= (*raw0 & 0x0000003fL) << 16;
		*cook   |= (*raw1 & 0x0003f000L) >>  4;
		*cook++ |= (*raw1 & 0x0000003fL);
	};
	des_usekey(dough);
	return;
}

void CSerialManager::des_cpkey(DWORD into[])
{
	register DWORD *from,*endp;
	int i=0;
	from = KnL;
	endp = &KnL[31];
	while( from < endp ) 
		into[i++] = *from++;
	into[i]=*from;
	return;
}

void CSerialManager::des_usekey(DWORD *from)
{
	register DWORD *to, *endp;
	//int i=0;
	to = KnL;
	endp = &KnL[31];
	while( to < endp ) 
		*to++ = *from++;
	*to=*from;
	from++;
	return;
}

void CSerialManager::des_des(BYTE *inblock, BYTE *outblock)
{
	DWORD work[2];
	des_scrunch(inblock, work);
	des_desfunc(work, KnL);
	des_unscrun(work, outblock);
	return;
}

void CSerialManager::des_scrunch(BYTE *outof, DWORD *into)
{
	*into    = (*outof++ & 0xffL) << 24;
	*into   |= (*outof++ & 0xffL) << 16;
	*into   |= (*outof++ & 0xffL) <<  8;
	*into++ |= (*outof++ & 0xffL);
	*into    = (*outof++ & 0xffL) << 24;
	*into   |= (*outof++ & 0xffL) << 16;
	*into   |= (*outof++ & 0xffL) <<  8;
	*into   |= (*outof   & 0xffL);
	return;
}

void CSerialManager::des_unscrun(DWORD *outof, BYTE *into)
{
	*into++ = (BYTE)((*outof >> 24) & 0x000000ffL);
	*into++ = (BYTE)((*outof >> 16) & 0x000000ffL);
	*into++ = (BYTE)((*outof >>  8) & 0x000000ffL);
	*into++ = (BYTE)( *outof++      & 0x000000ffL);
	*into++ = (BYTE)((*outof >> 24) & 0x000000ffL);
	*into++ = (BYTE)((*outof >> 16) & 0x000000ffL);
	*into++ = (BYTE)((*outof >>  8) & 0x000000ffL);
	*into   = (BYTE)( *outof        & 0x000000ffL);
	return;
}

void CSerialManager::des_desfunc(DWORD *block, DWORD *keys)
{
	register DWORD fval, work,right,leftt;
	register int round;
	leftt = block[0];      
	right = block[1];
	work = ((leftt >> 4) ^ right) & 0x0f0f0f0fL;
	right ^= work;
	leftt ^= (work << 4);
	work = ((leftt >> 16) ^ right) & 0x0000ffffL;
	right ^= work;
	leftt ^= (work << 16);
	work = ((right >> 2) ^ leftt) & 0x33333333L;
	leftt ^= work;
	right ^= (work << 2);
	work = ((right >> 8) ^ leftt) & 0x00ff00ffL;
	leftt ^= work;
	right ^= (work << 8);
	right = ((right << 1) | ((right >> 31) & 1L)) & 0xffffffffL;
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = ((leftt << 1) | ((leftt >> 31) & 1L)) & 0xffffffffL;
	for( round = 0; round < 8; round++ )
	{
		work = (right << 28) | (right >> 4);
		work ^= *keys++;
		fval  = SP7[ work        & 0x3fL];
		fval |= SP5[(work >>  8) & 0x3fL];
		fval |= SP3[(work >> 16) & 0x3fL];
		fval |= SP1[(work >> 24) & 0x3fL];
		work = right ^ *keys++;
		fval |= SP8[ work        & 0x3fL];
		fval |= SP6[(work >> 8)  & 0x3fL];
		fval |= SP4[(work >> 16) & 0x3fL];
		fval |= SP2[(work >> 24) & 0x3fL];
		leftt ^= fval;
		work = (leftt << 28) | (leftt >> 4);
		work ^= *keys++;
		fval  = SP7[ work        & 0x3fL];
		fval |= SP5[(work >> 8)  & 0x3fL];
		fval |= SP3[(work >> 16) & 0x3fL];
		fval |= SP1[(work >> 24) & 0x3fL];
		work  = leftt ^ *keys++;
		fval |= SP8[ work        & 0x3fL];
		fval |= SP6[(work >>  8) & 0x3fL];
		fval |= SP4[(work >> 16) & 0x3fL];
		fval |= SP2[(work >> 24) & 0x3fL];
		right ^= fval;
	}
	right = (right << 31) | (right >> 1);
	work = (leftt ^ right) & 0xaaaaaaaaL;
	leftt ^= work;
	right ^= work;
	leftt = (leftt << 31)  | (leftt >> 1);
	work = ((leftt >> 8) ^ right) & 0x00ff00ffL; 
	right ^= work;
	leftt ^= (work << 8);
	work = ((leftt >> 2) ^ right) & 0x33333333L;
	right ^= work;
	leftt ^= (work << 2);
	work = ((right >> 16) ^ leftt)& 0x0000ffffL;
	leftt ^= work;
	right ^= (work << 16);
	work = ((right >> 4) ^ leftt) & 0x0f0f0f0fL;
	leftt ^= work;
	right ^= (work << 4);
	*block++ = right;
	*block = leftt;
	return;
}

void CSerialManager::des_key(des_ctx *dc, BYTE *key)
{
	des_deskey(key,ENO);
	des_cpkey(dc->ek);
	//des_deskey(key,DE1);
	//des_cpkey(dc->dk);  
}

void CSerialManager::des_enc(des_ctx *dc, BYTE *data, int blocks)
{
	DWORD work[2];
	int i;
	BYTE *cp;
	cp = data;
	for(i=0;i<blocks;i++)
	{
		des_scrunch(cp,work);
		des_desfunc(work,dc->ek);
		des_unscrun(work,cp);
		cp+=8;
	}
}

void CSerialManager::des_dec(des_ctx *dc, BYTE *data, int blocks)
{
	DWORD work[2];
	int i;
	BYTE *cp;
	cp = data;
	for(i=0;i<blocks;i++)
	{
		des_scrunch(cp,work);
		des_desfunc(work,dc->dk);
		des_unscrun(work,cp);
		cp+=8;
	}
}

void CSerialManager::DesEnc(des_ctx *dc, BYTE *key, BYTE *data,int blocks)
{
	des_key(dc,key);
	des_enc(dc,data,blocks);
}

void CSerialManager::Rc5Enc(rc5_ctx *c, int rounds, BYTE *key, int keylen, DWORD *data, int blocks)
{
	rc5_init(c,rounds);
	rc5_key(c,key,keylen);
	rc5_encrypt(c,data,blocks);
	rc5_destroy(c);
}

void CSerialManager::SealEnc(seal_ctx *sc, BYTE *key, DWORD *data, int blocks)
{
	seal_key(sc,key);
	seal_encrypt(sc,data,blocks);
}
