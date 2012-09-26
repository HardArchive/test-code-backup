// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//cleanDos2011.5.31.3.cpp  
//win32����̨����,������������  
//û���ϸ���Թ�        
  
#include <windows.h>  
#include <stdio.h>  
#include <string.h>  
  
  
DWORD Total = 0;         //�ļ���Ŀ¼����  
DWORD dwFile = 0;        //�ļ�����  
DWORD dwFolder = 0;      //���ļ�������  
DWORD dwDeleteFile = 0;  //ɾ���ļ�����  
     
  
  //www.heatpress123.net
//����  
VOID  TimeDifference(SYSTEMTIME StOld, SYSTEMTIME StNow);//ʱ��  
VOID  Show(VOID);  
DWORD FindDirFile(LPSTR szPath);                   //�����ļ���Ŀ¼  
DWORD DeleteFileSuffix(LPSTR szPath, LPSTR Suffix);//ɾ���ض���׺�ļ�       //����ʱɾ�ĸɾ�,��ʱɾ���ɾ�����������ɾ�ļ��е�,  
DWORD DeleteDirFile(LPSTR szPath);                 //ɾ���ļ�������ļ�      //���淢��RemoveDirectory()ֻ��ɾ���ļ��У�����·�����󣬾Ͳ�Ҫ��.  
VOID  Clear0(DWORD *Total, DWORD *dwFile, DWORD *dwFolder, DWORD *dwDeleteFile);  
VOID  OS_Version(CHAR *szVersionInfo);  
  
//1  
VOID GetSystemDir(VOID); //��ȡϵͳλ��  
VOID SystemInfo(VOID);   //��ȡϵͳ��Ϣ  
VOID GetTime(VOID);      //��ȡʱ��  
VOID SpaceSize(CHAR  *RootPathName);  
VOID PrintSpaceSize();  
VOID AllSystemInfo(VOID);  
  
//2  
VOID DeleteRubbish(VOID);    //ɾ�������ļ�  
  
  
//3  
VOID QQClean(VOID);//Win7 / XP  
//VOID GetUserNameDocuments(LPSTR c);//��ȡ�û���  
  
//4  
VOID StatisticsFile(VOID);//ͳ��  
VOID Conversion(CHAR* c); //��ʽת��  
  
//5����ۼ�  
VOID DeleteTrace(VOID);  
  
