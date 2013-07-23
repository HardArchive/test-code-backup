
/******************************************************************************\
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (C) 1993 - 2000.  Microsoft Corporation.  All rights reserved.

*       This source code is only intended as a supplement to
*       Microsoft Development Tools and/or WinHelp documentation.
*       See these sources for detailed information regarding the
*       Microsoft samples programs.
\******************************************************************************/

// Various buffer sizes
#define IN_BUF_SIZE        1000
#define OUT_BUF_SIZE       1000
#define PLEASE_READ        1000
#define PLEASE_WRITE       1000
#define LINE_LEN           80
#define NAME_SIZE          25

// For the Client dialog box.
#define IDB_SEND           904
#define IDD_EDITWRITE      905
#define IDD_EDITREAD       906

// For the Init dialog box.
#define IDD_SVREDIT        701
#define IDB_INITOK         705
#define IDD_CLNTEDIT       704


#define WM_APP             0x8000
#define WM_INITCLIENT      WM_APP
#define WM_GO_AWAY         WM_APP+1

// Stringtable identifiers
#define IDS_CANTFINDPIPE    100
#define IDS_GENERALERROR    101
#define IDS_DEBUGTITLE      102
#define IDS_CONNECTBROKEN   103
#define IDS_READFAILED      104
#define IDS_CLIENTDBG       105



LONG CALLBACK MainWndProc   (HWND, UINT, WPARAM, LPARAM);
LONG CALLBACK ClientDlgProc (HWND, UINT, WPARAM, LPARAM);
LONG CALLBACK InitDlgProc   (HWND, UINT, WPARAM, LPARAM);
VOID          ReadPipe      (HANDLE *);
