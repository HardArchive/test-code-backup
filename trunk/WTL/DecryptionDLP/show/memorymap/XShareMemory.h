// XShareMemory.h: interface for the XShareMemory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSHAREMEMORY_H__32BEA564_49E7_4756_994E_AFC067505D25__INCLUDED_)
#define AFX_XSHAREMEMORY_H__32BEA564_49E7_4756_994E_AFC067505D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	DEFAULT_FILENAME	NULL
#define	DEFAULT_MAPNAME		L"_LT_SHARE_MEMORY_"
#define	DEFAULT_MAPSIZE		(0xFFFF + 1)

// 进程间共享内存

class XShareMemory  
{
public:
	XShareMemory();
	XShareMemory(WCHAR *szMapName, DWORD dwSize);
	virtual ~XShareMemory();

	bool Create(WCHAR *szMapName, DWORD dwSize);	// 创建共享内存, 已存在则打开
	bool Read(void *pData, DWORD dwSize);
	bool Write(const void *pData, DWORD dwSize);
	
	LPVOID GetBuffer();
	DWORD GetSize();
	bool IsCreated();

	HANDLE  m_hMemoryEvent;		// 同步事件, 写完重置为有信号, 用于应用程序获取写操作

private:
	void Destory();
	void Init();
protected:
	HANDLE  m_hBufEvent;		// 同步事件， 用于同步读写操作
	HANDLE	m_hFileMap;
	LPVOID	m_lpFileMapBuffer;

	WCHAR	*m_pMapName;
	DWORD	m_dwSize;

	int		m_iCreateFlag;		// 创建标志
};

#endif // !defined(AFX_XSHAREMEMORY_H__32BEA564_49E7_4756_994E_AFC067505D25__INCLUDED_)
