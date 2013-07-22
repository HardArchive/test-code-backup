//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*大数运算库头文件：CBigNumber.h

*****************************************************************/

//允许生成超过位（二进制）的中间结果
#ifndef BI_MAXLEN
#define BI_MAXLEN 64

class CBigNumber
{
public:
	//大数在x100000000进制下的长度   
	unsigned m_nLength;
	//用数组记录大数在x100000000进制下每一位的值
	unsigned long m_ulValue[BI_MAXLEN];

	CBigNumber();
	~CBigNumber();

	/*****************************************************************
	基本操作与运算
	Mov，赋值运算，可赋值为大数或普通整数
	Cmp，比较运算，比较两个大数或两个普通整数的大小
	Add，加，求大数与大数或大数与普通整数的和
	Sub，减，求大数与大数或大数与普通整数的差
	Mul，乘，求大数与大数或大数与普通整数的积
	Div，除，求大数与大数或大数与普通整数的商
	Mod，模，求大数与大数或大数与普通整数的模
	Pow，乘方，求大数与普通整数的乘方
	*****************************************************************/
	void Mov(unsigned __int64 A);
	void Mov(CBigNumber A);
	CBigNumber Add(CBigNumber A);
	CBigNumber Sub(CBigNumber A);
	CBigNumber Mul(CBigNumber A);
	CBigNumber Div(CBigNumber A);
	CBigNumber Mod(CBigNumber A);
	CBigNumber Add(unsigned long A);
	CBigNumber Sub(unsigned long A);
	CBigNumber Mul(unsigned long A);
	CBigNumber Div(unsigned long A);
	unsigned long Mod(unsigned long A); 
	int Cmp(CBigNumber A); 
	CBigNumber Pow(unsigned long A);

	/*****************************************************************
	输入输出
	Get，从字符串按进制或进制格式输入到大数
	Put，将大数按进制或进制格式输出到字符串
	*****************************************************************/
	// 	void Get(unsigned char *str, UINT  len);   
	CBigNumber Get(unsigned char *str, unsigned long  len);
	void Put10(CString &str);
	void Put256(CString &str);
	void Put16(CString &str);
	void Putchar(CString &str);


	/*****************************************************************
	RSA相关运算
	Rab，拉宾米勒算法进行素数测试
	Euc，欧几里德算法求解同余方程
	Mon，蒙格马利法进行幂模运算
	GetPrime，产生指定长度的随机大素数
	*****************************************************************/
	int Rab();
	CBigNumber Euc(CBigNumber A);

	//CBigNumber RsaTrans(CBigNumber A, CBigNumber B);
	CBigNumber Mon(CBigNumber A, CBigNumber B);
	void GetPrime(int bits);
};
#endif
