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

int main3 (int ac, char *av[])
{
    boost::program_options::options_description options("command line options");
    options.add_options() ("help,h", "Use -h or --help to list all arguments")
    ("file", boost::program_options::value<string>(),
     "Provide input file name")
    ("do-file", boost::program_options::value<string>(),
     "Specify commands file");

    boost::program_options::variables_map vmap;
    boost::program_options::positional_options_description poptd;
    poptd.add("file", 1);
    poptd.add("do-file", 2);

    boost::program_options::store(
        boost::program_options::command_line_parser(ac, av).
        options(options).positional(poptd).run(), vmap);
    boost::program_options::notify(vmap);

    if (vmap.count("file"))
    {
        cout << "file: " << vmap["file"].as<string> ( ) << endl;
    }

    if (vmap.count("do-file"))
    {
        cout << "do-file: " << vmap["do-file"].as<string> ( ) << endl;
    }

    return 0;
}