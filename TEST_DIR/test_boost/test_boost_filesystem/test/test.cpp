// test.cpp : 定义控制台应用程序的入口点。
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
	std::string s = p.string();    //得到路径的字符串表示 
	std::string s1 = p.leaf().string();     //得到该路径最右边的部分. 比如路径为 abc/x.txt 时. 返回 x.txt
	
	//判断p是否是一个文件夹
	if (fs::is_directory(p))
	{
		//如果p是一个文件夹. 我们可以用 directory_iterator 对象遍历它的下一层内容:
		boost::filesystem::directory_iterator iter(p), end_iter;
		for (; iter!=end_iter; ++iter)
		{
			// 若是文件夹. 
			if (is_directory(*iter))
			{ 
				//如果需要递归的遍历. 可以在这里做...
			}
			else if(is_regular(*iter)) //真正的文件...
			{
				//string s = extension(*iter);   //扩展名
			}
			else if(is_symlink(*iter)) //连接...
			{
			}
		} //for
	}

	//例如有路径为  abc/def/xyz.txt
	//我们可以通过如下遍历来得到 "abc" , "def" , "xyz..txt" 三个部分:
	for (boost::filesystem::path::iterator iter = p.begin(); iter != p.end(); ++iter)
	{ 
		//std::string s = *iter;//取出路径中的每个字符串.
		std::cout << *iter << std::endl;
	}

  

	//is_empty();
	//last_write_time();    //上次修改时间
	//create_directory();   //新建文件夹
	//remove();
	//remove_all();
	//copy_file();
	//current_path();



	//判断p表示的文件或文件夹是否存在.
	if ( !fs::exists( p ) )
	{
		std::cout << "not found: " << szPath << std::endl;
		return 1;
	}

	//指判断指定的路径是否标准(正确) 
	//或者判断文件是否完整
	if ( !fs::is_regular( p ) )
	{
		std::cout << "not a regular file: " << szPath << std::endl;
		return 1;
	}

	std::cout << "size of " << szPath << " is " << fs::file_size( p )
		<< std::endl;
	return 0;
}