VOID Show(VOID)  
{  
    printf("\n1: ϵͳ��Ϣ\t");  
    printf("2����������\t");  
    printf("3: QQ��ԭ\t\n");  
    printf("4: ͳ���ļ�\t");  
    printf("5: ����ۼ�\t");  
    printf("����: �˳�\n\n");  
}  
  
  
VOID GetSystemDir()  
{     
    CHAR UserName[MAX_PATH];  
    DWORD dw = MAX_PATH;  
  
    GetUserName(UserName, &dw);  
    printf("�û���: %s\n", UserName);  
  
    CHAR SystemDir[MAX_PATH];  
    CHAR WindowDir[MAX_PATH];  
  
    GetSystemDirectory(SystemDir, MAX_PATH);  
    GetWindowsDirectory(WindowDir, MAX_PATH);  
  
    printf("ϵͳĿ¼:%s\nWindowsĿ¼:%s", SystemDir, WindowDir);  
  
}  
  
  
VOID SystemInfo()  
{  
    SYSTEM_INFO SysInfo;  
    GetSystemInfo(&SysInfo);  
  
    printf("\n�ڴ��ҳ��С: 0x%.8X", SysInfo.dwPageSize);  
    printf("\n�����ڴ���ʼ: 0x%.8x", SysInfo.lpMinimumApplicationAddress);  
    printf("\n�����ڴ����: 0x%.8x", SysInfo.lpMaximumApplicationAddress);  
    printf("\nCPU����: %d", SysInfo.dwNumberOfProcessors);  
  
    printf("\nCPU����: ");  
    switch (SysInfo.dwProcessorType)  
    {  
    case PROCESSOR_INTEL_386:  
        printf("386\n");  
        break;  
    case PROCESSOR_INTEL_486:  
        printf("486\n");  
        break;  
    case PROCESSOR_INTEL_PENTIUM:  
        printf("pentium, Cpu Model 0x%.2X, Stepping 0x%.2X\n",(BYTE)(SysInfo.wProcessorRevision>>8), (BYTE)SysInfo.wProcessorRevision);  
        break;    
    case PROCESSOR_INTEL_IA64:  
        printf("IA64\n");  
        break;  
    case PROCESSOR_AMD_X8664:  
        printf("AMD x86 64\n");  
        break;  
    }  
  
    printf("�������ܹ�: ");  
    switch (SysInfo.wProcessorArchitecture)  
    {  
    case PROCESSOR_ARCHITECTURE_INTEL:  
        printf("intel");  
        printf(" CPU vendor is %d\n",SysInfo.wProcessorLevel);  
        break;  
    case PROCESSOR_ARCHITECTURE_IA64:  
        printf("64 bits intel\n");  
        break;  
    case PROCESSOR_ARCHITECTURE_AMD64:  
        printf("64 bits AMD\n");  
        break;  
    case PROCESSOR_ARCHITECTURE_UNKNOWN:  
        printf("UNKNOWN\n");  
        break;  
    }  
  
}  
  
  
VOID GetTime()  
{  
    SYSTEMTIME st;  
    // ��ȡʱ��  
    GetLocalTime( &st );  
    printf("����:  %d-%d-%d\n", st.wYear, st.wMonth, st.wDay);  
    printf("ʱ��:  %d:%d:%d\n", st.wHour, st.wMinute, st.wSecond);  
    printf("�ѿ���: %d����", GetTickCount()/1000/60);  
      
}  
  
  
VOID OS_Version(CHAR *szVersionInfo)  
{  
    OSVERSIONINFO OS;  
  
    // ���ò�����С�����ò��ж��Ƿ�ɹ�  
    OS.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
    if(!GetVersionEx(&OS))  
    {  
        printf("error %d\n",GetLastError());  
        return;  
    }  
    switch(OS.dwMajorVersion)  
    {  
    case 5:  
            switch(OS.dwMinorVersion)  
            {  
            case 0:  
                lstrcat(szVersionInfo,"2000");  
                break;  
            case 1:  
                lstrcat(szVersionInfo,"XP");  
                break;  
            case 2:  
                lstrcat(szVersionInfo,"Server 2003");  
                break;  
            }  
        break;  
  
    case 6:  
            switch(OS.dwMinorVersion)  
            {  
            case 0:  
                lstrcat(szVersionInfo,"Vista");  
                break;  
            case 1:  
                lstrcat(szVersionInfo,"7");  
                break;  
            }  
        break;  
    }  
}  
  
  
VOID SpaceSize(CHAR  *RootPathName)  
{  
      
    DWORD SectorsPerCluster;  
    DWORD BytesPerSector;  
    DWORD NumberOfFreeClusters;  
    DWORD TotalNumberOfClusters;      
  
  
    GetDiskFreeSpace(  
        RootPathName,             
        &SectorsPerCluster,  
        &BytesPerSector,  
        &NumberOfFreeClusters,  
        &TotalNumberOfClusters    
        );  
//  printf("%d, %d, %d\n",  NumberOfFreeClusters, BytesPerSector, SectorsPerCluster);  
    DWORD64 FreeSpaceSize  = (DWORD64)NumberOfFreeClusters * (DWORD64)BytesPerSector * (DWORD64)SectorsPerCluster;  
    if(FreeSpaceSize == 0)  
    {  
        return;  
    }  
//  printf("%I64d\n", FreeSpaceSize);  
    DWORD GB = FreeSpaceSize / ((DWORD64)1024 * (DWORD64)1024 * (DWORD64)1024);  
    FreeSpaceSize -= GB * ((DWORD64)1024 * (DWORD64)1024 * (DWORD64)1024);  
    DWORD MB = FreeSpaceSize / ((DWORD64)1024 * (DWORD64)1024);  
    FreeSpaceSize -= MB * ((DWORD64)1024 * (DWORD64)1024);  
    DWORD KB = FreeSpaceSize / 1024;  
    FreeSpaceSize -= KB * 1024;  
    DWORD B = FreeSpaceSize;  
    printf("%s ���ÿռ�: %d GB  %d MB  %d KB  %d B\n", RootPathName, GB, MB, KB, B);  
      
  
    DWORD64 TotalSpaceSize = (DWORD64)TotalNumberOfClusters * (DWORD64)BytesPerSector * (DWORD64)SectorsPerCluster;  
//  printf("%I64d\n", TotalSpaceSize);  
    GB = TotalSpaceSize / ((DWORD64)1024 * (DWORD64)1024 * (DWORD64)1024);  
    TotalSpaceSize -= GB * ((DWORD64)1024 * (DWORD64)1024 * (DWORD64)1024);  
    MB = TotalSpaceSize / ((DWORD64)1024 * (DWORD64)1024);  
    TotalSpaceSize -= MB * ((DWORD64)1024 * (DWORD64)1024);  
    KB = TotalSpaceSize / 1024;  
    TotalSpaceSize -= KB * 1024;  
    B = TotalSpaceSize;  
    printf("%s �ܿռ�:   %d GB  %d MB  %d KB  %d B\n", RootPathName, GB, MB, KB, B);  
}  
  
