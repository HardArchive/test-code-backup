// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//vector合并
#include <iostream>  
#include <string>  
#include <vector>  
#include <algorithm>
using namespace std;  

int main()
{
	vector<string> vec1, vec2;
	vec1.push_back("aa");
	vec1.push_back("bb");
	vec1.push_back("cc");
	vec1.push_back("dd");
	vec2.push_back("dd");
	vec2.push_back("ee");
	vec2.push_back("ff");
	//vec2.push_back("aa");
	//vec2.push_back("bb");
	//vec2.push_back("cc");
	vector<string> vec(vec1.size() + vec2.size());
	merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), vec.begin()); 
	//for (int j = 0; j < vec.size(); ++j) 
	//{
	//	cout << vec[j] << endl;
	//}

	sort( vec.begin(), vec.end() );   //先排序
	vec.erase(unique( vec.begin(), vec.end()), vec.end());  //删除重复元素 unique返回值是重复元素的开始位置。
	//for (int i = 0; i<vec.size(); i++)
	//{
	//	vec.erase(remove(vec.begin(), vec.end(), vec.at(i)), vec.end()); 
	//}
	
	for (int j = 0; j < vec.size(); ++j) 
	{
		cout << vec[j] << endl;
	}

	////按顺序插入
	//vector<string>  vec1;
	//vec1.reserve(5);
	//vec1.push_back("1");
	//vec1.push_back("3");
	//vec1.push_back("5");
	//vec1.push_back("5");
	//vec1.push_back("7");

	//vector<string>  vec2;
	//vec2.reserve(5);
	//vec2.push_back("2");
	//vec2.push_back("2");
	//vec2.push_back("4");
	//vec2.push_back("6");
	//vec2.push_back("8");

	//vector<string>  vec3;
	//vec3.resize(10);

	//vector<string>::iterator it1Begin = vec1.begin();
	//vector<string>::iterator it1End = vec1.end();
	//vector<string>::iterator it2Begin = vec2.begin();
	//vector<string>::iterator it2End = vec2.end();
	//vector<string>::iterator it3Begin = vec3.begin();

	//merge( it1Begin, it1End, it2Begin, it2End, it3Begin);

	//for (int i = 0; i < vec3.size(); i ++)
	//{
	//	cout << "vec3[" << i << "]=" << vec3[i].c_str() << endl;
	//}

	//vec3.erase(unique(vec3.begin(), vec3.end()), vec3.end());

	//vector<string> vec4;
	//vec4.swap(vec3);


	//for (int i = 0; i < vec4.size(); i ++)
	//{
	//	cout << "vec4[" << i << "]=" << vec4[i].c_str() << endl;
	//}

	getchar();
}