
/***********************************************************************
 ** Copyright (c)2007,智安邦科技股份有限公司
 ** All rights reserved.
 ** 
 ** File name  : comlog.h
 ** Author     : lizp (lizp@gmail.com)
 ** Date       : 2009-04-17 13:57:39
 ** Comments   : 增加了宏 DETACH_TRACE， 为TRACE、LOG是否输入到一个文件的开关量
 定义它就输入到不同文件中
 ***********************************************************************/



/*
 * $Id: comlog.h,v 1.2 2001/12/14 10:25:48 hucl Exp $
 *
 * $Log: comlog.h,v $
 * Revision 1.2  2001/12/14 10:25:48  hucl
 * format and some small bug.
 *
 * Revision 1.1.1.1  2001/11/22 15:18:55  hucl
 * An initial version, and a great start!
 *
 */

#ifndef _HEAD_COMLOG_H
#define _HEAD_COMLOG_H

#ifndef _DEBUG
//#define NLOG
#define NTRACE
//#define NERR 
#endif

//trace和log是否分开
// #define DETACH_TRACE
#define LOGFLUSHNOW

/*#include "os.h"*/
//#include "cTransfer.h"
//#include "cPackdef.h"
#include "cLog.h"
#include "cTrace.h"

//#ifndef WIN32
#ifdef NTRACE
#define FUNTRACE(X) { ; }
#else
#define FUNTRACE(X) CCTrace ____Trace (X, __LINE__, __FILE__);
#endif  // #ifdef NTRACE
//#endif

#ifdef NLOG

#define CHGLOG(pathname)    {;}
#define CHGERRLOG(pathname) {;}
#define CHGLOGSIZE(newsize) {;}

#define CHGTRACE(pathname)  {;}
#define CHGTRACESIZE(newsize)   {;}

#else

#define CHGLOG(pathname)    ccLog.fChangeLogFile(pathname);
#define CHGLOGEX(pathname, mode)    ccLog.fChangeLogFile(pathname, mode);
#define CHGERRLOG(pathname) ccLog.fChangeErrLogFile(pathname);
#define CHGLOGSIZE(newsize) ccLog.fSetLogSize(newsize);

#ifdef DETACH_TRACE
#define CHGTRACE(pathname)  cTraceOut.fChangeTraceFile(pathname);
#define CHGTRACESIZE(newsize)   cTraceOut.fSetTraceSize(newsize);
#endif

#endif

// 
// #ifdef OS_LINUX
// #ifdef NLOG 
// #define LOG(dumpdata, dumplen, format, arg...) { ; }
// #else
// #define LOG(dumpdata, dumplen, format, arg...)  \
//     ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, format, ##arg);
// #endif  // #ifdef NLOG 
// 
// #ifdef NERR 
// #define ERRLOG(dumpdata, dumplen, format, arg...) { ; }
// #else
// #define ERRLOG(dumpdata, dumplen, format, arg...)   \
//     ccLog.fOutMsg(dumpdata, dumplen, __FILE__, __LINE__, format, ##arg);
// #endif  // #ifdef NERR 
// #endif  // #ifdef OS_LINUX

//#if defined(OS_SUN) || defined(OS_HP)
#ifdef NLOG 
#define LOG(dumpdata, dumplen, msg) { ; }
#else
#define LOG(dumpdata, dumplen, msg) \
    ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, msg);
#endif  // #ifdef NLOG 

#ifdef NLOG 
#define LOG0(dumpdata, dumplen) { ; }
#else
#define LOG0(dumpdata, dumplen) \
ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, NULL);
#endif  // #ifdef NLOG 

#ifdef NERR 
#define ERRLOG(dumpdata, dumplen, msg) { ; }
#else
#define ERRLOG(dumpdata, dumplen, msg)  \
    ccLog.fOutMsg(dumpdata, dumplen, __FILE__, __LINE__, msg);
#endif  // #ifdef NERR 
//#endif  // #if defined(OS_SUN) || defined(OS_HP)

#ifdef NLOG 
#define LOG1(dumpdata, dumplen, format, a) { ; }
#define LOG2(dumpdata, dumplen, format, a, b) { ; }
#define LOG3(dumpdata, dumplen, format, a, b, c) { ; }
#define LOG4(dumpdata, dumplen, format, a, b, c, d) { ; }
#define LOG5(dumpdata, dumplen, format, a, b, c, d, e) { ; }
#define LOG6(dumpdata, dumplen, format, a, b, c, d, e, f) { ; }
#else
#define LOG1(dumpdata, dumplen, format, a)  \
    ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, format, a);
#define LOG2(dumpdata, dumplen, format, a, b)   \
    ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, format, a, b);
#define LOG3(dumpdata, dumplen, format, a, b, c)    \
    ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, format, a, b, c);
#define LOG4(dumpdata, dumplen, format, a, b, c, d) \
    ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, format, a, b, c, d);
#define LOG5(dumpdata, dumplen, format, a, b, c, d, e)  \
    ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, format, a, b, c, d, e);
#define LOG6(dumpdata, dumplen, format, a, b, c, d, e, f)  \
    ccLog.fOutMsg(dumpdata, dumplen, NULL, 0, format, a, b, c, d, e, f);
#endif  // #ifdef NLOG 

#ifdef NERR 
#define ERRLOG1(dumpdata, dumplen, format, a) { ; }
#define ERRLOG2(dumpdata, dumplen, format, a, b) { ; }
#define ERRLOG3(dumpdata, dumplen, format, a, b, c) { ; }
#define ERRLOG4(dumpdata, dumplen, format, a, b, c, d) { ; }
#define ERRLOG5(dumpdata, dumplen, format, a, b, c, d, e) { ; }
#else
#define ERRLOG1(dumpdata, dumplen, format, a)   \
    ccLog.fOutMsg(dumpdata, dumplen, __FILE__, __LINE__, format,a);
#define ERRLOG2(dumpdata, dumplen, format, a, b)    \
    ccLog.fOutMsg(dumpdata, dumplen, __FILE__, __LINE__, format,a,b);
#define ERRLOG3(dumpdata, dumplen, format, a, b, c) \
    ccLog.fOutMsg(dumpdata, dumplen, __FILE__, __LINE__, format,a,b,c);
#define ERRLOG4(dumpdata, dumplen, format, a, b, c, d)  \
    ccLog.fOutMsg(dumpdata, dumplen, __FILE__, __LINE__, format,a,b,c,d);
#define ERRLOG5(dumpdata, dumplen, format, a, b, c, d, e)   \
    ccLog.fOutMsg(dumpdata, dumplen, __FILE__, __LINE__, format,a,b,c,d,e);
#endif  // #ifdef NERR 

#endif  // #ifndef _HEAD_COMLOG_H


