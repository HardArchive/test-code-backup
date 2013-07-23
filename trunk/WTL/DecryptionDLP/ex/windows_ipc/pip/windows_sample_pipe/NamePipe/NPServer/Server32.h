
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

#define MAX_PIPE_INSTANCES 100

#define LINE_LEN   80
#define NAME_SIZE  25

// Bitmaps and coordinates.
#define END_LIVE   1
#define END_DEAD   2
#define MID_LIVE   3
#define MID_DEAD   4
#define BITMAP_X   20
#define BITMAP_Y   32
#define NAME_X     60
#define NAME_Y     10

// Buffers and pipe symbolic constants.
#define IN_BUF_SIZE        1000
#define OUT_BUF_SIZE       1000
#define TIME_OUT           0
#define PLEASE_READ        1000
#define PLEASE_WRITE       1000

//Stringtable identifiers
#define IDS_WRONGOS     100
#define IDS_WINDOWTITLE 101
#define IDS_ERRORCODE   102
#define IDS_DEBUGTITLE  103
#define IDS_READERROR   104
#define IDS_DEBUGINFO   105
#define IDS_DEBUGLAST   106

// Structure used to hold information about a pipe client.
// They're used as array elements.
typedef struct { HANDLE     hPipe;
                 HANDLE     hThread;
                 HANDLE     hEvent;
                 OVERLAPPED overLap;
                 BOOL       live;
                 CHAR Name[NAME_SIZE];
               } WRTHANDLE;


LRESULT CALLBACK MainWndProc (HWND, UINT, WPARAM, LPARAM);
VOID          ServerProc (HWND *);
VOID          TellAll    (CHAR *);
VOID          DrawBranch(HDC);
