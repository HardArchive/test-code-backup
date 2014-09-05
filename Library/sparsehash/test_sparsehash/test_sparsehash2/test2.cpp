// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
#include <string>
#include <iostream>
#include "my_modify_google_hashmap.h"


void testGoogleHashMap()
{
	{
		typedef TStringDenseHashMap<std::string> StringDenseHashMap;
		StringDenseHashMap map1;
		size_t nSize = 0;

		map1["张三"] = "张三";
		map1["李四"] = "李四";
		map1["王五"] = "王五";
		nSize = map1.size();

		StringDenseHashMap::iterator iter = map1.find("张三");
		std::string strResult;
		if (iter != map1.end())
		{
			strResult = iter->second;
		}
		std::cout << map1["张三"] << "," << map1["李四"] << "," << map1["王五"] << std::endl;

		map1.erase("张三");
		map1.erase("李四");
		map1.erase("王五");
		nSize = map1.size();
	}

	{
		typedef TStringSparseHashMap<std::string> StringSparseHashMap;
		StringSparseHashMap map1;
		size_t nSize = 0;

		map1["张三"] = "张三";
		map1["李四"] = "李四";
		map1["王五"] = "王五";
		nSize = map1.size();

		StringSparseHashMap::iterator iter = map1.find("张三");
		std::string strResult;
		if (iter != map1.end())
		{
			strResult = iter->second;
		}
		std::cout << map1["张三"] << "," << map1["李四"] << "," << map1["王五"] << std::endl;

		map1.erase("张三");
		map1.erase("李四");
		map1.erase("王五");
		nSize = map1.size();
	}

	{
		typedef TConstCharDenseHashMap<const char*> ConstCharDenseHashMap;
		ConstCharDenseHashMap map1;
		size_t nSize = 0;

		map1["张三"] = "张三";
		map1["李四"] = "李四";
		map1["王五"] = "王五";
		nSize = map1.size();

		ConstCharDenseHashMap::iterator iter = map1.find("张三");
		std::string strResult;
		if (iter != map1.end())
		{
			strResult = iter->second;
		}
		std::cout << map1["张三"] << "," << map1["李四"] << "," << map1["王五"] << std::endl;

		map1.erase("张三");
		map1.erase("李四");
		map1.erase("王五");
		nSize = map1.size();
	}

	{
		typedef TConstCharSparseHashMap<std::string> ConstCharSparseHashMap;
		ConstCharSparseHashMap map1;
		size_t nSize = 0;

		map1["张三"] = "张三";
		map1["李四"] = "李四";
		map1["王五"] = "王五";
		nSize = map1.size();

		ConstCharSparseHashMap::iterator iter = map1.find("张三");
		std::string strResult;
		if (iter != map1.end())
		{
			strResult = iter->second;
		}
		std::cout << map1["张三"] << "," << map1["李四"] << "," << map1["王五"] << std::endl;

		map1.erase("张三");
		map1.erase("李四");
		map1.erase("王五");
		nSize = map1.size();
	}
};

int main()
{
	testGoogleHashMap();

	std::cin.get();  
	return 0;
}
