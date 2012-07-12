#include "stdafx.h"
#include "test3.h"

#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	std::string strpath = "D:\\WinPath\\desktop\\test\\temp";
	vector<string> vstrFilePath;
	ScanAllFiles objScanAllFiles;
	std::cout << "自己写递归实现文件夹遍历……" << std::endl;
	vstrFilePath = objScanAllFiles.scanFiles(strpath, vstrFilePath);
	for (vector<string>::iterator iter = vstrFilePath.begin(); iter != vstrFilePath.end(); ++iter)
	{ 
		//std::string s = *iter;//取出路径中的每个字符串.
		std::cout << *iter << std::endl;
	}
	vstrFilePath.clear();

	std::cout << "直接用boost的filesystem里的recursive_directory_iterator……" << std::endl;
	vstrFilePath = objScanAllFiles.scanFilesUseRecursive(strpath, vstrFilePath);

	for (int i=0; i<(int)vstrFilePath.size(); i++)
	{
		std::cout << vstrFilePath.at(i) << std::endl;
	}
	return 0;
}
