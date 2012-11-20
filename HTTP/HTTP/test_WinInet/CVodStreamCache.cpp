#include "stdafx.h"
#include "CVodStreamCache.h"

HANDLE CVodStreamCache::Create(char token[], char path[])
{
	internetOpen = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (internetOpen == NULL)
	{
		cout << "Internet open failed!" << endl;
		return NULL;
	}

	internetOpenUrl = InternetOpenUrl(internetOpen, path,
		NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
	if (internetOpenUrl == NULL)
	{
		cout << "Internet open url failed! Error code = " << GetLastError() << endl;
		InternetCloseHandle(internetOpen);
		return NULL;
	}

	DWORD dwStatusSize = sizeof(dwStatusCode);
	HttpQueryInfo(internetOpenUrl, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwStatusSize, NULL);
	cout << "Status: " << dwStatusCode << endl;

	DWORD dwLengthSize = sizeof(dwContentLength);
	HttpQueryInfo(internetOpenUrl, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, &dwContentLength, &dwLengthSize, NULL);
	cout << "File size: " << dwContentLength << endl;

	createFile = CreateFile(_T("E:\\download.flv"), GENERIC_WRITE|GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (createFile == INVALID_HANDLE_VALUE)
	{
		cout << "Create file failed!" << endl;
		InternetCloseHandle(internetOpenUrl);
		return NULL;
	}

	return createFile;
}

int CVodStreamCache::ReadBlock(long offset, int len, char *buffer)
{
	return 0;
}

int CVodStreamCache::WriteBlock()
{
	BOOL internetReadFile;
	char buffer[BUFFERSIZE];
	memset(buffer, 0, sizeof(buffer));
	DWORD byteRead = 0;

	BOOL hwrite;
	DWORD written;

	DWORD byteDown = 0;
	int a = 0;
	cout << "已下载：" << a << "%";
	while (1)
	{
		internetReadFile = InternetReadFile(internetOpenUrl, buffer, sizeof(buffer), &byteRead);
		if (byteRead == 0)
		{
			break;
		}

		byteDown += byteRead;
		cout << "\b";
		do 
		{
			cout << "\b";
			a /= 10;
		} while (a);
		a = 100 * byteDown / dwContentLength;
		cout << a << "%";

		//SetFilePointer(createFile, a, NULL, FILE_BEGIN);
		hwrite = WriteFile(createFile, buffer, sizeof(buffer), &written, NULL);
		if (hwrite == 0)
		{
			cout << "Write to file failed!" << endl;
			CloseHandle(createFile);
			return 0;
		}
		//FlushFileBuffers(createFile);
	}

	cout << endl << "Finished downloading!" << endl;

	return 1;
}

void CVodStreamCache::Destory()
{
	;
}

void CVodStreamCache::CacheInfo()
{
	;
}


#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}


int main()
{
	char path[] = "www.9cpp.com";

	CVodStreamCache vsc;
	vsc.Create("", path);
	vsc.WriteBlock();
	//HINTERNET internetOpen;
	//internetOpen = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	//if (internetOpen == NULL)
	//{
	//	cout << "Internet open failed!" << endl;
	//	return 0;
	//}

	//HINTERNET internetOpenUrl;
	//internetOpenUrl = InternetOpenUrl(internetOpen, _T("http://f.youku.com/player/getFlvPath/sid/124945035454427719_00/st/flv/fileid/020064010046E63D64AEE90024810D0EF8CBA7-4993-FEC3-E5A1-65CADB030BC4?K=8821fe9672288e9414fe338"),
	//	NULL, 0, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_PRAGMA_NOCACHE, 0);
	//if (internetOpenUrl == NULL)
	//{
	//	cout << "Internet open url failed! Error code = " << GetLastError() << endl;
	//	InternetCloseHandle(internetOpen);
	//	return 0;
	//}

	//DWORD m_dwStatusCode;
	//DWORD dwStatusSize = sizeof(m_dwStatusCode);
	//HttpQueryInfo(internetOpenUrl, HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &m_dwStatusCode, &dwStatusSize, NULL);
	//cout << "Status: " << m_dwStatusCode << endl;

	//DWORD m_dwContentLength;
	//DWORD dwLengthSize = sizeof(m_dwContentLength);
	//HttpQueryInfo(internetOpenUrl, HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER, &m_dwContentLength, &dwLengthSize, NULL);
	//cout << "File size: " << m_dwContentLength << endl;

	//HANDLE createFile;
	//createFile = CreateFile(_T("E:\\download.flv"), GENERIC_WRITE|GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	//if (createFile == INVALID_HANDLE_VALUE)
	//{
	//	cout << "Create file failed!" << endl;
	//	InternetCloseHandle(internetOpenUrl);
	//	return 0;
	//}

	//BOOL internetReadFile;
	//char buffer[1024];
	//memset(buffer, 0, sizeof(buffer));
	//DWORD byteRead = 0;

	//BOOL hwrite;
	//DWORD written;

	//DWORD byteDown = 0;
	//int a = 0;
	//cout << "已下载：" << a << "%";
	//while (1)
	//{
	//	internetReadFile = InternetReadFile(internetOpenUrl, buffer, sizeof(buffer), &byteRead);
	//	if (byteRead == 0)
	//	{
	//		break;
	//	}

	//	byteDown += byteRead;
	//	cout << "\b";
	//	do 
	//	{
	//		cout << "\b";
	//		a /= 10;
	//	} while (a);
	//	a = 100 * byteDown / m_dwContentLength;
	//	cout << a << "%";

	//	//SetFilePointer(createFile, a, NULL, FILE_BEGIN);
	//	hwrite = WriteFile(createFile, buffer, sizeof(buffer), &written, NULL);
	//	if (hwrite == 0)
	//	{
	//		cout << "Write to file failed!" << endl;
	//		CloseHandle(createFile);
	//		return 0;
	//	}
	//	//FlushFileBuffers(createFile);
	//}

	//cout << endl << "Finished downloading!" << endl;

	return 0;
}