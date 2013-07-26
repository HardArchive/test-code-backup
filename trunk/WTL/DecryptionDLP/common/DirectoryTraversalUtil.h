//file DirectoryTraversalUtil.h
/***************************************************************************************************
* 1�� Class      �� DirectoryTraversalUtil
* 2�� Version    �� 1.0.1
* 3�� Description�� ����Ŀ¼ 
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-9-7 16:27:44
* 6�� History    �� �޸�Ϊ֧���ļ��д���  
* 7�� Remark     �� ���������� �����Ϊ ���ֻ��������Ŀ¼  ������Ŀ¼����  ���̱߳�����δ���
****************************************************************************************************/
#ifndef __DIEECTOR_TRAVERSAL_UTIL_H_
#define __DIEECTOR_TRAVERSAL_UTIL_H_
#include <Windows.h>
#include "TraceUtil.h"
#include "PathUtil.h"

namespace RG
{
	class CDirectoryTraversalUtil
	{
	public:
		CDirectoryTraversalUtil() {Reset();}
		~CDirectoryTraversalUtil(){Reset();}

	public:
		//�ļ�������
		virtual bool TraversalFileDoWork(const PTCHAR ptInFilePath) = 0;  //�����ļ�
		//�ļ��д�����
		virtual bool TraversalFolderDoWork(const PTCHAR ptInFilePath) = 0;  //�����ļ���

		//��ʼ������ ptInPath ������Ŀ¼�� ptInFileTpye ������ȡ���ļ���׺ iInterval ������ʱ����
		void Init(const PTCHAR ptInPath, const PTCHAR ptInFileTpye = _T(".*"), const int iInterval = 0)
		{
			m_iInterval = iInterval;
			_tcscpy_s(m_tszFileType, 16, ptInFileTpye);
			_tcscpy_s(m_tszDirectoryTraversalPath, MAX_PATH, ptInPath);
		}

		//��ʼ���� 
		int StartTraversal()
		{
			int iRet = 0;
			int iSleepTime = 0;
			m_bExitFlag = true;
			iSleepTime = m_iInterval?m_iInterval*1000:1;
			//��α��� 
			while(m_bExitFlag)
			{
				iRet = DirectoryTraversal(m_tszDirectoryTraversalPath, m_tszFileType);
				if (iRet<0)
				{
					//.....������
				}

				//ʱ����Ϊ��ʱֻ����һ��
				if (!m_iInterval)	break;
				//TRACE("������ɣ�����%d����ٿ�����������", iSleepTime/1000);

				Sleep(iSleepTime);
			}

			return iRet;
		}
		//ֹͣ����
		void StopTraversal(){m_bExitFlag = false; Reset();}

	private:
		void Reset()
		{
			m_iInterval = 0;                                 //ʱ����
			m_bExitFlag = false;                             //�˳���־
			memset(m_tszFileType, 0, sizeof(TCHAR)*16);
			memset(m_tszDirectoryTraversalPath, 0, sizeof(TCHAR)*MAX_PATH);
		}

		//һ�α���Ŀ¼
		int DirectoryTraversal(const PTCHAR ptInPath, const PTCHAR ptInFileTpye = _T(".*"))
		{
			HANDLE hFind = NULL;
			WIN32_FIND_DATA stuWFD = {0};	
			TCHAR tszFullPath[MAX_PATH] = {0};
			TCHAR tszFindFilter[MAX_PATH] = {0};
			DWORD dwAttributes = 0;

			if (!(ptInPath && ptInFileTpye))	return -1;

			//��鴴�����ļ���·��			
			if (!RG::ChickDirExist(ptInPath))
			{
				//if (!RG::CreateMultipleDirectory(ptInPath))
					return -2;				
			}

			_stprintf_s(tszFindFilter, MAX_PATH, _T("%s\\*%s"), ptInPath, ptInFileTpye);    //�����ļ�����

			if ((hFind = FindFirstFile(tszFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
			{
				return -3;
			}

			do
			{
				if (!m_bExitFlag) break;
				//���˱���Ŀ¼�͸�Ŀ¼
				if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
					continue;
				ZeroMemory(tszFullPath, sizeof(tszFullPath));
				_stprintf_s(tszFullPath, MAX_PATH, _T("%s\\%s"), ptInPath, stuWFD.cFileName);  //�õ��ļ�(��)����·��

				//����ҵ�����Ŀ¼��������Ŀ¼���еݹ�
				if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{	//����Ŀ¼
					if (!TraversalFolderDoWork(tszFullPath))
					{
						FindClose(hFind);
						return -5;
					}

					if (1 != DirectoryTraversal(tszFullPath, ptInFileTpye))
					{
						FindClose(hFind);
						return -4;
					}
				}
				else 
				{    //�ҵ������ļ�
					if (!TraversalFileDoWork(tszFullPath))
					{
						FindClose(hFind);
						return -5;
					}
				}	
			}while (FindNextFile(hFind, &stuWFD));

			FindClose(hFind);
			return 1;
		}

	public:
		bool m_bExitFlag;                                //�˳���־  true ִ�б��� false ֹͣ����
		CPathUtil m_clsPathUtil;
	private:		
		int m_iInterval;                                 //��α�����ʱ����   �����һ�α�����ʱ��Ϊ0
		TCHAR m_tszFileType[16];                         //������ȡ���ļ���׺
		TCHAR m_tszDirectoryTraversalPath[MAX_PATH];     //������Ŀ¼��		
	};
}
#endif

