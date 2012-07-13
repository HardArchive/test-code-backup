
/***********************************************************************
 ** Copyright (c)2007,智安邦科技股份有限公司
 ** All rights reserved.
 ** 
 ** File name  : cTrace.h
 ** Author     : lizp (lizp@gmail.com)
 ** Date       : 2009-04-17 13:57:03
 ** Comments   : 增加了trace和log可以在一个文件输入的功能，即trace到log文件中
 ***********************************************************************/


/*
 * $Id: cTrace.h,v 1.2 2001/12/14 10:25:48 hucl Exp $
 *
 * $Log: cTrace.h,v $
 * Revision 1.2  2001/12/14 10:25:48  hucl
 * format and some small bug.
 *
 * Revision 1.1.1.1  2001/11/22 15:18:55  hucl
 * An initial version, and a great start!
 *
 *Revision 1.1.1.2 2009-04-09 lizp
 *add time for trace
 */

#ifndef _HEAD_CTRACE_H
#define _HEAD_CTRACE_H

#ifndef WIN32
#include <unistd.h>
#else
#if defined(_MSC_VER)
# pragma warning(disable:4786)    // identifier was truncated in debug info
# pragma warning(disable:4996)    // identifier was truncated in debug info
#endif

#endif

#include <stdio.h>
#include <string>
#include "cLog.h"

#if defined(_MSC_VER)
# define snprintf	    _snprintf
# define vsnprintf    _vsnprintf
# define strcasecmp	  _stricmp
# define strncasecmp	_strnicmp
#endif

#define TRACEFLUSHNOW

typedef unsigned long DWORD;

class CCTraceOut
{
public:
    CCTraceOut(); 
    CCTraceOut(const char *name); 
    ~CCTraceOut(); 

    void fChangeTraceFile(const char *);
    void fSetTraceSize(int newsize);
    int  fOutText(char *s);
    int  fSetFlushNowFlag(int f);
private:
	void  fGetNowTime();
private:
    FILE *fp;  
    int   iFlushNowFlag;
    long  size;
    char  filename[256];
	char  date[64];

};

class CCTrace
{
    // = TITLE
    //     A C++ trace facility that keeps track of which methods are
    //     entered and exited.
    //
    // = DESCRIPTION
    //     This class uses C++ constructors and destructors to automate
    //     the CCTrace nesting.  In addition, thread-specific storage
    //     is used to enable multiple threads to work correctly.
public:
    // = Initialization and termination methods.

    CCTrace (char *n, int line = 0, char *file = NULL );
    // Perform the first part of the trace, which prints out the string
    // N, the LINE, and the FILE as the function is entered.

    ~CCTrace (void);
    // Perform the second part of the trace, which prints out the NAME
    // as the function is exited.

    // = Control the tracing level.
    static int  fIsTracing(void);
    // Determine if tracing is enabled (return == 1) or not (== 0)

    static void fStartTracing (void);
    // Enable the tracing facility.

    static void fStopTracing (void);
    // Disable the tracing facility. 

    static void fSetNestingIndent (int indent);
    // Change the nesting indentation level. 

    void fDump (void) const;
    // Dump the state of an object.

private:
    // Keeps track of how deeply the call stack is nested (this is
    // maintained in thread-specific storage to ensure correctness in
    // multiple threads of control.

    char *pcName;
    // Name of the method we are in.

    static int iNestingIndent;
    // Keeps track of how far to indent per trace call.
    
    static int iEnableTracing;
    // Is tracing enabled?

	//消耗时间
	DWORD ulTickCount_;
    // Default values.
    enum 
    {
        DEFAULT_INDENT  = 30,
        DEFAULT_TRACING = 1
    };

};

#ifdef DETACH_TRACE
extern CCTraceOut cTraceOut;
#endif

#endif  // #ifndef _HEAD_CTRACE_H


