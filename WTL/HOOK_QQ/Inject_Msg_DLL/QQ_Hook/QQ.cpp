#include "stdafx.h"
#include "detours.h"

#pragma data_seg("Shared")
HMODULE	hModDll	= NULL;
HHOOK	hHookKey = NULL;
#pragma data_seg()
#pragma comment(linker,"/Section:Shared,rws") 
BOOL m_inHook = FALSE;
int g_item = 0;
PFN_waveInOpen  True_PFN_waveInOpen = NULL;
PFN_waveOutOpen  True_PFN_waveOutOpen = NULL;
PFN_waveOutWrite  True_PFN_waveOutWrite = NULL;

//音频接获
typedef void (CALLBACK * PFN_SoundRecordCallback)(HWAVEIN hWaveIn, UINT nMessage, DWORD dwUserData, DWORD wParam, DWORD lParam);
typedef void (CALLBACK * PFN_SoundPlayCallback)(HWAVEOUT hWaveOut, UINT nMessage, DWORD dwUserData, DWORD wParam, DWORD lParam);
DWORD g_dwInCallback = 0;
DWORD g_dwOutCallback = 0;
char szRecordWavPath[MAX_PATH] = {0};
char szPlayWavPath[MAX_PATH] = {0};
char* m_dataBuf = NULL;
int m_dataLen = 0;
WAVEFORMATEX    format;
#include "WaveFileHelper.h"

RG::CWaveFileHelper g_clsWaveFileHelper;

void saveFile()  
{  
    //////////////////////////////////////////////////////////////////////////   
    //CFile m_file;  
    //CFileException fileException;  
    //CString m_csFileName= "c:\\test1.wav";  
    //m_file.Open(m_csFileName,CFile::modeCreate|CFile::modeReadWrite, &fileException);  
    /*if (m_file.Open(m_csFileName,CFile::modeCreate|CFile::modeReadWrite, &fileException)) 
    {MessageBox("open file"); 
}*/  
    HANDLE hLogFile= ::CreateFileA("c:\\test2.wav",
						    GENERIC_WRITE,
						    FILE_SHARE_READ,
						    NULL,CREATE_ALWAYS,
						    FILE_ATTRIBUTE_NORMAL,
						    NULL);
						    
    DWORD m_WaveHeaderSize = 38;  
    DWORD m_WaveFormatSize = 18;  
    m_WaveFormatSize = sizeof(WAVEFORMATEX);  
    //m_file.SeekToBegin();  
  
    // riff block   
	long  h = 0;
	::SetFilePointer(hLogFile,0,&h,FILE_BEGIN);
	DWORD written = 0;
    BOOL bRet = ::WriteFile(hLogFile,"RIFF",4 ,&written,NULL);

   unsigned int Sec=(m_dataLen + m_WaveHeaderSize);  // 整个wav文件大小 - 8 （RIFF+SIZE）   
    ::WriteFile(hLogFile,&Sec,sizeof(Sec) ,&written,NULL);//m_file.Write(&Sec,sizeof(Sec));  
    ::WriteFile(hLogFile,"WAVE",4 ,&written,NULL);//m_file.Write("WAVE",4);  
  
    // fmt block   
    ::WriteFile(hLogFile,"fmt ",4 ,&written,NULL);//m_file.Write("fmt ",4);  
    ::WriteFile(hLogFile,&m_WaveFormatSize,sizeof(m_WaveFormatSize),&written,NULL);//m_file.Write(&m_WaveFormatSize, sizeof(m_WaveFormatSize));  
      
//  m_file.Write(&format.wFormatTag, sizeof(format.wFormatTag));   
//  m_file.Write(&format.nChannels, sizeof(format.nChannels));   
//  m_file.Write(&format.nSamplesPerSec, sizeof(format.nSamplesPerSec));   
//  m_file.Write(&format.nAvgBytesPerSec, sizeof(format.nAvgBytesPerSec));   
//  m_file.Write(&format.nBlockAlign, sizeof(format.nBlockAlign));   
//  m_file.Write(&format.wBitsPerSample, sizeof(format.wBitsPerSample));   
//  m_file.Write(&format.cbSize, sizeof(format.cbSize));   
    ::WriteFile(hLogFile,&format, sizeof(WAVEFORMATEX),&written,NULL);//m_file.Write(&format, sizeof(WAVEFORMATEX));  
  
    // data block   
   ::WriteFile(hLogFile,"data", 4,&written,NULL);// m_file.Write("data",4);  
    ::WriteFile(hLogFile,&m_dataLen,sizeof(DWORD),&written,NULL);//m_file.Write(&m_dataLen,sizeof(DWORD));  
    ::WriteFile(hLogFile,m_dataBuf,m_dataLen,&written,NULL);//m_file.Write(m_dataBuf,m_dataLen);  
  
    //m_file.Seek(m_dataLen,CFile::begin);  
    ::CloseHandle(hLogFile);//m_file.Close();  
    delete[] m_dataBuf;
    m_dataLen = 0;     
  
    return ;  
}

