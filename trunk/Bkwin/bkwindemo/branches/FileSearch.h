/********************************************************************
created:	2008/10/17
filename: 	FileSearch.h
file base:	FileSearch
file ext:	h
author:		huchao

purpose:	Windows 文件搜索函数的封装
*********************************************************************/
#pragma once
#include <Windows.h>
#include <atlstr.h>
#include <vector>
#include <cassert>

// 保存文件全路径的结构体
typedef struct tagSearchFileInfo
{
	CString strFullPath;	// 文件的全路径
}SearchFileInfo, *pSearchFileInfo;

typedef std::vector<pSearchFileInfo> vecFileInfo;	// 保存文件全路径的 Vector

class CFileSearch
{
public:
	CFileSearch(void);
	~CFileSearch(void);

public:
	// 功能：初始化文件搜索
	// 返回值：FALSE--失败，否则成功
	BOOL InitSearch();

	// 功能：反初始化文件搜索
	// 返回值：FALSE--失败，否则成功
	BOOL UnInitSearch();

	// 功能：设置是否需要搜索子文件夹
	// 参数说明
	// bIsSearchChild：TRUE--需要搜索，FALSE--不需要搜索
	// 返回值：FALSE--失败，否则成功
	BOOL SetSearchChildDirectory(IN BOOL bIsSearchChild);

	// 功能：添加一个特定的文件到最终的搜索列表
	// 参数说明
	// strFileFullPath：文件的绝对路径
	// 返回值：FALSE--失败，否则成功
	BOOL AddFileToSearchList(IN const CString& strFileFullPath);

	// 功能：添加需要搜索的文件夹
	// 参数说明
	// strFullPath：文件夹的绝对路径
	// 返回值：FALSE--失败，否则成功
	BOOL AddSearchDiretory(IN const CString& strFullPath);

	// 功能：进行文件的搜索
	// 返回值：FALSE--失败，否则成功
	BOOL DoSearch();

	// 功能：获取最终文件搜索结果列表
	// 参数说明
	// vecFiles：最终文件列表的Vector（由于此处返回引用，注意防止类被析构）
	// 返回值：FALSE--失败，否则成功
	BOOL GetSearchResultList(OUT vecFileInfo& vecFiles);

protected:
	typedef enum enumSearchStatus
	{	// 搜索文件的状态
		enumStatus_Stoped = 0,	// 尚未开始搜索
		enumStatus_Inited,		// 初始化完成
		enumStatus_Search,		// 搜索
	}SearchStatus;

	BOOL FreeItemMemory();

private:
	BOOL _FindFiles();
	BOOL _AddFindFilesToList(const CString& strDirectory);

	SearchStatus m_CurSearchStatus;			// 当前搜索的状态
	vecFileInfo m_vecFileInfo;				// 保存搜索出来的文件列表
	BOOL m_bIsSearchChildDiretory;			// 是否搜索子文件夹
	std::vector<CString> m_vecSearchDir;	// 需要搜索的文件夹列表
};
