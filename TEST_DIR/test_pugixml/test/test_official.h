// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "pugixml.hpp"
#include "pugiconfig.hpp"

using namespace pugi;

//Loading document
int Loading_document();   //加载文件
	void check_xml(const char* source); //检查XML字符串
int load_error_handling();
bool Load_XML_memory();
int load_stream();  //从流中加载 


//Accessing document data
int traverse_base();  //基本遍历
int traverse_iter();  //迭代器遍历
//int traverse_rangefor(); //这个C++11有关 暂时不用
int traverse_walker();


//Modifying document data
int modify_base();
int modify_add();
int modify_remove();


//Saving document
int save_file();
int save_stream();
int save_custom_writer();