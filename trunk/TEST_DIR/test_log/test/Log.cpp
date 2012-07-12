/***************************************************************
* Author：		赵艳兴
* Company：		Intervision							  			
* Last Update：	2005-01-17		 								
* Copyright(c), Intervision Software Co., Ltd. Beijing. 
* 				All Rights Reserved	
*------------------------------------------------------------ 	
* FileName：		Log.cpp
* Function Note：	Sys Log implementation.
* Update note：		Created at 2003-09-10
*****************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
//#include <STAR/CORBA.h>
//#include <IVCLog.h>
#include "Log.h"

//void 
//INTERVISION::MTCout_T::write(const char* buf)
//{
//	MTLSynchronized sync(*this);
//	STAR_CERR << buf << STAR_ENDL;
//}


//#ifndef WIN32
//static pthread_mutexattr_t	_attr;
//#endif


//创建多级目录，如不存在则创建 失败返回false，其他返回true
bool CreatePath(char* pDirPath)
{
	int iNum = 0;
	char* pDir = NULL;
	char szParentDir[MAX_PATH] = {0};
	char szTempDir[MAX_PATH] = {0};

	//检验路径是否存在
	if (GetFileAttributes(pDirPath) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return TRUE;
	}

	pDir = (char*)strrchr(pDirPath, '\\');  //从尾部查找字符
	iNum = strlen(pDir);
	if (1 == iNum)
	{
		strncpy(szTempDir, pDirPath, strlen(pDirPath)-1);  //父目录名
		pDir = (char*)strrchr(szTempDir, '\\');
		//pwDir += 1;
		iNum += strlen(pDir);
	}

	strncpy(szParentDir, pDirPath, strlen(pDirPath)-iNum);

	//检验父级路径是否存在  如果存在则创建子目录
	if (GetFileAttributes(szParentDir) == FILE_ATTRIBUTE_DIRECTORY)
	{		
		if (!CreateDirectory(pDirPath, NULL))
		{
			//创建文件夹失败
			//FindClose(hParentFile);
			::MessageBox(NULL, pDirPath, "创建文件夹失败", MB_OK);
			return false;
		}
		else
		{
			SetFileAttributes(pDirPath, FILE_ATTRIBUTE_NORMAL);	
			return true;
		}
	}
	else
	{
		//如果不存在则递归
		if (!CreatePath(szParentDir))
		{
			return false;
		}
		else
		{//父目录创建完毕则创建子目录
			if (!CreatePath(pDirPath))
			{
				return false;
			}
			return true;
		}
	}

	return true;
}


INTERVISION::CLog_T::CLog_T()
{
	m_LogOptions = CLog_T::FileByDay | CLog_T::DateAndTime;
	m_LogLevel = 3;
	m_LogPath[0] = 0;
	this->m_LogFileName = "";
	this->m_pf = NULL;
	this->m_strBuf[0] = 0;
	m_bEnable = true;
	m_Prefix = "";
	m_Suffix = "log";
#ifdef WIN32
	InitializeCriticalSection(&_mutex);
#else
	//pthread_mutexattr_init(&_attr);
	pthread_mutex_init(&_mutex,NULL);
#endif

}

INTERVISION::CLog_T::CLog_T(unsigned level, unsigned options,const char *filename)
{
	this->m_LogOptions = options;
	this->m_LogLevel = level;
	this->m_LogFileName = "";
	m_LogPath[0] = 0;
	this->m_pf = NULL;
	this->m_strBuf[0] = 0;
	m_bEnable = true;
	m_Prefix = "";
	m_Suffix = "log";
	
	if( m_LogOptions&FileByDay )
	{
		if( !m_Suffix.empty() )
			m_LogFileName += m_Prefix;
		m_LogFileName += CLog_T::GetFileTime();
		m_LogFileName += ".";
		m_LogFileName += m_Suffix;
	}
	else if( m_LogOptions&FileByName )
	{
		assert( filename );
		this->m_LogFileName = filename;
	}
}

INTERVISION::CLog_T::~CLog_T()
{
	if( m_pf )
		fclose(m_pf );
#ifdef WIN32
	DeleteCriticalSection(&_mutex);
#else
	pthread_mutex_destroy(&_mutex);
#endif
}

string 
INTERVISION::CLog_T::GetSystemTime()
{
	string strSystemTime = "" ;
	char chTmp[15] = "";
	
#if	defined(WIN32)
	/*win32的取时间方法*/
	SYSTEMTIME temp;
	GetLocalTime(&temp);
	sprintf(chTmp, "%04d%02d%02d%02d%02d%02d" ,(short)temp.wYear,
		(char)temp.wMonth,(char)temp.wDay,(char)temp.wHour,(char)temp.wMinute,(char)temp.wSecond); 		
