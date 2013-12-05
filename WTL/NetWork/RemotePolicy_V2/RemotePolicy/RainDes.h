#pragma once
#include <afxwin.h>
class CRainDes
{
public:
	CRainDes(void);
	~CRainDes(void);
private:
	BYTE m_ip_table[64];
	BYTE m_inv_ip_table[64];
	BYTE m_exp_table[48];
	BYTE m_perm_table[48];
	BYTE m_perm_choice1_C[28];
	BYTE m_perm_choice1_D[28];
	BYTE m_perm_choice2[48];
	BYTE m_shifts_table[16];
	BYTE m_sel_table[512];
	BYTE m_bitmap[64];
	BYTE m_perm_keys[16][6];
	BYTE m_key[8];
private:
	void InitializeMembers();
	void cipher(BYTE* key_ptr, BYTE* input, BYTE* output, BYTE mode);
	void gen_keys(void);
	void rotate(BYTE* ptr, BYTE times);
	void function_rk(BYTE* input, BYTE* output, BYTE* key);
	void do_ip(BYTE* input, BYTE* output, BYTE* table);
public:
	void DesEnCrypt(BYTE *pIn,BYTE *pOut,BYTE *pPass, int nInLen, int nOutLen);
	void DesDeCrypt(BYTE *pIn,BYTE *pOut,BYTE *pPass, int nInLen, int nOutLen);


};
