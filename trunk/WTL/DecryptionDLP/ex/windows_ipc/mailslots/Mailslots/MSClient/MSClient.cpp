/*
==========================================================================

Purpose:

This is a sample code that demonstrates for the following:

* Working with mailslot server as a client

Notes:

* 

Author:

* Swarajya Pendharkar

Date:

* 6th April 2006

Updates:

* 
                      
==========================================================================
*/

#include "stdafx.h"
#include "windows.h"

#define g_szMailslot "\\\\.\\mailslot\\MyMailSlot"  //Name given to the Mailslot
//Mailslot name format - \\.\mailslot\mailslotname
//This mailslot is for server on the same computer, however, mailslots can be used to
//connect to a remote server

#define BUFFER_SIZE 1024 //1k

int main(int argc, char* argv[])
{
     HANDLE hMailslot;
     
     //Connect to the server mailslot using CreateFile()
     hMailslot = CreateFile( 
          g_szMailslot,          // mailslot name 
          GENERIC_WRITE,         // mailslot write only 
          FILE_SHARE_READ,       // required for mailslots
          NULL,                  // default security attributes
          OPEN_EXISTING,         // opens existing mailslot 
          FILE_ATTRIBUTE_NORMAL, // normal attributes 
          NULL);                 // no template file 
     
     if (INVALID_HANDLE_VALUE == hMailslot) 
     {
          printf("\nError occurred while connecting to the server: %d", GetLastError()); 
          return 1;  //Error
     }
     else
     {
          printf("\nCreateFile() was successful.");
     }
     
     //We are done connecting to the mailslot, 
     //Mailslot communication is one-way, client will just write to mailslot
     //Using WriteFile()
     
     char szBuffer[BUFFER_SIZE];
     
     printf("\nEnter a message to be sent to the server: ");
     gets(szBuffer);
     
     DWORD cbBytes;
     
     //Send the message to server
     BOOL bResult = WriteFile( 
          hMailslot,            // handle to mailslot 
          szBuffer,             // buffer to write from 
          strlen(szBuffer)+1,   // number of bytes to write, include the NULL
          &cbBytes,             // number of bytes written 
          NULL);                // not overlapped I/O 
     
     if ( (!bResult) || (strlen(szBuffer)+1 != cbBytes))
     {
          printf("\nError occurred while writing to the server: %d", GetLastError()); 
          CloseHandle(hMailslot);
          return 1;  //Error
     }
     else
     {
          printf("\nWriteFile() was successful.");
     }
     
     CloseHandle(hMailslot);
     return 0; //Success
}