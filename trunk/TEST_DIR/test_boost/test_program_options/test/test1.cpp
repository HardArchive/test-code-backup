// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
    // options_description ���������е���Ч������ѡ��
    boost::program_options::options_description options("command line options");
    //ʹ�÷��� add_options��������ע������͸����������Ĳ������͡�
    //help ѡ���Ҫ�κβ��������� file ѡ����Ҫһ���ַ�������
    options.add_options() ("help,h", "Use -h or --help to list all arguments")
    ("file", boost::program_options::value<vector<string> >( ),
     "Provide input file name");
    // variables_map ��������ʱ�洢������ѡ��������
    boost::program_options::variables_map vmap;

    //Boost �� parse_command_line �������� argc �� argv ������store �� notify ���������洢 vmap �����е����ݡ�
    boost::program_options::store(
        boost::program_options::parse_command_line(ac, av, options), vmap);
    boost::program_options::notify(vmap);

    //vmap.count()���ڼ����������ĸ������в���������ȡ�ʵ��Ķ���
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