// HwMCI.h: interface for the CHwMCI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HWMCI_H__2B2C2C5E_6A30_46E4_8396_E1E7F1CBB511__INCLUDED_)
#define AFX_HWMCI_H__2B2C2C5E_6A30_46E4_8396_E1E7F1CBB511__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "mmsystem.h"

class CHwMCI  
{
public:
	BOOL DeviceIDValid ();
	BOOL Rec ( int nChannels=2,WORD nSamplesPerSec=11025,int wBitsPerSample=16 );
	LPSTR GetErrorStr();
	BOOL Save ( CString Filename );
	BOOL Pause();
	BOOL Stop();
	BOOL Play ( DWORD dwFrom=0, DWORD dwTo=0 );
	void Close();
	BOOL Open ( LPCTSTR lpszWaveFileName=NULL );
	BOOL SetPara (
		WORD        wFormatTag,			/* format type */
		WORD        nChannels,			/* ��¼������ͨ���� */
		DWORD       nSamplesPerSec,		/* ��¼ÿ��ȡ���� */
		DWORD       nAvgBytesPerSec,	/* ��¼ÿ��������� */
		WORD        nBlockAlign,		/* ��¼����Ķ��뵥λ */
		WORD        wBitsPerSample		/* ��¼ÿ��ȡ�������λԪ�� */
	);
	CHwMCI();
	virtual ~CHwMCI();

private:
	MCIERROR hwMciSendCommand(
		MCIDEVICEID IDDevice, 
		UINT uMsg,             
		DWORD fdwCommand,      
		DWORD dwParam
	);

	char m_szErrorStr[256];
	MCIDEVICEID m_wDeviceID;
};

#endif // !defined(AFX_HWMCI_H__2B2C2C5E_6A30_46E4_8396_E1E7F1CBB511__INCLUDED_)
