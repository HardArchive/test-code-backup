//头文件

#ifndef SN_TEMPLATE_CLASS
#define SN_TEMPLATE_CLASS

//内存映射类
template <class Type_MemoryMap>
class CMemoryMap
{
public:
	CMemoryMap();
	~CMemoryMap(void);

	Type_MemoryMap* TemplateMemoryMap();								//根据模板类型返回类型指针
	LPVOID			MemoryMap();										//返回映射内存指针
	BOOL			OpenMap(LPCSTR lpcMapName,DWORD dwMapSize);			//创建(打开)内存映射(指定映射大小)
	BOOL			OpenMap(LPCSTR lpcMapName);							//创建(打开)内存映射(以模板类型大小)
	void			CloseMap();											//关闭内存映射

	BOOL Lock(DWORD dwTimeout=INFINITE);								//进程互斥锁定
	BOOL UnLock();														//进程互斥解锁
private:
	HANDLE				m_hMapFile;				//内存映射文件句柄
	Type_MemoryMap*     m_pMemoryMap;			//内存指针
	CMutex*             m_pMutex;				//内存互斥对象
};

template <class Type_MemoryMap>
BOOL CMemoryMap<Type_MemoryMap>::UnLock()
{
	return m_pMutex->Unlock();
}

template <class Type_MemoryMap>
BOOL CMemoryMap<Type_MemoryMap>::Lock( DWORD dwTimeout/*=INFINITE*/ )
{
	return m_pMutex->Lock(dwTimeout);
}

template <class Type_MemoryMap>
BOOL CMemoryMap<Type_MemoryMap>::OpenMap( LPCSTR lpcMapName )
{
	return OpenMap(lpcMapName,sizeof(Type_MemoryMap));
}

template <class Type_MemoryMap>
LPVOID CMemoryMap<Type_MemoryMap>::MemoryMap()
{
	return (LPVOID)m_pMemoryMap;
}

template <class Type_MemoryMap>
CMemoryMap<Type_MemoryMap>::~CMemoryMap( void )
{
	CloseMap();
}

template <class Type_MemoryMap>
CMemoryMap<Type_MemoryMap>::CMemoryMap()
{
	m_hMapFile=NULL;
	m_pMemoryMap=NULL;
	m_pMutex=NULL;
}

template <class Type_MemoryMap>
Type_MemoryMap* CMemoryMap<Type_MemoryMap>::TemplateMemoryMap()
{
	return m_pMemoryMap;
}

template <class Type_MemoryMap>
BOOL CMemoryMap<Type_MemoryMap>::OpenMap( LPCSTR lpcMapName,DWORD dwMapSize)
{
	CHAR szTmpName[128]={0};

	sprintf(szTmpName,"SilenceRet-Map:%s",lpcMapName);

	//打开(创建)内存文件映射句柄
	m_hMapFile=OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE,szTmpName);
	if(!m_hMapFile)
	{
		m_hMapFile=::CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, dwMapSize,szTmpName);
		if(!m_hMapFile)
		{
			CloseMap();
			return FALSE;
		}
	}

	//映射内存文件
	m_pMemoryMap=(Type_MemoryMap *)::MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if(!m_pMemoryMap)
	{
		CloseMap();
		return FALSE;
	}

	//创建互斥对象
	sprintf(szTmpName,"SilenceRet-Mutex:%s",lpcMapName);
	m_pMutex=new CMutex(FALSE,szTmpName);
	return TRUE;
}

template <class Type_MemoryMap>
void CMemoryMap<Type_MemoryMap>::CloseMap()
{
	if (m_pMemoryMap)
	{
		::UnmapViewOfFile(m_pMemoryMap);
		m_pMemoryMap=NULL;
	}
	if (m_hMapFile)
	{
		::CloseHandle(m_hMapFile);
		m_hMapFile=NULL;
	}
	if (m_pMutex)
	{
		delete m_pMutex;
		m_pMutex=NULL;
	}
}

#endif