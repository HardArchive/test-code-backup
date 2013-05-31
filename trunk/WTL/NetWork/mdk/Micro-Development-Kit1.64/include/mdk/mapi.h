// mapi.h: interface for the mapi class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_MAPI_H
#define MDK_MAPI_H

#include <string>
#include "FixLengthInt.h"

namespace mdk
{
	//˯��
	void m_sleep( long lMillSecond );
	//��ַ���浽int64
	bool addrToI64(uint64 &addr64, const char* ip, int port);
	//int64��������ַ
	void i64ToAddr(char* ip, int &port, uint64 addr64);
	//ѹ��del�������ַ�
	void TrimString( std::string &str, std::string del );
	//ѹ����del�������ַ�
	void TrimStringLeft( std::string &str, std::string del );
	//ѹ����del�������ַ�
	void TrimStringRight( std::string &str, std::string del );
	//ѹ���հ��ַ�
	char* Trim( char *str );
	//ѹ���ҿհ��ַ�
	char* TrimRight( char *str );
	//ѹ����հ��ַ�
	char* TrimLeft( char *str );
	//�ֽڸߵ�λ˳��ת
	int reversal(int i);
}

#endif // !defined MDK_MAPI_H 
