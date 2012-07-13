#ifndef __DateTimeN__H_
#define __DateTimeN__H_
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <sys/timeb.h>
#include <time.h>
using namespace std;


namespace Matr
{
	class DateTimeN
	{
	public:
		DateTimeN();
		/**
		*	���������ʽΪû�зָ��������ڣ��磺2001-02-11
		**/
		DateTimeN(int y,int m,int d,int h = 0,int mi = 0,int s = 0,int milli = 0);
		DateTimeN(time_t sec);
		static DateTimeN& seconds(time_t sec);
		static DateTimeN& now();
		/**
		*	�������
		*/
		int year();
		/**
		*	�����·�
		*/
		int month();
		/**
		*	��������
		*/
		int day();
		/**
		*	����Сʱ
		*/
		int hour();
		/**
		*	���ط���
		*/
		int minute();
		/**
		*	��������
		*/
		int second();
		/**
		*	���غ���
		*/
		int millisec();
		/**
		*	��������,0 �C 6; Sunday = 0
		*/
		int week();
		/**
		*	���ص�ǰ������һ���еĵڼ���
		*/
		int day_of_year();
		/**
		*	��ǰ���ں���
		*/
		DateTimeN& operator+(int d);
		/**
		*	��ǰ����ǰ����
		*/
		DateTimeN& operator-(int d);
		/**
		*	�������ڵļ��
		*/
		DateTimeN& operator-(DateTimeN& dt);
		/**
		*	�����������������
		*/
		long DaysInterval();
		/**
		*	�������������Сʱ��
		*/
		long HoursInterval();
		/**
		*	������������ķ�����
		*/
		long MinutesInterval();
		/**
		*	�����������������
		*/
		long SecondsInterval();
		/**
		*	������������ĺ�����
		*/
		long MilliSecInterval();
		/**
		*	�������ڵıȽ�
		*/
		bool operator==(const DateTimeN& dt);
		bool operator>=(const DateTimeN& dt);
		bool operator<=(const DateTimeN& dt);
		bool operator>(const DateTimeN& dt);
		bool operator<(const DateTimeN& dt);
		bool operator!=(const DateTimeN& dt);
		/**
		*	��ǰ���ں�ǰ���ߺ���
		*/
		DateTimeN& addYears(int y);
		/**
		*	��ǰ���ں�ǰ���ߺ���
		*/
		DateTimeN& addMonths(int m);
		/**
		*	��ǰ���ں�ǰ���ߺ���
		*/
		DateTimeN& addWeeks(int w);
		/**
		*	��ǰ���ں�ǰ���ߺ�Сʱ
		*/
		DateTimeN& addHours(int h);
		/**
		*	��ǰ���ں�ǰ���ߺ󼸷���
		*/
		DateTimeN& addMinutes(int m);
		/**
		*	��ǰ���ں�ǰ���ߺ���
		*/
		DateTimeN& addSeconds(int s);
		/**
		*	��ǰ����ת�����ַ���
		*/
		string toString(const string& format = "%Y%m%d%H%M%S");
	private:
		static DateTimeN	_dt;
		static tm			_tm;
		static int			_static_millisec;
		static time_t		_static_sec;
		int					_millisec;
		long				_secondstamp;
		long				_millisecstamp;
		tm&					getTm();
		tm					_currTm;
	};
};
#endif