// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "gflags/gflags.h"
#pragma comment(lib, "libgflags-debug.lib")

//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

#include <iostream>

#include "foo.h"
//int main(int argc, char ** argv) {��}
//����argcָ����argv����ĳ��ȣ���argv������ո�Ϊ�ָ�洢�����к��û�����Ĳ���
//int main(int argc,char*argv[])
//{
//	std::string usage("This program does nothing.  Sample usage:\n");
//	usage += std::string(argv[0])+" --port 1234 \n or :\n -flagfile=foo.conf";
//
//	google::SetUsageMessage(usage);  //�����趨usage˵��
//
//	//RegisterFlagValidatorע�������麯��  ��һ������Ϊ֮ǰ�����������Ĳ���	�ڶ�������Ϊ��麯��	����ĸ�����������ɼ�gflags.h
//	bool port_dummy = google::RegisterFlagValidator(&FLAGS_port, &ValidatePort);
//
//	//���������ע�ắ������ɺ󣬵���ParseCommandLineFlags������ʼ���������С�
//	google::ParseCommandLineFlags(&argc,&argv,true);
//
//	std::cout<<"port :"<< FLAGS_port<< std::endl;
//
//	return EXIT_SUCCESS;
//}


#include <Windows.h>
int main(int argc,char*argv[])
{
	std::string usage("This program does nothing.  Sample usage:\n");

	int iargc = 0;
	char szargv[MAX_PATH] = {0};
	usage += std::string(argv[0])+" --port 1234 \n or :\n -flagfile=foo.conf";

	google::SetUsageMessage(usage);  //�����趨usage˵��

	//RegisterFlagValidatorע�������麯��  ��һ������Ϊ֮ǰ�����������Ĳ���	�ڶ�������Ϊ��麯��	����ĸ�����������ɼ�gflags.h
	bool port_dummy = google::RegisterFlagValidator(&FLAGS_port, &ValidatePort);

	//���������ע�ắ������ɺ󣬵���ParseCommandLineFlags������ʼ���������С�
	google::ParseCommandLineFlags(&argc,&argv,true);

	std::cout<<"port :"<< FLAGS_port<< std::endl;

	return EXIT_SUCCESS;
}
