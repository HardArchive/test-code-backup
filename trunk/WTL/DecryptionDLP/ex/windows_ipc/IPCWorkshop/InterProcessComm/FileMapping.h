//////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                            
// IPCWorkshop							                                      
//                                                                            
// Environment:     Visual C++ 6.0, Windows 2000                              
//                                                                            
// Notes:	Class CFileMapping. This class provides an encapsulation for File mapping 
//			which is an efficient way for two or more processes on the same computer to 
//			share data.
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FILEMAPPING_H__
#define __FILEMAPPING_H__

class CFileMapping 
{
public:
	CFileMapping();
	~CFileMapping();

	// Operations
	bool Initialize(LPCTSTR, int);
	void Write(char*);
	void Read(char*&);

private:
	HANDLE m_hFileHandle;
	LPVOID m_pViewOfFile;
	int m_nFileSize;
};

#endif //__FILEMAPPING_H__