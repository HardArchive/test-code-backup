#pragma once
#include <Windows.h>
/******************************************************************************* 
* 1、 文件名称： UrlShortcut
* 2、 版　　本： Version *.*
* 3、 描    述： URL快捷方式对象
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-2-6 13:40:36
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 

class CUrlShortcuts
{
public:
	CUrlShortcuts(void);
	~CUrlShortcuts(void);

public:
	TCHAR m_tszTitle[MAX_PATH];       //标题
	TCHAR m_tszUrl[MAX_PATH];         //网址
	TCHAR m_tszParentDir[MAX_PATH];   //其所在的父目录
	int m_iParentLevel[MAX_PATH];     //父目录级别 根目录为零级，其次往上递增
	//
	int m_iUrlPosition;                  //保留使用  在列表控件中的位置，排序时保存的位置


};
