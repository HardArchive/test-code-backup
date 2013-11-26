#pragma once
#include "file.h"
namespace RG
{
//#define GETCURRENTTIME COleDateTime::GetCurrentTime().Format("%Y-%m-%d-%H:%M:%S")
#define GETCURRENTTIME COleDateTime::GetCurrentTime().Format("%H:%M:%S")
//#define DATA_BUFFER_LEN 1024*1024*4
#define DATA_BUFFER_LEN 1024*4

	//获取当前程序所在目录 成功返回true，失败返回false
	inline bool GetExePath(PTCHAR ptInPath)
	{
		PTCHAR ptTem = NULL;
		TCHAR tszTemp[MAX_PATH] = {0};
		//获取当前目录  //这里是获取当前进程文件的完整路径 
		if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
			return false; 

		ptTem = _tcsrchr(tszTemp, _T('\\'));
		memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
		return true;
	}


	typedef struct TEM_FILE_TAG
	{
		bool bIsCloseFlag;       //关闭标志
		RG::CFile clsFile;
		TCHAR tszTemFileName[MAX_PATH];
		TEM_FILE_TAG()	{Release();}
		~TEM_FILE_TAG() {Release();}

		bool CreateFile()
		{
			if (_tcslen(tszTemFileName))
			{
				DeleteFile(tszTemFileName);
				memset(tszTemFileName, 0, sizeof(TCHAR)*MAX_PATH);
			}

			if (!OpenFile())
			{
				return false;
			}

			return true;
		}

		DWORD AddData(PBYTE pbyBuf, const size_t nBufLen)
		{
			if (!clsFile.IsOpened())
			{
				if (!CreateFile())
					return 0;
			}
			
			return clsFile.Write(pbyBuf, nBufLen);
		}

		DWORD GetData(PBYTE pbyBuf, const size_t nBufLen)
		{
			if (!bIsCloseFlag)
			{
				clsFile.SeekStart();
				bIsCloseFlag = true;
			}
			return clsFile.Read(pbyBuf, nBufLen);
		}

		DWORD GetFileLen()
		{
			DWORD dwFileLen = 0;
			if (clsFile.IsOpened())
			{
				dwFileLen = clsFile.Length();
			}
			return dwFileLen;
		}

		inline void Release()
		{
			CloseFile();
			if (_tcslen(tszTemFileName))
			{
				DeleteFile(tszTemFileName);
				memset(tszTemFileName, 0, sizeof(TCHAR)*MAX_PATH);
			}
		}

	private:
		inline bool OpenFile()
		{
			if (_tcslen(tszTemFileName)<=0)
			{
				if (!GetExePath(tszTemFileName))
				{
					return false;
				}
				_stprintf_s(tszTemFileName, MAX_PATH, _T("%s\\wav.tm"), tszTemFileName/*, GETCURRENTTIME*/);
			}

			//a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。
			if (!clsFile.Open(tstring(tszTemFileName), "wb+"))//rb+
			{
				int iError = GetLastError();
				return false;
			}

			return true;
		}

		inline void CloseFile()
		{
			bIsCloseFlag = false;
			if (clsFile.IsOpened())
			{				
				clsFile.Close();
			}
		}
	}TEMFILE, *PTEMFILE;


	class CWaveFileHelper
	{
	public:

		CWaveFileHelper(void)
		{
			m_pPoint = NULL;
			memset(&m_stuWaveFormat, 0, sizeof(WAVEFORMATEX));
		}

		~CWaveFileHelper(void)
		{
			WaveClose();
			memset(&m_stuWaveFormat, 0, sizeof(WAVEFORMATEX));
		}

	public:
		bool WaveCreateFile(TCHAR* pszInFilePath)
		{
			if (!pszInFilePath) return false;
			if (_tcslen(pszInFilePath)<4) return false;

			if (m_clsFile.IsOpened())
			{
				WaveClose();
			}

			//wb+ 读写打开或建立一个二进制文件，允许读和写。
			if (!m_clsFile.Open(tstring(pszInFilePath), "wb+"))//rb+
			{
				//TRACE("^&^! OpenFile Open 打开文件:%s出错！！！\r\n", strFileName.c_str());
				return false;
			}

			return true;
		}

		void WaveClose()
		{
			m_pPoint = NULL;
			if (m_clsFile.IsOpened())
			{
				SaveFile();
				m_clsFile.Close();
			}
			m_stuTemFile.Release();
			memset(&m_stuWaveFormat, 0, sizeof(WAVEFORMATEX));
		}


		void SetWaveFormat(WAVEFORMATEX& stuInWaveFormat)
		{
			m_stuWaveFormat = stuInWaveFormat;
		}

		bool AddWaveData(PBYTE pbyBuf, const size_t nBufLen)
		{
			if (m_pPoint != pbyBuf)
			{	
				m_pPoint = pbyBuf;				
				return m_stuTemFile.AddData(pbyBuf, nBufLen) > 0;
			}
			return false;
		}


	private:
		bool WriteWaveHead()
		{
			DWORD dwDataLen = m_stuTemFile.GetFileLen();
			DWORD dwWaveFormatSize = 4+4+4+4+4;
			dwWaveFormatSize += sizeof(WAVEFORMATEX);  
			DWORD dwWaveFileSize = dwDataLen + dwWaveFormatSize;
			
			if (dwDataLen<=0)
			{
				return false;
			}

			m_clsFile.SeekStart();
			// riff block 
			m_clsFile.Write((PBYTE)"RIFF", 4);
			m_clsFile.Write((PBYTE)&dwWaveFileSize, 4);
			m_clsFile.Write((PBYTE)"WAVE", 4);
			// fmt block 
			m_clsFile.Write((PBYTE)"fmt ", 4);
			dwWaveFormatSize = sizeof(WAVEFORMATEX); //18 
			m_clsFile.Write((PBYTE)&dwWaveFormatSize, sizeof(DWORD));
			m_clsFile.Write((PBYTE)&m_stuWaveFormat, sizeof(WAVEFORMATEX));
			// data block 
			m_clsFile.Write((PBYTE)"data", 4);
			m_clsFile.Write((PBYTE)&dwDataLen, 4);

			return true;
		}	

		bool SaveFile()
		{
			if (!WriteWaveHead()) return false;
	
			BYTE szbyDateBuf[DATA_BUFFER_LEN] = {0};
			DWORD dwReadLen = 0;
			DWORD dwDataLen = m_stuTemFile.GetFileLen();

			while (dwDataLen>0)
			{
				memset(szbyDateBuf, 0, DATA_BUFFER_LEN);
				dwReadLen = m_stuTemFile.GetData(szbyDateBuf, DATA_BUFFER_LEN);
				if (dwReadLen<=0)
				{
					int iError = GetLastError();
					return false;
				}
				if (m_clsFile.Write(szbyDateBuf, dwReadLen)<=0)
				{
					return false;
				}
				dwDataLen -= DATA_BUFFER_LEN;
			}

			return true;
		}

	private:
		RG::CFile m_clsFile;
		RG::TEMFILE m_stuTemFile;    //临时文件
		PVOID m_pPoint;          //用于忽略第一次输入的数据，因为waveOutWrite会两次调用导致数据重复
		WAVEFORMATEX m_stuWaveFormat;
	};

}
