//file DirectoryTraversalUtil.h
/***************************************************************************************************
* 1�� Class      �� DirectoryTraversalUtil
* 2�� Version    �� 1.0
* 3�� Description�� ����Ŀ¼ 
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-9-7 16:27:44
* 6�� History    �� 
* 7�� Remark     �� ���������� �����Ϊ ���ֻ��������Ŀ¼  ������Ŀ¼����  ���̱߳�����δ���
****************************************************************************************************/
#ifndef __DIEECTOR_TRAVERSAL_UTIL_H_
#define __DIEECTOR_TRAVERSAL_UTIL_H_
#include <Windows.h>
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
		virtual bool TraversalDoWork(const PTCHAR ptInFilePath) = 0;  //�����ļ�
		//��ʼ������ ptInPath ������Ŀ¼�� ptInFileTpye ������ȡ���ļ���׺ iInterval ������ʱ����
		void Init(const PTCHAR ptInPath, const PTCHAR ptInFileTpye, const int iInterval = 0)
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
			while(m_bExitFlag)
			{
				iRet = DirectoryTraversal(m_tszDirectoryTraversalPath, m_tszFileType);
				if (iRet<0)
				{
					//.....������
				}

				Sleep(iSleepTime);
			}

			return iRet;
		}
		//ֹͣ����
		void StopTraversal(){m_bExitFlag = false;}

	private:
		void Reset()
		{
			m_iInterval = 0;                                 //ʱ����
			m_bExitFlag = false;                             //�˳���־
			memset(m_tszFileType, 0, sizeof(TCHAR)*16);
			memset(m_tszDirectoryTraversalPath, 0, sizeof(TCHAR)*MAX_PATH);
		}
		//һ�α���Ŀ¼
		int DirectoryTraversal(const PTCHAR ptInPath, const PTCHAR ptInFileTpye)
		{
			HANDLE hFind = NULL;
			WIN32_FIND_DATA stuWFD = {0};	
			TCHAR tszFullPath[MAX_PATH] = {0};
			TCHAR tszFindFilter[MAX_PATH] = {0};
			DWORD dwAttributes = 0;

			if (!(ptInPath && ptInFileTpye))	return -1;
			//��鴴�����ļ���·��
			if (!m_clsPathUtil.ChickDirExist(ptInPath))
			{
				if (!m_clsPathUtil.CreateMultipleDirectory(ptInPath))
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
				{	
					if (1 != DirectoryTraversal(tszFullPath, ptInFileTpye))
					{
						FindClose(hFind);
						return -4;
					}
				}
				else 
				{//�ҵ������ļ�
					if (!TraversalDoWork(tszFullPath))
					{
						FindClose(hFind);
						return -5;
					}
				}	
			}while (FindNextFile(hFind, &stuWFD));

			FindClose(hFind);
			return 1;
		}
	private:
		bool m_bExitFlag;                                //�˳���־  true ִ�б��� false ֹͣ����
		int m_iInterval;                                 //��α�����ʱ����  
		TCHAR m_tszFileType[16];                         //������ȡ���ļ���׺
		TCHAR m_tszDirectoryTraversalPath[MAX_PATH];     //������Ŀ¼��
		CPathUtil m_clsPathUtil;
	};
}
#endif

