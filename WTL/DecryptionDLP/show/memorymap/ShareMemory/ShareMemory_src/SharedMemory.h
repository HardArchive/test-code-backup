#pragma once
#include <string>
using std::string;
#include <map>
using std::map;
class MMF;
class SharedMemory
{
/************************************************************************/
/* 
the class SharedMemory uses the Singleton pattern
* @author Bony.Chen
* @description
				the SharedMemory object can contain multiple memory-mapped
				file objects and one MMF object has special name.the object
				is implemented using Singleton pattern.
* @mail bonyren@163.com
*/
/************************************************************************/
protected:
	SharedMemory();
private:
	SharedMemory(const SharedMemory& sm)
	{

	}
	SharedMemory& operator= (const SharedMemory& sm)
	{

	}
public:
	virtual ~SharedMemory(void);
public:
	///Get the only instance object
	static SharedMemory& Instance();
	///write data to the special MMF that named 'szName'
	bool WriteSharedMemory(const string& szName,const string& szValue);
	///read data from the special MMF that named 'szName'
	bool ReadSharedMemory(const string& szName,string& szValue);
	///delete the special MMF that named 'szName'
	void DeleteSharedMemory(const string& szName);
private:
	map<string,MMF*> m_mapMMF;//<the container of MMFs
};
