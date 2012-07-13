#pragma once

/******************************************************************************* 
* 1、 文件名称： Folde
* 2、 版　　本： Version *.*
* 3、 描    述： 文件夹对象-里面包含有URL快捷方式
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-2-6 16:04:24
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 
#include "UrlShortcuts.h"
class CFolder
{
public:
	CFolder(void);
	~CFolder(void);

public:
	TCHAR m_tszParentDir[MAX_PATH];    //父目录文件夹名，  如果为根目录则为根目录所有文件夹路径
	TCHAR m_tszDirName[MAX_PATH];      //文件夹名

	int m_iDirLevel;              //当前文件夹级别
	int m_iFileNum;               //其内的文件路数量
	int m_iDirPositon;            //保留使用， 在列表控件中的位置，排序时保存的位置


};
