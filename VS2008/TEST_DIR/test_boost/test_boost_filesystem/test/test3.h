#ifndef TEST3_H
#define TEST3_H

#include "stdafx.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <iostream>
using namespace std;

class ScanAllFiles
{
public:
	//����һ,�Լ�д�ݹ飬��filesystem���directory_iterator
	static const vector<string>& scanFiles(const string&,vector<string>&);
	//��������ֱ����boost��filesystem���recursive_directory_iterator
	static const vector<string>& scanFilesUseRecursive(const string&,vector<string>&); 
};


//����һʵ�֣��Լ�д�ݹ�
const vector<string>& ScanAllFiles::scanFiles(const string& rootPath,vector<string>& container=*(new vector<string>()))
{
	namespace fs = boost::filesystem;
	fs::path fullpath (rootPath, fs::native);
	vector<string> &ret = container;

	if(!fs::exists(fullpath))
	{
		return ret;
	}
	fs::directory_iterator end_iter;  /**�޲ι��캯��������Ǹ�iterator��value ժ������
									  *If the end of the directory elements is reached, the iterator becomes equal to the end iterator value. The constructor directory_iterator() with no arguments always constructs an end iterator object, which is the only legitimate iterator to be used for the end condition. The result of operator* on an end iterator is not defined. For any other iterator value a const directory_entry& is returned. The result ofoperator-> on an end iterator is not defined. For any other iterator value a const directory_entry* is returned.
									  *
									  **/
	for(fs::directory_iterator iter(fullpath); iter!=end_iter; iter++)
	{
		try{
			if (fs::is_directory(*iter))
			{
				std::cout<<*iter << "is dir.whose parent path is " << iter->path().branch_path() << std::endl;
				ret.push_back(iter->path().string()); //�ݹ�ǰpush_back��ȥһ��
				ScanAllFiles::scanFiles(iter->path().string(), ret);//�ݹ飬��vectorҲ����ȥ
			}
			else
			{
				ret.push_back(iter->path().string());
				//std::cout << *iter << " is a file" << std::endl;
			}
		}
		catch ( const std::exception & ex )
		{
			std::cerr << ex.what() << std::endl;
			continue;
		}
	}
	return ret;
}
//������ʵ�֣�ֱ����boost��filesystem���recursive_directory_iterator
const vector<string>& ScanAllFiles::scanFilesUseRecursive(const string& rootPath,vector<string>& container=*(new vector<string>()))
{
	namespace fs = boost::filesystem;
	fs::path fullpath (rootPath, fs::native);
	vector<string> &ret = container;

	if(!fs::exists(fullpath))
	{
		return ret;
	}
	fs::recursive_directory_iterator end_iter;

	for(fs::recursive_directory_iterator iter(fullpath);iter!=end_iter;iter++)
	{
		try{
			if (fs::is_directory( *iter ) )
			{
				std::cout<<*iter << "is dir" << std::endl;
				ret.push_back(iter->path().string());
				//ScanAllFiles::scanFiles(iter->path().string(),ret);
			}
			else
			{
				ret.push_back(iter->path().string());
			}
		} 
		catch ( const std::exception & ex )
		{
			std::cerr << ex.what() << std::endl;
			continue;
		}
	}
	return ret;
}
#endif