// test.cpp : �������̨Ӧ�ó������ڵ㡣
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

		map1["����"] = "����";
		map1["����"] = "����";
		map1["����"] = "����";
		nSize = map1.size();

		StringDenseHashMap::iterator iter = map1.find("����");
		std::string strResult;
		if (iter != map1.end())
		{
			strResult = iter->second;
		}
		std::cout << map1["����"] << "," << map1["����"] << "," << map1["����"] << std::endl;

		map1.erase("����");
		map1.erase("����");
		map1.erase("����");
		nSize = map1.size();
	}

	{
		typedef TStringSparseHashMap<std::string> StringSparseHashMap;
		StringSparseHashMap map1;
		size_t nSize = 0;

		map1["����"] = "����";
		map1["����"] = "����";
		map1["����"] = "����";
		nSize = map1.size();

		StringSparseHashMap::iterator iter = map1.find("����");
		std::string strResult;
		if (iter != map1.end())
		{
			strResult = iter->second;
		}
		std::cout << map1["����"] << "," << map1["����"] << "," << map1["����"] << std::endl;

		map1.erase("����");
		map1.erase("����");
		map1.erase("����");
		nSize = map1.size();
	}

	{
		typedef TConstCharDenseHashMap<const char*> ConstCharDenseHashMap;
		ConstCharDenseHashMap map1;
		size_t nSize = 0;

		map1["����"] = "����";
		map1["����"] = "����";
		map1["����"] = "����";
		nSize = map1.size();

		ConstCharDenseHashMap::iterator iter = map1.find("����");
		std::string strResult;
		if (iter != map1.end())
		{
			strResult = iter->second;
		}
		std::cout << map1["����"] << "," << map1["����"] << "," << map1["����"] << std::endl;

		map1.erase("����");
		map1.erase("����");
		map1.erase("����");
		nSize = map1.size();
	}

	{
		typedef TConstCharSparseHashMap<std::string> ConstCharSparseHashMap;
		ConstCharSparseHashMap map1;
		size_t nSize = 0;

		map1["����"] = "����";
		map1["����"] = "����";
		map1["����"] = "����";
		nSize = map1.size();

		ConstCharSparseHashMap::iterator iter = map1.find("����");
		std::string strResult;
		if (iter != map1.end())
		{
			strResult = iter->second;
		}
		std::cout << map1["����"] << "," << map1["����"] << "," << map1["����"] << std::endl;

		map1.erase("����");
		map1.erase("����");
		map1.erase("����");
		nSize = map1.size();
	}
};

int main()
{
	testGoogleHashMap();

	std::cin.get();  
	return 0;
}
