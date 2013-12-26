// Logger.h: interface for the Logger class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_LOGGER_H
#define MDK_LOGGER_H

#include <stdio.h>
#include "Lock.h"

namespace mdk
{

class Logger  
{
public:
	Logger();
	virtual ~Logger();

public:
	void SetPrintLog( bool bPrint );
	void DelLog( int nDay );//ɾ��nDay����ǰ����־
	void Error( const char *format, ... );//���������־
	void ErrorStream( unsigned char *stream, int nLen );//�����������־
	void Run( const char *format, ... );//�����־
	void RunStream( unsigned char *stream, int nLen );//�������־
private:
	bool OpenErrLog();
	bool OpenRunLog();
	void CreateLogDir();
	bool m_bPrint;
	Mutex m_writeMutex;
	bool m_bRLogOpened;
	bool m_bELogOpened;
	short m_nErrLogCurYear;
	unsigned char m_nErrLogCurMonth;
	unsigned char m_nErrLogCurDay;
	short m_nRunLogCurYear;
	unsigned char m_nRunLogCurMonth;
	unsigned char m_nRunLogCurDay;
	FILE *m_fpRunLog;
	FILE *m_fpErrLog;
};

}//namespace mdk

#endif // MDK_LOGGER_H
