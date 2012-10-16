// TestC.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
 

//�ж�pStr2�Ƿ���pStr1�ĺ�
bool IsSuffixNoCase(const char *pStr1,int len1,const char *pStr2,int len2)
{
	//�������û�к�׺��
	if(len1 <=len2)
	{
		return false;
	}

	pStr1 +=(len1-len2);
	for(int i=0;i<len2;i++)
	{
		if(pStr2[i]!=(pStr1[i]|0x20))
		{
			return false;
		}
	}
	return true;
}

int GetDomainLevel(const char *pDomain,int domainLen)
{
	//������׺�б� com.cn һ��Ҫ����cnǰ�� �������Ƶ�����Ҳһ��
	static struct SUFFIX
	{
		char *suffix;
		int len;
	}suffixList[]={
		".com",4,
		".com.cn",7,
		".cn",3,
		NULL,0
	};

	if(domainLen <4)
	{
		return 1;
	}
	if((pDomain[0]|0x20)=='w' &&(pDomain[1]|0x20)=='w' &&(pDomain[2]|0x20)=='w' && pDomain[3]=='.')
	{
		return 1;
	}

	for(SUFFIX *pCurSuffix=&suffixList[0];pCurSuffix->suffix!=NULL;pCurSuffix++)
	{
		//����Ǻ�׺
		if(IsSuffixNoCase(pDomain,domainLen,pCurSuffix->suffix,pCurSuffix->len))
		{
			//��ȡ��һ�����Ƕ�������
			bool bFindDot=false;
			int cutLen=domainLen-pCurSuffix->len;
			for(int j=0;j<cutLen;j++)
			{
				if(pDomain[j]=='.')
				{
					bFindDot=true;
					break;
				}
			}
			if(bFindDot)
			{
				return 2;
			}
			else
			{
				return 1;
			}
		}
	}
	//ִ�е���˵���ͺ�׺�б�ƥ�� ����ֻ���Ե�ĸ������ж�����
	int dotNums=0;
	for(int i=0;i<domainLen;i++)
	{
		if(pDomain[i]=='.')
		{
			dotNums++;
		}
	}
	if(dotNums>=2)
	{
		return 2;
	}
	else
	{
		return 1;
	}
	return 1;
}








int _tmain(int argc, _TCHAR* argv[])
{
	char *p="ab.com.ab";
	int level=GetDomainLevel(p,strlen(p));
	getchar();
	return 0;
}