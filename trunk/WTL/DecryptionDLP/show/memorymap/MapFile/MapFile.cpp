#include "stdafx.h"
#include "MapFile.h"
#include "OSError.h"

CMapFile::CMapFile(bool read_only, bool del_flag)
:m_file(INVALID_HANDLE_VALUE), m_map(NULL), m_mapView(NULL)
{
	m_filename[0] = 0;
	m_read_only = read_only;
	m_del_flag = del_flag; 
}

CMapFile::~CMapFile(void)
{
	Close(m_del_flag);
}

void CMapFile::Close(bool del_flag)
{
	if (m_mapView != NULL)
	{
		UnmapViewOfFile(m_mapView);
		m_mapView = NULL;
	}

	if (m_map != NULL)
	{
		CloseHandle(m_map);
		m_map = NULL;
	}

	if (m_file != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_file);
		m_file = INVALID_HANDLE_VALUE;
		if(del_flag)
		{
			_tremove(m_filename);
			*m_filename = 0;
		}
	}

}

void CMapFile::Close(void)
{
	Close(m_del_flag);
}

bool CMapFile::CrFile(void)
{
	if (m_read_only)
	{
		m_file = CreateFile(m_filename, 
			GENERIC_READ, 
			FILE_SHARE_READ, 
			NULL, 
			OPEN_EXISTING, 
			FILE_FLAG_SEQUENTIAL_SCAN, 
			NULL);
	}
	else
	{
		m_file = CreateFile(m_filename, 
			GENERIC_READ|GENERIC_WRITE, 
			FILE_SHARE_READ, 
			NULL, 
			OPEN_EXISTING, 
			FILE_FLAG_SEQUENTIAL_SCAN, 
			NULL);
	}
	if (m_file == INVALID_HANDLE_VALUE) 
	{
		COSError::LogOSError(_T("CreateFile:"), m_filename);
		return false;
	}
	return true;
}

bool CMapFile::CrMap(void)
{
	if (m_read_only)
	{
		m_map = CreateFileMapping(m_file, 
			NULL,
			PAGE_READONLY, 
			0, 
			0, 
			NULL);
	}
	else
	{
		m_map = CreateFileMapping(m_file, 
			NULL,
			PAGE_READWRITE, 
			0, 
			0, 
			NULL);
	}
	
	if (m_map==NULL) 
	{
		if(FileSize() != 0)
			COSError::LogOSError(_T("CreateFileMapping:"), m_filename);
		return false;
	}
	return true;
}

bool CMapFile::CrView(void)
{
	if (m_read_only)
	{
		m_mapView = MapViewOfFile(m_map, 
			FILE_MAP_READ, 
			0, 
			0, 
			0);
	}
	else
	{
		m_mapView = MapViewOfFile(m_map, 
			FILE_MAP_WRITE, 
			0, 
			0, 
			0);
	}

	if (m_mapView==NULL) 
	{
		COSError::LogOSError(_T("MapViewOfFile:"), m_filename);
		return false;
	}
	return true;
}

void* CMapFile::GetMapView(const TCHAR* filename)
{
	Close(m_del_flag);
	
	if (filename != NULL) 
	{
		_tcscpy_s(m_filename, _countof(m_filename), filename);
		
		if(CrFile() && CrMap() && CrView())
			return m_mapView;
	}
	Close(m_del_flag);
	return NULL;
}

unsigned CMapFile::FileSize(void)
{
	if (m_file != INVALID_HANDLE_VALUE)
		return GetFileSize(m_file, NULL);
	return 0;
}

HANDLE CMapFile::GetFileHandle(void)
{
	return m_file;
}

const TCHAR* CMapFile::FileName(void)
{
	return m_filename;
}
