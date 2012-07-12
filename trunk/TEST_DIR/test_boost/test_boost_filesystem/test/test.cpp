// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//  file_size program  -------------------------------------------------------//

//  Copyright Beman Dawes, 2004

//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/filesystem for documentation.

#include <boost/filesystem/operations.hpp>
#include <iostream>
//#include "test3.h"
namespace fs = boost::filesystem;

int main1( int argc, char* argv[] )
{
	char szPath[128] = "C:\\WINDOWS\\system32\\notepad.exe";

	//if ( argc != 2 )
	//{
	//	std::cout << "Usage: file_size path\n";
	//	return 1;
	//}

	std::cout << "sizeof(intmax_t) is " << sizeof(boost::intmax_t) << '\n';

	fs::path p(szPath, fs::native);
	std::string s = p.string();    //�õ�·�����ַ�����ʾ 
	std::string s1 = p.leaf().string();     //�õ���·�����ұߵĲ���. ����·��Ϊ abc/x.txt ʱ. ���� x.txt
	
	//�ж�p�Ƿ���һ���ļ���
	if (fs::is_directory(p))
	{
		//���p��һ���ļ���. ���ǿ����� directory_iterator �������������һ������:
		boost::filesystem::directory_iterator iter(p), end_iter;
		for (; iter!=end_iter; ++iter)
		{
			// �����ļ���. 
			if (is_directory(*iter))
			{ 
				//�����Ҫ�ݹ�ı���. ������������...
			}
			else if(is_regular(*iter)) //�������ļ�...
			{
				//string s = extension(*iter);   //��չ��
			}
			else if(is_symlink(*iter)) //����...
			{
			}
		} //for
	}

	//������·��Ϊ  abc/def/xyz.txt
	//���ǿ���ͨ�����±������õ� "abc" , "def" , "xyz..txt" ��������:
	for (boost::filesystem::path::iterator iter = p.begin(); iter != p.end(); ++iter)
	{ 
		//std::string s = *iter;//ȡ��·���е�ÿ���ַ���.
		std::cout << *iter << std::endl;
	}

  

	//is_empty();
	//last_write_time();    //�ϴ��޸�ʱ��
	//create_directory();   //�½��ļ���
	//remove();
	//remove_all();
	//copy_file();
	//current_path();



	//�ж�p��ʾ���ļ����ļ����Ƿ����.
	if ( !fs::exists( p ) )
	{
		std::cout << "not found: " << szPath << std::endl;
		return 1;
	}

	//ָ�ж�ָ����·���Ƿ��׼(��ȷ) 
	//�����ж��ļ��Ƿ�����
	if ( !fs::is_regular( p ) )
	{
		std::cout << "not a regular file: " << szPath << std::endl;
		return 1;
	}

	std::cout << "size of " << szPath << " is " << fs::file_size( p )
		<< std::endl;
	return 0;
}

