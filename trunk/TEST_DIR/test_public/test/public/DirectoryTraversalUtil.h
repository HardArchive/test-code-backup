//file DirectoryTraversalUtil.h
/***************************************************************************************************
* 1�� Class      �� DirectoryTraversalUtil
* 2�� Version    �� 1.0
* 3�� Description�� ����Ŀ¼ 
* 4�� Author     �� Ray (http://www.9cpp.com/)
* 5�� Created    �� 2014-4-9 16:27:44
* 6�� History    �� 
* 7�� Remark     �� ���������� �����Ϊ ���ֻ��������Ŀ¼  ������Ŀ¼����  Ŀ¼�������Ƿ񴴽��µ�Ŀ¼ ���̱߳�����δ���
* **************������Ҫ�̳�ʵ��TraversalDoWork��������ʹ��*****************
****************************************************************************************************/
#ifndef __DIEECTOR_TRAVERSAL_UTIL_H_
#define __DIEECTOR_TRAVERSAL_UTIL_H_
#include <Windows.h>
#include "TraceUtil.h"
#include "PathUtil.h"


/////////////////////////////////////Error Code/////////////////////////////////////
#define IN_PARAMETER_ERROR      -1                    //�����������
#define IN_PATH_NOT_EXIST       -2                    //·��������
#define FIND_FILE_HANDLE_VALUE  -3                    //�����ļ�ʧ��
#define DIR_TRAVERSAL_FAIL      -4                    //DirectoryTraversal����ʧ��
namespace RG
{
	//����Ŀ¼���ýṹ
	typedef struct TRAVERSAL_TAG
	{
		bool bIsCompletePath;                  //���봦��Ŀ¼�Ƿ���Ҫ�������ļ����� false ������Ҫ ����·��  true ��ʾֻ��Ҫ ������·�����ļ���
		bool bIsHandleFile;                    //���������ļ�/�ļ���  false �ļ�  true �ļ���  ��Ϊtrueʱ iDirDepthֵ�������� tszFileTpyeΪ*
		int iDirDepth;                         //����Ŀ¼���/��� 1��ʾֻ��ptInPath��  2 ptInPath��һ��  0��ʾ������Ŀ¼
		int iTimeInterval;                     //��α���ʱ��ʱ����  ���߳���ʵ�� ����Ŀ¼��� 0��ʾֻ����һ�α���
		TCHAR tszFileTpye[32];                 //������Ҫ�����ļ���׺�� *��ʾ�����ļ�����  **��ʾ��Ҫ�����Ϊ�ļ��� 
		TCHAR tszTPath[MAX_PATH];              //������Ŀ¼��

		TRAVERSAL_TAG(){Reset();}
		~TRAVERSAL_TAG(){Reset();}

		void Reset()
		{
			memset(this, 0, sizeof(*this));
			bIsHandleFile = false;
			bIsCompletePath = false;
			iDirDepth = 0;
			iTimeInterval = 0;
			_tcscpy_s(tszFileTpye, 32, _T("*"));
			memset(tszTPath, 0, MAX_PATH*sizeof(TCHAR));
		}

		//Init��ʼ���������� 
		// PTCHAR ptInPath               ������Ŀ¼��
		// PTCHAR ptInFileTpye           ������Ҫ�����ļ���׺�� *��ʾ�����ļ�����  **��ʾ��Ҫ�����Ϊ�ļ���
		// iInDirDepth                   ����Ŀ¼���/��� 1��ʾֻ��ptInPath��  2 ptInPath��һ��  0��ʾ������Ŀ¼ Ĭ��0
		// int iInterval                 ��α���ʱ��ʱ����  ���߳���ʵ�� ����Ŀ¼��� 0��ʾֻ����һ�α��� Ĭ��0
		// bInIsHandleFile               ���������ļ�/�ļ���  false �ļ�  true �ļ��� Ĭ��false
		void Init(const PTCHAR ptInPath, const PTCHAR ptInFileTpye = NULL, const int iInDirDepth = 0, const int iInterval = 0)
		{
			iDirDepth = iInDirDepth;
			iTimeInterval = iInterval;

			if (NULL == ptInFileTpye)
				_tcscpy_s(tszFileTpye, 32, _T(".*"));
			else
				_tcscpy_s(tszFileTpye, 32, ptInFileTpye);
			//�ļ���
			if (bIsHandleFile)
			{
				_tcscpy_s(tszFileTpye, 32, _T(".*"));
				iDirDepth = 0;
			}

			_tcscpy_s(tszTPath, MAX_PATH, ptInPath);
		}
	}TRAVERSAL, * PTRAVERSAL;

	class CDirectoryTraversalUtil
	{
	public:
		CDirectoryTraversalUtil() {Reset();}
		~CDirectoryTraversalUtil(){Reset();}

	public:
		//�ļ�/�ļ��д�����  ���麯�� ��Ҫ�̳�
		virtual bool TraversalDoWork(const PTCHAR ptInFilePath) = 0;  

