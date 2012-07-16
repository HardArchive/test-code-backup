#ifndef _RAINSOFT_SEARCHFILE_H_
#define _RAINSOFT_SEARCHFILE_H_
typedef void (* DoFileCallback)(const char *szFolderPath);
class CSearchFile
{
private:
	DoFileCallback m_funDoFile;

public:
	CSearchFile()
	{
		this->m_funDoFile=NULL;
	}
	void SetCallBackFun(DoFileCallback pFun);
	void StartSearch();
};
#endif