VOID PrintSpaceSize()  
{  
      
    CHAR DriverName[3] = "::";  
  
  
    for(      
        DWORD dwDriver = GetLogicalDrives(), dw = 1;  
        dwDriver != 0;   
        dwDriver /= 2, ++dw  
        )  
    {  
        if(dwDriver % 2 == 1)  
        {  
            switch(dw)  
            {  
        /*  case 1: 
                DriverName[0] = 'A';    //��WIN7����,XPʱ,��ʹSpacdSize()�е�GetDiskFreeSpace()������Ϊһ��û��A�� 
                SpaceSize(DriverName);  //��A:�����ж�,XP��Ĭ��A����3.5Ӣ������,GetLastError()����21Ϊ�豸δ���� 
                break;*/  
            case 2:  
                DriverName[0] = 'B';  
                SpaceSize(DriverName);  
                break;  
            case 3:  
                DriverName[0] = 'C';  
                SpaceSize(DriverName);  
                break;  
            case 4:  
                DriverName[0] = 'D';  
                SpaceSize(DriverName);  
                break;  
            case 5:  
                DriverName[0] = 'E';  
                SpaceSize(DriverName);  
                break;  
            case 6:  
                DriverName[0] = 'F';  
                SpaceSize(DriverName);  
                break;  
            case 7:  
                DriverName[0] = 'G';  
                SpaceSize(DriverName);  
                break;  
            case 8:  
                DriverName[0] = 'H';  
                SpaceSize(DriverName);  
                break;  
            case 9:  
                DriverName[0] = 'I';  
                SpaceSize(DriverName);  
                break;  
            case 10:  
                DriverName[0] = 'J';  
                SpaceSize(DriverName);  
                break;  
            }  
          
        }  
    }     
}  
  
  
VOID AllSystemInfo()  
{  
    CHAR OS[24] =   "Windows ";  
    OS_Version(OS);  
    printf("ϵͳ�汾: %s\n",OS);      
    GetSystemDir();  
    SystemInfo();  
    PrintSpaceSize();  
    GetTime();  
}  
  
  
DWORD DeleteDirFile(LPSTR szPath)  
{  
    CHAR szFilePath[MAX_PATH];  
  
    WIN32_FIND_DATA FindFileData;  
    HANDLE hListFile;  
    CHAR szFullPath[MAX_PATH];  
  
    //��Ŀ¼���ļ���·�����ַ�������ͨ�����*��  
    lstrcpy(szFilePath, szPath);  
    lstrcat(szFilePath, "\\*");  
  
    //��һ���ļ���Ŀ¼����þ��  
    hListFile = FindFirstFile(szFilePath,&FindFileData);  
  
    if(hListFile==INVALID_HANDLE_VALUE)  
    {  
        //���� ����  
        switch(GetLastError())  
        {  
        case 3:  
            printf("\n·������\n");  
            break;  
        case 5:  
            printf("\n�ļ��ܾ�����\n");  
            break;  
        case 21:  
            printf("\n�豸δ����\n");  
            break;  
        case 111:  
            printf("\n�ļ���̫��\n");  
            break;  
        default:  
            printf("\n�����ţ�%d\n", GetLastError());  
        }  
        return 1;  
    }  
    else  
    {         
        do  
        {  
            //��.���͡�..��������  
            if(lstrcmp(FindFileData.cFileName,TEXT("."))==0||  
                lstrcmp(FindFileData.cFileName,TEXT(".."))==0)  
            {  
                continue;  
            }  
              
            wsprintf(szFullPath,"%s\\%s",  
                szPath,FindFileData.cFileName);  
  
            printf("\nɾ��\t%s\t",szFullPath);  
  
            //Ŀ¼ �ͽ�ȥ  
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
            {  
                printf("---------------------Ŀ¼");  
                  
                DeleteDirFile(szFullPath);  
    //          RemoveDirectory(szFullPath);  
            }     
            else  
            {  
                DeleteFile(szFullPath);  
                dwDeleteFile++;  
            }  
        }  
        while(FindNextFile(hListFile, &FindFileData));  
    }  
    return 0;  
}  
  
  
  
  
VOID TimeDifference(SYSTEMTIME StOld, SYSTEMTIME StNow)  
{  
    WORD wMinute;  
    WORD wSecond;  
    WORD wMilliseconds;  
      
  
    if(StNow.wMilliseconds < StOld.wMilliseconds)  
    {  
        StNow.wSecond--;  
        StNow.wMilliseconds += 1000;  
        wMilliseconds = StNow.wMilliseconds - StOld.wMilliseconds;  
    }  
    else  
    {  
        wMilliseconds = StNow.wMilliseconds - StOld.wMilliseconds;  
    }  
  
    if(StNow.wSecond < StOld.wSecond)  
    {  
        StNow.wMinute--;  
        StNow.wSecond += 60;  
        wSecond = StNow.wSecond - StOld.wSecond;  
    }  
    else  
    {  
        wSecond = StNow.wSecond - StOld.wSecond;  
    }  
  
    if(StNow.wMinute < StOld.wMinute)  
    {         
        StNow.wMinute += 60;  
        wMinute = StNow.wMinute - StOld.wMinute;  
    }  
    else  
    {  
        wMinute = StNow.wMinute - StOld.wMinute;  
    }  
  
//  printf("\n��ʱ %d�� %d�� %d΢��\n", wMinute, wSecond, wMilliseconds);  
    printf("\n��ʱ");  
    if(wMinute != 0)  
    {  
        printf(" %d��", wMinute);  
    }  
    if(wSecond != 0)  
    {  
        printf(" %d��", wSecond);  
    }  
    if(wMilliseconds != 0)  
    {  
        printf(" %d΢��\n", wMilliseconds);  
    }  
}  
  
