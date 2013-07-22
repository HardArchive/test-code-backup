/*
Module : Memmap.h
Purpose: Interface for an C++ class to wrap memory mapped files
History: PJN / 30-07-1997 1. Initial Public release.
         PJN / 31-03-1998 1. Class now avoids trying to lock the mutex if only read access is required
                          2. User now has the option of specifying whether a file should be mapped with 
                             A Null terminator at the end. Can prove helpful when you want to use some
                             of the "C" runtime functions on the pointer returned.
         PJN / 20-04-1998 1. Now uses GetFileSize SDK call instead of GetFileInformationByHandle as a
                             more "reliable" way to determine file length.
                          2. Included TRACE statements to call GetLastError in all places where 
                             SDK functions fail
         PJN / 29-05-1998 1. Mapping a file now has the option of making it named or not.
         PJN / 22-10-1998 1. Fixed a bug in a number of calls to CreateMappingName when the classes were 
                             being used to share memory.
                          2. Tidy up of the demo app including:
                             a) Made the amount of text being shared a constant of MAX_EDIT_TEXT instead
                             of hardcoding it to 20 everywhere in the sample.
                             b) Changed where the timer is being created to OnInitDialog
                             c) Tidied up the initialisation sequence in OnInitDialog
                             d) Now using _tcscpy instead of _tcsncpy to ensure array is null terminated
                             e) Fixed resource.h which was causing the resources to fail to compile
                             f) Removed unnecessary symbols from resource.h
                             g) Optimized the way the OnTimer code works to only update the text when it
                             has changed in the MMF. This means that you can type continuously into the
                             edit control.
                          3. New documentation in the form of a HTML file.
                          4. Sample now ships as standard with VC 5 workspace files
         PJN / 30-03-1999 1. Code is now unicode compliant
                          2. Code now supports growable MMF's
                          3. Addition of accessor functions for file handle and file mapping handle
         PJN / 21-04-1999 1. Works around a Window bug where you try to memory map a zero length file on
                             Windows 95 or 98.
         PJN / 24-04-2000 1. Fixed a simple typo problem in a TRACE statement when compiled for UNICODE
         PJN / 07-03-2001 1. Updated copyright information   
                          2. Fixed problem where mutex was not being locked when read only access to 
                          the memory mapped file was desired. Access to the MMF should be synchronised
                          irrespective of the access mode.
         PJN / 02-04-2001 1. Now supports passing in a security descriptor to MapFile and MapMemory
                          2. Now supports opening a specified portion of a file, rather than always mapping
                          all of the file
         PJN / 05-10-2003 1. Updated copyright details.
                          2. Fixed a small typo in the description of the FSCTL_SET_SPARSE IOCTL. Thanks to 
                          amores perros for reporting this.
                          3. Fixed a minor tab indentation problem at the start of the MapFile method. Again
                          thanks to amores perros for reporting this.
                          4. Removed the unnecessary AssertValid function. Again thanks to amores perros for 
                          reporting this.
         PJN / 17-11-2003 1. Fixed a memory leak in UnMap as reported by Bart Duijndam using 
                          "Memory Validator". The memory leak may in fact not be real, but the code change 
                          avoids the reported problem.
         PJN / 06-06-2004 1. Fixed an issue in MapHandle where the wrong value was sent to CreateFileMapping. 
                          This issue only occurs when you are not mapping the whole of a file, but instead
                          decide to perform the mapping a chunk at a time. Thanks to Nicolas Stohler for
                          reporting this problem.
                          2. Removed the AppendNull option as it is incompatible with general use of memory 
                          mapped files.
                          3. Reviewed all the TRACE statements throughout the class
                          4. Added ASSERT validation at the top of functions which modify member variables
                          5. Failing to create mutex in MapHandle and MapExistingMemory not fails the function
         PJN / 23-12-2004 1. Removed unnecessary include of winioctl.h header file.
                          2. Fixed a problem with the declaration of the FSCTL_SET_SPARSE macro which is used
                          in the support of growable MMF. With the VC6 WinIOCTL.h, the macro generates the 
                          value 0x000980C4, but the correct value for the DeviceIoControl function to enable 
                          the sparse file option is 0x000900C4. Thanks to a posting on CodeProject for pointing
                          out this problem.
                          3. Optimized CMemMapFile::CreateMappingName by now using CString::Replace.
                          4. Addition of a bInheritHandle parameter to MapExistingMemory.
                          5. Fixed a bug in the handling of calls to GetFileSize()
                          6. Removed unnecessary check to verify that mapping size is greater than 4GB since
                          CreateFileMapping will do this for us.
                          7. Updated sample app to use a sample "input.txt" file for demonstration purposes
                          instead of using "c:\config.sys"
                          8. Code now uses unsigned __int64 for specifying indexes and lengths for memory 
                          mapping. This should allow the code to be easily used to map > 4GB on 64 bit 
                          versions of Windows.
                          9. Tidied up some of the ASSERT code at the top of each key function in the class.
         PJN / 30-04-2005 1. Removed derivation from CObject MFC class. In fact the class can now operate entirely
                          independently of MFC.
                          2. Destructor is now virtual.
                          3. Fixed a bug where the mutex name which is used to synchronize access to the MMF's data
                          could by default have the same name for multiple MMF's.
                          4. CreateMappingName and CreateMutexName methods are now virtual
         PJN / 02-05-2006 1. Updated the copyright details in the modules.
                          2. Updated the documentation to use the same style as the web site.
                          3. Addition of a CMEMMAPFILE_EXT_CLASS macro to allow the class to be easily incorporated
                          into an extension dll.
                          4. The sample app now uses filenames without "(" or ")" characters in their filenames. 
                          Thanks to Andrew MacGinitie for reporting this issue.
                          5. Fixed a bug in the sample app where it reports the wrong filename when doing the lowercase
                          conversion. Again thanks to Andrew MacGinitie for reporting this issue.
                          6. Fixed an issue in the download where the sample file called "input.txt" is now provided
                          out of the box. Again thanks to Andrew MacGinitie for reporting this issue.
                          7. Fixed an issue in the sample app when the code is compiled with /Wp64
         PJN / 07-07-2006 1. Code now uses newer C++ style casts instead of C style casts.
                          2. The code now requires the Platform SDK if compiled using VC 6.
                          3. Updated code to compile cleanly using VC 2005.
         PJN / 16-08-2008 1. Updated copyright details
                          2. Code now compiles cleanly using Code Analysis (/analyze)
                          3. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          4. The code now only supports VC 2005 or later. 
                          5. Removed VC 6 style AppWizard comments from the code.
                          6. Updated the code to correctly handle "Global\" and "Local\" prefixes which can be included
                          with any Win32 Named object. Thanks to Gert Rijs for reporting this bug. To achieve this the
                          parameters to the MapFile method has been reworked. This function now takes a pszMappingName
                          which allows client code to explicitly specify the mapping name
                          7. The mutex name use to serialize access to the contents of the memory mapped file is now
                          explicitly provided as a external parameter to the class.
                          8. MapExistingMemory now has a LPSECURITY_ATTRIBUTES parameter
         PJN / 06-07-2009 1. Updated copyright details
                          2. Updated the sample apps project settings to more modern default values. 
                          3. All the failure paths in the methods now preserve the Win32 last error value
                          4. Removed unnecessary code to get the length of the file in MapFile. Also simplified the logic
                          in this method.
                          5. All parameters which specify a mapping size now use a SIZE_T parameter which is consistent
                          with their equivalent API values.
                          6. All calls to MapViewOfFile are now checked for failure
         PJN / 20-11-2011 1. Updated copyright details
                          2. Updated code to clean compile on VC 2010
                          3. Replaced ASSERT calls with ATLASSERT
                          4. Updated MapMemory and MapFile methods to include a new BOOL bNoCache parameter. Thanks to 
                          HaeRim Lee for providing this nice update
                          5. Removed m_bOpen member variable and all code which uses it. This can avoid thread safety 
                          issues when a class instance of CMemMapFile is shared across threads. Thanks to HaeRim Lee 
                          for providing this nice update
                          6. Reworked the code in CMemMapFile::UnMap to call ReleaseMutex directly instead of calling
                          Close. This method now shares the same consistent ordering of object creation / destruction 
                          as the main Map... methods. This also fixes an issue where the mutex previously got released
                          twice. Thanks to HaeRim Lee for providing this nice update
                          7. The whole class is now implemented in Memmap.h and the Memmap.cpp module is now defunct 
                          and redundant. You now just need to #include the Memmap.h in client code to use the class.
         PJN / 25-11-2011 1. Updated class to be completely MFC independent. You can now use CMemMapFile in an ATL only
                          project. Thanks to HaeRim Lee for prompting this update.
         PJN / 15-03-2012 1. Updated copyright details
                          2. Made the class completely thread-safe meaning that you can share instances of CMemMapFile 
                          across threads without worrying about corruption of its member variables. Thanks to HaeRim 
                          Lee for prompting this update. To achieve this I used a nested class called CMemMapCriticalSection
                          which is derived from the ATL::CCriticalSection class. This CMemMapCriticalSection class provides 
                          for critical sections with spin locks and normal critical sections as well as stack based release
                          semantics for critical sections through the use of the ATL::CComCritSecLock class. It would be 
                          nice if the built in ATL CriticalSection class supported this but this is still not present as of
                          VC 2010 SP1.
         PJN / 16-03-2012 1. Thread protected the GetFileHandle and GetFileMappingHandle methods.

Copyright (c) 1997 - 2012 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


//////////////// Macros / Defines /////////////////////////////////////////////

#pragma once

#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#ifndef CMEMMAPFILE_EXT_CLASS
#define CMEMMAPFILE_EXT_CLASS
#endif


//////////////// Includes /////////////////////////////////////////////////////

#ifndef __ATLBASE_H__
#pragma message("To avoid this message, please put atlbase.h in your pre compiled header (normally stdafx.h)")
#include <atlbase.h>
#endif

#ifndef __ATLSYNC_H__
#pragma message("To avoid this message, please put atlsync.h in your pre compiled header (normally stdafx.h)")
#include <atlsync.h>
#endif


//////////////// Classes //////////////////////////////////////////////////////

class CMEMMAPFILE_EXT_CLASS CMemMapFile
{
public:
//Nested class which encapsulates our critical section based on ATL::CCriticalSecion with support
//for auto locking / unlocking by ATL::CComCritSecLock
  class CMemMapCriticalSection : public ATL::CCriticalSection
  {
  public:
  //Constructors
  #if (_WIN32_WINNT >= 0x0403)
	  explicit CMemMapCriticalSection(ULONG nSpinCount) : ATL::CCriticalSection(nSpinCount)
	  {
	  }
  #endif
  
  //Methods
	  HRESULT Lock()
	  {
		  Enter();
		  return S_OK;
	  }
	  
	  HRESULT Unlock() throw()
	  {
		  Leave();
		  return S_OK;
	  }
  };

//The main class
//Constructors / Destructors
  CMemMapFile(ULONG nSpinCount = 2000) : m_hFile(INVALID_HANDLE_VALUE), 
                                         m_hMapping(NULL), 
                                         m_lpData(NULL), 
                                       #if (_WIN32_WINNT >= 0x0403)
                                         m_cs(nSpinCount),
                                       #endif
                                         m_hMutex(NULL)
  {
  }

  virtual ~CMemMapFile()
  {
    UnMap();
  }

//Methods
  BOOL MapFile(LPCTSTR pszFilename, BOOL bReadOnly = FALSE, DWORD dwShareMode = 0, LPCTSTR pszMappingName = NULL, LPCTSTR pszMutexName = NULL, BOOL bGrowable = FALSE, 
               const unsigned __int64& dwStartOffset=0, const SIZE_T& nNumberOfBytesToMap=0, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, BOOL bNoCache = FALSE)
  {
    //Validate our parameters
    ATLASSERT(m_hFile == INVALID_HANDLE_VALUE); //Should not be already open
    ATLASSERT(m_hMapping == NULL);
    ATLASSERT(m_lpData == NULL);
    ATLASSERT(m_hMutex == NULL);
    
    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);

    //Open the real file on the file system
    m_hFile = CreateFile(pszFilename, bReadOnly ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE, dwShareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (m_hFile == INVALID_HANDLE_VALUE)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapFile, Failed in call to CreateFile, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    //Make the file sparse, if requested to make the memory mapped file growable
    if (bGrowable)
    {
      DWORD dwBytesReturned = 0;
      if (!DeviceIoControl(m_hFile, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &dwBytesReturned, NULL))
      {
        DWORD dwError = GetLastError();
        ATLTRACE(_T("CMemMapFile::MapFile, Failed in call to make file sparse, You need Windows 2000 or later and an NTFS 5 volume for this!, Error:%d\n"), dwError);
        UnMap();
        SetLastError(dwError);
        return FALSE;
      }
    }

    //Create the file mapping object
    DWORD dwFileMappingFlags = bReadOnly ? PAGE_READONLY : PAGE_READWRITE;
    if (bNoCache)
      dwFileMappingFlags |= SEC_NOCACHE;
    m_hMapping = CreateFileMapping(m_hFile, lpSecurityAttributes, dwFileMappingFlags, 0, 0, pszMappingName);
    if (m_hMapping == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapFile, Failed in call to CreateFileMapping, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    //Map the view
    m_lpData = MapViewOfFile(m_hMapping, bReadOnly ? FILE_MAP_READ : FILE_MAP_WRITE, static_cast<DWORD>((dwStartOffset & 0xFFFFFFFF00000000) >> 32), static_cast<DWORD>(dwStartOffset & 0xFFFFFFFF), nNumberOfBytesToMap);
    if (m_lpData == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapFile, Failed in call to MapViewOfFile, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    //Create the mutex to provide synchronised access to the mapping
    m_hMutex = CreateMutex(lpSecurityAttributes, FALSE, pszMutexName);
    if (m_hMutex == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapFile, Failed in call to CreateMutex, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }
  
    return (m_lpData != NULL);
  }

  BOOL MapMemory(LPCTSTR pszMappingName, LPCTSTR pszMutexName, const SIZE_T& nBytes, BOOL bReadOnly = FALSE, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, BOOL bNoCache = FALSE)
  {
    //Validate our parameters
    ATLASSERT(m_hMapping == NULL);
    ATLASSERT(m_lpData == NULL);
    ATLASSERT(m_hMutex == NULL);

    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);

    //Create the file mapping object
    DWORD dwFileMappingFlags = bReadOnly ? PAGE_READONLY : PAGE_READWRITE;
    if (bNoCache)
      dwFileMappingFlags |= SEC_NOCACHE;
    m_hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, lpSecurityAttributes, dwFileMappingFlags, static_cast<DWORD>((nBytes & 0xFFFFFFFF00000000) >> 32), static_cast<DWORD>(nBytes & 0xFFFFFFFF), pszMappingName);
    if (m_hMapping == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapMemory, Failed in call to CreateFileMapping, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    //Map the view
    m_lpData = MapViewOfFile(m_hMapping, bReadOnly ? FILE_MAP_READ : FILE_MAP_WRITE, 0, 0, nBytes);
    if (m_lpData == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapFile, Failed in call to MapViewOfFile, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    //Create the mutex to sync access
    m_hMutex = CreateMutex(lpSecurityAttributes, FALSE, pszMutexName);
    if (m_hMutex == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapMemory, Failed in call to CreateMutex, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    return (m_lpData != NULL);
  }

  BOOL MapExistingMemory(LPCTSTR pszMappingName, LPCTSTR pszMutexName, const SIZE_T& nBytes, BOOL bReadOnly = FALSE, BOOL bInheritHandle = FALSE, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL)
  {
    //Validate our parameters
    ATLASSERT(m_hMapping == NULL);
    ATLASSERT(m_lpData == NULL);
    ATLASSERT(m_hMutex == NULL);
    
    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);
  
    //Open the file mapping object
    DWORD dwDesiredAccess = bReadOnly ? FILE_MAP_READ : FILE_MAP_WRITE;
    m_hMapping = OpenFileMapping(dwDesiredAccess,	bInheritHandle, pszMappingName);
    if (m_hMapping == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapExistingMemory, Failed in call to OpenFileMapping, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    //Map the view
    m_lpData = MapViewOfFile(m_hMapping, dwDesiredAccess, 0, 0, nBytes);
    if (m_lpData == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapExistingMemory, Failed in call to MapViewOfFile, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    //Create the mutex to sync access
    m_hMutex = CreateMutex(lpSecurityAttributes, FALSE, pszMutexName);
    if (m_hMutex == NULL)
    {
      DWORD dwError = GetLastError();
      ATLTRACE(_T("CMemMapFile::MapExistingMemory, Failed in call to CreateMutex, Error:%d\n"), dwError);
      UnMap();
      SetLastError(dwError);
      return FALSE;
    }

    return (m_lpData != NULL);
  }

  void UnMap()
  {
    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);
  
    //Close any views which may be open
	  if (m_hMutex != NULL)
	  {
		  ReleaseMutex(m_hMutex);
		  CloseHandle(m_hMutex);
		  m_hMutex = NULL;
	  }

    //unmap the view
	  if (m_lpData != NULL)
	  {
		  FlushViewOfFile(m_lpData, 0);
		  UnmapViewOfFile(m_lpData);
		  m_lpData = NULL;
	  }

    //remove the file mapping
	  if (m_hMapping != NULL)
	  {
		  CloseHandle(m_hMapping);
		  m_hMapping = NULL;
	  }

    //close the file system file if its open
	  if (m_hFile != INVALID_HANDLE_VALUE)
	  {
		  CloseHandle(m_hFile);
		  m_hFile = INVALID_HANDLE_VALUE;
	  }
  }

  LPVOID Open(DWORD dwTimeout = INFINITE)
  {
    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);
  
    //Validate our parameters
    if (m_lpData == NULL)
    {
      SetLastError(ERROR_INVALID_FUNCTION);
      return NULL;
    }

    //Synchronise access to the MMF using the named mutex
    return (WaitForSingleObject(m_hMutex, dwTimeout) == WAIT_OBJECT_0) ? m_lpData : NULL;
  }

  BOOL Close()
  {
    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);

    //Validate our parameters
    ATLASSUME(m_hMutex != NULL); //Mutex should be valid

    return ReleaseMutex(m_hMutex);
  }

  BOOL Flush()
  {
    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);
  
    //No mapping open, so nothing to do
    if (m_lpData == NULL)
    {
      SetLastError(ERROR_INVALID_FUNCTION);
		  return FALSE;
    }

    return FlushViewOfFile(m_lpData, 0);
  }
  
//Accessors
  HANDLE GetFileHandle()
  { 
    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);

    return m_hFile; 
  }
  
  HANDLE GetFileMappingHandle()
  { 
    //Serialize access to our member variables
    ATL::CComCritSecLock<CMemMapCriticalSection> sl(m_cs);
  
    return m_hMapping; 
  }

protected:
//Member variables
  HANDLE                 m_hFile;
  HANDLE                 m_hMapping;
  LPVOID                 m_lpData;
  HANDLE                 m_hMutex;
  CMemMapCriticalSection m_cs;
};

#endif //__MEMMAP_H__
