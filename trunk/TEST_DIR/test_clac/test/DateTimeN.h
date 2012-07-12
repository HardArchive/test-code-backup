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
		*	构造参数格式为没有分隔符的日期，如：2001-02-11
		**/
		DateTimeN(int y,int m,int d,int h = 0,int mi = 0,int s = 0,int milli = 0);
		DateTimeN(time_t sec);
		static DateTimeN& seconds(time_t sec);
		static DateTimeN& now();
		/**
		*	返回年份
		*/
		int year();
		/**
		*	返回月份
		*/
		int month();
		/**
		*	返回日期
		*/
		int day();
		/**
		*	返回小时
		*/
		int hour();
		/**
		*	返回分钟
		*/
		int minute();
		/**
		*	返回秒数
		*/
		int second();
		/**
		*	返回毫秒
		*/
		int millisec();
		/**
		*	返回日期,0 – 6; Sunday = 0
		*/
		int week();
		/**
		*	返回当前日期是一年中的第几天
		*/
		int day_of_year();
		/**
		*	当前日期后几天
		*/
		DateTimeN& operator+(int d);
		/**
		*	当前日期前几天
		*/
		DateTimeN& operator-(int d);
		/**
		*	两个日期的间隔
		*/
		DateTimeN& operator-(DateTimeN& dt);
		/**
		*	两个日期相隔的天数
		*/
		long DaysInterval();
		/**
		*	两个日期相隔的小时数
		*/
		long HoursInterval();
		/**
		*	两个日期相隔的分钟数
		*/
		long MinutesInterval();
		/**
		*	两个日期相隔的秒数
		*/
		long SecondsInterval();
		/**
		*	两个日期相隔的毫秒数
		*/
		long MilliSecInterval();
		/**
		*	两个日期的比较
		*/
		bool operator==(const DateTimeN& dt);
		bool operator>=(const DateTimeN& dt);
		bool operator<=(const DateTimeN& dt);
		bool operator>(const DateTimeN& dt);
		bool operator<(const DateTimeN& dt);
		bool operator!=(const DateTimeN& dt);
		/**
		*	当前日期后前或者后几年
		*/
		DateTimeN& addYears(int y);
		/**
		*	当前日期后前或者后几月
		*/
		DateTimeN& addMonths(int m);
		/**
		*	当前日期后前或者后几周
		*/
		DateTimeN& addWeeks(int w);
		/**
		*	当前日期后前或者后几小时
		*/
		DateTimeN& addHours(int h);
		/**
		*	当前日期后前或者后几分钟
		*/
		DateTimeN& addMinutes(int m);
		/**
		*	当前日期后前或者后几秒
		*/
		DateTimeN& addSeconds(int s);
		/**
		*	当前日期转换成字符串
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