//file RateCalc.h
/***************************************************************************************************
* 1、 File       ： RateCalc.h
* 2、 Version    ： *.*
* 3、 Description： 
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2014-1-21 15:06:50
* 6、 History    ： 
* 7、 Remark     ： 计息起点:储蓄存款利息计算时，本金以“元”为起息点，元以下的角、分不计息，利息的金额算至分位，分位以下四舍五入。
							分段计息算至厘位，合计利息后分以下四舍五入。
					分段计息:分段计息是存款人或借款人在存（贷）款期内遇到国家调整利率，从存入（借入）日至调整日，银行按原合同同档次利率计付（计收）利息；
							从调整日开始按调整后的同档次利率计付或计收。如再有调整，同理类推。它是银行的计息专用术语，
							它适用于本外币的活期存款、外币的浮动利率流动资金贷款等。
							注意定期存款是不分段计息的，存期内如遇利率调整，仍沿用存款日的约定利率。
							定期存款不分段计息是由定期存款的性质决定的。
							在定期存款存入时，储户和银行双方就存款期限、利率等合同要素达成一致，并以存单的形式载明双方的权利和义务，
							在约定存期内，一方不得擅自更改约定的相关内容，这对于保护存款人和银行双方的利益具有重要意义
****************************************************************************************************/
#ifndef __RATE_CALC_H__
#define __RATE_CALC_H__

typedef struct STORAGE_TIME_NODE
{
	bool bIsMonth;
	int iTime;              //年数  或者月数     这个是定期存款用的 
}STORAGETIME, *PSTORAGETIME;

//Regular rates
typedef struct REGULAR_RATES_NODE
{
	STORAGETIME stuStorageTime;
	double dRates;               //利率 可能为年可能为月
}REGULARRATES, *PREGULARRATES;

//Bank Regular rates
typedef struct BANK_REGULAR_RATES_NODE
{
	TCHAR tszBank[32];
	REGULARRATES stuRegularRates[7];
}BANKREGULARRATES, *PBANKREGULARRATES;


namespace RG
{
	class CRateCalc
	{
	public:
		CRateCalc(){}

		~CRateCalc(){}


	
	public:
		//定时按年按月存款
		//dInPrincipal每次定存本金
		//约定利率 pstuInRegularRates
		//stuInIntervalTime 间隔存储时间  多长时间存一次
		//stuInStorageTime定存时间  比如一年 10年 
		double GetRegular(const double dInPrincipal, const PREGULARRATES pstuInRegularRates, const STORAGETIME stuInIntervalTime, const STORAGETIME stuInStorageTime)
		{
			int iCount = 0;    //计算的次数
			double dAllPrincipal = 0;

			STORAGETIME stuCalcStorageTime;
			stuCalcStorageTime.bIsMonth = stuInIntervalTime.bIsMonth;
			if (stuInIntervalTime.bIsMonth)
			{				
				stuCalcStorageTime.iTime = stuInStorageTime.bIsMonth?stuInStorageTime.iTime:Year2Month(stuInStorageTime.iTime);
			}
			else
			{
				double dTime = stuInStorageTime.bIsMonth?Month2Year(stuInStorageTime.iTime):stuInStorageTime.iTime;
				stuCalcStorageTime.iTime = (int)dTime;
				if (dTime>stuCalcStorageTime.iTime)
				{
					stuCalcStorageTime.iTime++;
				}
			}


			iCount = stuCalcStorageTime.iTime;
			for (int i=0; i<iCount; i++)
			{
				dAllPrincipal += GetPrincipalInterest(dInPrincipal, pstuInRegularRates, stuCalcStorageTime);
				stuCalcStorageTime.iTime--;
			}
			return dAllPrincipal;
		}

		//获取本息 进行复利计算  如按定期三月 存放一年取得的本息和  
		//dInPrincipal 本金 stuInRegularRates存放利率 stuInStorageTime存放时间
		double GetPrincipalInterest(const double dInPrincipal, const PREGULARRATES pstuInRegularRates, const STORAGETIME stuInStorageTime)
		{
			int iCount = 0;                    //倍数
			double dPrincipal = dInPrincipal;  //本金
			iCount = CalcSuntoryCount(pstuInRegularRates->stuStorageTime, stuInStorageTime);
			for (int i=0; i<iCount; i++)
			{
				dPrincipal = GetRegularPrincipalInterest((int)dPrincipal, pstuInRegularRates);
				dPrincipal = HandleNum(dPrincipal);
			}
			return dPrincipal;
		}



		//定期取得本息 dInPrincipal本金 pstuInRegularRates定期利率 按照定期的时间后计算应得和本金+利息
		double GetRegularPrincipalInterest(const int dInPrincipal, const PREGULARRATES pstuInRegularRates)
		{
			double dTime = 0;
			double dInterest = 0;
			if (!pstuInRegularRates) return 0;
			if (pstuInRegularRates->stuStorageTime.bIsMonth)
			{
				dInterest = dInPrincipal*pstuInRegularRates->dRates;
				dInterest = pstuInRegularRates->stuStorageTime.iTime*dInterest;
				dInterest = dInterest/12;
				//dInterest = dInPrincipal*pstuInRegularRates->dRates*(pstuInRegularRates->stuStorageTime.iTime/12);
			}
			else
			{
				dInterest = dInPrincipal*pstuInRegularRates->dRates;
				dInterest = pstuInRegularRates->stuStorageTime.iTime*dInterest;
				//dInterest = dInPrincipal*pstuInRegularRates->dRates*pstuInRegularRates->stuStorageTime.iTime;
			}

			dInterest = dInterest/100;
			dInterest = HandleNum(dInterest);
			//计算本息和
			dInterest += dInPrincipal;
			return dInterest;
		}
	private:
		//年份转化成月份
		int Year2Month(int iInYear){return 12*iInYear;}
		//月份转化成年份
		double Month2Year(double dInMonth){return dInMonth/12;}

		//一个double数值，保留两位小数对第三位四舍五入
		double HandleNum(double dInNum)
		{
			double dRet = dInNum*100;
			dRet = (int)(dRet+0.5);
			dRet = dRet/100.0;
			return dRet;
		}
		
		//计算 定期月份和实存月份之间的倍数 如有余数则忽略
		//stuInRatesStorageTime 利率计算时间 stuInStorageTime实际存储时间 bIsRemainder 是否有余数
		int CalcSuntoryCount(const STORAGETIME stuInRatesStorageTime, const STORAGETIME stuInStorageTime)
		{
			int iRet = 0;
			double dTime = 0;
			if (stuInRatesStorageTime.bIsMonth)
			{
				dTime = stuInStorageTime.bIsMonth?stuInStorageTime.iTime:Year2Month(stuInStorageTime.iTime);
				iRet = (int)dTime/stuInRatesStorageTime.iTime;
			}
			else
			{
				dTime = stuInStorageTime.bIsMonth?Month2Year(stuInStorageTime.iTime):stuInStorageTime.iTime;
				iRet = (int)dTime/stuInRatesStorageTime.iTime;
			}
			return iRet;
		}
	};
}
#endif /*__RATE_CALC_H__*/