void CALLBACK SoundRecordCallback(HWAVEIN hWaveIn, UINT nMessage, DWORD dwUserData, DWORD wParam, DWORD lParam)
{
	//CSoundRecorder *pRecorder = (CSoundRecorder *) dwUserData;
    static DWORD nIndex = 0;
	static HANDLE hLogFile = INVALID_HANDLE_VALUE;
	switch (nMessage) {
		case WIM_OPEN:
			//pRecorder->OnOpen(hWaveIn, nMessage, wParam, lParam);
			::OutputDebugString("WIM_OPEN\n");
			break;
		case WIM_DATA:
			//pRecorder->OnData(hWaveIn, nMessage, wParam, lParam);
			::OutputDebugString("WIM_DATA\n");
			{
				PWAVEHDR pWaveHeader = (PWAVEHDR) wParam;

				if (pWaveHeader == NULL || pWaveHeader->dwBytesRecorded == 0) {
					break;
				}
				char szDebug[256] = {0};
				sprintf(szDebug,"len:%d\n",pWaveHeader->dwBytesRecorded);
				
				::OutputDebugString(szDebug);
				
				
                if (NULL == m_dataBuf){  
                    m_dataBuf = (char *) malloc(pWaveHeader->dwBytesRecorded+1);  
                    memcpy(m_dataBuf, pWaveHeader->lpData, pWaveHeader->dwBytesRecorded);  
                    m_dataBuf[pWaveHeader->dwBytesRecorded]=0;  
                    m_dataLen += pWaveHeader->dwBytesRecorded;  
                }  
                else{  
                    char* newbuf =(char *) realloc(m_dataBuf, m_dataLen+pWaveHeader->dwBytesRecorded);  
                    if (newbuf == NULL)  
                    {  
                        //waveInReset(hWaveIn);  
                        return ;  
                    }  
                    m_dataBuf = newbuf;  
                    CopyMemory(m_dataBuf+m_dataLen, pWaveHeader->lpData, pWaveHeader->dwBytesRecorded);  
                    m_dataLen += pWaveHeader->dwBytesRecorded;  
                }  

/*				if (hLogFile == INVALID_HANDLE_VALUE)
				{
				    sprintf(szRecordWavPath,"c:\\ARe%u.wav",nIndex);
					hLogFile= ::CreateFileA(szRecordWavPath,
											GENERIC_WRITE,
											FILE_SHARE_READ,
											NULL,CREATE_ALWAYS,
											FILE_ATTRIBUTE_NORMAL,
											NULL);
				}
				if(hLogFile == INVALID_HANDLE_VALUE)
					return;

				long  h = 0;
				::SetFilePointer(hLogFile,0,&h,FILE_END);
				DWORD written = 0;
				BOOL bRet = ::WriteFile(hLogFile,pWaveHeader->lpData,pWaveHeader->dwBytesRecorded ,&written,NULL);
				sprintf(szDebug,"written:%d %d\n",written,bRet);
				::OutputDebugString(szDebug);
				*/
					
			}
			break;
		case WIM_CLOSE:
			::OutputDebugString("WIM_CLOSE\n");
			/*
			if (hLogFile != INVALID_HANDLE_VALUE)
				::CloseHandle(hLogFile);
			hLogFile = INVALID_HANDLE_VALUE;*/
			if (m_dataLen != 0)
			    saveFile();
			nIndex ++;

			//pRecorder->OnClose(hWaveIn, nMessage, wParam, lParam);
			break;
	}
	((PFN_SoundRecordCallback)g_dwInCallback)(hWaveIn, nMessage, dwUserData, wParam, lParam);
}

