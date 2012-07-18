// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//#include <gtest.h>
#include <gtest/gtest.h>
//#pragma comment(lib, "gtestd.lib")

//�����Լ��
int Foo(int a, int b)
{
	if (a == 0 || b == 0)
	{
		throw "don't do that";
	}
	int c = a % b;
	if (c == 0)
		return b;
	return Foo(b, c);
}

//gtest�У����Եĺ�������Ϊ��Ϊ���࣬һ����ASSERTϵ�У�һ����EXPECTϵ�С�һ��ֱ�۵Ľ��;��ǣ�
//1. ASSERT_* ϵ�еĶ��ԣ�������ʧ��ʱ���˳���ǰ������ע�⣺�����˳���ǰ��������
//2. EXPECT_* ϵ�еĶ��ԣ�������ʧ��ʱ����������ִ�С�
TEST(FooTest, HandleNoneZeroInput)
{
	EXPECT_EQ(2, Foo(4, 10));  // int�ͱȽϣ�Ԥ��ֵ��3��ʵ��ֵ��Foo(4, 10)
	EXPECT_EQ(6, Foo(30, 18)); //�������Foo(4, 10) ���Ϊ4�Ļ������ڽ���������
	// g:\myproject\c++\gtestdemo\gtestdemo\gtestdemo.cpp(16): error: Value of: Foo(4, 10)
	// Actual: 4
	// Expected:2
	EXPECT_EQ(7, Foo(30, 18));
}

int _tmain(int argc, _TCHAR* argv[])
{
	testing::GTEST_FLAG(output) = "xml:";  //���������XML��
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

	return 0;
}