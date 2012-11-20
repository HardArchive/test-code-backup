// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "pugixml.hpp"
#include "pugiconfig.hpp"

using namespace pugi;

//Loading document
int Loading_document();   //�����ļ�
	void check_xml(const char* source); //���XML�ַ���
int load_error_handling();
bool Load_XML_memory();
int load_stream();  //�����м��� 


//Accessing document data
int traverse_base();  //��������
int traverse_iter();  //����������
//int traverse_rangefor(); //���C++11�й� ��ʱ����
int traverse_walker();


//Modifying document data
int modify_base();
int modify_add();
int modify_remove();


//Saving document
int save_file();
int save_stream();
int save_custom_writer();