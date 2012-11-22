/*
Module : FileFind.h
Purpose: class for WTL which performs local file searches

Copyright (c) 2003 by iLYA Solnyshkin. 
E-mail: isolnyshkin@yahoo.com 
All rights reserved.
*/

#if !defined FILEFIND_H
#define FILEFIND_H

#include <string>

class CFileFind
{
public:
	CFileFind();
	virtual ~CFileFind();

// Attributes
public:
	DWORD GetLength() const;
	std::string GetFileName() const;
	std::string GetFilePath() const;
	std::string GetFileTitle() const;
	std::string GetFileURL() const;
	std::string GetRoot() const;

	BOOL GetLastWriteTime(FILETIME* pTimeStamp) const;
	BOOL GetLastAccessTime(FILETIME* pTimeStamp) const;
	BOOL GetCreationTime(FILETIME* pTimeStamp) const;

	


	BOOL MatchesMask(DWORD dwMask) const;

	BOOL IsDots() const;
	// these aren't virtual because they all use MatchesMask(), which is
	BOOL IsReadOnly() const { return MatchesMask(FILE_ATTRIBUTE_READONLY); };
	BOOL IsDirectory() const { return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); }
	BOOL IsCompressed() const { return MatchesMask(FILE_ATTRIBUTE_COMPRESSED); }
	BOOL IsSystem() const { return MatchesMask(FILE_ATTRIBUTE_SYSTEM); }
	BOOL IsHidden() const { return MatchesMask(FILE_ATTRIBUTE_HIDDEN); }
	BOOL IsTemporary() const { return MatchesMask(FILE_ATTRIBUTE_TEMPORARY); }
	BOOL IsNormal() const { return MatchesMask(FILE_ATTRIBUTE_NORMAL); }
	BOOL IsArchived() const { return MatchesMask(FILE_ATTRIBUTE_ARCHIVE); }

// Operations
	void Close();
	BOOL FindFile(LPCTSTR pstrName = NULL, DWORD dwUnused = 0);
	BOOL FindNextFile();

protected:
	virtual void CloseContext();

// Implementation
protected:
	void* m_pFoundInfo;
	void* m_pNextInfo;
	HANDLE m_hContext;
	BOOL m_bGotLast;
	std::string m_strRoot;
	TCHAR m_chDirSeparator;     // not '\\' for Internet classes
};

#endif // !defined 