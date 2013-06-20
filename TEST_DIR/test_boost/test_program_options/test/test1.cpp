// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
using namespace std;

int main (int ac, char *av[])
{
    // options_description 类声明所有的有效命令行选项
    boost::program_options::options_description options("command line options");
    //使用方法 add_options，您可以注册命令和跟在命令后面的参数类型。
    //help 选项不需要任何参数，但是 file 选项需要一个字符串参数
    options.add_options() ("help,h", "Use -h or --help to list all arguments")
    ("file", boost::program_options::value<vector<string> >( ),
     "Provide input file name");
    // variables_map 类在运行时存储命令行选项及其参数。
    boost::program_options::variables_map vmap;

    //Boost 的 parse_command_line 函数解析 argc 和 argv 参数。store 和 notify 方法帮助存储 vmap 对象中的数据。
    boost::program_options::store(
        boost::program_options::parse_command_line(ac, av, options), vmap);
    boost::program_options::notify(vmap);

    //vmap.count()用于检测输入的是哪个命令行参数，并采取适当的动作
    if (vmap.count("help"))
    {
        cout << options << endl;
    }

    if (vmap.count("file"))
    {
        vector<string> ifiles(vmap["file"].as< vector<string> > ());
        vector<string>::iterator vI;
        cout << "Number of input files: " << ifiles.size() << endl;
        cout << "Input file list: " << endl;
        for(vI = ifiles.begin(); vI != ifiles.end(); ++vI)
            cout << "\t" << *vI << endl;
    }
    else
    {
        cout << "No file specified\n";
    }

    return 0;
}