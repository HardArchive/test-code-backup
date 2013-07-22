//////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                            
// IPCWorkshop							                                      
//                                                                            
// Environment:     Visual C++ 6.0, Windows 2000                              
//                                                                            
// Notes:	Class CClipboard. This class provides an encapulation of		  
//			clipboard, which is a set of functions and messages that enable 
//			applications to transfer data. Because all applications have 
//			access to the clipboard, data can be easily	transferred between 
//			applications or within an application.		  
//			Widely used for Cut & Paste and Drag & Drop operations			  
//                                                                            
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

class CClipboard
{
public:
	CClipboard();
	~CClipboard();
	
	//Operations
	bool Open();
	bool Close();
	bool Empty();
	void Register(LPCTSTR);
	bool Write(char*, int);
	bool Read(char*&, int);

private:
	static CLIPFORMAT NEAR m_cfPrivate;
};

#endif //__CLIPBOARD_H__ 