// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "test_official.h"
#include <fstream>
#include <iomanip>
#include <iostream>

//pugixml提供了几个功能 - 文件加载XML数据，从不同的地方，C + +的iostream，内存缓冲区。所有的功能使用一个非常快速的非验证解析器。
//这个解析器是不充分W3C符合性 - 它可以加载任何有效的XML文件，但不执行某些格式检查。
//虽然作出相当大的努力，拒绝无效的XML文件，不进行一些验证，因为性能的原因。将被转换成XML数据解析之前，内部字符格式。
//pugixml支持所有流行的Unicode编码（UTF-8，UTF-16（大和小endian），UTF-32（大和小尾数法）; UCS-2是自然的支持，
//因为它是一个严格的子集为UTF-16）和处理所有自动编码转换。

//最常见的来源XML数据是的文件; pugixml提供了一个单独的函数从文件加载XML文件。
//这个函数的第一个参数接受文件路径，还有两个可选的参数，用来指定解析选项和输入数据的编码，这是手册中描述的。

//这是一个例子，从文件加载XML文档（采样/ load_file.cpp）：

int Loading_document()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("text_tree.xml");
	std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
	return 1;
}



//pugixml错误检查
void check_xml(const char* source)
{
	//[code_load_error_handling
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load(source);

	if (result)
		std::cout << "XML [" << source << "] parsed without errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n\n";
	else
	{
		std::cout << "XML [" << source << "] parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
		std::cout << "Error description: " << result.description() << "\n";
		std::cout << "Error offset: " << result.offset << " (error at [..." << (source + result.offset) << "]\n\n";
	}
	//]
}

//错误XML内容加载测试
int load_error_handling()
{
	check_xml("<node attr='value'><child>text</child></node>");
	check_xml("<node attr='value'><child>text</chil></node>");
	check_xml("<node attr='value'><child>text</child>");
	check_xml("<node attr='value\"><child>text</child></node>");
	check_xml("<node attr='value'><#tag /></node>");
	return 1;
}

//加载xml格式的字符串，使用xml_document类的load接口
bool Load_XML_memory()
{
	//[code_load_memory_decl
	const char source[] = "<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>";
	size_t size = sizeof(source);
	//]

	pugi::xml_document doc;

	{
		//[code_load_memory_buffer
		// You can use load_buffer to load document from immutable memory block:
		pugi::xml_parse_result result = doc.load_buffer(source, size);
		//]

		std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
	}

	{
		//[code_load_memory_buffer_inplace
		// You can use load_buffer_inplace to load document from mutable memory block; the block's lifetime must exceed that of document
		char* buffer = new char[size];
		memcpy(buffer, source, size);

		// The block can be allocated by any method; the block is modified during parsing
		pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, size);

		//<-
		std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
		//->
		// You have to destroy the block yourself after the document is no longer used
		delete[] buffer;
		//]
	}

	{
		//[code_load_memory_buffer_inplace_own
		// You can use load_buffer_inplace_own to load document from mutable memory block and to pass the ownership of this block
		// The block has to be allocated via pugixml allocation function - using i.e. operator new here is incorrect
		char* buffer = static_cast<char*>(pugi::get_memory_allocation_function()(size));
		memcpy(buffer, source, size);

		// The block will be deleted by the document
		pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, size);
		//]

		std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
	}

	{
		//[code_load_memory_string
		// You can use load to load document from null-terminated strings, for example literals:
		pugi::xml_parse_result result = doc.load("<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>");
		//]

		std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
	}
	return true;
}




void print_doc(const char* message, const pugi::xml_document& doc, const pugi::xml_parse_result& result)
{
	std::cout
		<< message
		<< "\t: load result '" << result.description() << "'"
		<< ", first character of root name: U+" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << pugi::as_wide(doc.first_child().name())[0]
	<< ", year: " << doc.first_child().first_child().first_child().child_value()
		<< std::endl;
}

