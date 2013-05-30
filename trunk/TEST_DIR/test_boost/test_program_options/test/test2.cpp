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

int main2 (int ac, char *av[])
{
	//在使用 add_options 来添加命令选项时，较长和较短的选项之间使用逗号进行分隔。
	//请注意，较长的选项 (help) 必须在较短的选项 (h) 之前，代码才能正常工作。
	//与使用单个字符串不同，file 选项现在是使用一个字符串向量来定义的。
	//如果指定了 --file 选项多次，则会将所有收集到的file命令选项参数存储在关联的
    boost::program_options::options_description options("command line options");
    options.add_options() ("help,h", "Use -h or --help to list all arguments")
    ("file", boost::program_options::value<vector<string> >( ),
     "Provide input file name");
    boost::program_options::variables_map vmap;
    boost::program_options::store(
        boost::program_options::parse_command_line(ac, av, options), vmap);
    boost::program_options::notify(vmap);

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