#else
	struct tm *newtime;
    time_t long_time;
    time( &long_time );                
    newtime = localtime( &long_time ); 
	sprintf(chTmp, "%04d%02d%02d%02d%02d%02d" ,newtime->tm_year + 1900,
		newtime->tm_mon + 1,newtime->tm_mday,newtime->tm_hour,newtime->tm_min,newtime->tm_sec); 	
#endif
	strSystemTime = (char *)chTmp;	 
	
	return strSystemTime ;
}

string 
INTERVISION::CLog_T::GetFileTime()
{
	string strSystemTime = "" ;
	char chTmp[15] = "";
	
#if	defined(WIN32)
	/*win32的取时间方法*/
	SYSTEMTIME temp;
	GetLocalTime(&temp);
	sprintf(chTmp, "%04d%02d%02d" ,(short)temp.wYear,
		(char)temp.wMonth,(char)temp.wDay); 		
#else
	struct tm *newtime;
    time_t long_time;
    time( &long_time );                
    newtime = localtime( &long_time ); 
	sprintf(chTmp, "%04d%02d%02d" ,newtime->tm_year + 1900,
		newtime->tm_mon + 1,newtime->tm_mday); 	
#endif
	strSystemTime = (char *)chTmp;	 
	
	return strSystemTime ;
}

string 
INTERVISION::CLog_T::FormatTime(string systemTime)
{
	string tmp;
	tmp = systemTime.substr(0,4);
	tmp += "-";
	tmp += systemTime.substr(4,2);
	tmp += "-";
	tmp += systemTime.substr(6,2);
	tmp += " ";
	tmp += systemTime.substr(8,2);
	tmp += ":";
	tmp += systemTime.substr(10,2);
	tmp += ":";
	tmp += systemTime.substr(12,2);
	//tmp += "sec";
	return tmp;
}

void 
INTERVISION::CLog_T::SetOptions(unsigned options)
{
	m_LogOptions =options;
}

void 
INTERVISION::CLog_T::ClearOptions(unsigned options)
{
	m_LogOptions &= ~options;
}

void 
INTERVISION::CLog_T::AddOptions(unsigned options)
{
	m_LogOptions |= options;
}

unsigned 
INTERVISION::CLog_T::GetOptions()
{
	return m_LogOptions;
}

void 
INTERVISION::CLog_T::SetLevel(unsigned level)
{
	if (level>=MAX_LEVEL) 
	{
		m_LogLevel=1;
	}
	else 
	{
		m_LogLevel = level;
	}
}

unsigned 
INTERVISION::CLog_T::GetLevel()
{
	return m_LogLevel;
}

bool 
INTERVISION::CLog_T::CanTrace(unsigned level)
{
	return ( level <= m_LogLevel && m_bEnable );
}

string 
INTERVISION::CLog_T::AddPath(const char *fileName)
{
	string FileName;
	FileName = m_LogPath;
	FileName += fileName;
	return FileName;
}

void  
INTERVISION::CLog_T::ReturnFileName(char *filename)
{
	//按天产生日志的文件名
	string FileName;
	if( !m_Suffix.empty() )
	{
		FileName += m_Prefix;
	}
	FileName += CLog_T::GetFileTime();
	FileName += ".";
	FileName += m_Suffix;
	strcpy(filename,FileName.c_str());
}

