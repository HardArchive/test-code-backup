#ifndef __9CPPFILE_H__
#define __9CPPFILE_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////
/******************************************************************************* 
* 1�� �ļ����ƣ� 9CppFile
* 2�� �桡������ Version *.*
* 3�� ��    ���� �ļ���д������
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-7-5 14:05:55
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 

namespace QNA
{
	class CFile
	{
	public:
		HANDLE m_hFile;    //�ļ����

		BOOL Open(LPCTSTR pstrFileName, 
			DWORD dwAccess = GENERIC_READ, 
			DWORD dwShareMode = FILE_SHARE_READ, 
			DWORD dwFlags = OPEN_EXISTING,
			DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL)
		{
			_ASSERTE(pstrFileName!=NULL);
			Close();
			// Attempt file creation
			HANDLE hFile = ::CreateFile(pstrFileName, 
				dwAccess, 
				dwShareMode, 
				NULL,
				dwFlags, 
				dwAttributes, 
				NULL);
			if( hFile == INVALID_HANDLE_VALUE ) return FALSE;
			m_hFile = hFile;
			return TRUE;
		}
	
	};
}


#endif // __9CPPFILE_H___