DWORD DeleteFileSuffix(LPSTR szPath, LPSTR Suffix)  
{  
    CHAR szFilePath[MAX_PATH];  
  
    WIN32_FIND_DATA FindFileData;  
    HANDLE hListFile;  
    CHAR szFullPath[MAX_PATH];  
  
    //��Ŀ¼���ļ���·�����ַ�������ͨ�����*��  
    lstrcpy(szFilePath, szPath);  
    lstrcat(szFilePath, "\\*");  
  
    //��һ���ļ���Ŀ¼����þ��  
    hListFile = FindFirstFile(szFilePath, &FindFileData);  
  
    if(hListFile == INVALID_HANDLE_VALUE)  
    {     
        //������  
        switch(GetLastError())  
        {  
        case 3:  
            printf("\n·������\n");  
            break;  
        case 5:  
            printf("\n�ļ��ܾ�����\n");  
            break;  
        case 21:  
            printf("\n�豸δ����\n");  
            break;  
        case 111:  
            printf("\n�ļ���̫��\n");  
            break;  
        default:  
            printf("\n�����ţ�%d\n", GetLastError());  
        }  
        return 1;  
    }  
    else  
    {         
        do  
        {  
            //��.���͡�..��������  
            if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 || lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)  
            {  
                continue;  
            }  
          
            wsprintf(szFullPath, "%s\\%s", szPath, FindFileData.cFileName);//·��  
  
      
  
            //�����Ŀ¼�������  
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
            {  
                DeleteFileSuffix(szFullPath, Suffix);  
            }  
            else  
            {  
                if(  
                    szFullPath[strlen(szFullPath) - 1] == Suffix[2]   
                                        &&  
                    szFullPath[strlen(szFullPath) - 2] == Suffix[1]  
                                        &&  
                    szFullPath[strlen(szFullPath) - 3] == Suffix[0]   
                  )  
                  {  
                     ++dwDeleteFile;  
                     printf("\nɾ��\t%s", szFullPath);  
                     DeleteFile(szFullPath);  
                  }  
            }  
        }  
        while(FindNextFile(hListFile, &FindFileData));  
    }  
    return 0;  
}  
          
  
VOID DeleteRubbish()  
{  
  
    SYSTEMTIME StOld;  
    SYSTEMTIME StNow;  
  
    //�����ļ���׺  
    CHAR TmpSuffix[4] = "tmp";  
    CHAR LogSuffix[4] = "log";  
    CHAR GidSuffix[4] = "gid";  
    CHAR ChkSuffix[4] = "chk";  
    CHAR OldSuffix[4] = "old";  
    CHAR BakSuffix[4] = "bak";  
  
    //ͳ��ɾ�������ļ���  
    DWORD dwPrefetch = 0;  
    DWORD dwTmp = 0;  
    DWORD dwLog = 0;  
    DWORD dwGid = 0;  
    DWORD dwChk = 0;  
    DWORD dwOld = 0;  
    DWORD dwBak = 0;  
  
    //λ��  
    //CHAR DeletePrefetch[MAX_PATH] = "C:\\Windows\\prefetch";  
    //                              //"D:\\a\\a";  
    //CHAR DeleteTemp[MAX_PATH] = "C:\\Windows\\Temp";  
    //                              //"D:\\a\\b";  
    //CHAR DeleteTmp[MAX_PATH] = "C:";  
    //                       //"D:\\a\\c";    
    //CHAR DeleteLog[MAX_PATH] = "C:";  
    //                       //"D:\\a\\c";  
    //CHAR DeleteGid[MAX_PATH] = "C:";  
    //                      //"D:\\a\\c";  
    //CHAR DeleteChk[MAX_PATH] = "C:";  
    //                       //"D:\\a\\c";  
    //CHAR DeleteOld[MAX_PATH] = "C:";  
    //                       //"D:\\a\\c";  
    //CHAR DeleteBak[MAX_PATH] = "C:\\Windows";  
    //                       //"D:\\a\\c";  
  
    CHAR DeletePrefetch[MAX_PATH];  
    GetWindowsDirectory(DeletePrefetch, MAX_PATH);  
    strcat(DeletePrefetch, "\\Prefetch");  
  
    CHAR DeleteTemp[MAX_PATH];  
    GetWindowsDirectory(DeleteTemp, MAX_PATH);  
    strcat(DeleteTemp, "\\Temp");  
  
    CHAR DeleteTmp[MAX_PATH];  
    GetWindowsDirectory(DeleteTmp, MAX_PATH);  
    DeleteTmp[3] = '\0';  
  
    CHAR DeleteLog[MAX_PATH];  
    GetWindowsDirectory(DeleteLog, MAX_PATH);  
    DeleteLog[3] = '\0';  
  
    CHAR DeleteGid[MAX_PATH];  
    GetWindowsDirectory(DeleteGid, MAX_PATH);  
    DeleteGid[3] = '\0';  
  
    CHAR DeleteChk[MAX_PATH];  
    GetWindowsDirectory(DeleteChk, MAX_PATH);  
    DeleteChk[3] = '\0';  
  
    CHAR DeleteOld[MAX_PATH];  
    GetWindowsDirectory(DeleteOld, MAX_PATH);  
    DeleteOld[3] = '\0';  
  
    CHAR DeleteBak[MAX_PATH];  
    GetWindowsDirectory(DeleteBak, MAX_PATH);  
  
  
    CHAR RootPathName[MAX_PATH];  
    GetWindowsDirectory(RootPathName, MAX_PATH);  
    RootPathName[2] = '\0';  
  
    DWORD SectorsPerCluster;  
    DWORD BytesPerSector;  
    DWORD NumberOfFreeClusters;  
    DWORD TotalNumberOfClusters;  
    GetDiskFreeSpace(  
            RootPathName,  
            &SectorsPerCluster,  
            &BytesPerSector,  
            &NumberOfFreeClusters,  
            &TotalNumberOfClusters  
        );  
  
    DWORD64 OldFreeSpaceSize = (DWORD64)NumberOfFreeClusters * (DWORD64)BytesPerSector * (DWORD64)SectorsPerCluster;  
  
    GetLocalTime(&StOld);  
  
  
    printf("Ԥ��ȡ�ļ�\tɾ����");  
    DeleteDirFile(DeletePrefetch);  
    dwPrefetch = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n��ʱ�ļ�\tɾ����");  
    DeleteFileSuffix(DeleteTmp, TmpSuffix);  
    DeleteDirFile(DeleteTemp);  
    dwTmp += dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n��־�ļ�\tɾ����");  
    DeleteFileSuffix(DeleteLog, LogSuffix);  
    dwLog = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n�����ļ�\tɾ����");  
    DeleteFileSuffix(DeleteGid, GidSuffix);  
    dwGid = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n�ļ���Ƭ\tɾ����");  
    DeleteFileSuffix(DeleteChk, ChkSuffix);  
    dwChk = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n�����ļ�\tɾ����");  
    DeleteFileSuffix(DeleteOld, OldSuffix);  
    dwOld = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n�����ļ�\tɾ����");  
    DeleteFileSuffix(DeleteBak, BakSuffix);  
    dwBak = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    GetLocalTime(&StNow);  
  
    GetDiskFreeSpace(  
        RootPathName,  
        &SectorsPerCluster,  
        &BytesPerSector,  
        &NumberOfFreeClusters,  
        &TotalNumberOfClusters  
    );  
  
    DWORD64 NowFreeSpaceSize = (DWORD64)NumberOfFreeClusters * (DWORD64)BytesPerSector * (DWORD64)SectorsPerCluster;  
    DWORD64 DeleteSpaceSize = NowFreeSpaceSize - OldFreeSpaceSize;  
  
    DWORD GB = DeleteSpaceSize / ((DWORD64)1024 * (DWORD64)1024 * (DWORD64)1024);  
    DeleteSpaceSize -= GB * ((DWORD64)1024 * (DWORD64)1024 * (DWORD64)1024);  
  
    DWORD MB = DeleteSpaceSize / ((DWORD64)1024 * (DWORD64)1024);  
    DeleteSpaceSize -= MB * ((DWORD64)1024 * (DWORD64)1024);  
  
    DWORD KB = DeleteSpaceSize / 1024;  
    DeleteSpaceSize -= KB * 1024;  
  
    DWORD B = DeleteSpaceSize;  
  
    printf("\n���\n");  
    printf("ɾ�� Ԥ��ȡ�ļ�\t%d\n", dwPrefetch);  
    printf("ɾ�� ��ʱ�ļ�\t%d\n", dwTmp);  
    printf("ɾ�� ��־�ļ�\t%d\n", dwLog);  
    printf("ɾ�� �����ļ�\t%d\n", dwGid);  
    printf("ɾ�� �ļ���Ƭ\t%d\n", dwChk);  
    printf("ɾ�� �����ļ�\t%d\n", dwOld);  
    printf("ɾ�� �����ļ�\t%d\n", dwBak);  
//  printf("ɾ��  %d GB  %d MB  %d KB  %d B �������ļ�", GB, MB, KB, B);  
  
    if(GB == 0 && MB == 0 && KB == 0 && B == 0)  
    {  
        printf("����ϵͳ�ǳ��ɾ�\n");  
        return;  
    }  
  
    printf("ɾ��");  
    if(GB != 0)  
    {     
        if(GB < 0)  
        {  
            GB = -GB;  
        }  
        printf("  %d GB", GB);  
    }  
    if(MB != 0)  
    {  
        printf("  %d MB", MB);  
    }  
    if(KB != 0)  
    {  
        printf("  %d KB", KB);  
    }  
    if(B != 0)  
    {  
        printf("  %d B", B);  
    }  
    printf(" �������ļ�");  
    TimeDifference(StOld, StNow);  
  
}  
  
