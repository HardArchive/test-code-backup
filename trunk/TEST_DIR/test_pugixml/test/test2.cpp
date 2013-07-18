// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include "pugixml.hpp"
#include "pugiconfig.hpp"

using namespace pugi;

int testPrintxgconsoleXML()
{
	pugi::xml_document doc;
	if (!doc.load_file("xgconsole.xml")) return -1;
	pugi::xml_node tools = doc.child("Profile").child("Tools");

	//[code_traverse_base_basic
	for (pugi::xml_node tool = tools.first_child(); tool; tool = tool.next_sibling())
	{
		std::cout << "Tool:";
		for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute())
		{
			std::cout << " " << attr.name() << "=" << attr.value();
		}
		std::cout << std::endl;
	}
	//]
	
	return 1;
}
//新建xml的时候
void TestCreatXML()
{
	pugi::xml_document doc;
	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-8";
	doc.save(std::cout, "", pugi::format_raw);
	std::cout << std::endl;

	// print document to standard output as a regular node (prints <foo bar="baz"><call>hey</call></foo>)
	doc.print(std::cout, "", pugi::format_raw);
	std::cout << std::endl;

	//******************************Rg添加*********************************************
	pugi::xml_node node = doc.append_child("node");

	// add description node with text child
	pugi::xml_node descr = node.append_child("description");
	descr.append_child(pugi::node_pcdata).set_value("Simple node");

	// add param node before the description
	pugi::xml_node param = node.insert_child_before("param", descr);

	// add attributes to param node
	param.append_attribute("name") = "version";
	param.append_attribute("value") = 1.1;
	param.insert_attribute_after("type", param.attribute("name")) = "float";
	//]

	doc.print(std::cout);
	//******************************Rg添加*********************************************

	// print a subtree to standard output (prints <call>hey</call>)
	doc.child("foo").child("call").print(std::cout, "", pugi::format_raw);
	std::cout << std::endl;
	std::cout << "Saving result: " << doc.save_file("save_file_output.xml") << std::endl;
}


//创建XML文件
bool TestCreateXML2()
{
	pugi::xml_document doc;
	//添加指定类型的子节点。返回添加的节点，或空节点错误。 <?xml version="1.0" encoding="UTF-8"?>
	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	//添加属性指定的名称。返回添加的属性，或空属性错误。
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "UTF-8";

	//添加根结点
	pugi::xml_node node = doc.append_child("node");
	//根结点下添加子节点
	pugi::xml_node descr = node.append_child("description");
	//添加纯字符数据
	descr.append_child(pugi::node_pcdata).set_value("Simple node");


	//在节点descr之前插入节点
	pugi::xml_node param = node.insert_child_before("param", descr);

	//设置节点属性
	param.append_attribute("name") = "version";
	param.append_attribute("value") = 1.1;
	//设置type属性在name属性之后
	param.insert_attribute_after("type", param.attribute("name")) = "float";
	//保存XML
	doc.save_file("save_file_output2.xml");
	return true;
}

int test_paramsxml()
{
	std::string strFile = "params.xml";
	pugi::xml_document doc;
	if (!doc.load_file(strFile.c_str()))
		return 0;
	pugi::xml_node form = doc.child("root").child("form");
	std::string ip = form.attribute("ip").value();
	std::string port = form.attribute("port").value();

	char cBuf[2083];
	sprintf(cBuf, "http://%s:%s/?", ip.c_str(), port.c_str());
	std::string strTemp(cBuf);
	std::string m_strURLBase = strTemp;

	for (pugi::xml_node input = form.first_child(); input;
		input = input.next_sibling())
	{
		std::string strValue = input.attribute("value").value();
		if (!strValue.empty())
		{
			std::string strName = input.attribute("name").value();
			sprintf(cBuf, "%s=%s&", strName.c_str(), strValue.c_str());
			std::string strTemp(cBuf);
			m_strURLBase += strTemp;
		}
	}

	//读取xpath
	pugi::xml_node xpath = doc.child("root").child("xpath");
	std::string m_strPOIRoot = xpath.attribute("poiroot").value();
	std::string m_strPOIID = xpath.attribute("idfield").value();

	//读取评分权重
	pugi::xml_node weight = doc.child("root").child("weight");
	float m_fThred = atof(weight.child_value("threshold"));
	float m_fStep = atof(weight.child_value("step"));

	return 1;
}

int test_xpath()
{
	//其中，xpath根路径：m_strPOIRoot="//list/poi",
	//需要取值的项：strPOIID=“pguid”，strPOINam=“name”。
	//读取代码：
	//从strWebContent内容中解析出pguid和name
	std::string m_strPOIRoot="//list/poi";
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load("XPath.xml");
	if (!result)
	{return -1;}
	pugi::xpath_node_set tools = doc.select_nodes(m_strPOIRoot.c_str());
	for (pugi::xpath_node_set::const_iterator it = tools.begin();
		it !=  tools.end(); ++it)
	{
		pugi::xpath_node node = *it;
		std::string strPOI = node.node().child_value("pguid");
		std::string strName = node.node().child_value("name");
	}
	return 1;
}

int _tmain2(int argc, _TCHAR* argv[])
{ 
	test_paramsxml();
	int test_xpath();
	TestCreatXML();
	TestCreateXML2();
	/*printf("#####testPrintxgconsoleXML Start\r\n");
	testPrintxgconsoleXML();
	printf("#####testPrintxgconsoleXML End\r\n\n");
	xml_document doc;
	xml_parse_result result = doc.load_file("text_tree.xml");
	xml_node xnode = doc.child("mesh").first_child();

	printf("xnode.child(\"xxx\").text().get():%s\r\n\n", xnode.child("xxx").text().get());
	printf("xnode.child(\"xxx\").value():%s\r\n\n", xnode.child("xxx").value());
	printf("xnode.child(\"xxx\")first_child().value():%s\r\n\n", xnode.child("xxx").first_child().value());
	
	while (xnode.empty() == false)
	{
		if (strcmpi(xnode.name(), "node") == 0)
		{
			printf("xnode.attribute(\"attr1\"):%s\n", xnode.attribute("attr1").value());  
		}
		printf("xnode.name:%s\n", xnode.name());

		if (strcmpi(xnode.name(), "xxx") == 0)
		{
			printf("xnode.value():%s\n", xnode.value()); 
			xnode.first_child().set_value("useless comment");
			printf("xnode.text().get():%s\n", xnode.text().get());
			printf("xnode.first_child().value():%s\n", xnode.first_child().value());

		}
		xnode = xnode.next_sibling();
	}*/
	getchar();
	return 0;
}

