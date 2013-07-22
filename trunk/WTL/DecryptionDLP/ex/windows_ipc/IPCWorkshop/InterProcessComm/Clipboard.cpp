#include "stdafx.h"
#include "Clipboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CLIPFORMAT NEAR CClipboard::m_cfPrivate = NULL;


////////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor													  //
////////////////////////////////////////////////////////////////////////////////
CClipboard::CClipboard()
{
	// Standard clipboard format.
	m_cfPrivate = CF_TEXT;
}


CClipboard::~CClipboard()
{
	Close();
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////
// opens Clipboard
bool CClipboard::Open()
{
	if( !::OpenClipboard(AfxGetMainWnd()->GetSafeHwnd() )) {
		TRACE("Failed to open the Clipboard. Error is %d", GetLastError() );
		return false;
	}
	return true;
}

// closes clipboards
bool CClipboard::Close()
{
	if( CloseClipboard() == false ) {
		TRACE("Unable to close clipboard. Error is %d", GetLastError());
		return false;
	}
	return true;
}

// Empties clipboard
bool CClipboard::Empty()
{
	if( EmptyClipboard() == 0) {
		TRACE("Unable to empty Clipboard. Error is %d", GetLastError() );
		return false;
	}
	return true;
}

// Register a new clipboard format inorder to support more applications 
// to work on the clipboard at the same time. (Optional)
void CClipboard::Register(LPCTSTR pClipboardFormat)
{
	m_cfPrivate = (CLIPFORMAT)::RegisterClipboardFormat(_T(pClipboardFormat));	
}


bool CClipboard::Write( char* pData, int pSize)
{
	// create a global allocation and lock for the data 
	HGLOBAL globalHandle = GlobalAlloc( GHND, pSize );
	LPVOID globalData = GlobalLock( globalHandle );
	memcpy( globalData, pData, pSize);
	// set to the clipboard in the registered format.
	if( SetClipboardData( m_cfPrivate, globalData ) == NULL)
		return false;
	CloseClipboard();
	return true;
}


bool CClipboard::Read(char*& pBuffer, int pSize)
{
	// Check if specified clipboard format is available
	if ( !IsClipboardFormatAvailable( m_cfPrivate ) ) {
		TRACE("Clipboard format unavailable. Error is %d", GetLastError() );
	}		 

	// Get the global handle to data from clipboard
	HANDLE  globalHandle  = GetClipboardData( m_cfPrivate ); 
	if( globalHandle == NULL ) {
		TRACE("Get Clipboard data failed, Error is %d", GetLastError());
		return false;
	}

	// Unlock the handle
	GlobalUnlock( globalHandle );
	char* buffer = ( char* )GlobalLock( globalHandle ); 
	memcpy( pBuffer, buffer, pSize );
	return true;
}