bool try_imbue(std::wistream& stream, const char* name)
{
	try
	{
		stream.imbue(std::locale(name));

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}

int load_stream()
{
	pugi::xml_document doc;

	{
		//[code_load_stream
		std::ifstream stream("weekly-utf-8.xml");
		pugi::xml_parse_result result = doc.load(stream);
		//]

		// first character of root name: U+9031, year: 1997
		print_doc("UTF8 file from narrow stream", doc, result);
	}

	{
		std::ifstream stream("weekly-utf-16.xml");
		pugi::xml_parse_result result = doc.load(stream);

		// first character of root name: U+9031, year: 1997
		print_doc("UTF16 file from narrow stream", doc, result);
	}

	{
		// Since wide streams are treated as UTF-16/32 ones, you can't load the UTF-8 file from a wide stream
		// directly if you have localized characters; you'll have to provide a UTF8 locale (there is no
		// standard one; you can use utf8_codecvt_facet from Boost or codecvt_utf8 from C++0x)
		std::wifstream stream("weekly-utf-8.xml");

		if (try_imbue(stream, "en_US.UTF-8")) // try Linux encoding
		{
			pugi::xml_parse_result result = doc.load(stream);

			// first character of root name: U+00E9, year: 1997
			print_doc("UTF8 file from wide stream", doc, result);
		}
		else
		{
			std::cout << "UTF-8 locale is not available\n";
		}
	}

	{
		// Since wide streams are treated as UTF-16/32 ones, you can't load the UTF-16 file from a wide stream without
		// using custom codecvt; you can use codecvt_utf16 from C++0x
	}

	{
		// Since encoding names are non-standard, you can't load the Shift-JIS (or any other non-ASCII) file
		// from a wide stream portably
		std::wifstream stream("weekly-shift_jis.xml");

		if (try_imbue(stream, ".932") || // try Microsoft encoding
			try_imbue(stream, "ja_JP.SJIS")) // try Linux encoding; run "localedef -i ja_JP -c -f SHIFT_JIS /usr/lib/locale/ja_JP.SJIS" to get it
		{
			pugi::xml_parse_result result = doc.load(stream);

			// first character of root name: U+9031, year: 1997
			print_doc("Shift-JIS file from wide stream", doc, result);
		}
		else
		{
			std::cout << "Shift-JIS locale is not available\n";
		}
	}
	return 1;
}


int traverse_base()
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

	std::cout << std::endl;

	//[code_traverse_base_data
	for (pugi::xml_node tool = tools.child("Tool"); tool; tool = tool.next_sibling("Tool"))
	{
		std::cout << "Tool " << tool.attribute("Filename").value();
		std::cout << ": AllowRemote " << tool.attribute("AllowRemote").as_bool();
		std::cout << ", Timeout " << tool.attribute("Timeout").as_int();
		std::cout << ", Description '" << tool.child_value("Description") << "'\n";
	}
	//]

	std::cout << std::endl;

	//[code_traverse_base_contents
	std::cout << "Tool for *.dae generation: " << tools.find_child_by_attribute("Tool", "OutputFileMasks", "*.dae").attribute("Filename").value() << "\n";

	for (pugi::xml_node tool = tools.child("Tool"); tool; tool = tool.next_sibling("Tool"))
	{
		std::cout << "Tool " << tool.attribute("Filename").value() << "\n";
	}
	//]
}


int traverse_iter()
{
	pugi::xml_document doc;
	if (!doc.load_file("xgconsole.xml")) return -1;

	pugi::xml_node tools = doc.child("Profile").child("Tools");

	//[code_traverse_iter
	for (pugi::xml_node_iterator it = tools.begin(); it != tools.end(); ++it)
	{
		std::cout << "Tool:";

		for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
		{
			std::cout << " " << ait->name() << "=" << ait->value();
		}

		std::cout << std::endl;
	}
	//]
}
//这个C++11有关 暂时不用
//int traverse_rangefor()
//{
//	pugi::xml_document doc;
//	if (!doc.load_file("xgconsole.xml")) return -1;
//
//	pugi::xml_node tools = doc.child("Profile").child("Tools");
//
//	//[code_traverse_rangefor
//	for (pugi::xml_node tool: tools.children("Tool"))
//	{
//		std::cout << "Tool:";
//
//		for (pugi::xml_attribute attr: tool.attributes())
//		{
//			std::cout << " " << attr.name() << "=" << attr.value();
//		}
//
//		for (pugi::xml_node child: tool.children())
//		{
//			std::cout << ", child " << child.name();
//		}
//
//		std::cout << std::endl;
//	}
//	//]
//}


const char* node_types[] =
{
	"null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
};

//[code_traverse_walker_impl
struct simple_walker: pugi::xml_tree_walker
{
	virtual bool for_each(pugi::xml_node& node)
	{
		for (int i = 0; i < depth(); ++i) std::cout << "  "; // indentation

		std::cout << node_types[node.type()] << ": name='" << node.name() << "', value='" << node.value() << "'\n";

		return true; // continue traversal
	}
};
//]

