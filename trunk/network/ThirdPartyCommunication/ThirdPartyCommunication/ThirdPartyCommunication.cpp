// ThirdPartyCommunication.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "ThirdPartyCommunication.h"


// ���ǵ���������һ��ʾ��
THIRDPARTYCOMMUNICATION_API int nThirdPartyCommunication=0;

// ���ǵ���������һ��ʾ����
THIRDPARTYCOMMUNICATION_API int fnThirdPartyCommunication(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� ThirdPartyCommunication.h
CThirdPartyCommunication::CThirdPartyCommunication()
{
	return;
}

