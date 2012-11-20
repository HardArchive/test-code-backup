// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "test_official.h"

//XML加载选项
bool load_XML_options()
{
	pugi::xml_document doc;

	//[code_load_options
	const char* source = "<!--comment--><node>&lt;</node>";

	//默认选项 注释节点将不会添加到树中,和实体参考&lt;扩大
	// Parsing with default options; note that comment node is not added to the tree, and entity reference &lt; is expanded
	doc.load(source);
	std::cout << "First node value: [" << doc.first_child().value() << "], node child value: [" << doc.child_value("node") << "]\n";

	// Parsing with additional parse_comments option; comment node is now added to the tree
	doc.load(source, pugi::parse_default | pugi::parse_comments);
	std::cout << "First node value: [" << doc.first_child().value() << "], node child value: [" << doc.child_value("node") << "]\n";

	// Parsing with additional parse_comments option and without the (default) parse_escapes option; &lt; is not expanded
	doc.load(source, (pugi::parse_default | pugi::parse_comments) & ~pugi::parse_escapes);
	std::cout << "First node value: [" << doc.first_child().value() << "], node child value: [" << doc.child_value("node") << "]\n";

	// Parsing with minimal option mask; comment node is not added to the tree, and &lt; is not expanded
	doc.load(source, pugi::parse_minimal);
	std::cout << "First node value: [" << doc.first_child().value() << "], node child value: [" << doc.child_value("node") << "]\n";
	//]
	return true;
}



int _tmain(int argc, _TCHAR* argv[])
{
	//test_official

	//Loading document
	//Loading_document();
	//load_error_handling();
	//Load_XML_memory();
	//load_stream();

	//Accessing document data
	//traverse_base();
	//traverse_iter();
	//traverse_rangefor();
	//traverse_walker();


	//Modifying document data
	//modify_base();
	//modify_add();
	//modify_remove();

	//Saving document
	//save_file();
	//save_stream();
	save_custom_writer();


	getchar();
	return 0;
}

