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

int main2 (int ac, char *av[])
{
	//��ʹ�� add_options ���������ѡ��ʱ���ϳ��ͽ϶̵�ѡ��֮��ʹ�ö��Ž��зָ���
	//��ע�⣬�ϳ���ѡ�� (help) �����ڽ϶̵�ѡ�� (h) ֮ǰ�������������������
	//��ʹ�õ����ַ�����ͬ��file ѡ��������ʹ��һ���ַ�������������ġ�
	//���ָ���� --file ѡ���Σ���Ὣ�����ռ�����file����ѡ������洢�ڹ�����
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