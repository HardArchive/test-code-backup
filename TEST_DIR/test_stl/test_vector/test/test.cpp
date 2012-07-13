// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//创建向量对象,以及初始化
	vector<int> intvec0;        //创建一个int类型的空的vector对象
	vector<int> intvec1(5);     //创建一个包含5个int类型数据的vector,//默认值是0
	vector<int> intvec2(5, 6);  //创建一个包含5个int类型数据的vector，并且都初始化为6
	vector<int> intvec3(3, 1, intvec2.get_allocator()); //用其他管理器的副本创建
	vector<int> intvec4(intvec2);    //创建一个int的vector拷贝
	vector<int> intvec5(intvec4.begin()+1, intvec4.begin()+2);  //通过复制intvec4[_First,_Last]创建v5

	//FOR  循环输出 向量的值
	cout<<"FOR  循环输出intvec:   "<<endl;
	cout<<"intvec1:   ";
	for(int i=0; i<(int)intvec1.size(); i++)
		cout<<intvec1[i]<<"  ";
	cout<<"\n";

	cout<<"intvec2:   ";
	for(int i=0; i<(int)intvec2.size(); i++)
		cout<<intvec2[i]<<"  ";
	cout<<"\n";

	cout<<"intvec3:   ";
	for(int i=0; i<(int)intvec3.size(); i++)
		cout<<intvec3[i]<<"  ";
	cout<<"\n";

	cout<<"intvec4:   ";
	for(int i=0; i<(int)intvec4.size(); i++)
		cout<<intvec4[i]<<"  ";
	cout<<"\n";

	cout<<"intvec5:   ";
	for(int i=0; i<(int)intvec5.size(); i++)
		cout<<intvec5[i]<<"  ";
	cout<<"\n"<<"\n";
	//vector 中的操作
	cout<<"vector intvec1中的操作:   "<<endl;
	intvec1.push_back(5);   //一个拷贝插入到vector的末尾
	intvec1.push_back(6);
	intvec1.push_back(7);
	intvec1.push_back(8);
	intvec1.push_back(9);

	cout<<"intvec第一个元素是:";
	cout<<intvec1.front()<<"\n";    //返回第一个元素　（不检查容器是否为空）

	cout<<"intvec1[1]第二个元素是:";
	cout<<intvec1[1]<<"\n";        //返回由指定的位置上的元素

	cout<<"intvec1.at(2)第三个元素是:";
	cout<<intvec1.at(2)<<"\n";     //返回由指定的位置上的元素

	cout<<"intvec1.back()第四个元素是:";
	cout<<intvec1.back()<<"\n";   //返回最后一个元素（不检查容器是否为空）
	cout<<"\n"<<"\n";



	//迭代器 
	//迭代器上的这些操作和指针上的相应操作是相同的。运算符*作为单目运算符使用时，称为递引用运算符。
	//vector类包含了一个typedef iterator，这是一个public成员。通过iterator，可以声明向量容器中的迭代器。
	cout<<"迭代器循环输出intvec1:   "<<endl;
	//因为iterator是一个定义在vector类中的typedef，所以必须使用容器名（vector）、容器元素类型和作用域符来使用iterator
	vector<int>::iterator intiter;    //将intiter声明为int类型的向量容器迭代器
	//++intiter将迭代器intiter加１，使其指向容器中的下一个元素。表达式：*intiter返回当前迭代器位置上的元素。
	//所有容器都包含成员函数begin和end。函数begin返回容器中第一个元素的位置；函数end返回容器中最后一个元素的位置。
	for(intiter = intvec1.begin(); intiter!=intvec1.end(); intiter++)
		cout<<*intiter<<"  "; 
	cout<<"\n"<<"\n";

	cout<<"计算向量容器大小的操作      "<<endl;
	cout<<"最原始的数据容量：      ";
	cout<<intvec1.size()<<"\t";     //返回容器intvec中当前的个数
	cout<<intvec1.capacity()<<"\t"; //返回不重新分配空间可以插入到容器intvec中的元素的最大个数
	cout<<intvec1.max_size()<<"\t"; //返回可以插入到容器intvec中的元素的最大个数
	cout<<"\n";

	cout<<"执行resize后的数据容量：";
	intvec1.resize(100);  //将元素个数改为num。如果size()增加，默认的构造函数负责创建这些新元素
	cout<<intvec1.size()<<"\t";
	cout<<intvec1.capacity()<<"\t";
	cout<<intvec1.max_size()<<"\t";
	cout<<"\n";

	cout<<"执行reserve后的数据容量：";
	intvec1.reserve(200);   //保存向量存储的最小的长度
	cout<<intvec1.size()<<"\t";
	cout<<intvec1.capacity()<<"\t";
	cout<<intvec1.max_size()<<"\t";
	cout<<"\n"<<"\n";

	cout<<"原始容器的当前元素个数"<<intvec1.size()<<endl;
	cout<<"原始容器中的数据："<<endl;
	for(intiter=intvec1.begin();intiter!=intvec1.end();intiter++)
		cout<<*intiter<<"  ";
	cout<<"\n";


	intvec1.insert(intvec1.begin()+2,10); //将elem(10)的一个拷贝插入到由position(intvec1.begin()+2)指定的位置上，并返回新元素的位置
	cout<<"执行insert后容器的当前元素个数"<<intvec1.size()<<endl;
	cout<<"执行insert后容器中的数据："<<endl;
	for(intiter=intvec1.begin();intiter!=intvec1.end();intiter++)
		cout<<*intiter<<"  ";
	cout<<"\n";

	intvec1.erase(intvec1.begin()+1);//删除由position(intvec1.begin()+1)指定的位置上的元素
	cout<<"执行erase后容器的当前元素个数"<<intvec1.size()<<endl;
	cout<<"执行erase后容器中的数据："<<endl;
	for(intiter=intvec1.begin();intiter!=intvec1.end();intiter++)
		cout<<*intiter<<"  ";
	cout<<"\n";

	intvec1.pop_back();  //删除最后元素
	cout<<"执行pop_back后容器的当前元素个数"<<intvec1.size()<<endl;
	cout<<"执行pop_back后容器中的数据："<<endl;
	for(intiter=intvec1.begin();intiter!=intvec1.end();intiter++)
		cout<<*intiter<<"  ";
	cout<<"\n";

	intvec1.clear();  //从容器中删除所有元素
	cout<<"执行clear后容器中的大小：";
	cout<<intvec1.size();
	getchar();
	getchar();
	return 0;
}

