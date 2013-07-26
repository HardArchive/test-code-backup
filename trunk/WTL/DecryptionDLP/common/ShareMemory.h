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
//#include "GeneralHelper.h"

namespace RG
{
	/***************************************************************************************************
	* 1�� File       �� CMapFile
	* 2�� Version    �� *.*
	* 3�� Description�� �����ļ�ӳ��Ĵ��ļ��� Ĭ��Ϊ�����ļ�  ����CShareMemory����
	*				    ͨ��GetMapFileHandle��GetMapFileHandle�ļ����ΪINVALID_HANDLE_VALUE
	* 4�� Author     �� RG (http://www.9cpp.com/)
	* 5�� Created    �� 2013-4-27 15:15:39
	* 6�� History    �� 
	* 7�� Remark     �� //case GENERIC_EXECUTE: ������ʱ������
	****************************************************************************************************/
	class CMapFile
	{

	public:
		CMapFile(LPCTSTR lpFileName = NULL,                    //ӳ���ļ���
			DWORD dwDesiredAccess = GENERIC_READ)             //���ʷ�ʽ�Ŀɶ�
		{
			m_hFile = INVALID_HANDLE_VALUE;
		}

		~CMapFile()
		{
			Close();
		}

	public:
		//����ļ��Ƿ����
		BOOL CheckFileExists(LPCTSTR lpFileName)
		{
			DWORD dwAttr = 0;
			UINT iPrevErrMode = 0;
			if (!lpFileName) return false;

			iPrevErrMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
			dwAttr = ::GetFileAttributes(lpFileName);
			::SetErrorMode(iPrevErrMode);
			return dwAttr == INVALID_FILE_ATTRIBUTES ? false : true;
		}
		//ȡ��ӳ�䵽�ڴ���ļ����
		HANDLE GetMapFileHandle(){return m_hFile;}

		//ȡ���ļ�����
		DWORD GetMapFileSize(LPCTSTR lpFileName = NULL,      //ӳ���ļ���
			DWORD dwDesiredAccess = GENERIC_READ)            //Ĭ�Ϸ��ʷ�ʽ�Ŀɶ�
		{
			DWORD dwFileSize = INVALID_FILE_SIZE;
			if (INVALID_HANDLE_VALUE == m_hFile)
			{
				m_hFile = CreateMapFile(lpFileName, dwDesiredAccess);
			}
			//����ӳ���ļ�ʧ��
			if (INVALID_HANDLE_VALUE == m_hFile) 
				return dwFileSize;
			
			dwFileSize = ::GetFileSize(m_hFile, NULL);
			return dwFileSize;
		}

		//���������ӳ����ļ� �ɹ����� �ļ����
		HANDLE CreateMapFile(LPCTSTR lpFileName = NULL,      //ӳ���ļ���
			DWORD dwDesiredAccess = GENERIC_READ)            //Ĭ�Ϸ��ʷ�ʽ�Ŀɶ�
		{
			Close();
			if (NULL == lpFileName) return m_hFile;
			if (!CheckFileExists(lpFileName)) return m_hFile;            //�ļ�������

			DWORD dwShareMode = 0;      //���б�������
			DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;          //Ĭ������
			switch(dwDesiredAccess)
			{
			case GENERIC_READ:                                           //���Դ��ļ��ж�ȡ����
				dwShareMode = FILE_SHARE_READ;
				dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN;        //����������ʶ��ļ���������Ż�
				break;
			case GENERIC_WRITE:                                          //���Խ�����д���ļ�
				dwShareMode = FILE_SHARE_WRITE;break;
			case GENERIC_READ|GENERIC_WRITE:                             //���Դ��ļ��ж�ȡ����,Ҳ���Խ�����д���ļ�
				dwShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;break;
			//case GENERIC_EXECUTE: ������ʱ������
			}

			m_hFile = ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, dwFlagsAndAttributes, NULL);
			if (INVALID_HANDLE_VALUE == m_hFile)
			{
				//.....��ӡ����GetLastError();
				//::MessageBox(NULL, _T("���ܴ��ļ�����������"), _T("Error"), MB_OK);
			}

