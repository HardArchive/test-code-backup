// test.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#pragma comment(lib, "lua51.lib")
};


#include <iostream>
#include <string>
using namespace std;


/******************************************************************************* 
1、 函数名称： initLuaEnv
2、 功能描述： 初始化Lua环境.
3、 输入参数： 无
4、 返 回 值： lua_State* ->lua_State*所指向的结构中封装了Lua的运行时环境
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-12-27 16:45:59
8、 备    注： lua_open函数用来获取一个新环境，然后我们
			   用luaopen_base打开Lua的基础库，然后用luaL_openlibs打开Lua的io、string、math、table等高级库。
********************************************************************************/ 
lua_State* initLuaEnv()
{
	lua_State* luaEnv = lua_open();
	luaopen_base(luaEnv);
	luaL_openlibs(luaEnv);

	return luaEnv;
}

/******************************************************************************* 
1、 函数名称： bool loadLuaFile(lua_State* luaEnv, const string& fileName)
2、 功能描述： 加载Lua文件到Lua运行时环境中.
3、 输入参数： lua_State* luaEnv  指向lua环境，即lua环境, const string& fileName  lua文件名
4、 返 回 值： 这两个函数如果返回非0，表示加载失败——你的Lua脚本文件可能未找到或Lua脚本有语法错误……
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-12-27 16:47:58
8、 备    注：  我们用luaL_loadfile和lua_pcall来将一个Lua脚本加载到对应的Lua运行时环境中——注意：并不自动执行，只是加载。
********************************************************************************/ 
bool loadLuaFile(lua_State* luaEnv, const string& fileName)
{
	int result = luaL_loadfile(luaEnv, fileName.c_str());
	if (result)
	{
		return false;
	}

	result = lua_pcall(luaEnv, 0, 0, 0);
	return result == 0;
}

/******************************************************************************* 
1、 函数名称： getGlobalProc
2、 功能描述： 获取全局函数.
3、 输入参数： lua_State* luaEnv, const string& procName
4、 返 回 值： 
5、 动态内存： 
6、 代码设计：  阿毛
7、 开发日期： 2011-12-27 17:17:48
8、 备    注： 用lua_getglobal函数将Lua脚本中的全局函数、全局变量等入栈，放在栈顶
********************************************************************************/ 
lua_CFunction getGlobalProc(lua_State* luaEnv, const string& procName)
{
	lua_getglobal(luaEnv, procName.c_str());
	if (!lua_iscfunction(luaEnv, 1))
	{
		return 0;
	}

	return lua_tocfunction(luaEnv, 1);
}


int main()
{
	// 初始化Lua运行时环境.
	lua_State* luaEnv = initLuaEnv();
	if (!luaEnv)
	{
		return -1;
	}

	// 加载脚本到Lua环境中.
	if (!loadLuaFile(luaEnv, "GenerateNoteXML.lua"))
	{
		cout<<"Load Lua File FAILED!"<<endl;
		return -1;
	}

	// 获取Note信息.
	string fromName;
	string toName;
	string msgContent;

	cout<<"Enter your name:"<<endl;
	cin>>fromName;

	cout<<"\nEnter destination name:"<<endl;
	cin>>toName;

	cout<<"\nEnter message content:"<<endl;
	getline(cin, msgContent);
	getline(cin, msgContent);

	// 将要调用的函数和函数调用参数入栈.
	lua_getglobal(luaEnv, "generateNoteXML");
	//我们用lua_push系列函数来将要调用函数所需参数全部入栈，入栈顺序为Lua函数对应参数从左到右的顺序。
	lua_pushstring(luaEnv, fromName.c_str());  //将字符串压入堆栈
	lua_pushstring(luaEnv, toName.c_str());
	lua_pushstring(luaEnv, msgContent.c_str());

	// 调用Lua函数（3个参数,一个返回值）.
	lua_pcall(luaEnv, 3, 1, 0);
	/*
	调用Lua函数------最后用lua_pcall来调用函数。Lua_pcall函数原型如下：
	int lua_pcall(lua_State* L, int nargs, int nresults, int errfunc);  
	其中，L为此函数执行的Lua环境，nargs为此函数所需的参数个数，nresults为此函数的返回值个数，errfunc为发生错误时错误处理函数在堆栈中的索引。
	*/

	// 获取返回值.
	if (lua_isboolean(luaEnv, -1))
	{
		int success = lua_toboolean(luaEnv, -1);
		if (success)
		{
			cout<<"\nGenerate Note File Successful!"<<endl;
		}
	}
	//我们可以通过检测栈顶的位置（从-1开始），来获取返回值。获取返回值后，用lua_pop将栈顶元素出栈——出栈个数为
	// 将返回值出栈.
	lua_pop(luaEnv, 1);

	// 释放Lua运行时环境.
	lua_close(luaEnv);  //通过lua_close函数来关闭Lua环境并释放资源。

	system("pause");
	return 0;
}

//:-)