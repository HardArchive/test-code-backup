// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include "atlwfile.h"
//#include <atlfile.h>
//Interface
//Name                 Description 
//Create            Creates a new file 
//Open              Opens an existing file. 
//Close             Closes the file. 
//Read              Reads a number of bytes from the file. 
//Write             Writes a number of bytes to the file. 
//Seek              Sets the file position. 
//GetPosition       Returns the current file position. 
//Flush             Flushes all uncommitted data to the file. 
//DuplicateHandle   Duplicate file handle. 
//GetSize           Returns the size of the file. 
//GetFileTime       Returns the file creation-, modification- and access-times. 
//FileExists        Returns whether a filename exists. 
//Delete            Deletes a file. 
//Rename            Renames a file. 
//using namespace 9CPPL;

//#include "9CppFile.h"
using namespace QNA;
#include "file.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//char szFilename[MAX_PATH] = {"D:\\WinPath\\desktop\\test\\reec.html"};
	//CFile f;
	//if( f.Open(szFilename)==TRUE ) 
	//{
	//	FILETIME ftModified;
	//	f.GetFileTime(NULL, NULL, &ftModified);
	//	DWORD dwSize = f.GetSize();
	//	LPSTR pBuffer = (LPSTR)new CHAR[dwSize + 1];
	//	f.Read(pBuffer, dwSize);
	//	pBuffer[dwSize] = '\0';
	//	f.Close();
	//	delete [] pBuffer;
	//}
	
	PCHAR ptem = NULL;
	size_t nTemLen = 0;
	TCHAR tszFileName[MAX_PATH] =  {_T("D:\\b.txt")};
	BYTE szbyFileTem[1024*10] = {0};

	RG::CFile clsFile(tszFileName, _T("rt+"));

	nTemLen = 1024*10;
	ptem = (PCHAR)szbyFileTem;
	memset(szbyFileTem, 0, 1024*10);
	nTemLen = clsFile.ReadAll(szbyFileTem, nTemLen);
	
	if (clsFile.Eof())
	{
		clsFile.SeekStart();
	}
	
	memset(szbyFileTem, 0, 1024*10);
	nTemLen = 16;
	nTemLen = clsFile.Read(szbyFileTem, nTemLen);
	ptem = (PCHAR)szbyFileTem;


	clsFile.Write(_T("阿毛，你真帅！！！！!"));
	clsFile.Flush();
	clsFile.Write(_T("123456789"));
	clsFile.Write(_T("Go beyond myself, transcend myself!"));

	clsFile.Write(_T("123456789"));
	clsFile.Write(_T("123456789"));
	clsFile.Flush();
	

	clsFile.Close();



	return 0;
}

