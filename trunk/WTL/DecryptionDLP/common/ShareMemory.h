//file ShareMemory.h
/***************************************************************************************************
* 1�� File       �� ShareMemory.h
* 2�� Version    �� *.*
* 3�� Description�� �ļ�ӳ�乲���ڴ��װ��
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-26 15:41:26
* 6�� History    �� 
* 7�� Remark     �� ������Ҫ�Ľ�  1 ���Ӷ�д����  2���Ӷ�д��ȫͬ��
****************************************************************************************************/
#ifndef __SHARE_MENORY_H__
#define __SHARE_MENORY_H__
#include "GeneralHelper.h"


namespace RG
{
	class CShareMemory
	{
	public:
		CShareMemory()
		{
			m_hFile = INVALID_HANDLE_VALUE;
			m_hMapping = NULL;
			m_lpBaseAddress = NULL;
		}
		~CShareMemory(){Close();}

	public:
		bool Open(tstring& MapName = tstring(_T("")), DWORD dwBufSize = 0, tstring& tstrFileName = tstring(_T("")), DWORD dwShareMode = FILE_SHARE_READ)
		{
			m_hFile = GetFileHandle(tstrFileName, dwShareMode);
			//����һ���������ļ�ӳ�����
			if (!(m_hMapping = CreateFileMapping(
				m_hFile, 
				0, 
				dwShareMode | SEC_COMMIT,
				0,
				dwBufSize, 
				0)))
			{
				::MessageBox(NULL, _T("����һ���������ļ�ӳ��������ʧ�ܣ���������"), _T("Error"), MB_OK);
				Close();
				return false;
			}

			// Map view of file into baseointer.
			//���������ļ�ӳ��������ͼӳ�����ַ	�ռ䣬ͬʱ�õ���ӳ����ͼ����ַ��###��ë###��������ұ������ݵĵ�ַ��
			if (!(m_lpBaseAddress = MapViewOfFile(m_hMapping, FILE_MAP_READ,0,0,0)))
			{
				::MessageBox(NULL, _T("View failed."), _T("Error"), MB_OK);
				Close();
				return false;
			}
			return true;
		}

		void Close()
		{
			if (m_hMapping)
			{
				CloseHandle(m_hMapping);
				m_hMapping = NULL;
			}
			if (m_hFile)
			{
				CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;;
			}

			if (m_lpBaseAddress)
			{
				UnmapViewOfFile(m_lpBaseAddress);
				m_lpBaseAddress = NULL;
			}	
		}

		//ȡ��ָ�����ڴ��ָ��
		PVOID GetBasePoint(){return m_lpBaseAddress;}

	private:
		//���ļ������ڴ�tstrFileName, dwShareMode����ģʽ Ĭ�϶�   dwCreationDisposition �ļ�����
		HANDLE GetFileHandle(tstring& tstrFileName, DWORD dwShareMode = FILE_SHARE_READ, DWORD dwCreationDisposition = OPEN_EXISTING)
		{
			HANDLE hFile = INVALID_HANDLE_VALUE;
			if (!tstrFileName.length())	return hFile;

			// �����ļ�����
			if ((hFile = ::CreateFile(tstrFileName.c_str(), 
				GENERIC_READ, 
				dwShareMode, 
				0, 
				dwCreationDisposition, 
				FILE_FLAG_SEQUENTIAL_SCAN,
				0)) == INVALID_HANDLE_VALUE)
			{
				//.....��ӡ����GetLastError();
				::MessageBox(NULL, _T("���ܴ��ļ�����������"), _T("Error"), MB_OK);
			}
			return hFile;
		}

	private:
		HANDLE m_hFile;           //�ļ����
	    HANDLE m_hMapping;        //�ڴ�ӳ�������
		PVOID m_lpBaseAddress;		  //ָ�����ڴ��ָ��
	};
}

#endif /*__SHARE_MENORY_H__*/