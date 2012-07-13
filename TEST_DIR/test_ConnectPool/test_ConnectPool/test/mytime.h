#ifndef __MY_TIME_H
#define __MY_TIME_H
/******************************************************************************* 
* 1、 文件名称： mytime
* 2、 版　　本： Version *.*
* 3、 描    述： 以秒计算超时类
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-2-20 14:22:21
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 
#include <time.h> 

namespace QNA
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
		WTime(){ m_lOutTime = 0;}
		/* ********************
		 * a method 
		 * @func IsTimeout [whether time out or not]
		 * @return  [true if time out,or false]
		 */
		bool IsTimeout()
		{
			EndTime();
			if (!m_lOutTime)
			{
				m_lOutTime = 30;
			}
			
			if(m_end - m_start >= m_lOutTime)
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
			InitilizeValue(m_start);
		}

		void SetTime(long lOutTime = 30)
		{
			m_lOutTime = lOutTime;
		}
		
	private:
		timeValues m_start, m_end;
		tm * m_ptm;
		time_t m_ts;
		long m_lOutTime;


	private:
		inline void InitilizeValue(timeValues &t)
		{
			t.tm_year = m_ptm->tm_year;
			t.tm_mon = m_ptm->tm_mon;
			t.tm_mday = m_ptm->tm_mday;
			t.tm_hour = m_ptm->tm_hour;
			t.tm_min = m_ptm->tm_min;
			t.tm_sec = m_ptm->tm_sec;
		}
		inline void GetTime()
		{
			m_ts = time(NULL);
			m_ptm = localtime(&m_ts);
		}
		void EndTime()
		{
			GetTime();
			InitilizeValue(m_end);
		}

	};

}

#endif