DWORD FindDirFile(LPSTR szPath)  
{  
    CHAR szFilePath[MAX_PATH];  
  
    WIN32_FIND_DATA FindFileData;  
    HANDLE hListFile;  
    CHAR szFullPath[MAX_PATH];  
  
    //��Ŀ¼���ļ���·�����ַ�������ͨ�����*��  
    lstrcpy(szFilePath, szPath);  
    lstrcat(szFilePath, "\\*");  
  
    //��һ���ļ���Ŀ¼����þ��  
    hListFile = FindFirstFile(szFilePath, &FindFileData);  
  
    if(hListFile == INVALID_HANDLE_VALUE)  
    {  
        //������  
        switch(GetLastError())  
        {  
        case 3:  
            printf("\n·������\n");  
            break;  
        case 5:  
            printf("\n�ļ��ܾ�����\n");  
            break;  
        case 21:  
            printf("\n�豸δ����\n");  
            break;  
        case 111:  
            printf("\n�ļ���̫��\n");  
            break;  
        default:  
            printf("\n�����ţ�%d\n", GetLastError());  
        }  
        return 1;  
    }  
    else  
    {         
        do  
        {  
            //��.���͡�..��������  
            if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 || lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)  
            {  
                continue;  
            }  
          
            wsprintf(szFullPath, "%s\\%s", szPath, FindFileData.cFileName);//·��  
  
            Total++;  
            dwFile++;  
              
  
            printf("\n%d\t%s", Total, szFullPath);  
  
            //�����Ŀ¼�������  
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
            {  
                printf("--------------------------------Ŀ¼");  
                dwFolder++;  
                dwFile--;  
                FindDirFile(szFullPath);  
            }             
        }  
        while(FindNextFile(hListFile, &FindFileData));  
    //  while(0);//�ҵ�����վ  
    }  
    return 0;  
}  
  
