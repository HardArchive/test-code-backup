// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//#include <iostream>
//#include <boost/regex.hpp>
//
//using namespace std;


#include <cstdlib>
#include <stdlib.h>
#include <boost/regex.hpp>
#include <string>
#include <iostream>
using namespace std;
using namespace boost;


//���������롰select �ַ��� from �ַ�������ʱ�򣬻�õ��������������������ʽ���ͻ����error��
//  ([a-zA-Z]*) ��ʾ���������ĸ����Сд���ɣ���ɵ��ַ���
regex expression("select ([a-zA-Z]*) from ([a-zA-Z]*)");

int main(int argc, char* argv[])
{
	std::string in;
	cmatch what;
	cout << "enter test string" << endl;
	// select aaa from bbb
	getline(cin, in);
	if(regex_match(in.c_str(), what, expression))
	{
		//for(int i=0;i<what.size();i++)
		//	cout<<"str :"<<what.str()<<endl;
		//��ʾ�����Ӵ�
		for(boost::cmatch::iterator itr= what.begin(); itr != what.end(); ++itr)
		{
			//       ָ���Ӵ���Ӧ��λ��        ָ���Ӵ���Ӧβλ��          �Ӵ�����
			cout << "�Ӵ���Ӧ��λ�ã�" << itr->first-in.c_str() ;
			cout << "  �Ӵ���Ӧβλ�ã�" << itr->second-in.c_str();
			cout << "  �Ӵ�����      ��" << *itr << endl;
		}
	}
	else
	{
		cout<<"Error Input"<<endl;
	}
	return 0;
}