/*******************************************************************/
/* Purpose : The purpose of the code is to demonstrate the IO Completion
/*			 Port technique. This example is to copy the very large files
/*           in a very short duration by using Asynchronous I/O,multi-threading
/*			 and IO Completion technique. 
/*******************************************************************/

#include <windows.h>
#include <iostream>

using std::cout;
using std::endl;

HANDLE hSource;
HANDLE hDestination;

DWORD WINAPI ThreadProc(LPVOID lpParameter);


void CopySourceToDestination(unsigned long fileSize);

HANDLE hIOCompPort;

unsigned long read = 0;
unsigned long write = 1;

int g_NoOfThreads = 0;

// Define the size of the buffers 
// We have taken as 8192 Bytes.
#define BUFFER_SIZE (8*1024)

// An array of this structure will be used by the spawned writer threads.
// This structure will contain the info about the offset positio at which
// the write operation will start and no of bytes which will be written from
// offset position.
typedef struct BUFFER_DATA
{
	OVERLAPPED overLapped;
    LPVOID lpData;
	DWORD dwDataLen;
	BUFFER_DATA()
	{
		ZeroMemory(&overLapped,sizeof(OVERLAPPED));
		lpData = NULL;
	}
	
}	BUFFER_DATA,* PBUFFER_DATA;


DWORD g_pageSize = 0;                     //系统内存分页大小
 
PBUFFER_DATA g_lpBufferData = NULL;
LPHANDLE	g_lpHandle = NULL;

int main(int argc, char ** argv)
{

	DWORD fileSize = 0;
	DWORD destFileSize = 0;
	SYSTEM_INFO sysInfo;
	
	LPVOID lpError = NULL;
	

	ZeroMemory(&sysInfo,sizeof(SYSTEM_INFO));

	// Retrieve system information
	GetSystemInfo(&sysInfo);
	g_pageSize = sysInfo.dwPageSize;

	// Create an file handle for the source file.
	// FLAG FILE_FLAG_NO_BUFFERING will ensure that all the read and write
	// commands should be submitted as asynchronous.
	hSource = CreateFile(/*"D:\\WinPath\\desktop\\NetDetective.sql",*/argv[1],
							GENERIC_READ | GENERIC_WRITE,
							NULL,
							NULL,
							OPEN_EXISTING,
							FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,
							NULL);

	if (hSource == INVALID_HANDLE_VALUE)
	{
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						0, // Default language
						(LPTSTR) &lpError,
						0,
						NULL );
		cout<<"::Error Occured::Source File::"<<(char*)lpError;
		return -1;
	}

	fileSize = GetFileSize(hSource,NULL);

	if (GetLastError() != NO_ERROR)
	{
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						0, // Default language
						(LPTSTR) &lpError,
						0,
						NULL );
		cout<<"::Error Occured::Reading Source File::"<<(char*)lpError;

	}

	hDestination = CreateFile(/*"D:\\WinPath\\desktop\\Source_Code\\Debug\\abc2.txt",*/argv[2],
								GENERIC_READ | GENERIC_WRITE,
								NULL,
								NULL,
								OPEN_EXISTING,
								FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING,
								NULL);
	if (hDestination == INVALID_HANDLE_VALUE)
	{
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						0, // Default language
						(LPTSTR) &lpError,
						0,
						NULL );
		cout<<"::Error Occured::Destination File::"<<(char*)lpError;
		return -1;
	}

	// Extending the size of the destination file. Read MSDN for more detail.
	destFileSize = (fileSize + g_pageSize - 1) & ~(g_pageSize-1);
	//在一个文件中设置当前的读写位置
	DWORD dwStatus = SetFilePointer(hDestination,
									destFileSize,
									NULL,
									FILE_BEGIN);

	if ((dwStatus == 0xFFFFFFFF) || (GetLastError() != NO_ERROR))
	{
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						0, // Default language
						(LPTSTR) &lpError,
						0,
						NULL );
		cout<<"::Error Occured::Setting File Pointer::Destination File::"<<(char*)lpError;
		return -1;

	}
	//针对一个打开的文件，将当前文件位置设为文件末尾 
	BOOL bReturn = SetEndOfFile(hDestination);

	if (!bReturn) {
		cout<<"::Error Occured::Setting EOF Pointer::Destination File::";
    }

	// Associate Source file with IO Completion Port.
	hIOCompPort = CreateIoCompletionPort(hSource,
											NULL,
											read,
											10);

	if (hIOCompPort == INVALID_HANDLE_VALUE)
	{
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						0, // Default language
						(LPTSTR) &lpError,
						0,
						NULL );
		cout<<"::Error Occured::IO Complete Port Initialization For Read::"<<(char*)lpError;
		return -1;
	}
						
	// Associate Destination File with IO Completion Port.
	hIOCompPort = CreateIoCompletionPort(hDestination,
											hIOCompPort,
											write,
											10);

	if (hIOCompPort == INVALID_HANDLE_VALUE)
	{
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						0, // Default language
						(LPTSTR) &lpError,
						0,
						NULL );
		cout<<"::Error Occured::IO Complete Port Initialization For Write::"<<(char*)lpError;
		return -1;
	}

	// Number of threads spawned to perform write operation on destination file.
	g_NoOfThreads = destFileSize/BUFFER_SIZE;


	g_lpBufferData = new BUFFER_DATA[g_NoOfThreads];
	g_lpHandle	 = new HANDLE[g_NoOfThreads];

	// Spawing writer threads which will wait for IO packets to be dispatched
	// on IO Completion Port.
	for (int iThreads = 0; iThreads < g_NoOfThreads; iThreads++)
	{
		g_lpHandle[iThreads] = CreateThread(NULL,
										  NULL,
										  ThreadProc,
										  NULL,
										  0,
										  NULL);
											
										  		
	}

	// Issuing asynchoronus IO Read request, which will dispatch the IO Packets
	// once the IO requested gets served.
	CopySourceToDestination(fileSize);

	//Wait for all the thread objects to get signaled.
	// This is not required in server applications as each thread serves client
	// independently and hence no need to wait.
	// In our example, we need this because all the writer threads should
	// finish their work before the application exits.
	WaitForMultipleObjects(g_NoOfThreads,g_lpHandle,TRUE,60000);

	CloseHandle(hDestination);
	CloseHandle(hSource);

	return 0;

}

