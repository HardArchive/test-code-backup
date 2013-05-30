// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
using namespace boost::program_options;
using namespace std;

int print_vm(const options_description &opts, const variables_map &vm)
{
    if (vm.size() == 0)
    {
        cout << opts << endl;
        return 0;
    }

    // ������ɣ�ʵ��ѡ����߼�
    if (vm.count("help"))
    {
        cout << opts << endl; //���������Ϣ
        return 0;
    }

    //��������ļ�������Ϊ��ȱʡֵ�����ܴ���
    cout << "find opt: " << vm["filename"].as<string>() << endl;

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

    return 1;
}

int main0(int argc, char *argv[]) // ��Ҫ�����в���
{
    string filename;
    options_description opts("Mysql performance options"); //������������ѡ��
    opts.add_options()
    ("help,h", "help message")
    ("filename,f", value<string>(&filename)->default_value("text"), "to find a file")
    ("dir,D", value<vector<string> >()->multitoken(), "search dir")
    ("depth,d", value<int>()->implicit_value(5), "search depth");

    variables_map vm; // ѡ��洢map����
    store(parse_command_line(argc, argv, opts), vm); //�����������洢��vm��
    notify(vm);
    print_vm(opts, vm);
	return 1;
}