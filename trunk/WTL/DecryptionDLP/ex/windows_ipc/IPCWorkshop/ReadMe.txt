========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : IPCWorkshop
========================================================================


AppWizard has created this IPCWorkshop application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your IPCWorkshop application.

IPCWorkshop.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

IPCWorkshop.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CIPCWorkshopApp application class.

IPCWorkshop.cpp
    This is the main application source file that contains the application
    class CIPCWorkshopApp.

IPCWorkshop.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

IPCWorkshop.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\IPCWorkshop.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file IPCWorkshop.rc.

res\IPCWorkshop.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.




/////////////////////////////////////////////////////////////////////////////

AppWizard creates one dialog class:

IPCWorkshopDlg.h, IPCWorkshopDlg.cpp - the dialog
    These files contain your CIPCWorkshopDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's
    template is in IPCWorkshop.rc, which can be edited in Microsoft
	Visual C++.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named IPCWorkshop.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////


========================================================================
							INTRODUCTION
========================================================================


The main objective of this application is to describe how simple the various 
Inter Process Communication methods can be used for data transfer. This 
application also help us to decide which of the available IPC methods are 
best to use.Inorder to explain the IPC methods effectively and simply, 
I have taken a structure pointer for description. The structure named 
EmployeeData is created and its pointer is passed from the client to the 
server. This demo application describes one way inter process communication 
(ie) from the client to the server for simplicity. The server will just 
listen to its clients. Here I have described 5 types of IPCs.( though Sockets  
are not said to be an IPC, it satisfies the data transfer purpose).

1	Clipboard			Local Mc
2	File Mapping		Local Mc
3	Named Pipes			Local Mc/Intranet
4	Mailslots			Local Mc/Intranet
5	Sockets				Local Mc/Intranet/Internet


========================================================================
							CLIPBOARD
========================================================================
   
The clipboard is a set of functions and messages that enable applications to 
transfer data. Because all applications have access to the clipboard, data can 
be easily transferred between applications or within an application.

Procedure

To Write in the Clipboard

1. OpenClipboard
2. Register Clipboard Format (Optional)
3. Use global allocation for the data
4. SetClipboardData    
5. CloseClipboard.
 
To Read from the Clipbaord

1. OpenClipboard
2. Register Clipboard Format (Optional)  
3. Check the clipboard format is available
4. Get the clipboard data
5. Unlock the handle

Clipboard format registration is Optional. Standard clipboard formats are available.
Since Clipboard is used to transfer data in the local machine only, the server can be 
notified by posting message using HWND_BROADCAST. 


========================================================================
							FILE MAPPING
========================================================================

File mapping is an efficient way for two or more processes on the same computer to 
share data. Here i have not described about the synchronization of the process.
In order to access the file's contents, the processes uses virtual address space
called file view. Processes read from and write to the file view using pointers, 
just as they would with dynamically allocated memory. 

Procedure

To write in the file

1. Create File Mapping
2. map the handle to the process virtual address space (MapViewOfFile)
3. copy the data in the file

To Read from the File

1. Create File Mapping
2. MapViewOfFile					(maps the handle to the process virtual address space)
3. copy the data from the file.


Notify the server using HWND_BROADCAST handle. we have successfully trasferred the
pointer between two processes on the same local machine. we will get curious of how 
to pass this pointer in the Intranet if this two processes resides on different computers.
Next section is Named Pipes and Mailslots.


========================================================================
							MAILSLOTS
========================================================================


Any win32 application can store messages in a mailslot and the owner will retrieve 
messages that are stored there. Mailslot is a simple way for a process to broadcast
messages to multiple processes using datagram. Hence there won't be any confirmation 
of the receipt. This is a mechanism for one way inter process communication.

Procedure

To write in the Mailslot

1. CreateMailslot			(Creates Mailslot)
2. WriteFile				(write it to the mailslot)

To Read from the Mailslot

1. CreateFile				(opens the mailslot)
2. GetMailslotInfo			(check for the messages in the mail slot) 
3. Read from the Mailslot	(ReadFile)
  
A mailslot server is a process that creates and owns a mailslot. When the server creates
a mailslot named MAILSLOT_EMPDATA it receives a mailslot handle.

m_strMailslot = "\\\\.\\mailslot\\MAILSLOT_EMPDATA";
m_hInputslot = CreateMailslot(	m_strMailslot,              // mailslot name
					 			0,					        // message can be of any size
								MAILSLOT_WAIT_FOREVER,      // waits forever for a message
								NULL						// It will not be inherited
							 );  

This handle must be used when a process reads messages from the mailslot. Before this operation
just check for the messages present in the mailslot.

int size = 256;
char* buffer = new buffer[size];
DWORD message, noofMessages, dataRead;
BOOL result = GetMailslotInfo(	m_hInputslot,				// mailslot handle
								0,							// message can be of any size
								&message,					// size of next message
								&noofMessages,				// number of messages	
								NULL						// read time-out interval
							 );
