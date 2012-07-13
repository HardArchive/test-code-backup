// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
1�� �������ƣ� initLuaEnv
2�� ���������� ��ʼ��Lua����.
3�� ��������� ��
4�� �� �� ֵ�� lua_State* ->lua_State*��ָ��Ľṹ�з�װ��Lua������ʱ����
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-12-27 16:45:59
8�� ��    ע�� lua_open����������ȡһ���»�����Ȼ������
			   ��luaopen_base��Lua�Ļ����⣬Ȼ����luaL_openlibs��Lua��io��string��math��table�ȸ߼��⡣
********************************************************************************/ 
lua_State* initLuaEnv()
{
	lua_State* luaEnv = lua_open();
	luaopen_base(luaEnv);
	luaL_openlibs(luaEnv);

	return luaEnv;
}

/******************************************************************************* 
1�� �������ƣ� bool loadLuaFile(lua_State* luaEnv, const string& fileName)
2�� ���������� ����Lua�ļ���Lua����ʱ������.
3�� ��������� lua_State* luaEnv  ָ��lua��������lua����, const string& fileName  lua�ļ���
4�� �� �� ֵ�� ����������������ط�0����ʾ����ʧ�ܡ������Lua�ű��ļ�����δ�ҵ���Lua�ű����﷨���󡭡�
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-12-27 16:47:58
8�� ��    ע��  ������luaL_loadfile��lua_pcall����һ��Lua�ű����ص���Ӧ��Lua����ʱ�����С���ע�⣺�����Զ�ִ�У�ֻ�Ǽ��ء�
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
1�� �������ƣ� getGlobalProc
2�� ���������� ��ȡȫ�ֺ���.
3�� ��������� lua_State* luaEnv, const string& procName
4�� �� �� ֵ�� 
5�� ��̬�ڴ棺 
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-12-27 17:17:48
8�� ��    ע�� ��lua_getglobal������Lua�ű��е�ȫ�ֺ�����ȫ�ֱ�������ջ������ջ��
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
	// ��ʼ��Lua����ʱ����.
	lua_State* luaEnv = initLuaEnv();
	if (!luaEnv)
	{
		return -1;
	}

	// ���ؽű���Lua������.
	if (!loadLuaFile(luaEnv, "GenerateNoteXML.lua"))
	{
		cout<<"Load Lua File FAILED!"<<endl;
		return -1;
	}

	// ��ȡNote��Ϣ.
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

	// ��Ҫ���õĺ����ͺ������ò�����ջ.
	lua_getglobal(luaEnv, "generateNoteXML");
	//������lua_pushϵ�к�������Ҫ���ú����������ȫ����ջ����ջ˳��ΪLua������Ӧ���������ҵ�˳��
	lua_pushstring(luaEnv, fromName.c_str());  //���ַ���ѹ���ջ
	lua_pushstring(luaEnv, toName.c_str());
	lua_pushstring(luaEnv, msgContent.c_str());

	// ����Lua������3������,һ������ֵ��.
	lua_pcall(luaEnv, 3, 1, 0);
	/*
	����Lua����------�����lua_pcall�����ú�����Lua_pcall����ԭ�����£�
	int lua_pcall(lua_State* L, int nargs, int nresults, int errfunc);  
	���У�LΪ�˺���ִ�е�Lua������nargsΪ�˺�������Ĳ���������nresultsΪ�˺����ķ���ֵ������errfuncΪ��������ʱ���������ڶ�ջ�е�������
	*/

	// ��ȡ����ֵ.
	if (lua_isboolean(luaEnv, -1))
	{
		int success = lua_toboolean(luaEnv, -1);
		if (success)
		{
			cout<<"\nGenerate Note File Successful!"<<endl;
		}
	}
	//���ǿ���ͨ�����ջ����λ�ã���-1��ʼ��������ȡ����ֵ����ȡ����ֵ����lua_pop��ջ��Ԫ�س�ջ������ջ����Ϊ
	// ������ֵ��ջ.
	lua_pop(luaEnv, 1);

	// �ͷ�Lua����ʱ����.
	lua_close(luaEnv);  //ͨ��lua_close�������ر�Lua�������ͷ���Դ��

	system("pause");
	return 0;
}

//:-)