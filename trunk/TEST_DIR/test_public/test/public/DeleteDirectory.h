#include <string>
using namespace std;

inline BOOL IsDirectory(const char *pDir)
{
	char szCurPath[500];
	ZeroMemory(szCurPath, 500);
	sprintf_s(szCurPath, 500, "%s//*", pDir);
	WIN32_FIND_DATAA FindFileData;		
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));

	HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData); /**< find first file by given path. */

	if( hFile == INVALID_HANDLE_VALUE )	
	{
		FindClose(hFile);
		return FALSE; /** ��������ҵ���һ���ļ�����ôû��Ŀ¼ */
	}else
	{	
		FindClose(hFile);
		return TRUE;
	}
	
}

inline BOOL DeleteDirectory(const char * DirName)
{
//	CFileFind tempFind;		//����һ��CFileFind�����������������
	char szCurPath[MAX_PATH];		//���ڶ���������ʽ
	_snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName);	//ƥ���ʽΪ*.*,����Ŀ¼�µ������ļ�
	WIN32_FIND_DATAA FindFileData;		
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
	HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);
	BOOL IsFinded = TRUE;
	while(IsFinded)
	{
		IsFinded = FindNextFileA(hFile, &FindFileData);	//�ݹ������������ļ�
 		if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) //�������"." ".."Ŀ¼
 		{
			string strFileName = "";
			strFileName = strFileName + DirName + "//" + FindFileData.cFileName;
			string strTemp;
			strTemp = strFileName;
			if( IsDirectory(strFileName.c_str()) ) //�����Ŀ¼����ݹ�ص���
			{	
				printf("Ŀ¼Ϊ:%s/n", strFileName.c_str());
				DeleteDirectory(strTemp.c_str());
			}
			else
			{
				DeleteFileA(strTemp.c_str());
			}
		}
	}
	FindClose(hFile);

	BOOL bRet = RemoveDirectoryA(DirName);
	if( bRet == 0 ) //ɾ��Ŀ¼
	{
		printf("ɾ��%sĿ¼ʧ�ܣ�/n", DirName);
		return FALSE;
	}
	return TRUE;
}