void CALLBACK SoundPlayCallback(HWAVEOUT hWaveOut, UINT nMessage, DWORD dwUserData, DWORD wParam, DWORD lParam)
{
	//static HANDLE hLogFile = INVALID_HANDLE_VALUE;
	switch (nMessage) {
		case WOM_OPEN:
			::OutputDebugString("WOM_OPEN\n");
			g_clsWaveFileHelper.WaveCreateFile("c:\\test.wav");
			break;
		case WOM_DONE:
			{
			    ::OutputDebugString("WOM_DONE\n");
				//PWAVEHDR pWaveHeader = (PWAVEHDR) wParam;

				//if (pWaveHeader == NULL || pWaveHeader->dwBytesRecorded == 0) 
				//{
				//	break;
				//}
				//char szDebug[256] = {0};
				//sprintf(szDebug,"SoundPlayCallback len:%d\n",pWaveHeader->dwBytesRecorded);
				//
				//::OutputDebugString(szDebug);

				//if (hLogFile == INVALID_HANDLE_VALUE)
				//	hLogFile= ::CreateFileA("c:\\abc2",
				//							GENERIC_WRITE,
				//							FILE_SHARE_READ,
				//							NULL,CREATE_ALWAYS,
				//							FILE_ATTRIBUTE_NORMAL,
				//							NULL);
				//if(hLogFile == INVALID_HANDLE_VALUE)
				//	return;

				//long  h = 0;
				//::SetFilePointer(hLogFile,0,&h,FILE_END);
				//DWORD written = 0;
				//BOOL bRet = ::WriteFile(hLogFile,pWaveHeader->lpData,pWaveHeader->dwBytesRecorded ,&written,NULL);
				//sprintf(szDebug,"SoundPlayCallback written:%d %d\n",written,bRet);
				//::OutputDebugString(szDebug);			
					
			}
			break;
		case WOM_CLOSE:
			::OutputDebugString("WOM_CLOSE\n");
			g_clsWaveFileHelper.WaveClose();
			//if (hLogFile != INVALID_HANDLE_VALUE)
			//	::CloseHandle(hLogFile);
			//hLogFile = INVALID_HANDLE_VALUE;
			break;
	}
	((PFN_SoundPlayCallback)g_dwOutCallback)(hWaveOut, nMessage, dwUserData, wParam, lParam);
}


MMRESULT WINAPI Hook_waveInOpen(
  LPHWAVEIN       phwi,      
  UINT_PTR       uDeviceID,  
  LPWAVEFORMATEX pwfx,       
  DWORD_PTR      dwCallback, 
  DWORD_PTR      dwCallbackInstance, 
  DWORD          fdwOpen     
)
{

	char szDebug[256] = {0};
	sprintf(szDebug,
	"param %d %d %d %d %d %d %d\n",
	pwfx->cbSize,
	pwfx->nAvgBytesPerSec,
	pwfx->nBlockAlign,
	pwfx->nChannels,
	pwfx->nSamplesPerSec,
	pwfx->wBitsPerSample,
	pwfx->wFormatTag);
	::OutputDebugString(szDebug);
	::OutputDebugString("Hook_waveInOpen\n");

	DWORD dwCallbackProc = dwCallback;
	if (fdwOpen == CALLBACK_FUNCTION)
	{
		g_dwInCallback = dwCallbackProc;
		dwCallbackProc = (DWORD)SoundRecordCallback;	
	    format = *pwfx;
	}

	return (True_PFN_waveInOpen)(		
		       phwi,      
		       uDeviceID,  
		       pwfx,       
		      (DWORD)dwCallbackProc, 
		      dwCallbackInstance, 
		          fdwOpen     
		);

	return TRUE;


}

MMRESULT WINAPI Hook_waveOutOpen(
  LPHWAVEOUT     phwo,      
  UINT_PTR       uDeviceID, 
  LPWAVEFORMATEX pwfx,      
  DWORD_PTR      dwCallback,
  DWORD_PTR      dwCallbackInstance,
  DWORD          fdwOpen    
)
{
	DWORD dwCallbackProc = dwCallback;
	if ((CALLBACK_FUNCTION == fdwOpen) || (CALLBACK_FUNCTION+1 == fdwOpen))
	{
		g_dwOutCallback = dwCallbackProc;
		dwCallbackProc = (DWORD)SoundPlayCallback;
		//g_clsWaveFileHelper.WaveCreateFile("c:\\test.wav");
		g_clsWaveFileHelper.SetWaveFormat(*pwfx);
	}

	return (True_PFN_waveOutOpen)(		
		       phwo,      
		       uDeviceID,  
		       pwfx,       
		      /*dwCallback,*/(DWORD_PTR)dwCallbackProc, 
		      dwCallbackInstance, 
		          fdwOpen     
		);

	return TRUE;
}

