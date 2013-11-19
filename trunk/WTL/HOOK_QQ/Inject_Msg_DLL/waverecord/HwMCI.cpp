// HwMCI.cpp: implementation of the CHwMCI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xuefeng.h"
#include "HwMCI.h"

// Ϊ MCI ������ӿ��ļ�
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHwMCI::CHwMCI()
	: m_wDeviceID ( NULL )
{
	memset ( m_szErrorStr, 0, sizeof(m_szErrorStr) );
}

CHwMCI::~CHwMCI()
{
	Close ();
}

MCIERROR CHwMCI::hwMciSendCommand(
		MCIDEVICEID IDDevice, 
		UINT uMsg,             
		DWORD fdwCommand,      
		DWORD dwParam
	)
{
	MCIERROR mciError = ::mciSendCommand ( IDDevice, uMsg, fdwCommand, dwParam );
	if ( mciError != 0 )
	{
		memset ( m_szErrorStr, 0, sizeof(m_szErrorStr) );
		mciGetErrorString ( mciError, m_szErrorStr, sizeof(m_szErrorStr) );
#ifdef _DEBUG
		AfxMessageBox ( m_szErrorStr );
#endif
	}

	return mciError;
}

BOOL CHwMCI::Open(LPCTSTR lpszWaveFileName/*=NULL*/)
{
	MCI_OPEN_PARMS OpenParms = {0};
	OpenParms.lpstrDeviceType = (LPCSTR)(lpszWaveFileName?NULL:MCI_DEVTYPE_WAVEFORM_AUDIO);
	OpenParms.lpstrElementName = (LPCSTR)(lpszWaveFileName?lpszWaveFileName:"");	// �򿪵������ļ���
	m_wDeviceID = NULL;	// �򿪵���Ƶ�豸��ID
	if ( hwMciSendCommand ( NULL,
		MCI_OPEN,
		( lpszWaveFileName?0:(MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID) ) | MCI_WAIT | MCI_OPEN_ELEMENT,
		(DWORD)(LPVOID) &OpenParms ) != 0 )
		return FALSE;

	m_wDeviceID = OpenParms.wDeviceID;
	return TRUE;
}

void CHwMCI::Close()
{
	if ( m_wDeviceID != NULL )
	{
		hwMciSendCommand ( m_wDeviceID, MCI_CLOSE, NULL, NULL );
	}
	m_wDeviceID = NULL;	// �򿪵���Ƶ�豸��ID

}

BOOL CHwMCI::Play(DWORD dwFrom/*=0*/, DWORD dwTo/*=0*/)
{
	Stop ();
	// ����wave�ļ�
	MCI_PLAY_PARMS PlayParms = {0};
	PlayParms.dwFrom = dwFrom;		// ����Ϊ��ָ����ʲô�ط���ʱ�䣩����WAVE�ļ�
	PlayParms.dwTo = dwTo;
	if ( hwMciSendCommand ( m_wDeviceID, MCI_PLAY, MCI_FROM,
		(DWORD)(LPVOID)&PlayParms ) != 0 )
		return FALSE;

	return TRUE;

}

BOOL CHwMCI::Stop()
{
	if ( m_wDeviceID == NULL ) return FALSE;

	return ( hwMciSendCommand ( m_wDeviceID, MCI_STOP, NULL, NULL ) == 0 );
}

BOOL CHwMCI::Pause()
{
	if ( m_wDeviceID == NULL ) return FALSE;

	return ( hwMciSendCommand ( m_wDeviceID, MCI_PAUSE, NULL, NULL ) == 0 );
}

BOOL CHwMCI::Save(CString Filename)
{
	Stop ();
	MCI_SAVE_PARMS SaveParms = {0};
	SaveParms.lpfilename = (LPCSTR) Filename;
	return ( hwMciSendCommand ( m_wDeviceID, 
		MCI_SAVE, 
		MCI_SAVE_FILE | MCI_WAIT, 
		(DWORD)(LPVOID) &SaveParms) == 0 );
}

LPSTR CHwMCI::GetErrorStr()
{
	return m_szErrorStr;
}

BOOL CHwMCI::Rec ( int nChannels,WORD nSamplesPerSec,int wBitsPerSample)
{
	if ( nChannels != 1 && nChannels != 2 )
		return FALSE;

	Close ();	// Ϊ�ɹ����豸��¼��ǰ�ر�һ���豸
	if ( !Open () ) return FALSE;
	SetPara ( WAVE_FORMAT_PCM, nChannels, nSamplesPerSec, nSamplesPerSec*nChannels*wBitsPerSample/8, nChannels*wBitsPerSample/8, wBitsPerSample);

	MCI_RECORD_PARMS RecordParms = {0};
	return ( hwMciSendCommand ( m_wDeviceID, 
		MCI_RECORD, 
		NULL, 
		(DWORD)(LPVOID) &RecordParms) == 0 );
}

BOOL CHwMCI::DeviceIDValid()
{
	return ( m_wDeviceID != NULL );
}

//
// ���ò���
//
BOOL CHwMCI::SetPara (
		WORD        wFormatTag,			/* format type */
		WORD        nChannels,			/* ��¼������ͨ���� */
		DWORD       nSamplesPerSec,		/* ��¼ÿ��ȡ���� */
		DWORD       nAvgBytesPerSec,	/* ��¼ÿ��������� */
		WORD        nBlockAlign,		/* ��¼����Ķ��뵥λ */
		WORD        wBitsPerSample		/* ��¼ÿ��ȡ�������λԪ�� */
	)
{
	if ( m_wDeviceID == NULL )
		return FALSE;

	MCI_WAVE_SET_PARMS  SetParms = {0};
	SetParms.wFormatTag = wFormatTag;
	SetParms.nChannels = nChannels;
	SetParms.nSamplesPerSec = nSamplesPerSec;
	SetParms.nAvgBytesPerSec = nAvgBytesPerSec;
	SetParms.nBlockAlign = nBlockAlign;
	SetParms.wBitsPerSample = wBitsPerSample;

	return ( hwMciSendCommand ( m_wDeviceID, MCI_SET, MCI_WAIT |
		MCI_WAVE_SET_FORMATTAG | MCI_WAVE_SET_CHANNELS | MCI_WAVE_SET_SAMPLESPERSEC | MCI_WAVE_SET_AVGBYTESPERSEC |
		MCI_WAVE_SET_BLOCKALIGN | MCI_WAVE_SET_BITSPERSAMPLE,
		(DWORD)(LPVOID) &SetParms ) == 0 );

}

//u ���¼��ʱ���Ĵ����������ʱ�䣬Ӧ���Զ����ļ�����