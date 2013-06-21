// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
#include <Windows.h>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
using namespace boost::program_options;
using namespace std;

int print_vm(const options_description &opts, const variables_map &vm)
{
    if (vm.size() == 0)
    {
        //cout << opts << endl;
        return 0;
    }

    // 解析完成，实现选项处理逻辑
    if (vm.count("help"))
    {
        cout << opts << endl; //输出帮助信息
        return 0;
    }

    //输出查找文件名，因为有缺省值，故总存在
    if (vm.count("filename"))
    {
		cout << "find opt: " << vm["filename"].as<string>() << endl;
    }    

    if (vm.count("dir"))
    {
        cout << "dir opt:";
        BOOST_FOREACH(string str, vm["dir"].as<vector<string> >())
        {
            cout << str << ",";
        }
        cout << endl;
    }

    if (vm.count("depth"))
    {
        cout << "depth opt: " << vm["depth"].as<int>() << endl;
    }

	if (vm.count("input"))
	{
		string strInput;
		char szInput[MAX_PATH] = {0};
		cout << "Please Input Something!" << endl << "input:";
		cin>>szInput;
		cout << endl;
		cout << "You Input: "<< szInput << endl;
	}

    return 1;
}

int main(int argc, char *argv[]) // 需要命令行参数
{
    string filename;
    options_description opts("Mysql performance options"); //增加两个程序选项 声明所有的有效命令行选项。
    opts.add_options()
    ("help,h", "help message")
    ("filename,f", value<string>(&filename)->default_value("text"), "to find a file")
    ("dir,D", value<vector<string> >()->multitoken(), "search dir")
    ("depth,d", value<int>()->implicit_value(5), "search depth")
	("input,i", "Input Command!");

    variables_map vm; // 选项存储map容器
    store(parse_command_line(argc, argv, opts), vm); //解析参数并存储到vm中
    notify(vm);
    print_vm(opts, vm);
	return 1;
}