MMRESULT WINAPI Hook_waveOutWrite(
  HWAVEOUT hwo,  
  LPWAVEHDR pwh, 
  UINT cbwh      
)
{
	//if (pwh->dwBytesRecorded > 0)
	//{
	//	static int iIndex = 0;
	//	char szPath[MAX_PATH] = {0};
	//	//sprintf(szPath,"c:\\testOut%d",iIndex);
	//	sprintf(szPath,"c:\\testOut.wav",iIndex);
	//	HANDLE	hLogFile= ::CreateFileA(szPath,
	//		GENERIC_WRITE,
	//		FILE_SHARE_READ,
	//		NULL,CREATE_ALWAYS,
	//		FILE_ATTRIBUTE_NORMAL,
	//		NULL);
	//	if(hLogFile != INVALID_HANDLE_VALUE)
	//	{

	//		long  h = 0;
	//		::SetFilePointer(hLogFile,0,&h,FILE_END);
	//		DWORD written = 1;
	//		BOOL bRet = ::WriteFile(hLogFile, pwh->lpData, pwh->dwBufferLength , &written, NULL);
	//		::CloseHandle(hLogFile);
	//		iIndex ++;
	//	}
	//}

	if (pwh->dwBufferLength > 0)
	{
		//char szPath[MAX_PATH] = {0};
		//sprintf(szPath,"c:\\testOut%d.wav", g_item++);
		//g_clsWaveFileHelper.WaveCreateFile(szPath);
		//g_item++;
		g_clsWaveFileHelper.AddWaveData((PBYTE)pwh->lpData, pwh->dwBufferLength);
		//g_clsWaveFileHelper.WaveClose();	
	}
    
	return (True_PFN_waveOutWrite)(		
      hwo,  
      pwh, 
      cbwh      
		);

	return TRUE;


}//



BOOL APIENTRY DllMain( HMODULE	hModule, 
                       DWORD	ul_reason_for_call, 
                       LPVOID	lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (::hModDll == NULL) ::hModDll = hModule;
		if (!m_inHook)
		{
		LONG lRet = 0;
			True_PFN_waveInOpen = (PFN_waveInOpen)GetProcAddress(GetModuleHandle("Winmm.dll"),"waveInOpen");   //为录音而打开一个波形输入设备
			True_PFN_waveOutOpen = (PFN_waveOutOpen)GetProcAddress(GetModuleHandle("Winmm.dll"),"waveOutOpen");//为播放打开一个波形输出设备
			True_PFN_waveOutWrite = (PFN_waveOutWrite)GetProcAddress(GetModuleHandle("Winmm.dll"),"waveOutWrite");	//向指定的波形输出设备发送一个数据块
			CString szDebug;
    		lRet = DetourTransactionBegin();
			lRet = DetourUpdateThread(GetCurrentThread());
		   if (True_PFN_waveInOpen)
		        lRet = DetourAttach(&(PVOID&)True_PFN_waveInOpen, (PBYTE)Hook_waveInOpen);
		   if (True_PFN_waveOutOpen)
		        lRet = DetourAttach(&(PVOID&)True_PFN_waveOutOpen, (PBYTE)Hook_waveOutOpen);
		   if (True_PFN_waveOutWrite)
		        lRet = DetourAttach(&(PVOID&)True_PFN_waveOutWrite, (PBYTE)Hook_waveOutWrite);
			lRet = DetourTransactionCommit();
			m_inHook = true;
		}
		break;
	case DLL_THREAD_ATTACH:
			//::OutputDebugStringA("DLL_THREAD_ATTACH\n");
		break;
	case DLL_THREAD_DETACH:
			//::OutputDebugStringA("DLL_THREAD_DETACH\n");
		break;
	case DLL_PROCESS_DETACH:
			//::OutputDebugStringA("DLL_PROCESS_DETACH\n");
		if (m_inHook)
		{
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
		   if (True_PFN_waveInOpen)
		        DetourDetach(&(PVOID&)True_PFN_waveInOpen, (PBYTE)Hook_waveInOpen);
		   if (True_PFN_waveOutOpen)
		        DetourDetach(&(PVOID&)True_PFN_waveOutOpen, (PBYTE)Hook_waveOutOpen);
		   if (True_PFN_waveOutWrite)
		        DetourDetach(&(PVOID&)True_PFN_waveOutWrite, (PBYTE)Hook_waveOutOpen);
			DetourTransactionCommit();
		}
		g_clsWaveFileHelper.WaveClose();

		break;
	}
	return TRUE;
}

//开始 Hook
HHOOK __declspec(dllexport) WINAPI InitHookProc(HWND dwHWND, DWORD dwTid)
{
	HHOOK hHookProc = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)WndProc, hModDll, dwTid);

	return hHookProc;
}

//结束Hook
BOOL __declspec(dllexport) WINAPI EndHook(HHOOK hHook)
{
	return UnhookWindowsHookEx(hHook);
}

//窗口过程Hook回调
LRESULT CALLBACK WndProc( int nCode, WPARAM wParam, LPARAM lParam)
{  
	return 0;
}
