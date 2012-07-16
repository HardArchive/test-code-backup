#include "stdafx.h"

extern "C" __declspec(dllexport) bool LogonImage(const WCHAR* pwszImagePath); 
//替换当前系统的开机界面，只需将路径传入即可，此函数会自己判断系统版本和图片是否符合要求

extern "C" __declspec(dllexport) bool UnLogonImage();    //移除启动界面修改,成功true，失败false

extern "C" __declspec(dllexport) int GetOSVer();
/******************************************************************************* 
1、 函数名称： GetOSVer
2、 功能描述： 获取操作系统的版本号
3、 输入参数： 无
4、 返 回 值： 失败返回 0； xp 返回 1｜ windows 2003 返回 2 ｜windows 2003 返回 3｜vista and 2008 返回 4｜2008 R2 and Windows 7 返回  5｜
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-8-30 14:15:56
8、 备    注： 
********************************************************************************/ 
extern "C" __declspec(dllexport) bool CreateMultiDir(const WCHAR* pwInDir);
//跟据pwInDir创建多级目录