//ת����ʽ��\�����ڼӸ�\��eg����\������\\��  
VOID Conversion(CHAR* c)  
{  
    for(int i = 0; i < strlen(c)+1; ++i)  
    {  
  
        if(c[i] == '\\')  
        {         
            for(int j = strlen(c)+1; j >= i ; --j)  
            {  
                c[j+1] = c[j];   
            }  
  
            c[strlen(c)+1] = '\0';  
            i += 2;  
        }  
    }  
}  
  
VOID StatisticsFile(VOID)  
{     
          
        SYSTEMTIME StOld;  
        SYSTEMTIME StNow;  
  
        CHAR szCurrentPath[MAX_PATH];  
        printf("�����̷����ļ���·��(���� D: ���� D:\\360safe )\n·��:");  
        scanf("%s", szCurrentPath);  
        Conversion(szCurrentPath);  
  
        GetLocalTime(&StOld);  
        FindDirFile(szCurrentPath);  
        GetLocalTime(&StNow);  
  
      
        printf("\n�ļ�����%d:\t", dwFile);  
        printf("�ļ�������%d:", dwFolder);  
        TimeDifference(StOld, StNow);  
}  
  
  
//VOID GetUserNameDocuments(LPSTR c)  
//{  
//  CHAR Document[30] = "\\Documents\\Tencent Files";  
//  CHAR UserName[50];  
//  DWORD Size = MAX_PATH;  
//  GetUserName(UserName, &Size);  
////    printf("%s\n", UserName);  
//  strcat(c, UserName);    
//    strcat(c, Document);    
//}  
  
  
VOID QQClean()  
{  
    SYSTEMTIME StOld;  
    SYSTEMTIME StNow;  
      
    printf("QQ����");  
        CHAR DeleteQQ[MAX_PATH];  
    GetWindowsDirectory(DeleteQQ, MAX_PATH);  
    DeleteQQ[3] = '\0';  
  
    //printf("%s\n", DeleteQQ);  
      
    OSVERSIONINFO OS;  
    OS.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
    GetVersionEx(&OS);  
  
    switch(OS.dwMajorVersion)  
    {  
    //XP  
    case 5:  
        strcat(DeleteQQ, "Documents and Settings\\");  
        break;  
  
    //Win7  
    case 6:  
        strcat(DeleteQQ, "Users\\");  
        break;  
    }  
  
    DWORD Size = MAX_PATH;  
    CHAR  UserName[20];  
    GetUserName(UserName, &Size);  
  
    strcat(DeleteQQ, UserName);  
    switch(OS.dwMajorVersion)  
    {  
    //XP  
    case 5:  
        strcat(DeleteQQ, "\\My Documents\\Tencent Files");  
        break;  
  
    //Win7  
    case 6:  
        strcat(DeleteQQ, "\\Documents\\Tencent Files");  
        break;  
    }  
  
    GetLocalTime(&StOld);  
    DeleteDirFile(DeleteQQ);  
    GetLocalTime(&StNow);  
  
//  printf("\n��ɾ�� %d QQ�ļ�\n\n", dwDeleteFile);  
    printf("\n��ԭ���\n");  
    TimeDifference(StOld, StNow);  
  
}  
  
