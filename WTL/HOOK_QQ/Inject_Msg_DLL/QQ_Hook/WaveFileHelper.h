#pragma once
#include "file.h"
namespace RG
{

#define DATA_BUFFER_LEN 1024*1024*4
	class CWaveFileHelper
	{
	public:

		CWaveFileHelper(void)
		{
			m_pPoint = NULL;
			m_dwDataLen = 0;
			m_pszbyData = NULL;
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
			m_pszbyData = new BYTE[DATA_BUFFER_LEN];
			if (!m_pszbyData) return false;
			memset(m_pszbyData, 0, DATA_BUFFER_LEN);
			if (!pszInFilePath) return false;
			if (_tcslen(pszInFilePath)<4) return false;

			if (!m_clsFile.Open(tstring(pszInFilePath), "wb"))
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
				WriteWaveHead();
				m_clsFile.Write(m_pszbyData, m_dwDataLen);
				m_clsFile.Close();
			}
			m_dwDataLen = 0;
			if (m_pszbyData)
			{
				memset(m_pszbyData, 0, DATA_BUFFER_LEN);
				delete m_pszbyData;
				m_pszbyData = NULL;
			}	
			//memset(&m_stuWaveFormat, 0, sizeof(WAVEFORMATEX));
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
				if (DATA_BUFFER_LEN-m_dwDataLen >= nBufLen)
				{
					memcpy_s(m_pszbyData+m_dwDataLen, DATA_BUFFER_LEN-m_dwDataLen, pbyBuf, nBufLen);
					m_dwDataLen += nBufLen;
					return true;
				}
			}

			return false;
		}

		bool SaveFile()
		{
			WriteWaveHead();
			m_clsFile.Write(m_pszbyData, m_dwDataLen);
		}
	private:
		bool WriteWaveHead()
		{
			//DWORD dwWaveHeaderSize = 38;  
			DWORD dwWaveFormatSize = 4+4+4+4+4;
			dwWaveFormatSize += sizeof(WAVEFORMATEX);  
			DWORD dwWaveFileSize = m_dwDataLen + dwWaveFormatSize;//38;//dwWaveHeaderSize;
			
			if (m_dwDataLen<=0)
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
			m_clsFile.Write((PBYTE)&m_dwDataLen, 4);
		}


	private:
		BYTE* m_pszbyData;
		DWORD m_dwDataLen;      //Date数据内容长度
		WAVEFORMATEX m_stuWaveFormat;
		RG::CFile m_clsFile;
		PVOID m_pPoint;        //
	};

}