void CopySourceToDestination(unsigned long fileSize) 
{
	DWORD fileReadPosition = 0;
	BOOL bSuccess = false;
	//DWORD bytesRead ;


	// Issuing asynchronous I/O Read calls.
	for (int iReadAsynOperations = 0; iReadAsynOperations < g_NoOfThreads ; iReadAsynOperations++)
	{
		g_lpBufferData[iReadAsynOperations].lpData = VirtualAlloc( NULL,
																 BUFFER_SIZE,
																 MEM_COMMIT,
																 PAGE_READWRITE);
		if ( g_lpBufferData[iReadAsynOperations].lpData == NULL)
		{
			cout<<"::Error While Allocating Memory::"<<endl;
			return ;
		}


		// Initializing the BUFFER_DATA structure for each thread, which will be used
		// by the writer thread.
		g_lpBufferData[iReadAsynOperations].overLapped.Offset = fileReadPosition;
        g_lpBufferData[iReadAsynOperations].overLapped.OffsetHigh = 0;
        g_lpBufferData[iReadAsynOperations].overLapped.hEvent = NULL; // not needed

		bSuccess = ReadFile(hSource,
                           g_lpBufferData[iReadAsynOperations].lpData,
                           BUFFER_SIZE,
                           &g_lpBufferData[iReadAsynOperations].dwDataLen,
                           &g_lpBufferData[iReadAsynOperations].overLapped);

		if (!bSuccess && (GetLastError() != ERROR_IO_PENDING)) {
			LPVOID lpError;
			FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						0, 
						(LPTSTR) &lpError,
						0,
						NULL );
            cout<<"::Error While Reading File::"<<endl;
        } else {
			// Incrementing offset position so that the writer threads should
			// be able to write in a file , independently of each other
            fileReadPosition += BUFFER_SIZE;
            
        }
														
	}

}

// Thread Function for writer threads.
DWORD WINAPI ThreadProc(LPVOID lpParameter) 
{
	BOOL  bSuccess;
	DWORD bytesTransferred;
	DWORD key;
	LPOVERLAPPED overlappedComp;
	PBUFFER_DATA pBufferData;

	// Worker Thread will be waiting on IO Completion Port 
	// to process IO Completion packets.
	bSuccess = GetQueuedCompletionStatus(hIOCompPort,
                                            &bytesTransferred,
                                            &key,
                                            &overlappedComp,
                                            (DWORD)-1);

	if( !bSuccess && (overlappedComp == NULL))
	{
		cout<<"::GetQueuedCompletionStatus Failed::"<<endl;
		ExitThread(-1);
	}

	// Typecasting the OVERLAPPED structure to BUFFER_DATA, so that we can
	// have an access to data which has been read() from file.
	pBufferData = (PBUFFER_DATA)overlappedComp;

	if (key == read)
	{
		bytesTransferred = (bytesTransferred + g_pageSize - 1) & ~(g_pageSize-1);

		bSuccess = WriteFile(hDestination,
                             pBufferData->lpData,
                             bytesTransferred,
                             &bytesTransferred,
                             &pBufferData->overLapped);

		if (!bSuccess && (GetLastError() != ERROR_IO_PENDING)) {
			LPVOID lpError;
			FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						GetLastError(),
						0, // Default language
						(LPTSTR) &lpError,
						0,
						NULL );
            cout<<"::Error While Writing File::"<<endl;
        } 
	}

	return 0;
}