VOID Clear0(DWORD *Total, DWORD *dwFile, DWORD *dwFolder, DWORD *dwDeleteFile)  
{  
    *Total = 0;  
    *dwFile = 0;  
    *dwFolder = 0;        
    *dwDeleteFile = 0;  
}  
  
VOID DeleteTrace()  
{  
    SYSTEMTIME StOld;  
    SYSTEMTIME StNow;  
      
    CHAR DeleteCookies[MAX_PATH];  
    CHAR DeleteHistory[MAX_PATH];  
    CHAR UserName[MAX_PATH];  
  
    DWORD Size = MAX_PATH;  
    GetUserName(UserName, &Size);  
  
    printf("����ۼ�\n");  
    GetLocalTime(&StOld);  
  
    GetWindowsDirectory(DeleteCookies, MAX_PATH);  
    DeleteCookies[3] = '\0';  
      
    GetWindowsDirectory(DeleteHistory, MAX_PATH);  
    DeleteHistory[3] = '\0';  
  
    OSVERSIONINFO OS;  
    OS.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
    GetVersionEx(&OS);  
  
    switch(OS.dwMajorVersion)  
    {  
    //XP  
    case 5:  
        strcat(DeleteCookies, "Documents and Settings\\");  
        strcat(DeleteHistory, "Documents and Settings\\");  
  
        strcat(DeleteCookies, UserName);  
        strcat(DeleteHistory, UserName);  
  
        strcat(DeleteCookies, "\\Local Settings\\Temporary Internet Files");  
        strcat(DeleteHistory, "\\Local Settings\\History\\History.IE5");  
  
        DeleteDirFile(DeleteCookies);  
        DeleteDirFile(DeleteHistory);  
        break;  
  
    //Win7  
    case 6:  
        strcat(DeleteCookies, "Users\\");  
  
        strcat(DeleteCookies, UserName);  
  
        strcat(DeleteCookies, "\\AppData\\Roaming\\Microsoft\\Windows\\Cookies");  
  
        DeleteDirFile(DeleteCookies);  
        break;  
    }  
      
    GetLocalTime(&StNow);  
  
    TimeDifference(StOld, StNow);  
  
}  
  
  
int main()  
{     
    while(1)  
    {  
        Show();  
        int i;  
        scanf("%d", &i);  
        switch(i)  
        {  
        case 1:  
            AllSystemInfo();  
            Clear0(&Total, &dwFile, &dwFolder, &dwDeleteFile);  
            break;  
  
        case 2:  
            DeleteRubbish();          
            Clear0(&Total, &dwFile, &dwFolder, &dwDeleteFile);  
            break;  
  
        case 3:  
            QQClean();  
            Clear0(&Total, &dwFile, &dwFolder, &dwDeleteFile);  
            break;  
  
        case 4:  
            StatisticsFile();  
            Clear0(&Total, &dwFile, &dwFolder, &dwDeleteFile);     
            break;  
  
        case 5:  
            DeleteTrace();  
            Clear0(&Total, &dwFile, &dwFolder, &dwDeleteFile);    
            break;  
  
        default:  
            return 0;  
        }     
    }  
}  