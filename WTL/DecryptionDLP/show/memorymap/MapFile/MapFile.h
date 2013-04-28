#pragma once

class CMapFile
{
protected:
	HANDLE m_file;
	HANDLE m_map;
	void* m_mapView;

	TCHAR m_filename[128];
	bool m_read_only;
	bool m_del_flag;
	
private:
	bool CrFile(void);
	bool CrMap(void);
	bool CrView(void);

public:
	void Close(bool del_flag);
	void Close(void);
	void* GetMapView(const TCHAR* filename);
	unsigned FileSize(void);
	const TCHAR* FileName(void);
	HANDLE GetFileHandle(void);

public:
	CMapFile(bool read_only, bool del_flag);
	~CMapFile(void);
};