int traverse_walker()
{
	pugi::xml_document doc;
	if (!doc.load_file("text_tree.xml")) return -1;

	//[code_traverse_walker_traverse
	simple_walker walker;
	doc.traverse(walker);
	//]
}

int modify_base()
{
	pugi::xml_document doc;
	if (!doc.load("<node id='123'>text</node><!-- comment -->", pugi::parse_default | pugi::parse_comments)) return -1;

	std::cout << "保存结果1：: " << doc.save_file("save_file_output1.xml") << std::endl;
	//[code_modify_base_node
	pugi::xml_node node = doc.child("node");

	// change node name  修改结点名
	std::cout << "修改结点名" << std::endl;
	std::cout << node.set_name("notnode");
	std::cout << ", new node name: " << node.name() << std::endl;
	std::cout << "保存结果2：: " << doc.save_file("save_file_output2.xml") << std::endl;
	
	std::cout << "修改注释" << std::endl;
	// change comment text
	std::cout << doc.last_child().set_value("useless comment");
	std::cout << ", new comment text: " << doc.last_child().value() << std::endl;	
	std::cout << "保存结果3：: " << doc.save_file("save_file_output3.xml") << std::endl;

	std::cout << "不能改变值的元素或名称的注释" << std::endl;
	// we can't change value of the element or name of the comment
	std::cout << node.set_value("1") << ", " << doc.last_child().set_name("2") << std::endl;
	//]
	std::cout << "保存结果4：: " << doc.save_file("save_file_output4.xml") << std::endl;

	//[code_modify_base_attr
	pugi::xml_attribute attr = node.attribute("id");

	std::cout << "修改属性名/值" << std::endl;
	// change attribute name/value
	std::cout << attr.set_name("key") << ", " << attr.set_value("345");
	std::cout << ", new attribute: " << attr.name() << "=" << attr.value() << std::endl;
	std::cout << "保存结果5：: " << doc.save_file("save_file_output5.xml") << std::endl;

	std::cout << "可以使用数字或者布尔型" << std::endl;
	// we can use numbers or booleans
	attr.set_value(1.234);
	std::cout << "new attribute value: " << attr.value() << std::endl;
	std::cout << "保存结果6：: " << doc.save_file("save_file_output6.xml") << std::endl;

	std::cout << "还可以使用更简洁的代码赋值运算符" << std::endl;
	// we can also use assignment operators for more concise code
	attr = true;
	std::cout << "final attribute value: " << attr.value() << std::endl;
	//]
	std::cout << "保存结果7：: " << doc.save_file("save_file_output7.xml") << std::endl;

	std::cout << "还可以修改节点内容" << std::endl;
	std::cout << "xnode.text().get():" << node.text().get() << std::endl;

	std::cout << "node.first_child().set_value(\"useless comment\"): " << node.first_child().set_value("useless comment") << std::endl;	
	std::cout << "xnode.first_child().value():" << node.first_child().value() << std::endl;
	std::cout << "保存结果8：: " << doc.save_file("save_file_output8.xml") << std::endl;
}


int modify_add()
{
	pugi::xml_document doc;

	//[code_modify_add
	// add node with some name   增加根结点node  此时其对应结点并未添加  只有部分
	pugi::xml_node node = doc.append_child("node");
	std::cout << "append_child(\"node\"): " << doc.save_file("save_file_output1.xml") << std::endl;

	//增加子节点 此时子节点的对应节点并未添加，但其父点的已经添加
	// add description node with text child
	pugi::xml_node descr = node.append_child("description");
	std::cout << "append_child(\"description\"): " << doc.save_file("save_file_output2.xml") << std::endl;
	//设置结点元素内容为Simple node 其节点的对节点均忆
	descr.append_child(pugi::node_pcdata).set_value("Simple node");
	std::cout << "append_child(pugi::node_pcdata).set_value(\"Simple node\"): " << doc.save_file("save_file_output3.xml") << std::endl;

	//添加PARAM节点的描述的前面部分 
	// add param node before the description
	pugi::xml_node param = node.insert_child_before("param", descr);
	std::cout << "insert_child_before(\"param\", descr): " << doc.save_file("save_file_output4.xml") << std::endl;

	// add attributes to param node 添加到参数节点属性
	param.append_attribute("name") = "version";
	param.append_attribute("value") = 1.1;
	std::cout << "append_attribute(\"value\"): " << doc.save_file("save_file_output5.xml") << std::endl;
	param.insert_attribute_after("type", param.attribute("name")) = "float";
	//]
	std::cout << "insert_attribute_after(\"type\", param.attribute(\"name\")): " << doc.save_file("save_file_output5.xml") << std::endl;

	doc.print(std::cout);
	return 1;
}

