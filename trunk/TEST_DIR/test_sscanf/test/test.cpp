// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>

// C ����ʱͷ�ļ�

#include <ATLComTime.h>

DATE string2date(char* ptInTime)
{
	DATE dtRet = 0;
	if (!ptInTime) return dtRet;

	int iYear = 0, iMonth = 0, iDay = 0, iHour = 0, iMin = 0, iSec = 0;
	sscanf_s("1970-02-04 08:57:32", "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);

	COleDateTime dtCurrent(iYear, iMonth, iDay, iHour, iMin, iSec);

	dtRet = dtCurrent.m_dt;
	return dtRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//1. �����÷���
	char buf[512] ;
	sscanf("123456 ", "%s", buf);//�˴�buf����������������˼�ǽ�123456��%s����ʽ����buf�У�
	printf("%s\n", buf);
	//���Ϊ��123456

	//2. ȡָ�����ȵ��ַ��������������У�ȡ��󳤶�Ϊ4�ֽڵ��ַ�����
	sscanf("123456 ", "%4s", buf);
	printf("%s\n", buf);
	//���Ϊ��1234

	//3. ȡ��ָ���ַ�Ϊֹ���ַ��������������У�ȡ�����ո�Ϊֹ�ַ�����
	sscanf("123456 abcdedf", "%[^ ]", buf);
	printf("%s\n", buf);
	//���Ϊ��123456

	//4. ȡ������ָ���ַ������ַ��������������У�ȡ������1��9��Сд��ĸ���ַ�����
	sscanf("123456abcdedfBCDEF", "%[1-9a-z]", buf);
	printf("%s\n", buf);
	//���Ϊ��123456abcdedf	
	//�����룺
	sscanf("123456abcdedfBCDEF","%[1-9A-Z]",buf);
	printf("%s\n",buf);
	//���Ϊ��123456

	//5. ȡ��ָ���ַ���Ϊֹ���ַ��������������У�ȡ������д��ĸΪֹ���ַ�����
	sscanf("123456abcdedfBCDEF", "%[^A-Z]", buf);
	printf("%s\n", buf);
	//���Ϊ��123456abcdedf

	//6������һ���ַ���iios/12DDWDFF@122����ȡ / �� @ ֮����ַ������Ƚ� "iios/"���˵����ٽ���'@'��һ�������͵�buf��
	sscanf("iios/12DDWDFF@122", "%*[^/]/%[^@]", buf);
	printf("%s\n", buf);
	//���Ϊ��12DDWDFF

	//7������һ���ַ���"hello, world"��������world����ע�⣺"��"֮����һ�ո�%s���ո�ֹͣ����*���Ǻ��Ե�һ���������ַ�����
	sscanf("hello, world", "%*s%s", buf);
	printf("%s\n", buf);
	//���Ϊ��world
	//%*s��ʾ��һ��ƥ�䵽��%s�����˵�����hello��������
	//���û�пո�����ΪNULL��
	//sscanf�Ĺ��ܺ�������������ʽ, ��ȴû��������ʽǿ��,����������ڱȽϸ��ӵ��ַ�������,����ʹ��������ʽ.


	//-------------------------------------------------------
	//�������ָ������������ַ���2006:03:18:
	int a, b, c;
	/*sscanf("2006:03:18", "%d:%d:%d", a, b, c); */ /*���󷽷�, Ҫ�ڱ���a,b,cǰ����ȡ��ַ��, modified by huanmie_09*/
	sscanf("2006:03:18", "%d:%d:%d", &a, &b, &c);
	//�Լ�2006:03:18 - 2006:04:18:
	char sztime1[16] = "", sztime2[16] = "";
	sscanf("2006:03:18 - 2006:04:18", "%s - %s", sztime1, sztime2);
	//���Ǻ���������Ҫ����2006:03:18-2006:04:18
	//������ȡ���ˡ�-�����ߵĿո�ȴ������%s���ַ����Ľ綨��
	//����Ҫ�������һ������������������������Ⲣ�����ӣ����ǣ�Ϊ��ʹ���еĴ��붼��ͳһ�ķ������Ҫ�Ķ��ܶ�ط��������е�sscanf�滻�����Լ��ķָ����
	//����Ϊ�ҿ϶���Ҫ���������������Ŷ�sscanf��ǿ�Ҳ�������˯��һ��������������ʵ���ء�
	//format-type����%[]������type field�������ȡ���ַ����������Կո����ָ��Ļ����Ϳ���ʹ��%[]��
	//%[]������һ��������ʽ��[a-z]��ʾ��ȡa-z�������ַ���[^a-z]��ʾ��ȡ��a-z����������ַ��������Ǹ�����Ҳ��ӭ�ж�����:
	sscanf("2006:03:18 - 2006:04:18", "%[0-9,:] - %[0-9,:]", sztime1, sztime2);
	//-------�Ǳ�������---------------------------------------------------------------------------------------------------
	sscanf("2006:03:18-2006:04:18","%[^-]-%s",sztime1,sztime2);
	//������%[^-]ƥ�䵽��-��Ĭ��Ϊ�浽sztime1���ַ�����2006:03:18���ӿո��������%s��Ĭ�����롰-2006:04:18����sztime2��Ҳ����%sǰ��ӡ�-����ԭ�����������һ���������׶����׶���
	//----------------------------------------------------------------------------------------------------end-----------------
	

	//1970-01-01 08:00:00
	int iYear = 0, iMonth = 0, iDay = 0, iHour = 0, iMin = 0, iSec = 0;
	//sscanf("1970-02-04 08:57:32", "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);
	sscanf_s("1970-02-04 08:57:32", "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);

	char szTem[MAX_PATH] = {0};
	sprintf_s(szTem, MAX_PATH, "%04d%02d%02d%02d%02d%02d00", iYear, iMonth, iDay, iHour, iMin, iSec);

	DATE dtTime = string2date("1970-02-04 08:57:32");

	
	memset(szTem, 0, MAX_PATH);
	strcpy_s(szTem, MAX_PATH, "310107100063290000Z");
	char* pTem = szTem+strlen(szTem)-5;


	int iTem = atoi(pTem);


	return 0;
}

