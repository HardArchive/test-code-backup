#include <string> 
#include <AtlComTime.h>
#include "Mmsystem.h"
#include <list>
BOOL  __declspec(dllexport) WINAPI Transform2Winsrc( LPSTR  szAccount ,LPSTR  content,LPSTR  sender,LPSTR  senderAccount,LPSTR  receiver,LPSTR  receiverAccount );

//“Ù∆µΩÿªÒ
typedef MMRESULT (WINAPI * PFN_waveInOpen)(
	LPHWAVEIN       phwi,      
	UINT_PTR       uDeviceID,  
	LPWAVEFORMATEX pwfx,       
	DWORD_PTR      dwCallback, 
	DWORD_PTR      dwCallbackInstance, 
	DWORD          fdwOpen     
	);
typedef MMRESULT (WINAPI * PFN_waveOutOpen)(
  LPHWAVEOUT     phwo,      
  UINT_PTR       uDeviceID, 
  LPWAVEFORMATEX pwfx,      
  DWORD_PTR      dwCallback,
  DWORD_PTR      dwCallbackInstance,
  DWORD          fdwOpen    
);
typedef MMRESULT (WINAPI* PFN_waveOutWrite)(
  HWAVEOUT hwo,  
  LPWAVEHDR pwh, 
  UINT cbwh      
);

//


LRESULT CALLBACK WndProc(int nCode, WPARAM wParam, LPARAM lParam);