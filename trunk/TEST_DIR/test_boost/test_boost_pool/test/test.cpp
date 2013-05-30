// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//////////////////////////////////////////////////////////////
//////////////////////////pool//////////////////////////////// 
//////////////////////////////////////////////////////////////
#include <boost/pool/pool.hpp>  
using namespace boost;  

//pool是最简单也最容易使用的内存池类，可以返回一个简单数据类型（POD） 的内存指针。
//它pool很容易使用，可以像C中的malloc()一样分配内存，然后随意使用。除非有特殊要求，否则不必对分配的内存调用free()释放，pool会很好地管理内存。例如：
int main0()  
{      
	pool<> pl(sizeof(int));             //一个可分配int的内存池       
	int *p = (int *)pl.malloc();        //必须把void*转换成需要的类型      
	assert(pl.is_from(p));       
	pl.free(p);                         //释放内存池分配的内存块      
	for (int i = 0;i < 100; ++i)        //连续分配大量的内存      
	{   
		pl.ordered_malloc(10);  
	}  

	return 1;
} 
//////////////////////////////////////////////////////////////
//////////////////////object_pool///////////////////////////// 
//////////////////////////////////////////////////////////////
#include <string>
using namespace std;
#include <boost/pool/object_pool.hpp> 
using namespace boost;  
struct demo_class                           //一个示范用的类  
{  
public:      
	int a,b,c;      
	demo_class(int x = 1, int y = 2, int z = 3):a(x),b(y),c(z){}  
};  

int main1()  
{      
	object_pool<demo_class> pl;             //对象内存池       
	demo_class *p = pl.malloc();            //分配一个原始内存块      
	assert(pl.is_from(p));       //p指向的内存未经过初始化      
	assert(p->a!=1 || p->b != 2 || p->c !=3);       
	p = pl.construct(7, 8, 9);              //构造一个对象,可以传递参数      
	assert(p->a == 7);       
	object_pool<string> pls;                //定义一个分配string对象的内存池      
	for (int i = 0; i < 10 ; ++i)           //连续分配大量string对象      
	{          
		string *ps = pls.construct("hello object_pool");          
		cout << *ps << endl;      
	}  
	return 1;
}                                           //所有创建的对象在这里都被正确析构、释放内存 



//////////////////////////////////////////////////////////////////////////
//////////////////////////singleton_pool//////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include <boost/pool/singleton_pool.hpp>  
using namespace boost;  
//struct pool_tag{};                                  //仅仅用于标记的空类  
//typedef singleton_pool<pool_tag, sizeof(int)> spl;  //内存池定义 
// 
typedef singleton_pool<struct pool_tag, sizeof(int)> spl;   //可以由以上两句简化

int main()  
{      
	int *p = (int *)spl::malloc();                      //分配一个整数内存块    

	assert(spl::is_from(p));      
	spl::release_memory();                              //释放所有未被分配的内存  
}                                                       //spl的内存直到程序结束才完 