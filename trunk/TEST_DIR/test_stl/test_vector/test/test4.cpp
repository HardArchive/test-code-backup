// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>



#include "time.h"
using namespace std;

template<class T>
class fun: public binary_function<T, T, bool>
{
public:
	result_type operator()( const first_argument_type a, const second_argument_type b ) const
	{
		return a>b;
	}
}; 

int _rand()
{
	return rand()%100;
} 

int main4(int argc, char* argv[])
{
	srand(time(NULL));
	vector<int> vec(16);

	generate(vec.begin(), vec.end(), _rand);

	copy(vec.begin(), vec.end(), ostream_iterator<int>(cout," "));

	cout << endl; 

	vector<int>::iterator itr =vec.begin();
	while(itr != vec.end())
	{
		itr = find_if(itr, vec.end(), binder2nd<fun<int>>(fun<int>(), 30));
		if(itr != vec.end())
		{
			cout <<*itr << " ";
			itr++;
		}
	}

	getchar();
	return 0;
}