			return m_hFile;
		}

		//�ر��ļ� ���ļ������ΪINVALID_HANDLE_VALUE
		void Close()
		{
			if (INVALID_HANDLE_VALUE != m_hFile)
			{
				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;;
			}
		}

	private:
		HANDLE m_hFile;           //�ļ����
	};

	/***************************************************************************************************
	* 1�� File       �� CShareMemory
	* 2�� Version    �� *.*
	* 3�� Description�� �����ڴ��װ��
	* 4�� Author     �� RG (http://www.9cpp.com/)
	* 5�� Created    �� 2013-4-27 15:21:12
	* 6�� History    �� 
	* 7�� Remark     �� ������Ҫ�Ľ�  ���Ӷ�д��ȫͬ��
	****************************************************************************************************/
	class CShareMemory
	{
	public:
		CShareMemory()
		{
			m_hMapping = NULL;			
			m_lpBaseAddress = NULL;
			m_dwMapBufLen = INVALID_FILE_SIZE;
		}
		~CShareMemory(){Close();}

	public:
		bool Create(LPCTSTR lpMapName = NULL, DWORD dwBufSize = 0, LPCTSTR lpFileName = NULL, DWORD dwDesiredAccess = GENERIC_READ)
		{
			Close();
			//PAGE_READONLY ��ֻ����ʽ��ӳ��
			//PAGE_READWRITE �Կɶ�����д��ʽ��ӳ��
			//PAGE_WRITECOPY Ϊд�������±���
			//�����ʹ������һ������������
			//SEC_COMMIT Ϊ�ļ�ӳ��һ��С���е�����ҳ�����ڴ�
			//SEC_IMAGE �ļ��Ǹ���ִ���ļ�
			//SEC_RESERVE Ϊû�з���ʵ���ڴ��һ��С�ڱ��������ڴ�ռ�
			//����һ���������ļ�ӳ�����
			m_dwMapBufLen = dwBufSize?dwBufSize:m_clsMapFile.GetMapFileSize(lpFileName);

			m_hMapping = ::CreateFileMapping(
				m_clsMapFile.CreateMapFile(lpFileName, dwDesiredAccess),   //�����ļ����
				NULL,                                                      //��ȫ���� NULLʹ��Ĭ�ϵİ�ȫ����
				PAGE_READONLY | SEC_COMMIT,                                //��������
				0,                                                         //��λ�ļ���С 32λ��Ϊ0
				dwBufSize,                                                 //��λ�ļ���С
				lpMapName);                                  			   //�����ڴ�����

			if (NULL == m_hMapping)                                              
			{
				_asm int 3;
				DWORD dwError = GetLastError();
				::MessageBox(NULL, _T("����һ���������ļ�ӳ��������ʧ�ܣ���������"), _T("Error"), MB_OK);				
				Close();
				return false;
			}

			DWORD dwMapViewDesiredAccess = FILE_MAP_READ;
			switch(dwDesiredAccess)
			{
			case GENERIC_READ:                                           //���Դ��ļ��ж�ȡ����
				dwMapViewDesiredAccess = FILE_MAP_READ;break;
			case GENERIC_WRITE:                                          //���Խ�����д���ļ�
				dwMapViewDesiredAccess = FILE_MAP_WRITE;;break;
			case GENERIC_READ|GENERIC_WRITE:                             //���Դ��ļ��ж�ȡ����,Ҳ���Խ�����д���ļ�
				dwMapViewDesiredAccess = FILE_MAP_ALL_ACCESS;break;
			//case GENERIC_EXECUTE: ������ʱ������
			}

			// Map view of file into baseointer.
			//���������ļ�ӳ��������ͼӳ�����ַ	�ռ䣬ͬʱ�õ���ӳ����ͼ����ַ��###��ë###��������ұ������ݵĵ�ַ��
			if (!(m_lpBaseAddress = ::MapViewOfFile(m_hMapping, dwMapViewDesiredAccess, 0, 0, 0)))
			{				
				::MessageBox(NULL, _T("View failed."), _T("Error"), MB_OK);
				Close();
				return false;
			}
			return true;
		}

		//��һ���Ѿ��������ڴ�ӳ����� 
		//dwDesiredAccess
		//FILE_MAP_ALL_ACCESS Includes all access rights to a file mapping object except FILE_MAP_EXECUTE
		//FILE_MAP_COPY       Copy-on-write access.
		//FILE_MAP_EXECUTE    Execute access. 
		//FILE_MAP_READ       Read access. 
		//FILE_MAP_WRITE      Write access
		bool Open(LPCTSTR lpMapName = NULL,                   //ӳ�������
			DWORD dwDesiredAccess = FILE_MAP_READ,            //Ĭ�Ϸ��ʷ�ʽ�Ŀɶ�
			bool bInheritHandle = false)   
		{
			Close();
			m_hMapping = ::OpenFileMapping(dwDesiredAccess, bInheritHandle, lpMapName);

			if (NULL == m_hMapping)                                              
			{
				//::MessageBox(NULL, _T("��һ���������ļ�ӳ��������ʧ�ܣ���������"), _T("Error"), MB_OK);
				//DWORD dwError = GetLastError();
				return false;
			}

			// Map view of file into baseointer.
			//���������ļ�ӳ��������ͼӳ�����ַ	�ռ䣬ͬʱ�õ���ӳ����ͼ����ַ��###��ë###��������ұ������ݵĵ�ַ��
			if (!(m_lpBaseAddress = ::MapViewOfFile(m_hMapping, dwDesiredAccess, 0, 0, 0)))
			{
				//::MessageBox(NULL, _T("View failed."), _T("Error"), MB_OK);
				Close();
				return false;
			}

			return true;
		}
					
		//����ͼ�ı仯ˢ��д�����  �����ļ�ӳ��ʱ��Ҫ�õ�
		//lpBaseAddress������ˢ�»�����ַ dwNumberOfBytesToFlush��ˢ�µ��ֽ���
		bool Flush(LPCVOID lpBaseAddress, SIZE_T dwNumberOfBytesToFlush)
		{
			if (!(m_hMapping && m_lpBaseAddress)) return false;
			return ::FlushViewOfFile(lpBaseAddress, dwNumberOfBytesToFlush);
		}

		//�ر�ӳ��
		void Close()
		{
			if (m_lpBaseAddress)
			{
				//�ͷ���ͼ���ѱ仯д���ļ�
				::UnmapViewOfFile(m_lpBaseAddress);
				m_lpBaseAddress = NULL;
			}	

			if (m_hMapping)
			{
				::CloseHandle(m_hMapping);
				m_hMapping = NULL;
			}
			m_dwMapBufLen = 0;
			m_clsMapFile.Close();
		}

		//ȡ��ָ�����ڴ��ָ��
		PVOID GetBasePoint(){return m_lpBaseAddress;}

		//ȡ��ӳ���ڴ�ռ��С
		DWORD GetFileMapLen() {return m_dwMapBufLen;}

		////ͬ��������  ����д���ݵ�ʱ�������Lock ����Unlock
		//void Lock()		{}
		//void Unlock()	{}

	private:
		DWORD m_dwMapBufLen;          //�򿪵Ĺ����ڴ泤��
		HANDLE m_hMapping;            //�ڴ�ӳ�������
		PVOID m_lpBaseAddress;		  //ָ�����ڴ��ָ��
		CMapFile m_clsMapFile;        //�ڴ�ӳ����ļ�������
	};
}

#endif /*__SHARE_MENORY_H__*/