if( !result)
	TRACE("Unable to get mail slot information");
if( noofMessages == MAILSLOT_NO_MESSAGE)
	TRACE("There are no messages in the mailslot");

while( noofMessages != 0)
{
	ReadFile( m_hInputslot, buffer, size, &dataRead, NULL);
	GetMailslotInfo(	m_hInputslot,
						NULL,
						&message,
						&noofMessages,
						NULL
					);
}

  

A mailslot client is a process that writes a message to a mailslot. Any process that has the 
name of a mailslot can put a message there. New messages follow any existing messages in the mailslot. 
The client opens the mailslot using CreateFile.

m_strMailslot.Format("\\\\*\\mailslot\\%s", pMailslotName);
	m_hOutputslot = CreateFile(	m_strMailslot,					// file name
								GENERIC_WRITE,					// Only Write Permission
								0,								// cannot be shared
								NULL,							// SD
								OPEN_EXISTING,					// Opens the file. fails if file doesn't exist			
								FILE_ATTRIBUTE_NORMAL,			// file attributes
								NULL							// handle to template file
							  );
if( !m_hOutputslot )
	TRACE("Unable to create a file");

Write the data with the help of opened handle.

DWORD sizeWritten;
WriteFile( m_hOutputslot, pData, m_nSize, &sizeWritten, NULL);
if( sizeWritten == 0 )
	TRACE(" Unable to write to the Mail slot. Error is %s", GetLastError() );


========================================================================
							NAMEDPIPES
========================================================================

A named pipe is a named, one-way or duplex pipe for communication between the pipe 
server and one or more pipe clients. Named pipes are like telephone calls: you talk 
only to one party, but you know that the message is being received.

Procedure

To Write in the pipe.

1. Create a named pipe. (with or without security attributes)
2. Write the data in the file. (at the time of writing the server should listen)

To read from the pipe

1. Open the named pipe.
2. Continuously read from the pipe
3. Notify that the message has been received.


A named pipe server, refers to a process that creates a named pipe. 

m_strPipeName = "\\\\.\\pipe\\PIPE_EMPDATA";
m_hInputPipeHandle = CreateNamedPipe(   m_strPipeName,				// pipe name
										PIPE_ACCESS_INBOUND,        // client to the server
										PIPE_WAIT,					// Blocking mode is enabled
										1,					        // support given to only one client
										0,							// output buffer size
										0,							// input buffer size
										PIPE_TIMEOUT,			    // time-out interval
										NULL						// Security Attributes
									);

Note, that the security attribute is given as NULL. Hence support will be 
available to only Local machine. Inorder to support across the network, set the security
attribute.

SECURITY_ATTRIBUTES saPipe;
saPipe.lpSecurityDescriptor = (PSECURITY_DESCRIPTOR) malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
InitializeSecurityDescriptor(saPipe.lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
// Access Control list is asiigned as NULL inorder to allow all access to the object.
SetSecurityDescriptorDacl(saPipe.lpSecurityDescriptor, TRUE, (PACL) NULL,FALSE);
saPipe.nLength = sizeof(saPipe);
saPipe.bInheritHandle = TRUE; 

Since the named pipe is created in the blocking mode, a thread should be created 
so that the server will be always in listen mode. The client will get hanged, if the
server is not in the listen mode. This thread nmaed as Listenerproc will continuously read the file.
If the data is read then it will notify the server.

UINT ListenerProc( LPVOID pNamedPipe )
{
	CNamedPipe* This = (CNamedPipe*)pNamedPipe;
	if( This == NULL )
		return 0;
	int size = This->GetSize();

	// assign the size of the data to be read.
	ASSERT( size != 0 );
	ASSERT( This->GetTransferMessage() != NULL);
	char* buff = new char[size];
	DWORD dataRead;
	while(1) {
		BOOL isRead = ReadFile( This->GetInputPipeHandle(), buff, size, &dataRead, NULL);
		if( dataRead > 0) {
			// acts as a post thread message
			// since this is internal to the process, it can also work on the network
			// satisfying the named pipe advantage.
			::PostMessage( HWND_BROADCAST, This->GetTransferMessage(), (WPARAM)buff, This->GetLparam());
		}		
	}
	return 1;
}

A named pipe client, opens the named pipe handle using CreateFile. With the help of the handle 
the client writes the data to the server.


========================================================================
							SOCKETS
========================================================================

A socket is a communication endpoint - an object through which a Windows Sockets application
sends or receives packets of data across a network.Here I have used a only a bit of portion 
for data transfer from the huge topic Window Sockets. This application uses stream socket.
MFC class CSocket provides encapsulation of the windows socket API.

Procedure

Server Socket					client Socket
	
Construct a socket				Construct a socket
Create the socket				Create the socket
Start listening
						<---	Connect	
 
Accept the client				
						<---	Send Data	
Receive data.





