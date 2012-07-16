// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// PhoneServer.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������
// ���ӵ���ģʽ��־
BOOL g_bRunInDebugMode = false;

void TracePrint(int nLogLevel, LPCTSTR lpstrTrace,... )
{
	if (0 == (nLogLevel & LOG_DEFAULT_LEVEL)) return;

	va_list args;
	int len;
	LPTSTR buffer;

	va_start( args, lpstrTrace );

	try
	{
		// _vscprintf doesn't count // terminating '\0'
		len = _vsctprintf( lpstrTrace, args ) + 1; 

		buffer = (LPTSTR)malloc( len * sizeof(LPTSTR) );
		_vstprintf( buffer, lpstrTrace, args );

		// �����޸� 2010-08-03 ����ģʽʱֱ�������������̨
		if (g_bRunInDebugMode) _tprintf(buffer);
		else OutputDebugString(buffer);

		free(buffer);
	}
	catch (...)
	{
	}
}