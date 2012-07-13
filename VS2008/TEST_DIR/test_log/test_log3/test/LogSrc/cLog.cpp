/*
 * $Id: cLog.cpp,v 1.1.1.1 2001/11/22 15:18:55 hucl Exp $
 *
 * $Log: cLog.cpp,v $
 * Revision 1.1.1.1  2001/11/22 15:18:55  hucl
 * An initial version, and a great start!
 *
 */

//#include "stdafx.h"
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>

#ifndef WIN32

#include <strings.h>
#include <unistd.h>
#include <errno.h>
extern int errno;
#else
#include <process.h>
#include <windows.h>
#endif

#include "cLog.h"


CCLog::CCLog()
{
    memset(filename, 0, sizeof(filename));
    memset(errfilename, 0, sizeof(errfilename));
    size      = 10 * 1024 * 1024;
    different = 0;
    fp        = stdout;
    fperr     = stdout;
    
#ifdef LOGFLUSHNOW
    fSetFlushNowFlag();
#else
    fSetFlushNowFlag(0);
#endif

#ifdef WIN32
	InitializeCriticalSection(&_mutex);
#else
	//pthread_mutexattr_init(&_attr);
	pthread_mutex_init(&_mutex,NULL);
#endif
}

CCLog::CCLog(const char *s)
{
    size      = 10 * 1024 * 1024;
    different = 0;
    fperr     = NULL;
    fp        = NULL;
    memset(filename, 0, sizeof(filename));
    memset(errfilename, 0, sizeof(errfilename));

    if (s && *s)
    {
        fp = fopen(s, "a");
    }
    
    if (fileno(fp) == -1)
    {
        fp = freopen(s, "a", fp);
    }
    
    if (!fp)
    {
        fp = stdout;
    }
    
    fperr = fp;

    if (fp && (fp != stdout))
    {
        strcpy(filename, s);
        strcpy(errfilename, s);
    }
    
#ifdef LOGFLUSHNOW
    fSetFlushNowFlag();
#else
    fSetFlushNowFlag(0);
#endif

#ifdef WIN32
	InitializeCriticalSection(&_mutex);
#else
	//pthread_mutexattr_init(&_attr);
	pthread_mutex_init(&_mutex,NULL);
#endif

}

CCLog::~CCLog(void)
{
    if (different && (fperr != stdout))
    {
        fclose(fperr);
    }
    
    if (fp && (fp != stdout))
    {
        fclose(fp);
    }

#ifdef WIN32
	DeleteCriticalSection(&_mutex);
#else
	pthread_mutex_destroy(&_mutex);
#endif
}

int CCLog::fSetFlushNowFlag(int flag)
{
    int i;

    if ((flag != 1) && (flag != 0))
    {
        return ( - 1);
    }

    i             = iFlushNowFlag;
    iFlushNowFlag = flag;

    if (iFlushNowFlag)
    {
        setbuf(fp, NULL);
        
        if (different)
        {
            setbuf(fperr, NULL);
        }
    }
    else
    {
        setbuf(fp, caLogBuffer);
        
        if (different)
        {
            setbuf(fperr, caLogBuffer);
        }
    }

    return (i);
}

void CCLog::fChangeLogFile(char *s, int mode)
{
    int     fd = -1;

	lock();

    if (fp && (fp != stdout))
    {
        fclose(fp);
    }
    
    fp = NULL;

    if (s && *s)
    {
        fp = fopen(s, (mode ? "w" : "a"));
    }
        
    if (fp)
    {
        fd = fileno(fp);
    }
    
    if (fd == -1 || fd == 0)
    {
        FILE*   m_ptrfp;
        m_ptrfp = fopen(s, (mode ? "w" : "a"));
        
        if (fp)
        {
            fclose(fp);
        }
        
        fp = m_ptrfp;
    }
    
    if (fp)
    {
        fd = fileno(fp);
    }
    
    if (fp)
    {
        strcpy(filename, s);
        
        if (!different)
        {
            fperr = fp;
            strcpy(errfilename, s);
        }
    }
    else
    {
        memset(filename, 0, sizeof(filename));
        fp = stdout;
        
        if (!different)
        {
            memset(errfilename, 0, sizeof(errfilename));
            fperr = stdout;
        }
    }

    fSetFlushNowFlag(iFlushNowFlag);

	unlock();
}

void CCLog::fChangeErrLogFile(char *s, int mode)
{

	lock();

    if (different && fperr && (fperr != stdout))
    {
        fclose(fperr);
    }
    
    fperr = NULL;

    if (s &&  *s)
    {
        fperr = fopen(s, (mode ? "w" : "a"));
    }
    
    if (fperr)
    {
        strcpy(errfilename, s);
        different = 1;
    }
    else
    {
        fperr = fp;
        different = 0;
        strcpy(errfilename, filename);
    }

    fSetFlushNowFlag(iFlushNowFlag);

	unlock();
}

