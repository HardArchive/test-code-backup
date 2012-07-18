// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include <gtest.h>
#include <gtest/gtest.h>
//#pragma comment(lib, "gtestd.lib")

//求最大公约数
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

//gtest中，断言的宏可以理解为分为两类，一类是ASSERT系列，一类是EXPECT系列。一个直观的解释就是：
//1. ASSERT_* 系列的断言，当检查点失败时，退出当前函数（注意：并非退出当前案例）。
//2. EXPECT_* 系列的断言，当检查点失败时，继续往下执行。
TEST(FooTest, HandleNoneZeroInput)
{
	EXPECT_EQ(2, Foo(4, 10));  // int型比较，预期值：3，实际值：Foo(4, 10)
	EXPECT_EQ(6, Foo(30, 18)); //假如你的Foo(4, 10) 结果为4的话，会在结果中输出：
	// g:\myproject\c++\gtestdemo\gtestdemo\gtestdemo.cpp(16): error: Value of: Foo(4, 10)
	// Actual: 4
	// Expected:2
	EXPECT_EQ(7, Foo(30, 18));
}

int _tmain(int argc, _TCHAR* argv[])
{
	testing::GTEST_FLAG(output) = "xml:";  //将结果输入XML中
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();

	return 0;
}