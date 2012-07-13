

#ifndef __MY_TIME_H
#define __MY_TIME_H
#include <time.h> 

namespace pool
{
	struct timeValues
	{
		int tm_year;                  /* Year - 1900.  */
		int tm_mon;                   /* Month.       [0-11] */
		int tm_mday;                  /* Day.         [1-31] */
		int tm_hour;                  /* Hours.       [0-23] */
		int tm_min;                   /* Minutes.     [0-59] */
		int tm_sec;                   /* Seconds.     [0-60] (1 leap second) */
	public:
		long long operator - (timeValues t1)
		{
			long long temp;

			temp = (tm_year - t1.tm_year)*12;
			temp = (tm_mon+temp - t1.tm_mon)*30;
			temp = (tm_mday + temp - t1.tm_mday)*24;
			temp = (tm_hour + temp - t1.tm_hour)*60;
			temp = (tm_min + temp - t1.tm_min)*60;

			temp = tm_sec + temp - t1.tm_sec;

			return temp;
		}

	};

	/*
	* WTime：计时类，判断是否超时
	* function :
	* @ bool IsTimeout(void) 判断是否超时
	* @return false 没有超时，true 已经超时
	* @ void StartTime() 开始计时
	*/
	class WTime
	{
	public:
		WTime(){ }
		/* ********************
		 * a method 
		 * @func IsTimeout [whether time out or not]
		 * @return  [true if time out,or false]
		 */
		bool IsTimeout()
		{
			EndTime();
			if(end - start >= 30)
				return true;
			else
				return false;
		}
		/* *********************
		 * a method 
		 * @func StartTime [get current time]
		 * @param no param
		 * @return no return 
		 */
		void StartTime()
		{
			GetTime();
			InitilizeValue(start);
		}
		
	private:
		timeValues start,end;
		tm *ptm;
		time_t ts;

	private:
		inline void InitilizeValue(timeValues &t)
		{
			t.tm_year = ptm->tm_year;
			t.tm_mon = ptm->tm_mon;
			t.tm_mday = ptm->tm_mday;
			t.tm_hour = ptm->tm_hour;
			t.tm_min = ptm->tm_min;
			t.tm_sec = ptm->tm_sec;
		}
		inline void GetTime()
		{
			ts = time(NULL);
			ptm = localtime(&ts);
		}
		void EndTime()
		{
			GetTime();
			InitilizeValue(end);
		}

	};

}

#endif