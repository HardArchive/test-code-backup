// XShareMemory.cpp: implementation of the XShareMemory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XShareMemory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XShareMemory::XShareMemory()
{
	Init();
//	Create(DEFAULT_FILENAME, DEFAULT_MAPNAME, DEFAULT_MAPSIZE);
}

XShareMemory::XShareMemory(WCHAR *szMapName, DWORD dwSize)
{
	Init();
	Create(szMapName, dwSize);
}

XShareMemory::~XShareMemory()
{
	Destory();
}

void XShareMemory::Init()
{
	m_hFileMap = NULL;
	m_hMemoryEvent = NULL;
	m_hBufEvent = NULL;
	m_lpFileMapBuffer = NULL;
	m_pMapName = NULL;
	m_dwSize = 0;
	m_iCreateFlag = 0;
}

void XShareMemory::Destory()
{
	if (m_lpFileMapBuffer)
	{
		UnmapViewOfFile(m_lpFileMapBuffer);
		m_lpFileMapBuffer = NULL;
	}
	
	if (m_hFileMap)
	{
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
	}
	
	if(m_hMemoryEvent)
	{
		CloseHandle(m_hMemoryEvent);
		m_hMemoryEvent = NULL;
	}

	if (m_hBufEvent)
	{
		CloseHandle(m_hBufEvent);
		m_hBufEvent = NULL;
	}

	if (m_pMapName)
	{
		free(m_pMapName);
		m_pMapName = NULL;
	}

	Init();
}

bool XShareMemory::Create( WCHAR *szMapName, DWORD dwSize)
{
	if (m_iCreateFlag)
		Destory();

	//OutputDebugInfoW(L"==%d, %s==\n", dwSize, szMapName);

	WCHAR szEventName[MAX_PATH];
	WCHAR szWREventName[MAX_PATH];
	wcscpy(szEventName, szMapName);
	wcscat(szEventName, L"_EVENT");
	wcscpy(szWREventName, szMapName);
	wcscat(szWREventName, L"_W_R");
	m_hMemoryEvent = CreateEvent(NULL, FALSE, FALSE, szEventName);
	m_hBufEvent = CreateEvent(NULL, FALSE, TRUE, szWREventName);
	//m_hBufEvent = CreateEvent(NULL, FALSE, TRUE, NULL);

	if (szMapName)
		m_pMapName = _wcsdup(szMapName);
	else m_pMapName = _wcsdup(DEFAULT_MAPNAME);

	if (dwSize > 0)
		m_dwSize = dwSize;
	else 
		m_dwSize = DEFAULT_MAPSIZE;


	m_hFileMap = CreateFileMapping(
			(HANDLE)INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			m_dwSize,
			m_pMapName
			);

	if (m_hFileMap)
	{
		m_lpFileMapBuffer = MapViewOfFile(
			m_hFileMap,
			FILE_MAP_ALL_ACCESS,//FILE_MAP_WRITE|FILE_MAP_READ,
			0,
			0,
			m_dwSize
			);
	}

	if (m_lpFileMapBuffer == NULL)
		return false;

	m_iCreateFlag = 1;
	return true;
}

bool XShareMemory::IsCreated()
{
	return (m_iCreateFlag == 1)? true : false;
}

LPVOID XShareMemory::GetBuffer()
{
	return (m_lpFileMapBuffer)?(m_lpFileMapBuffer):(NULL);
}

DWORD XShareMemory::GetSize()
{
	return m_dwSize;
}

bool XShareMemory::Write(const void *pData, DWORD dwSize)
{
	void *p = (void*)GetBuffer();
	if(p)
	{
		::WaitForSingleObject(m_hBufEvent, 5000);
		memcpy(p, pData, dwSize);
		SetEvent(m_hBufEvent);
		SetEvent(m_hMemoryEvent);
		return true;
	}
	else
		return false;
}

bool XShareMemory::Read(void *pData, DWORD dwSize)
{
	void *p = (void*)GetBuffer();
	if(!p)
		return false;
	
	::WaitForSingleObject(m_hBufEvent, 5000);
	memcpy(pData, p, min(dwSize, m_dwSize));
	SetEvent(m_hBufEvent);

	return true;
}
