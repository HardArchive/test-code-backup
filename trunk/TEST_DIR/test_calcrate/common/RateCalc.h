//file RateCalc.h
/***************************************************************************************************
* 1�� File       �� RateCalc.h
* 2�� Version    �� *.*
* 3�� Description�� 
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2014-1-21 15:06:50
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __RATE_CALC_H__
#define __RATE_CALC_H__

typedef struct STORAGE_TIME_NODE
{
	bool bIsMonth;
	int iTime;              //����  ��������     ����Ƕ��ڴ���õ� 
}STORAGETIME, *PSTORAGETIME;

//Regular rates
typedef struct REGULAR_RATES_NODE
{
	STORAGETIME stuStorageTime;
	double dRates;               //���� ����Ϊ�����Ϊ��
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
		//��ʱ���갴�´��
		//dInPrincipalÿ�ζ��汾��
		//Լ������ pstuInRegularRates
		//stuInIntervalTime ����洢ʱ��  �೤ʱ���һ��
		//stuInStorageTime����ʱ��  ����һ�� 10�� 
		double GetRegular(const double dInPrincipal, const PREGULARRATES pstuInRegularRates, const STORAGETIME stuInIntervalTime, const STORAGETIME stuInStorageTime)
		{
			int iCount = 0;    //����Ĵ���
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

		//��ȡ��Ϣ ���и�������  �簴�������� ���һ��ȡ�õı�Ϣ��  
		//dInPrincipal ���� stuInRegularRates������� stuInStorageTime���ʱ��
		double GetPrincipalInterest(const double dInPrincipal, const PREGULARRATES pstuInRegularRates, const STORAGETIME stuInStorageTime)
		{
			int iCount = 0;                    //����
			double dPrincipal = dInPrincipal;  //����
			iCount = CalcSuntoryCount(pstuInRegularRates->stuStorageTime, stuInStorageTime);
			for (int i=0; i<iCount; i++)
			{
				dPrincipal = GetRegularPrincipalInterest((int)dPrincipal, pstuInRegularRates);
				dPrincipal = HandleNum(dPrincipal);
			}
			return dPrincipal;
		}



		//����ȡ�ñ�Ϣ dInPrincipal���� pstuInRegularRates�������� ���ն��ڵ�ʱ������Ӧ�úͱ���+��Ϣ
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
			//���㱾Ϣ��
			dInterest += dInPrincipal;
			return dInterest;
		}
	private:
		//���ת�����·�
		int Year2Month(int iInYear){return 12*iInYear;}
		//�·�ת�������
		double Month2Year(double dInMonth){return dInMonth/12;}

		//һ��double��ֵ��������λС���Ե���λ��������
		double HandleNum(double dInNum)
		{
			double dRet = dInNum*100;
			dRet = (int)(dRet+0.5);
			dRet = dRet/100.0;
			return dRet;
		}
		
		//���� �����·ݺ�ʵ���·�֮��ı��� �������������
		//stuInRatesStorageTime ���ʼ���ʱ�� stuInStorageTimeʵ�ʴ洢ʱ�� bIsRemainder �Ƿ�������
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

