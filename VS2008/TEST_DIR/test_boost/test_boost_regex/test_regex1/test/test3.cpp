#include "stdafx.h"

#include <iostream>
#include <cassert>
#include <string>
#include <boost/regex.hpp>
using namespace std;
using namespace boost;
//regex_search()������ʹ��

int main3()
{
	regex reg("(new)|(delete)");
	smatch m;
	string s = "Calls to new mustbe followed by delete.  Calling simply new results in aleak!";
	int new_counter = 0;
	int delete_counter = 0;
	string::const_iterator it = s.begin();
	//    string::const_iterator end = s.end();
	while (regex_search(it, (string::const_iterator)s.end(), m, reg)) 
	{
		m[1].matched ? ++new_counter : ++delete_counter;  //���ҽ��
		it = m[0].second;                             //���Ҵ��λ��
	}
	if (new_counter != delete_counter)
		cout<<"Leak detected!\n";
	else
		cout<<"Seems OK...\n";
	return 0;
}
//
//������������У�ͳ��new��delete���������ʵ���Ŀ�ǲ���һ����
//ע�⣺smatch��������ʵ��һ��match_results���ͣ��� ��������¼reg�е������Ƿ�ƥ�䡣
//�����new����ƥ���ˣ���ôm[1].matched�ͻ�Ϊ��(true)��m[0].second��ʾit��� �ƶ��ˣ�����ƥ��ʣ�µ��ַ�����
//
//m[0]�����ص��Ƕ�ƥ������������ʽ����ƥ������ã���˿���ȷ�����ƥ��Ľ�����ͨ�����´�����regex_search��������ʼ�㡣