int modify_remove()
{
	pugi::xml_document doc;
	if (!doc.load("<node><description>Simple node</description><param name='id' value='123'/></node>")) return -1;
	std::cout << "doc.load: " << doc.save_file("save_file_output1.xml") << std::endl;

	//[code_modify_remove
	// remove description node with the whole subtree 删除描述节点的整个子树
	pugi::xml_node node = doc.child("node");  
	node.remove_child("description");
	std::cout << "remove_child(\"description\"): " << doc.save_file("save_file_output2.xml") << std::endl;

	// remove id attribute 删除id属性
	pugi::xml_node param = node.child("param");
	param.remove_attribute("value");
	std::cout << "remove_attribute(\"value\"): " << doc.save_file("save_file_output3.xml") << std::endl;

	// we can also remove nodes/attributes by handles 我们也可以删除节点/属性手柄
	pugi::xml_attribute id = param.attribute("name");
	param.remove_attribute(id);
	std::cout << "remove_attribute(id): " << doc.save_file("save_file_output4.xml") << std::endl;
	//]

	node.remove_child("param");
	doc.print(std::cout);
	return 1;
}



int save_file()
{
	// get a test document
	pugi::xml_document doc;
	doc.load("<foo bar='baz'>hey</foo>");

	//[code_save_file
	// save document to file
	std::cout << "Saving result: " << doc.save_file("save_file_output.xml") << std::endl;
	//]
	return 1;
}

int save_stream()
{
	// get a test document
	pugi::xml_document doc;
	doc.load("<foo bar='baz'><call>hey</call></foo>");

	//[code_save_stream
	// save document to standard output
	std::cout << "Document:\n";
	doc.save(std::cout);
	//]
	return 1;
}


//[code_save_custom_writer
struct xml_string_writer: pugi::xml_writer
{
	std::string result;

	virtual void write(const void* data, size_t size)
	{
		result += std::string(static_cast<const char*>(data), size);
	}
};
//]

struct xml_memory_writer: pugi::xml_writer
{
	char* buffer;
	size_t capacity;

	size_t result;

	xml_memory_writer(): buffer(0), capacity(0), result(0)
	{
	}

	xml_memory_writer(char* buffer, size_t capacity): buffer(buffer), capacity(capacity), result(0)
	{
	}

	size_t written_size() const
	{
		return result < capacity ? result : capacity;
	}

	virtual void write(const void* data, size_t size)
	{
		if (result < capacity)
		{
			size_t chunk = (capacity - result < size) ? capacity - result : size;

			memcpy(buffer + result, data, chunk);
		}

		result += size;
	}
};

std::string node_to_string(pugi::xml_node node)
{
	xml_string_writer writer;
	node.print(writer);

	return writer.result;
}

char* node_to_buffer(pugi::xml_node node, char* buffer, size_t size)
{
	if (size == 0) return buffer;

	// leave one character for null terminator
	xml_memory_writer writer(buffer, size - 1);
	node.print(writer);

	// null terminate
	buffer[writer.written_size()] = 0;

	return buffer;
}

char* node_to_buffer_heap(pugi::xml_node node)
{
	// first pass: get required memory size
	xml_memory_writer counter;
	node.print(counter);

	// allocate necessary size (+1 for null termination)
	char* buffer = new char[counter.result + 1];

	// second pass: actual printing
	xml_memory_writer writer(buffer, counter.result);
	node.print(writer);

	// null terminate
	buffer[writer.written_size()] = 0;

	return buffer;
}

int save_custom_writer()
{
	// get a test document
	pugi::xml_document doc;
	doc.load("<foo bar='baz'>hey</foo>");

	// get contents as std::string (single pass)
	printf("contents: [%s]\n", node_to_string(doc).c_str());

	// get contents into fixed-size buffer (single pass)
	char large_buf[128];
	printf("contents: [%s]\n", node_to_buffer(doc, large_buf, sizeof(large_buf)));

	// get contents into fixed-size buffer (single pass, shows truncating behavior)
	char small_buf[22];
	printf("contents: [%s]\n", node_to_buffer(doc, small_buf, sizeof(small_buf)));

	// get contents into heap-allocated buffer (two passes)
	char* heap_buf = node_to_buffer_heap(doc);
	printf("contents: [%s]\n", heap_buf);
	delete[] heap_buf;
	return 1;
}