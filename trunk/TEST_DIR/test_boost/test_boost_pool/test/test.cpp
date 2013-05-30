// test.cpp : �������̨Ӧ�ó������ڵ㡣
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

//pool�����Ҳ������ʹ�õ��ڴ���࣬���Է���һ�����������ͣ�POD�� ���ڴ�ָ�롣
//��pool������ʹ�ã�������C�е�malloc()һ�������ڴ棬Ȼ������ʹ�á�����������Ҫ�󣬷��򲻱ضԷ�����ڴ����free()�ͷţ�pool��ܺõع����ڴ档���磺
int main0()  
{      
	pool<> pl(sizeof(int));             //һ���ɷ���int���ڴ��       
	int *p = (int *)pl.malloc();        //�����void*ת������Ҫ������      
	assert(pl.is_from(p));       
	pl.free(p);                         //�ͷ��ڴ�ط�����ڴ��      
	for (int i = 0;i < 100; ++i)        //��������������ڴ�      
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
struct demo_class                           //һ��ʾ���õ���  
{  
public:      
	int a,b,c;      
	demo_class(int x = 1, int y = 2, int z = 3):a(x),b(y),c(z){}  
};  

int main1()  
{      
	object_pool<demo_class> pl;             //�����ڴ��       
	demo_class *p = pl.malloc();            //����һ��ԭʼ�ڴ��      
	assert(pl.is_from(p));       //pָ����ڴ�δ������ʼ��      
	assert(p->a!=1 || p->b != 2 || p->c !=3);       
	p = pl.construct(7, 8, 9);              //����һ������,���Դ��ݲ���      
	assert(p->a == 7);       
	object_pool<string> pls;                //����һ������string������ڴ��      
	for (int i = 0; i < 10 ; ++i)           //�����������string����      
	{          
		string *ps = pls.construct("hello object_pool");          
		cout << *ps << endl;      
	}  
	return 1;
}                                           //���д����Ķ��������ﶼ����ȷ�������ͷ��ڴ� 



//////////////////////////////////////////////////////////////////////////
//////////////////////////singleton_pool//////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include <boost/pool/singleton_pool.hpp>  
using namespace boost;  
//struct pool_tag{};                                  //�������ڱ�ǵĿ���  
//typedef singleton_pool<pool_tag, sizeof(int)> spl;  //�ڴ�ض��� 
// 
typedef singleton_pool<struct pool_tag, sizeof(int)> spl;   //���������������

int main()  
{      
	int *p = (int *)spl::malloc();                      //����һ�������ڴ��    

	assert(spl::is_from(p));      
	spl::release_memory();                              //�ͷ�����δ��������ڴ�  
}                                                       //spl���ڴ�ֱ������������� 