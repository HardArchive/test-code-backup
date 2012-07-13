/*
 * $Id: cTrace.cpp,v 1.2 2001/12/14 10:26:08 hucl Exp $
 *
 * $Log: cTrace.cpp,v $
 * Revision 1.2  2001/12/14 10:26:08  hucl
 * format and some small bug.
 *
 * Revision 1.1.1.1  2001/11/22 15:18:55  hucl
 * An initial version, and a great start!
 *
 */

//#include "stdafx.h"
#include <string.h>
#include <stdarg.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#include <windows.h>
#endif
#include <ctype.h>
#include <time.h>

#include "cTrace.h"

//#ifndef NTRACE

CCTraceOut::CCTraceOut()
{
    fp            = NULL;
    size          = 0;
#ifdef TRACEFLUSHNOW
    iFlushNowFlag = 1;
#else
    iFlushNowFlag = 0;
#endif
}

CCTraceOut::CCTraceOut(const char *s)
{
    fp            = NULL;
    size          = 0;
#ifdef TRACEFLUSHNOW
    iFlushNowFlag = 1;
#else
    iFlushNowFlag = 0;
#endif

    fChangeTraceFile(s);
}

void
CCTraceOut::fChangeTraceFile(const char *s)
{
    if((fp != NULL) && (fp != stdout))
        fclose(fp);

    fp = NULL;

    if((s != NULL) && (*s != '\0'))
        fp = fopen(s, "at");

    if(iFlushNowFlag && fp)
            setbuf(fp, 0);

    strcpy(filename, s);
}

void
CCTraceOut::fSetTraceSize(int newsize)
{
    if((newsize >= 0) && (newsize < 300))
        size = ((long )newsize) * 1024 * 1024;
}

CCTraceOut::~CCTraceOut()
{
    if((fp != NULL) && (fp != stdout))
        fclose(fp);
}

int
CCTraceOut::fOutText(char *s)
{
    if(fp == NULL)
    {
#ifdef WIN32
//		OutputDebugString(s);
#endif
		return(-1);
	}

    if((size != 0) && (fp != stdout) && (ftell(fp) >= size)) {
        char   bakfile[300];

        fclose(fp);
        fp = NULL;

        sprintf(bakfile, "%s.bak", filename);
        unlink(bakfile);
        rename(filename, bakfile);

        fChangeTraceFile(filename);

        if(fp == NULL)
            return(-1);
    }

	fGetNowTime();
	
    fprintf(fp, "%d|%s", getpid(), date);
    fprintf(fp, "TRACE: %s\n", s);
    return 1;
}

void CCTraceOut::fGetNowTime()
{
    time_t  timet;
    char*   ptr;
	
    timet = time(NULL);
    ptr = ctime(&timet);
    strcpy(date, ptr);
}

#ifdef DETACH_TRACE
CCTraceOut cTraceOut;
#endif

// = Static initialization.

// Keeps track of how far to indent per trace call.
int CCTrace::iNestingIndent = 0;//CCTrace::DEFAULT_INDENT;

// Is tracing enabled?
int CCTrace::iEnableTracing = 1;

//DWORD CCTrace::ulTickCount_ = 0;

void
CCTrace::fDump (void) const
{
}

// Determine whether or not tracing is enabled

int
CCTrace::fIsTracing(void)
{
    return CCTrace::iEnableTracing;
}

// Enable the tracing facility.

void
CCTrace::fStartTracing (void)
{
    CCTrace::iEnableTracing = 1;
}

// Disable the tracing facility.

void
CCTrace::fStopTracing (void)
{
    CCTrace::iEnableTracing = 0;
}

void
CCTrace::fSetNestingIndent (int indent)
{
    CCTrace::iNestingIndent = indent;
}

// Change the nesting indentation level.
// Perform the first part of the trace, which prints out the string N,
// the LINE, and the FILE as the function is entered.

CCTrace::CCTrace (char *n, int line, char *file)
{
// 	if ( CCTrace::iNestingIndent > CCTrace::DEFAULT_INDENT )//嵌套数量不能超过规定层数
// 		return;
		
    char str[256];

    this->pcName = n;
    iNestingIndent++;

    if (CCTrace::iEnableTracing)
    {
#ifdef DETACH_TRACE
        CCTraceOut *lm = &cTraceOut;
#else
		CCLog *lm = &ccLog;
#endif
        snprintf(str, 255, "%d: calling [%s] in file[%s] on line[%d]",
            CCTrace::iNestingIndent, this->pcName, file, line);
		str[255] = 0;//防止溢出
#ifdef DETACH_TRACE
        lm->fOutText(str);
#else
		lm->fOutMsg(NULL, -1, NULL, 0, "%s", str);
#endif
		ulTickCount_ = GetTickCount();
    }
}

// Perform the second part of the trace, which prints out the NAME as
// the function is exited.

CCTrace::~CCTrace (void)
{
// 	if ( CCTrace::iNestingIndent > CCTrace::DEFAULT_INDENT )//嵌套数量不能超过规定层数
// 		return;

    char str[256];

    if (CCTrace::iEnableTracing)
    {
#ifdef DETACH_TRACE
        CCTraceOut *lm = &cTraceOut;
#else
		CCLog *lm = &ccLog;
#endif
		DWORD uiTick = GetTickCount() - ulTickCount_;
        snprintf(str,255, "%dms:%d: leaving [%s] \n",
            uiTick, CCTrace::iNestingIndent, this->pcName);
		str[255] = 0;//防止溢出
#ifdef DETACH_TRACE
        lm->fOutText(str);
#else
		lm->fOutMsg(NULL, -1, NULL, 0, "%s", str);
#endif
    }
    iNestingIndent--;
}


//#endif    // #ifndef NTRACE
