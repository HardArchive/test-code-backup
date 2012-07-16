#include <stdio.h>
#include "packet32.h"
#include "winpcap_common.h"


int open_adapter(LPADAPTER *lpAdapter) 
{
  /* ASCII strings for WIN9x */
  char  AdapterNameA[ADAPTER_NAMES_SIZE];
  char  *TmpAdapterA;
  char  *AnotherTmpAdapterA; 
  
  /* UNICODE strings for WINNT */
  WCHAR  AdapterNameU[ADAPTER_NAMES_SIZE];
  WCHAR  *TmpAdapterU;
  WCHAR  *AnotherTmpAdapterU; 
  
  /* Others */
  char  AdapterList[MAX_ADAPTER_NUM][ADAPTER_NAMES_SIZE];
  int   AdapterNum;
  int   i = 0;
  int   SelectedAdapter;
  ULONG AdapterLength = ADAPTER_NAMES_SIZE;
  DWORD Version;
  DWORD WindowsMajorVersion;


  /* List adapter names */
  printf("\nAdapters installed :\n");

  /* Get operating system version */
  Version = GetVersion();
  WindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(Version)));

  /* Get adapter names */
  if(!(Version >= 0x80000000 && WindowsMajorVersion >= 4)) {
    // Windows NT
    if(PacketGetAdapterNames((PTSTR)AdapterNameU, &AdapterLength) == FALSE) {
      printf("Unable to retrieve the list of adapters!\n");

      return(EXIT_FAILURE);
    }

    TmpAdapterU = AdapterNameU;
    AnotherTmpAdapterU = AdapterNameU;

    while((*TmpAdapterU != '\0') || (*(TmpAdapterU - 1) != '\0')) {
      if(*TmpAdapterU == '\0') {
        memcpy(AdapterList[i], AnotherTmpAdapterU, (TmpAdapterU - AnotherTmpAdapterU ) * 2);
		AdapterList[i][(TmpAdapterU - AnotherTmpAdapterU ) * 2] = '\0';
        AnotherTmpAdapterU = TmpAdapterU + 1;
        i++;
      }
				
      TmpAdapterU ++;
    }
			
    AdapterNum = i;

    for(i = 0 ; i < AdapterNum ; i++) {
      wprintf(L"\n%d- %s\n", i + 1, AdapterList[i]);
    }						
  }		
  else {
    // Windows 9X
    if(PacketGetAdapterNames((PTSTR)AdapterNameA, &AdapterLength) == FALSE) {
      printf("Unable to retrieve the list of the adapters!\n");

      return(EXIT_FAILURE);
    }

    TmpAdapterA = AdapterNameA;
    AnotherTmpAdapterA = AdapterNameA;
			
    while((*TmpAdapterA != '\0') || (*(TmpAdapterA - 1) != '\0')) {
      if(*TmpAdapterA == '\0') {				
        memcpy(AdapterList[i], AnotherTmpAdapterA, TmpAdapterA - AnotherTmpAdapterA);
		AdapterList[i][TmpAdapterA - AnotherTmpAdapterA] = '\0';
        AnotherTmpAdapterA = TmpAdapterA + 1;
        i++;
      }
				
      TmpAdapterA ++;
    }
			
    AdapterNum = i;

    for(i = 0 ; i < AdapterNum ; i++) {
      printf("\n%d- %s\n", i + 1, AdapterList[i]);				
    }
  }

  /* Select an adapter */
  do {
    printf("\nSelect the number of the adapter to open : ");
    scanf("%d", &SelectedAdapter);
    
    if((SelectedAdapter > AdapterNum) || (SelectedAdapter <= 0)) {
      printf("\nWrong selection\n"); 
	}

  } while((SelectedAdapter > AdapterNum) || (SelectedAdapter <= 0));
      
  *lpAdapter =  PacketOpenAdapter(AdapterList[SelectedAdapter - 1]);

  if(!(*lpAdapter) || ((*lpAdapter)->hFile == INVALID_HANDLE_VALUE)) {    
    fprintf(stderr, "\nError : unable to open the driver\n"); 
    
    return(EXIT_FAILURE);
  }  

  return(EXIT_SUCCESS);
}


void close_adapter(LPADAPTER lpAdapter) 
{ 
  PacketCloseAdapter(lpAdapter);
}
