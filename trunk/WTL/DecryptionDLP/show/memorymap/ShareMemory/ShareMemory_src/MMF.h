#pragma once
#include <string>
using std::string;
/**
* @author Bony.Chen
* @description A wrapped class managing file mapping object 
				for the specified file.It encapsulates some
				basic operations such as Create,Open,MapViewOfFile.
* @mail bonyren@163.com

*/
class MMF
{
public:
	MMF(const string& szMMName);
public:
	virtual ~MMF(void);
public:
	///Create memory mapping file
	bool CreateMapFile();
	///open memory mapping file that has been exist
	bool OpenMapFile();
	///close the handle of memory mapping file
	void CloseMapFile();
	///write data to the memory mapping file
	bool WriteData(const char* szData,int nLen);
	///read data from the memory mapping file
	bool ReadData(string& szData);
private:
	HANDLE m_hMapFile;//<the handle of memory-mapped file
	string m_szMMName;//<the name of memory-mapped file
};
