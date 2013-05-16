// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include <iostream>

#include <sparsehash/dense_hash_map>
using std::cout;
using std::endl;
using std::cin;
#include <sparsehash/sparse_hash_set>
#include <sparsehash/sparse_hash_map>
#include <sparsehash/dense_hash_set>
#include <sparsehash/dense_hash_map>
#include <sparsehash/template_util.h>
#include <sparsehash/type_traits.h>

#define CHECK_IFF(cond, when) do {                                      \
	if (when) {                                                           \
	if (!(cond)) {                                                      \
	puts("ERROR: " #cond " failed when " #when " is true\n");         \
	exit(1);                                                          \
	}                                                                   \
	} else {                                                              \
	if (cond) {                                                         \
	puts("ERROR: " #cond " succeeded when " #when " is false\n");     \
	exit(1);                                                          \
	}                                                                   \
	}                                                                     \
} while (0)


int test2() 
{
	// Run with an argument to get verbose output
	const bool verbose = true;

	google::sparse_hash_set<int> sset;
	google::sparse_hash_map<int, int> smap;
	google::dense_hash_set<int> dset;
	google::dense_hash_map<int, int> dmap;
	dset.set_empty_key(-1);
	dmap.set_empty_key(-1);

	for (int i = 0; i < 100; i += 10) {   // go by tens
		sset.insert(i);
		smap[i] = i+1;
		dset.insert(i + 5);
		dmap[i+5] = i+6;
	}

	if (verbose) {
		for (google::sparse_hash_set<int>::const_iterator it = sset.begin();
			it != sset.end(); ++it)
			printf("sset: %d\n", *it);
		for (google::sparse_hash_map<int,int>::const_iterator it = smap.begin();
			it != smap.end(); ++it)
			printf("smap: %d -> %d\n", it->first, it->second);
		for (google::dense_hash_set<int>::const_iterator it = dset.begin();
			it != dset.end(); ++it)
			printf("dset: %d\n", *it);
		for (google::dense_hash_map<int,int>::const_iterator it = dmap.begin();
			it != dmap.end(); ++it)
			printf("dmap: %d -> %d\n", it->first, it->second);
	}

	for (int i = 0; i < 100; i++) {
		CHECK_IFF(sset.find(i) != sset.end(), (i % 10) == 0);
		CHECK_IFF(smap.find(i) != smap.end(), (i % 10) == 0);
		CHECK_IFF(smap.find(i) != smap.end() && smap.find(i)->second == i+1,
			(i % 10) == 0);
		CHECK_IFF(dset.find(i) != dset.end(), (i % 10) == 5);
		CHECK_IFF(dmap.find(i) != dmap.end(), (i % 10) == 5);
		CHECK_IFF(dmap.find(i) != dmap.end() && dmap.find(i)->second == i+1,
			(i % 10) == 5);
	}
	printf("PASS\n");
	return 0;
}

int main()
{
	test2();


	//key的类型,value的类型
	google::dense_hash_map<int, int> dMap;
	// Deletion and empty routines
	// THESE ARE NON-STANDARD!  I make you specify an                   "impossible" key
	// value to identify deleted and empty buckets.  You can change the
	// deleted key as time goes on, or get rid of it entirely to be insert-only.
	// YOU MUST CALL void set_empty_key(const key_type& key)      
	//必须设置empty key,否则断言失败
	//assertion failed !use_empty ...
	dMap.set_empty_key(-1);
	typedef google::dense_hash_map<int, int>::iterator IntIterator;
	typedef std::pair<int, int> PairInt; 

	const int SIZE = 1000;

	//insert
	cout << "insert lot of number" << endl;
	for(int i = 0; i<SIZE; ++i)
	{
		dMap[i] = -i; //返回value的引用,在赋-i
	}

	//find
	//iterator find(const key_type& key)
	//const_iterator find(const key_type& key)
	IntIterator ite = dMap.find(5);
	cout << "find 5 typed key " << endl
		<< ite->second << endl;    

	//插入,这种方式,如果key已经存在了,就会不插入了。
	//pair<iterator, bool> insert(const value_type& obj)
	//std::pair<const Key,T> 
	PairInt pairInt(SIZE-5, -SIZE-5);
	std::pair<IntIterator, bool> pairInserted = dMap.insert(pairInt);
	cout << "insert pair(int, int)" << endl;
	if(pairInserted.second)
	{
		cout << "Key ="<<pairInserted.first->first 
			<< ", Value" << pairInserted.second<<endl;
	}
	else
	{
		cout << "insert failed" << endl;
	}

	//没有找到
	IntIterator itFind = dMap.find(SIZE+10);
	if(itFind != dMap.end())
	{
		cout << "exist key" << SIZE+10 << endl;
	}
	else
	{
		cout << "does't exist key" << SIZE+10 << endl;
	}

	//这种插入方式的特点:
	//如果在dMaP存在这个key,就返回这个key的value的引用
	//如果不存在则向dMap插入这个key,还返回引用
	int & value = dMap[SIZE-10] = -926;
	if(value)
	{
		cout << "inserted success" << endl;
	}
	else
	{
		cout << "inserted failed" << endl;
	}

	std::cin.get();  
	return 0;
}
