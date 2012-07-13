/***************************************************************
* Author��		aron zhao
* Company��		Intervision							  			
* Last Update��	2005-01-17		 								
* Copyright(c), Intervision Software Co., Ltd. Beijing. 
* 				All Rights Reserved	
*------------------------------------------------------------ 	
* FileName��		IVCLog.h
* Function Note��	Sys Log implementation.
* Update note��		Created at 2003-09-10
*****************************************************************/

//Level 1���������̲�Ӧ�÷����Ĺ��ϣ���new����ʧ�ܣ��ļ�IO����ʧ�ܣ��쳣�˳��ȵȣ�
//Level 2��������Ч���жϣ��Լ�ָ��NULL�ж�Ϊ�����͹���
//Level 3: ������;�쳣���أ�
//Level 4: ������Ϣ�Ĵ�ӡ�����
//Level 5: ������������Ҫ�������ݵĵ��������
//Ĭ����־������3�����������־����־��¼��ʱ���;���û������FileByDay|FileByNameѡ�Ĭ��ΪDisplayScreen���������Ļ

#ifndef  _IV_CLOG_H_
#define  _IV_CLOG_H_

#define MAX_LEVEL  256

#define TBUF_SIZE 102400
#define PATH_MAX_SIZE 256

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#define ANSI

#ifndef HAVE_STD_IOSTREAM
#define HAVE_STD_IOSTREAM
#endif

#include <string>
#include <stdarg.h>

//#include <MTL/MTL.h>

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif


namespace INTERVISION
{

	//class MTCout_T : public MTLMonitor
	//{
	//public:

	//	void write(const char* buf);
	//};


	class CLog_T
	{
	#if defined(WIN32)
		CRITICAL_SECTION	_mutex;
	#else
		pthread_mutex_t		_mutex;  //linux
	#endif
	protected:
		FILE *m_pf;
		string m_Suffix;
		string m_Prefix;

		unsigned m_LogOptions;       //��־����
		unsigned m_LogLevel;         //��־�ȼ� 
		string m_LogFileName;        //��־�ļ���
		char m_strBuf[TBUF_SIZE];
		char m_LogPath[PATH_MAX_SIZE];
		bool m_bEnable;
		string m_DebugFileName;
		int m_LineNum;
		
		//��ʱ���ʽ��20030910061630��Ϊ2003 09-10 10:30:30
		string FormatTime(string systemTime);
		void lock();
		void unlock();
		//д��־��¼��ͷ
		short BeginLog(unsigned level);
		//����ʱ���,ÿ��ֻдһ��
		short BeginLogHead(unsigned level);	
		//���ļ���ǰ����·��
		string AddPath(const char *fileName);

		void SetFileAndLine(const char *fileName,int lineNum)
		{
			m_DebugFileName = fileName;
			m_LineNum = lineNum;
		}

	public:		
		// �������ѡ��
		enum Options {    
			FileByDay = 1,	     //�������־
			DateAndTime = 2,     //����ʱ���			
			TraceLevel = 4,      //�����ǰ�ļ���
			FileAndLine = 16,    //��ѡ����ʱû����Ϊ����׼����
			DisplayScreen = 32,  //����Ļ����ʾ
			FileByName = 64		 //��ָ���ļ���������־
		};	

		CLog_T();
		~CLog_T();
		CLog_T( unsigned level,unsigned options,const char *filename );
		void  ReturnFileName(char *filename);

		// ����ѡ��
		void SetOptions(unsigned options );	
		void ClearOptions(unsigned options );	
		unsigned GetOptions();
		void AddOptions(unsigned options);
		
		// ���õȼ�
		void SetLevel(unsigned level );	
		unsigned GetLevel();
		
		//�жϵ��Եȼ��Ƿ�ﵽ
		bool CanTrace(unsigned level );
		//������־�ļ�����ֻ�������ó�FileByNameʱ����Ч
		short SetFile( const char *fileName );
		void SetLogPath( const char *logPath );
		//�򿪹ر���־���
		void Enable(bool enable=true )
		{
			m_bEnable = enable;
		}
		//���������־ʱ�����ļ�����ǰ׺���׺������prefix��ivsender, suffix��log��ʱ����2005-01-17���ļ���Ϊivsender20050117.log
		void SetPrefixSuffix(const char *prefix, const char *suffix)
		{
			m_Prefix = prefix;
			m_Suffix = suffix;
		}
		//����ļ���
		void Flush()
		{
			fflush(m_pf);
		}
		//��ȡϵͳʱ�䣬���β���ϵͳ
		static string GetSystemTime();
		static string GetFileTime();
		//д��־�ĺ�������д��ʽ��printf���ƽ�������һ����־�������
	#ifdef ANSI	
		long Userlog(unsigned level,const char *fmt,...);
	#else
		long Userlog(unsigned level,const char *fmt,va_alist);
	#endif	

	#ifdef ANSI	
		long UserlogHead(unsigned level,const char *fmt,...);
	#endif
	};
		
} // End of namespace INTERVISION

#endif // (_IV_CLOG_H_)
