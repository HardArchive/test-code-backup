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

	// �����д��
	for (itor = m.begin(); itor != m.end(); ++itor)
	{
		if (itor->second == "def")
		{
			m.erase(itor) ; // map�ǹ���ʽ����������erase�󣬵�ǰ�������Ѿ�ʧЧ
		}
	}

	// ��ȷ��д��
	for (itor = m.begin(); itor != m.end();)
	{
		if (itor->second == "def")
		{
			m.erase(itor++) ; // erase֮���ǰ������ָ�����̡�
		}
		else
		{
			++itor;
		}
	}

	// ��һ����ȷ��д��������erase�ķ���ֵ��ע�⣬��Щ�汾��stl-mapû�з���ֵ������SGI�棬��vc�����
	for (itor = m.begin(); itor != m.end();)
	{
		if (itor->second == "def")
		{
			itor = m.erase(itor) ; // erase�ķ���ֵ��ָ��ɾ��Ԫ�صĺ��Ԫ�صĵ�����
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