//ͷ�ļ�

#ifndef SN_TEMPLATE_CLASS
#define SN_TEMPLATE_CLASS

//�ڴ�ӳ����
template <class Type_MemoryMap>
class CMemoryMap
{
public:
	CMemoryMap();
	~CMemoryMap(void);

	Type_MemoryMap* TemplateMemoryMap();								//����ģ�����ͷ�������ָ��
	LPVOID			MemoryMap();										//����ӳ���ڴ�ָ��
	BOOL			OpenMap(LPCSTR lpcMapName,DWORD dwMapSize);			//����(��)�ڴ�ӳ��(ָ��ӳ���С)
	BOOL			OpenMap(LPCSTR lpcMapName);							//����(��)�ڴ�ӳ��(��ģ�����ʹ�С)
	void			CloseMap();											//�ر��ڴ�ӳ��

	BOOL Lock(DWORD dwTimeout=INFINITE);								//���̻�������
	BOOL UnLock();														//���̻������
private:
	HANDLE				m_hMapFile;				//�ڴ�ӳ���ļ����
	Type_MemoryMap*     m_pMemoryMap;			//�ڴ�ָ��
	CMutex*             m_pMutex;				//�ڴ滥�����
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

	//��(����)�ڴ��ļ�ӳ����
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

	//ӳ���ڴ��ļ�
	m_pMemoryMap=(Type_MemoryMap *)::MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if(!m_pMemoryMap)
	{
		CloseMap();
		return FALSE;
	}

	//�����������
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