short 
INTERVISION::CLog_T::BeginLogHead(unsigned level)
{
	//按天产生日志
	if( m_LogOptions&FileByDay )
	{
		string FileName;
		FileName = CLog_T::GetFileTime();
		FileName = FileName.substr(0,10);
		if( !m_Suffix.empty() )
		{
			FileName += "_";
			FileName += m_Suffix;
		}
		FileName += ".log";
		
		//日期改变因此文件名改变
		if( FileName != m_LogFileName )
		{
			if( m_pf )
				fclose(m_pf);
			m_LogFileName = FileName;
			FileName = AddPath( FileName.c_str() );
			m_pf = fopen(FileName.c_str(),"a");			
		}
		//日期没变但是还没有打开过文件
		else if( !m_pf )
		{
			m_LogFileName = FileName;
			FileName = AddPath( FileName.c_str() );
			m_pf = fopen(FileName.c_str(),"a");
		}
	}
	//不按天产生日志,文件名不为空并且文件还没有打开
	else if( m_LogOptions&FileByName )
	{
		if( m_LogFileName.c_str() != NULL && !m_pf)
		{
			string file;
			file = AddPath( m_LogFileName.c_str() );
			m_pf = fopen(m_LogFileName.c_str(),"a");
		}
	}
	//输出到屏幕
	else 
		m_pf = stdout;

	if( !m_pf )
		return -1;
		
	short size=0;
	
	//lock();	//lock是为了不使写文件出现混乱
	
	if ((m_LogOptions&FileAndLine) != 0 && m_DebugFileName != "" ) 
	{
		const char * file = strrchr(m_DebugFileName.c_str(), '/');
		if (file != NULL)
		{
			file++;
		}
		else 
		{
			file = strrchr(m_DebugFileName.c_str(), '\\');
			if (file != NULL)
			{
				file++;
			}
			else
			{
				file = m_DebugFileName.c_str();
			}
		}	
		size += fprintf(m_pf,"%s%s%d%s",file,"(",m_LineNum,")---");	
	}
	//unlock();

	return size;
}

short 
INTERVISION::CLog_T::BeginLog(unsigned level)
{
	//按天产生日志
	if( m_LogOptions&FileByDay )
	{
		string FileName;
		if( !m_Suffix.empty() )
		{
			FileName = m_Prefix;
		}
		FileName += CLog_T::GetFileTime();
		FileName += ".";
		FileName += m_Suffix;

		//日期改变因此文件名改变
		if( FileName != m_LogFileName )
		{
			if( m_pf )
				fclose(m_pf);
			m_LogFileName = FileName;
			FileName = AddPath( FileName.c_str() );
			m_pf = fopen(FileName.c_str(),"a");			
		}
		//日期没变但是还没有打开过文件
		else if( !m_pf )
		{
			m_LogFileName = FileName;
			FileName = AddPath( FileName.c_str() );
			m_pf = fopen(FileName.c_str(),"a");
		}
	}
	//不按天产生日志,文件名不为空并且文件还没有打开
	else if( m_LogOptions&FileByName )
	{
		if( m_LogFileName.c_str() != NULL && !m_pf)
		{
			string file;
			file = AddPath( m_LogFileName.c_str() );
			m_pf = fopen(m_LogFileName.c_str(),"a");
		}
	}
	//输出到屏幕
	else 
		m_pf = stdout;

	if( !m_pf )
		return -1;
		
	short size=0;
	
	//lock();	//lock是为了不使写文件出现混乱
	if ((m_LogOptions&TraceLevel) != 0)
	{
		size = fprintf(m_pf,"%s%d%s","Level(",level,") ");
	}
	
    if ((m_LogOptions&DateAndTime) != 0) 
	{
		size += fprintf(m_pf,"%s  ",FormatTime(CLog_T::GetSystemTime()).c_str() );	  
    }
	
	if ((m_LogOptions&FileAndLine) != 0 && m_DebugFileName != "" ) 
	{
		const char * file = strrchr(m_DebugFileName.c_str(), '/');
		if (file != NULL)
		{
			file++;
		}
		else 
		{
			file = strrchr(m_DebugFileName.c_str(), '\\');
			if (file != NULL)
			{
				file++;
			}
			else
			{
				file = m_DebugFileName.c_str();
			}
		}
		
		size += fprintf(m_pf,"%s%s%d%s",file,"(",m_LineNum,")---");		
	}
	//unlock();

	return size;
}

