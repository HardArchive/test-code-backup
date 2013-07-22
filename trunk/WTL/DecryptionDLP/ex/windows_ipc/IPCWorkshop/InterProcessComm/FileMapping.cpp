#include "stdafx.h"
#include "FileMapping.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor													  //
////////////////////////////////////////////////////////////////////////////////
CFileMapping::CFileMapping()
{
	m_hFileHandle = NULL;
	m_pViewOfFile = NULL;
	m_nFileSize = 0;
}


CFileMapping::~CFileMapping()
{
	if( m_pViewOfFile != NULL ){
		if( UnmapViewOfFile(m_pViewOfFile) == 0)
			TRACE("Unable to unmap view of file. Error is %d", GetLastError());
	}
	if( m_hFileHandle!= NULL)
		CloseHandle(m_hFileHandle);
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////
bool CFileMapping::Initialize(LPCTSTR pFileName, int pFileSize)
{
	m_nFileSize = pFileSize;
	// creates as well as opens a file map.
	m_hFileHandle = CreateFileMapping(	INVALID_HANDLE_VALUE,               // handle to file
										NULL,								// security
										PAGE_READWRITE,                     // protection
										0,						            // high-order DWORD of size
										m_nFileSize,			            // low-order DWORD of size
										pFileName	  	                    // object name
									 );

	if( !m_hFileHandle ) {
		TRACE("Unable to create file mapping. Error is %d", GetLastError());
		return false;
	}
	
	// Map the file into the current address space.
	m_pViewOfFile = MapViewOfFile(	m_hFileHandle,							// handle to file-mapping object
									FILE_MAP_ALL_ACCESS,					// access mode
									0,										// high-order DWORD of offset
									0,										// low-order DWORD of offset
									0										// number of bytes to map
								);
	if( m_pViewOfFile == NULL ) {
		TRACE("Unable to map to the current address space. Error is %d", GetLastError());
		return false;
	}
	return true;
}


// Since the file Object is in the same address space, we can use just like dynamic
// allocated memory  
void CFileMapping::Write(char* pData)
{
	ASSERT( m_nFileSize != 0 );
	memcpy( m_pViewOfFile, pData, m_nFileSize );
}


void CFileMapping::Read(char*& pBuffer)
{
	ASSERT( m_nFileSize != 0 );
	memcpy( pBuffer, m_pViewOfFile, m_nFileSize );
}
