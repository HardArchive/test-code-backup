/*
 * $Id: cLog.h,v 1.1.1.1 2001/11/22 15:18:55 hucl Exp $
 *
 * $Log: cLog.h,v $
 * Revision 1.1.1.1  2001/11/22 15:18:55  hucl
 * An initial version, and a great start!
 *
 *Revision 1.1.1.2 2009-04-09 lizp
 *add lock for mutithread
 */

#ifndef cLog_H
#define cLog_H

#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#if defined(_MSC_VER)
# pragma warning(disable:4786)    // identifier was truncated in debug info
# pragma warning(disable:4996)    // identifier was truncated in debug info
#endif

#else
#include <pthread.h>
#endif

#define LOGFLUSHNOW

class CCLog
{
public:
    CCLog(); 
    CCLog(const char *logfilename); 
    ~CCLog(); 

    void fOutMsg(const void *, int, const char *, int, const char *, ...);
    int  fSetFlushNowFlag(int flag = 1);
    void fChangeLogFile(char *s, int mode = 0);
    void fChangeErrLogFile(char *s, int mode = 0);
    void fSetLogSize(int newsize);

protected:
	void lock();
	void unlock();
private:
    char  filename[256], errfilename[256];
    char  date[64];
    char  caLogBuffer[BUFSIZ];

    FILE *fp, *fperr;

    int   different;
    int   iFlushNowFlag;
    long  size;

    void  fGetNowTime();
    FILE *fCheckSize(FILE *fp, char *name);

	//added by lizp at 2009-04-09
#if defined(WIN32)
	CRITICAL_SECTION	_mutex;
#else
	pthread_mutex_t		_mutex;
	#endif
};

extern CCLog   ccLog;

#endif