#ifdef ANSI
long 
INTERVISION::CLog_T::UserlogHead(unsigned level,const char *fmt,...)
{	
	if (!CanTrace(level)) 
		return 0;

	lock();//加锁是为了写文件出现混乱
	
	int ret = BeginLogHead(level);
	if( ret < 0 )
	{
		unlock();
		return ret;	
	}

	try
	{
		va_list argptr;
		va_start(argptr, fmt);
#ifdef WIN32
		_vsnprintf((char*)&m_strBuf, TBUF_SIZE, fmt, argptr);
#else
		vsnprintf((char*)&m_strBuf, TBUF_SIZE, fmt, argptr);
#endif
		va_end(argptr);
	}
	catch (...)
	{
		unlock();
		return -2;
	}
	long size = fprintf(m_pf,"%s\n",m_strBuf);
#ifdef _DEBUG
	Flush();
#endif
	unlock();
	return size;
}
#endif

#ifdef ANSI
long 
INTERVISION::CLog_T::Userlog(unsigned level,const char *fmt,...)
{
	if (!CanTrace(level)) 
		return 0;
	lock();//加锁是为了写文件出现混乱
	int ret = BeginLog(level);
	if( ret<0 )
	{
		unlock();
		return ret;	
	}
	try
	{
		va_list argptr;
		va_start(argptr, fmt);
#ifdef WIN32
		_vsnprintf((char*)&m_strBuf, TBUF_SIZE, fmt, argptr);
#else
		vsnprintf((char*)&m_strBuf, TBUF_SIZE, fmt, argptr);
#endif
		va_end(argptr);
	}
	catch (...)
	{
		unlock();
		return -2;
	}
	long size = fprintf(m_pf,"%s\n",m_strBuf);
#ifdef _DEBUG
	Flush();
#endif

/***********************/
	fflush(m_pf);
/************************/
	
	unlock();

	return size;
}
#else
long 
INTERVISION::CLog_T::Userlog(unsigned level,const char *fmt,va_alist)
va_dcl
{

}
#endif
	
short 
INTERVISION::CLog_T::SetFile( const char *fileName )
{
	assert(fileName);
	
	if( m_LogOptions&FileByDay || m_LogOptions&DisplayScreen || m_LogFileName == fileName )
		return 0;
	m_LogFileName = fileName;
	
	if (!CanTrace(m_LogLevel)) 
		return 0;

	string tmp;
	tmp = AddPath(fileName);

	if( m_pf )
		fclose(m_pf);
	m_pf = fopen(tmp.c_str(),"a");
	if( !m_pf )
		return -1;
	return 1;
}

void 
INTERVISION::CLog_T::SetLogPath(const char *logPath)
{
	if( !strcmp(logPath,"") )
		return;
	strcpy((char*)m_LogPath,logPath);
#ifdef WIN32
	if( !strcmp(logPath,".") )
		strcat(m_LogPath,"\\");
	if( logPath[strlen(logPath)-1]!='\\' )
			strcat((char*)m_LogPath,"\\");
#else
	if( !strcmp(logPath,".") )
		strcat(m_LogPath,"/");
	if( logPath[strlen(logPath)-1]!='/' )
			strcat((char*)m_LogPath,"/");
#endif			
	CreatePath(m_LogPath);
}

void 
INTERVISION::CLog_T::lock()
{
	#if defined(WIN32)
	EnterCriticalSection(&_mutex);
#else
	if( pthread_mutex_lock(&_mutex) != 0 )
		printf("Error:	Lock fail!\n");
#endif
}

void 
INTERVISION::CLog_T::unlock()
{
	#if defined(WIN32)
	LeaveCriticalSection(&_mutex);
#else
	if ( pthread_mutex_unlock(&_mutex) != 0 )
		printf("Error:	Unlock fail!\n");
#endif
}
		
