//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*���������ͷ�ļ���CBigNumber.h

*****************************************************************/

//�������ɳ���λ�������ƣ����м���
#ifndef BI_MAXLEN
#define BI_MAXLEN 64

class CBigNumber
{
public:
	//������x100000000�����µĳ���   
	unsigned m_nLength;
	//�������¼������x100000000������ÿһλ��ֵ
	unsigned long m_ulValue[BI_MAXLEN];

	CBigNumber();
	~CBigNumber();

	/*****************************************************************
	��������������
	Mov����ֵ���㣬�ɸ�ֵΪ��������ͨ����
	Cmp���Ƚ����㣬�Ƚ�����������������ͨ�����Ĵ�С
	Add���ӣ��������������������ͨ�����ĺ�
	Sub�������������������������ͨ�����Ĳ�
	Mul���ˣ��������������������ͨ�����Ļ�
	Div�������������������������ͨ��������
	Mod��ģ���������������������ͨ������ģ
	Pow���˷������������ͨ�����ĳ˷�
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
	�������
	Get�����ַ��������ƻ���Ƹ�ʽ���뵽����
	Put�������������ƻ���Ƹ�ʽ������ַ���
	*****************************************************************/
	// 	void Get(unsigned char *str, UINT  len);   
	CBigNumber Get(unsigned char *str, unsigned long  len);
	void Put10(CString &str);
	void Put256(CString &str);
	void Put16(CString &str);
	void Putchar(CString &str);


	/*****************************************************************
	RSA�������
	Rab�����������㷨������������
	Euc��ŷ������㷨���ͬ�෽��
	Mon���ɸ�������������ģ����
	GetPrime������ָ�����ȵ����������
	*****************************************************************/
	int Rab();
	CBigNumber Euc(CBigNumber A);

	//CBigNumber RsaTrans(CBigNumber A, CBigNumber B);
	CBigNumber Mon(CBigNumber A, CBigNumber B);
	void GetPrime(int bits);
};
#endif
