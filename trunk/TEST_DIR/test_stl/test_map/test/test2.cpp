#include <iostream>
#include <map>
#include <string>
using namespace std ;

int main(void) 
{ 
	map<int, string> m ;
	m.insert(pair<int, string>(1, "abc")) ;
	m.insert(pair<int, string>(2, "def")) ;
	m.insert(pair<int, string>(3, "def")) ;
	m.insert(pair<int, string>(4, "ghi")) ;


	map<int, string>::iterator itor ;

	// 错误的写法
	for (itor = m.begin(); itor != m.end(); ++itor)
	{
		if (itor->second == "def")
		{
			m.erase(itor) ; // map是关联式容器，调用erase后，当前迭代器已经失效
		}
	}

	// 正确的写法
	for (itor = m.begin(); itor != m.end();)
	{
		if (itor->second == "def")
		{
			m.erase(itor++) ; // erase之后，令当前迭代器指向其后继。
		}
		else
		{
			++itor;
		}
	}

	// 另一个正确的写法，利用erase的返回值，注意，有些版本的stl-map没有返回值，比如SGI版，但vc版的有
	for (itor = m.begin(); itor != m.end();)
	{
		if (itor->second == "def")
		{
			itor = m.erase(itor) ; // erase的返回值是指向被删除元素的后继元素的迭代器
		}
		else
		{
			++itor;
		}
	}

	// Print m
	map<int, string>::const_iterator citor ;
	for (citor = m.begin(); citor != m.end(); ++citor)
	{
		cout << citor->first << ":" << citor->second << endl ;
	}

	getchar() ; 
	return 0 ; 
}