void CCLog::fSetLogSize(int newsize)
{
    if ((newsize >= 0) && (newsize < 300))
    {
        size = ((long)newsize) *1024 * 1024;
    }
}

void CCLog::fGetNowTime()
{
    time_t  timet;
    char*   ptr;

    timet = time(NULL);
    ptr = ctime(&timet);
    strcpy(date, ptr);
}

//dumplen -1 表示函数跟踪
void CCLog::fOutMsg(const void *dumpdata, int dumplen, const char *file, int line, const char *format, ...)
{
    int offset, i, j, k;
	int err = 0;
	
#ifndef WIN32
	err = errno;
#else
	err = GetLastError();
#endif 
    va_list ap;
    unsigned char *ucptr = (unsigned char*)dumpdata;
    FILE *fptmp;

	lock();

    if ((fptmp = (!different || !file || (line == 0)) ? fp : fperr) == NULL)
    {
		unlock();
        return ;
    }

    if ((size != 0) && (fptmp != stdout) && (ftell(fptmp) >= size))
    {
        char bakfile[300];
        char *name = (fptmp == fp) ? filename : errfilename;

        fclose(fptmp);
        
        if (!different || !file || (line == 0))
        {
            fp = NULL;
        }
        else
        {
            fperr = NULL;
        }

        sprintf(bakfile, "%s.bak", name);
        unlink(bakfile);
        rename(name, bakfile);
        
        if (!different || !file || (line == 0))
        {
            fChangeLogFile(name, 1);
            fptmp = fp;
        }
        else
        {
            fChangeErrLogFile(name, 1);
            fptmp = fperr;
        }
    }
    if (!fptmp)
    {
		unlock();
        return ;
    }

    fGetNowTime();

    fprintf(fptmp, "PID:%d|TID:%d|%s", getpid(), GetCurrentThreadId(), date);
    
	if ( dumplen == -1 ) //-1 表示函数跟踪
	{
		fprintf(fptmp, "TRACE: ");
	}
	else
	{
		if ((file == NULL) || (line == 0))
		{
			fprintf(fptmp, "LOG: ");
		}
		else
		{
			fprintf(fptmp, "FILE[%s], LINE[%d], ERRNO[%d], ERR: ", file, line, err);
		}
	}

	if ( format != NULL )
	{
		va_start(ap, format);
		vfprintf(fptmp, format, ap);
		va_end(ap);
	}

    fprintf(fptmp, "\n");

    if ((dumplen <= 0) || (ucptr == NULL))
    {
        fprintf(fptmp, "\n");
		unlock();
        return ;
    }

    fprintf(fptmp, "OFFSET       -0--1--2--3--4--5--6--7-*-8--9--a--b--c--d--e--f- DISPLAY\n");
    offset = i = 0;
    
    while (i < dumplen)
    {
        fprintf(fptmp, "%05x(%05d) ", offset, offset);
        for (j = 0; (j < 16) && (i < dumplen); j++, i++)
        {
            if (j < 7)
            {
                fprintf(fptmp, "%02x ", *(ucptr + i));
            }
            else if (j == 7)
            {
                fprintf(fptmp, "%02x -", *(ucptr + i));
            }
            else
            {
                fprintf(fptmp, " %02x", *(ucptr + i));
            }
        }
        
        for (k = j; k < 16; k++)
        {
            fprintf(fptmp, "   ");
        }
        
        if (j < 8)
        {
            fprintf(fptmp, " ");
        }
        
        fprintf(fptmp, " ");

        for (k = 0; k < j; k++)
        {
            if (isprint(*(ucptr + i - j + k)))
            {
                fprintf(fptmp, "%c", *(ucptr + i - j + k));
            }
            else
            {
                fprintf(fptmp, ".");
            }
        }
        
        fprintf(fptmp, "\n");

        offset += 16;
    }
    
    fprintf(fptmp, "\n");

	unlock();
}

void CCLog::lock()
{
#if defined(WIN32)
	EnterCriticalSection(&_mutex);
#else
	if( pthread_mutex_lock(&_mutex) != 0 )
		printf("Error:	Lock fail!\n");
#endif
}

void CCLog::unlock()
{
#if defined(WIN32)
	LeaveCriticalSection(&_mutex);
#else
	if ( pthread_mutex_unlock(&_mutex) != 0 )
		printf("Error:	Unlock fail!\n");
#endif
}

CCLog ccLog;

