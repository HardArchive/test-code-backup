#include "stdafx.h"
#include "DateTimeN.h"

namespace Matr
{
	DateTimeN DateTimeN::_dt;
	tm	DateTimeN::_tm;
	time_t DateTimeN::_static_sec = 0;
	int DateTimeN::_static_millisec = 0;
	DateTimeN::DateTimeN()
	{
		struct timeb tb;
		::ftime(&tb);
		memcpy(&_currTm,localtime(&tb.time),sizeof(tm));
		_secondstamp = tb.time;
		_millisec = tb.millitm;
		_millisecstamp = _secondstamp * 1000 + _millisec;
	}
	
	DateTimeN::DateTimeN(int y,int m,int d,int h,int mi,int s,int milli):_millisec(milli),
																				_secondstamp(0)
	{
		_currTm.tm_year = y;
		_currTm.tm_mon = m;
		_currTm.tm_mday = d;
		_currTm.tm_hour = h;
		_currTm.tm_min = mi;
		_currTm.tm_sec = s;
		_secondstamp = mktime(&_currTm);
		_millisecstamp = _secondstamp * 1000 + _millisec;
	}
	DateTimeN::DateTimeN(time_t sec)
	{
		memcpy(&_currTm,localtime(&sec),sizeof(tm));
	}
	DateTimeN& DateTimeN::seconds(time_t sec)
	{
		memcpy(&_tm,localtime(&sec),sizeof(tm));
		return _dt;
	}
	DateTimeN& DateTimeN::now()
	{
		struct timeb tb;
		::ftime(&tb);
		memcpy(&_tm,localtime(&tb.time),sizeof(tm));
		_static_millisec = tb.millitm;
		_static_sec = tb.time;
		return _dt;
	}
	
	int DateTimeN::year()
	{
		return _tm.tm_year;
	}
	
	int DateTimeN::month()
	{
		return _tm.tm_mon;
	}
	
	int DateTimeN::day()
	{
		return _tm.tm_mday;
	}
	
	int DateTimeN::hour()
	{
		return _tm.tm_hour;
	}
	
	int DateTimeN::minute()
	{
		return _tm.tm_min;
	}
	
	int DateTimeN::second()
	{
		return _tm.tm_sec;
	}
	
	int DateTimeN::millisec()
	{
		return _static_millisec;
	}
	
	int DateTimeN::week()
	{
		return _tm.tm_wday;
	}
	
	int DateTimeN::day_of_year()
	{
		return _tm.tm_yday;
	}
	
	DateTimeN& DateTimeN::operator+(int d)
	{
		_static_sec = _static_sec + d * 24 * 60 * 60;
		memcpy(&_tm,localtime(&_static_sec),sizeof(tm));
		return _dt;
	}
	
	DateTimeN& DateTimeN::operator-(int d)
	{
		_static_sec = _static_sec - d * 24 * 60 * 60;
		memcpy(&_tm,localtime(&_static_sec),sizeof(tm));
		return _dt;
	}
	
	DateTimeN& DateTimeN::operator-(DateTimeN& dt)
	{
		_currTm.tm_year = _currTm.tm_year - dt.getTm().tm_year;
		_currTm.tm_mon = _currTm.tm_mon - dt.getTm().tm_mon;
		_currTm.tm_mday = _currTm.tm_mday - dt.getTm().tm_mday;
		_currTm.tm_hour = _currTm.tm_hour - dt.getTm().tm_hour;
		_currTm.tm_min = _currTm.tm_min - dt.getTm().tm_min;
		_currTm.tm_sec = _currTm.tm_sec - dt.getTm().tm_sec;
		_currTm.tm_wday = _currTm.tm_wday - dt.getTm().tm_wday;
		_currTm.tm_yday = _currTm.tm_yday - dt.getTm().tm_yday;
		this->_secondstamp = this->_secondstamp - dt._secondstamp;
		this->_millisecstamp = this->_millisecstamp - dt._millisecstamp;
		return *this;
	}
	long DateTimeN::DaysInterval()
	{
		return this->_secondstamp / (3600 * 24);
	}
	long DateTimeN::HoursInterval()
	{
		return this->_secondstamp / (3600);
	}
	long DateTimeN::MinutesInterval()
	{
		return this->_secondstamp / (60);
	}
	long DateTimeN::SecondsInterval()
	{
		return this->_secondstamp;
	}
	long DateTimeN::MilliSecInterval()
	{
		return this->_millisecstamp;
	}
	
	bool DateTimeN::operator==(const DateTimeN& dt)
	{
		return (this->_secondstamp == dt._secondstamp);
	}
	bool DateTimeN::operator>=(const DateTimeN& dt)
	{
		return (this->_secondstamp >= dt._secondstamp);
	}
	bool DateTimeN::operator<=(const DateTimeN& dt)
	{
		return (_secondstamp <= dt._secondstamp);
	}
	bool DateTimeN::operator>(const DateTimeN& dt)
	{
		return (_secondstamp > dt._secondstamp);
	}
	bool DateTimeN::operator<(const DateTimeN& dt)
	{
		return (_secondstamp < dt._secondstamp);
	}
	bool DateTimeN::operator!=(const DateTimeN& dt)
	{
		return (_secondstamp != dt._secondstamp);
	}
	
	DateTimeN& DateTimeN::addYears(int y)
	{
		_tm.tm_year += y;
		return _dt;
	}
	DateTimeN& DateTimeN::addMonths(int m)
	{
		_tm.tm_year += (_tm.tm_mon + m) / 12; 
		_tm.tm_mon += (_tm.tm_mon + m) % 12; 
		return _dt;
	}
	DateTimeN& DateTimeN::addWeeks(int w)
	{
		_static_sec = _static_sec  + w * 7 * 24 * 60 * 60;
		memcpy(&_tm,localtime(&_static_sec),sizeof(tm));
		return _dt;
	}
	DateTimeN& DateTimeN::addHours(int h)
	{
		_static_sec = _static_sec  + h * 60 * 60;
		memcpy(&_tm,localtime(&_static_sec),sizeof(tm));
		return _dt;
	}
	DateTimeN& DateTimeN::addMinutes(int m)
	{
		_static_sec = _static_sec  + m * 60;
		memcpy(&_tm,localtime(&_static_sec),sizeof(tm));
		return _dt;
	}
	DateTimeN& DateTimeN::addSeconds(int s)
	{
		_static_sec = _static_sec  + s;
		memcpy(&_tm,localtime(&_static_sec),sizeof(tm));
		return _dt;
	}
	string DateTimeN::toString(const string& format)
	{
		char tmpbuf[128] = {0};
		string ret = "";
		std::string::size_type pos = format.find("%l");
		if(pos == std::string::npos)
		{
			strftime(tmpbuf,128,format.c_str(),&_tm);
			ret = tmpbuf;
		}
		else
		{
			strftime(tmpbuf,128,format.substr(0,pos).c_str(),&_tm);
			ret = tmpbuf;
			std::ostringstream formatStream;
			formatStream << tmpbuf << std::setw(3) << std::setfill('0') << _static_millisec;
			ret = formatStream.str();
		}
		return ret;
	}
	tm&	DateTimeN::getTm()
	{
		return _currTm;
	}
};