		//��ʼ������ 
		// ptInPath ������Ŀ¼�� 
		// ptInFileTpye ������ȡ���ļ���׺  NULL��ʾ�����ļ����� **��ʾ��Ҫ�����Ϊ�ļ���
		// iInDirDepth ����Ŀ¼����� 
		// iInterval ������ʱ���� ��λ ��
		// bInIsHandleFile  ���������ļ�/�ļ���  false �ļ�  true �ļ��� Ĭ��false
		bool Init(const PTCHAR ptInPath, const PTCHAR ptInFileTpye = NULL, const int iInDirDepth = 0, const int iInterval = 0)
		{
			//�������Ƿ���ȷ
			if (!ptInPath || _tcsclen(ptInPath)<4 || iInDirDepth<0 || iInterval<0)
				return false;

			m_stuTraversal.Init(ptInPath, ptInFileTpye, iInDirDepth, iInterval);

			return true;
		}

		//��ʼ���� 
		int StartTraversal()
		{
			int iRet = 0;
			int iSleepTime = 0;
			m_bExitFlag = true;
			iSleepTime = m_stuTraversal.iTimeInterval?m_stuTraversal.iTimeInterval*1000:1;
			while(m_bExitFlag)
			{
				iRet = DirectoryTraversal(m_stuTraversal.tszTPath, m_stuTraversal.tszFileTpye, m_stuTraversal.iDirDepth);
				if (iRet<0)
				{
					//...����ʧ��..������  �ݲ�����
				}
				if (!m_stuTraversal.iTimeInterval) break;
				TRACE(_T("������ɣ�����%d����ٴα�������������\r\n"), iSleepTime/1000);

				Sleep(iSleepTime);
			}

			return iRet;
		}

		//ֹͣ����
		void StopTraversal(){m_bExitFlag = false; Reset();}

		///////���� ��������///////
		//���ô�����TraversalDoWork������Ƿ�Ϊ����·��  Ĭ�ϲ����� ��������·��  bInIsCompletePath true ��ʾֻ�����ļ���
		void SetDoworkPath(bool bInIsCompletePath = true)
		{
			m_stuTraversal.bIsCompletePath = bInIsCompletePath;
		}

		//���ô�����TraversalDoWork ���������ļ������ļ��� bInIsHandleFile  false �ļ�  true �ļ���  
		//Ĭ�ϲ����� �����ļ�
		void SetDoWorkIsFile(bool bInIsHandleFile = true)
		{
			m_stuTraversal.bIsHandleFile = bInIsHandleFile;
		}
	private:


		//һ�α���Ŀ¼
		int DirectoryTraversal(const PTCHAR ptInPath, const PTCHAR ptInFileTpye, const int iInDirDepth = 0)
		{
			HANDLE hFind = NULL;
			WIN32_FIND_DATA stuWFD = {0};	
			TCHAR tszFullPath[MAX_PATH] = {0};
			TCHAR tszFindFilter[MAX_PATH] = {0};                           //�����ļ�����
			PTCHAR ptPath = NULL;
			DWORD dwAttributes = 0;

			if (!(ptInPath && ptInFileTpye))	return IN_PARAMETER_ERROR;

			//��鴴�����ļ���·��
			if (!CheckDirExist(ptInPath))
			{
				return IN_PATH_NOT_EXIST;
			}

			if (!m_stuTraversal.bIsCompletePath)
			{
				ptPath = tszFullPath;
			}
			else
			{
				ptPath = stuWFD.cFileName; 
			}

			_stprintf_s(tszFindFilter, MAX_PATH, _T("%s\\*%s"), ptInPath, ptInFileTpye);    

			if ((hFind = FindFirstFile(tszFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
			{
				return FIND_FILE_HANDLE_VALUE;
			}

			do
			{
				if (!m_bExitFlag) break;
				//���˱���Ŀ¼�͸�Ŀ¼
				if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
					continue;

				ZeroMemory(tszFullPath, sizeof(tszFullPath));
				_stprintf_s(tszFullPath, MAX_PATH, _T("%s\\%s"), ptInPath, stuWFD.cFileName);  

				//����ҵ�����Ŀ¼
				if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{	
					if (!m_stuTraversal.bIsHandleFile)
					{
						if (1 == iInDirDepth) continue;

						int iDirDepth = iInDirDepth?(iInDirDepth-1):0;
						if (1 != DirectoryTraversal(tszFullPath, ptInFileTpye, iDirDepth))
						{
							FindClose(hFind);
							return DIR_TRAVERSAL_FAIL;
						}

						continue;
					}

					if (!TraversalDoWork(ptPath))
					{
						FindClose(hFind);
						return DIR_TRAVERSAL_FAIL;
					}
				}
				else 
				{//�ҵ������ļ�
					if (m_stuTraversal.bIsHandleFile) continue;
					if (!TraversalDoWork(ptPath))
					{
						FindClose(hFind);
						return -5;
					}
				}	
			}while (FindNextFile(hFind, &stuWFD));

			FindClose(hFind);
			return 1;
		}

		//����
		void Reset()
		{
			m_bExitFlag = false;                             //�˳���־
			memset(&m_stuTraversal, 0, sizeof(TRAVERSAL));
			m_stuTraversal.Reset();
		}

	private:	
		bool m_bExitFlag;                                //�˳���־  true ִ�б��� false ֹͣ����
		TRAVERSAL m_stuTraversal;                        //Ŀ¼�������ýṹ
	};
}
#endif

