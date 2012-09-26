// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//cleanDos2011.5.31.3.cpp  
//win32控制台程序,界面做不出来  
//没有严格测试过        
  
#include <windows.h>  
#include <stdio.h>  
#include <string.h>  
  
  
DWORD Total = 0;         //文件和目录数量  
DWORD dwFile = 0;        //文件数量  
DWORD dwFolder = 0;      //所文件夹数量  
DWORD dwDeleteFile = 0;  //删除文件数量  
     
  
  //www.heatpress123.net
//公共  
VOID  TimeDifference(SYSTEMTIME StOld, SYSTEMTIME StNow);//时差  
VOID  Show(VOID);  
DWORD FindDirFile(LPSTR szPath);                   //遍历文件和目录  
DWORD DeleteFileSuffix(LPSTR szPath, LPSTR Suffix);//删除特定后缀文件       //这有时删的干净,有时删不干净，本来是想删文件夹的,  
DWORD DeleteDirFile(LPSTR szPath);                 //删除文件夹里的文件      //后面发现RemoveDirectory()只能删空文件夹，导致路径错误，就不要了.  
VOID  Clear0(DWORD *Total, DWORD *dwFile, DWORD *dwFolder, DWORD *dwDeleteFile);  
VOID  OS_Version(CHAR *szVersionInfo);  
  
//1  
VOID GetSystemDir(VOID); //获取系统位置  
VOID SystemInfo(VOID);   //获取系统信息  
VOID GetTime(VOID);      //获取时间  
VOID SpaceSize(CHAR  *RootPathName);  
VOID PrintSpaceSize();  
VOID AllSystemInfo(VOID);  
  
//2  
VOID DeleteRubbish(VOID);    //删除垃圾文件  
  
  
//3  
VOID QQClean(VOID);//Win7 / XP  
//VOID GetUserNameDocuments(LPSTR c);//获取用户名  
  
//4  
VOID StatisticsFile(VOID);//统计  
VOID Conversion(CHAR* c); //格式转化  
  
//5清理痕迹  
VOID DeleteTrace(VOID);  
  
VOID Show(VOID)  
{  
    printf("\n1: 系统信息\t");  
    printf("2：垃圾清理\t");  
    printf("3: QQ还原\t\n");  
    printf("4: 统计文件\t");  
    printf("5: 清理痕迹\t");  
    printf("其他: 退出\n\n");  
}  
  
  
VOID GetSystemDir()  
{     
    CHAR UserName[MAX_PATH];  
    DWORD dw = MAX_PATH;  
  
    GetUserName(UserName, &dw);  
    printf("用户名: %s\n", UserName);  
  
    CHAR SystemDir[MAX_PATH];  
    CHAR WindowDir[MAX_PATH];  
  
    GetSystemDirectory(SystemDir, MAX_PATH);  
    GetWindowsDirectory(WindowDir, MAX_PATH);  
  
    printf("系统目录:%s\nWindows目录:%s", SystemDir, WindowDir);  
  
}  
  
  
VOID SystemInfo()  
{  
    SYSTEM_INFO SysInfo;  
    GetSystemInfo(&SysInfo);  
  
    printf("\n内存分页大小: 0x%.8X", SysInfo.dwPageSize);  
    printf("\n可用内存起始: 0x%.8x", SysInfo.lpMinimumApplicationAddress);  
    printf("\n可用内存结束: 0x%.8x", SysInfo.lpMaximumApplicationAddress);  
    printf("\nCPU个数: %d", SysInfo.dwNumberOfProcessors);  
  
    printf("\nCPU类型: ");  
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
  
    printf("处理器架构: ");  
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
    // 获取时间  
    GetLocalTime( &st );  
    printf("日期:  %d-%d-%d\n", st.wYear, st.wMonth, st.wDay);  
    printf("时间:  %d:%d:%d\n", st.wHour, st.wMinute, st.wSecond);  
    printf("已开机: %d分钟", GetTickCount()/1000/60);  
      
}  
  
  
VOID OS_Version(CHAR *szVersionInfo)  
{  
    OSVERSIONINFO OS;  
  
    // 设置参数大小，调用并判断是否成功  
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
    printf("%s 可用空间: %d GB  %d MB  %d KB  %d B\n", RootPathName, GB, MB, KB, B);  
      
  
    DWORD64 TotalSpaceSize = (DWORD64)TotalNumberOfClusters * (DWORD64)BytesPerSector * (DWORD64)SectorsPerCluster;  
//  printf("%I64d\n", TotalSpaceSize);  
    GB = TotalSpaceSize / ((DWORD64)1024 * (DWORD64)1024 * (DWORD64)1024);  
    TotalSpaceSize -= GB * ((DWORD64)1024 * (DWORD64)1024 * (DWORD64)1024);  
    MB = TotalSpaceSize / ((DWORD64)1024 * (DWORD64)1024);  
    TotalSpaceSize -= MB * ((DWORD64)1024 * (DWORD64)1024);  
    KB = TotalSpaceSize / 1024;  
    TotalSpaceSize -= KB * 1024;  
    B = TotalSpaceSize;  
    printf("%s 总空间:   %d GB  %d MB  %d KB  %d B\n", RootPathName, GB, MB, KB, B);  
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
                DriverName[0] = 'A';    //在WIN7正常,XP时,会使SpacdSize()中的GetDiskFreeSpace()出错，因为一般没插A盘 
                SpaceSize(DriverName);  //“A:”会中断,XP下默认A盘是3.5英寸软盘,GetLastError()返回21为设备未就绪 
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
    printf("系统版本: %s\n",OS);      
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
  
    //子目录和文件夹路径的字符串，用通配符“*”  
    lstrcpy(szFilePath, szPath);  
    lstrcat(szFilePath, "\\*");  
  
    //第一个文件和目录，获得句柄  
    hListFile = FindFirstFile(szFilePath,&FindFileData);  
  
    if(hListFile==INVALID_HANDLE_VALUE)  
    {  
        //错误 处理  
        switch(GetLastError())  
        {  
        case 3:  
            printf("\n路径错误\n");  
            break;  
        case 5:  
            printf("\n文件拒绝访问\n");  
            break;  
        case 21:  
            printf("\n设备未就绪\n");  
            break;  
        case 111:  
            printf("\n文件名太长\n");  
            break;  
        default:  
            printf("\n错误编号：%d\n", GetLastError());  
        }  
        return 1;  
    }  
    else  
    {         
        do  
        {  
            //“.”和“..”不遍历  
            if(lstrcmp(FindFileData.cFileName,TEXT("."))==0||  
                lstrcmp(FindFileData.cFileName,TEXT(".."))==0)  
            {  
                continue;  
            }  
              
            wsprintf(szFullPath,"%s\\%s",  
                szPath,FindFileData.cFileName);  
  
            printf("\n删除\t%s\t",szFullPath);  
  
            //目录 就进去  
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
            {  
                printf("---------------------目录");  
                  
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
  
//  printf("\n花时 %d分 %d秒 %d微秒\n", wMinute, wSecond, wMilliseconds);  
    printf("\n花时");  
    if(wMinute != 0)  
    {  
        printf(" %d分", wMinute);  
    }  
    if(wSecond != 0)  
    {  
        printf(" %d秒", wSecond);  
    }  
    if(wMilliseconds != 0)  
    {  
        printf(" %d微秒\n", wMilliseconds);  
    }  
}  
  
DWORD DeleteFileSuffix(LPSTR szPath, LPSTR Suffix)  
{  
    CHAR szFilePath[MAX_PATH];  
  
    WIN32_FIND_DATA FindFileData;  
    HANDLE hListFile;  
    CHAR szFullPath[MAX_PATH];  
  
    //子目录和文件夹路径的字符串，用通配符“*”  
    lstrcpy(szFilePath, szPath);  
    lstrcat(szFilePath, "\\*");  
  
    //第一个文件和目录，获得句柄  
    hListFile = FindFirstFile(szFilePath, &FindFileData);  
  
    if(hListFile == INVALID_HANDLE_VALUE)  
    {     
        //错误处理  
        switch(GetLastError())  
        {  
        case 3:  
            printf("\n路径错误\n");  
            break;  
        case 5:  
            printf("\n文件拒绝访问\n");  
            break;  
        case 21:  
            printf("\n设备未就绪\n");  
            break;  
        case 111:  
            printf("\n文件名太长\n");  
            break;  
        default:  
            printf("\n错误编号：%d\n", GetLastError());  
        }  
        return 1;  
    }  
    else  
    {         
        do  
        {  
            //“.”和“..”不遍历  
            if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 || lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)  
            {  
                continue;  
            }  
          
            wsprintf(szFullPath, "%s\\%s", szPath, FindFileData.cFileName);//路径  
  
      
  
            //如果是目录，则进入  
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
                     printf("\n删除\t%s", szFullPath);  
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
  
    //垃圾文件后缀  
    CHAR TmpSuffix[4] = "tmp";  
    CHAR LogSuffix[4] = "log";  
    CHAR GidSuffix[4] = "gid";  
    CHAR ChkSuffix[4] = "chk";  
    CHAR OldSuffix[4] = "old";  
    CHAR BakSuffix[4] = "bak";  
  
    //统计删除垃圾文件数  
    DWORD dwPrefetch = 0;  
    DWORD dwTmp = 0;  
    DWORD dwLog = 0;  
    DWORD dwGid = 0;  
    DWORD dwChk = 0;  
    DWORD dwOld = 0;  
    DWORD dwBak = 0;  
  
    //位置  
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
  
  
    printf("预读取文件\t删除中");  
    DeleteDirFile(DeletePrefetch);  
    dwPrefetch = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n临时文件\t删除中");  
    DeleteFileSuffix(DeleteTmp, TmpSuffix);  
    DeleteDirFile(DeleteTemp);  
    dwTmp += dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n日志文件\t删除中");  
    DeleteFileSuffix(DeleteLog, LogSuffix);  
    dwLog = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n索引文件\t删除中");  
    DeleteFileSuffix(DeleteGid, GidSuffix);  
    dwGid = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n文件碎片\t删除中");  
    DeleteFileSuffix(DeleteChk, ChkSuffix);  
    dwChk = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n过期文件\t删除中");  
    DeleteFileSuffix(DeleteOld, OldSuffix);  
    dwOld = dwDeleteFile;  
    dwDeleteFile = 0;  
  
    printf("\n备份文件\t删除中");  
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
  
    printf("\n完毕\n");  
    printf("删除 预读取文件\t%d\n", dwPrefetch);  
    printf("删除 临时文件\t%d\n", dwTmp);  
    printf("删除 日志文件\t%d\n", dwLog);  
    printf("删除 索引文件\t%d\n", dwGid);  
    printf("删除 文件碎片\t%d\n", dwChk);  
    printf("删除 过期文件\t%d\n", dwOld);  
    printf("删除 备份文件\t%d\n", dwBak);  
//  printf("删除  %d GB  %d MB  %d KB  %d B 的垃圾文件", GB, MB, KB, B);  
  
    if(GB == 0 && MB == 0 && KB == 0 && B == 0)  
    {  
        printf("您的系统非常干净\n");  
        return;  
    }  
  
    printf("删除");  
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
    printf(" 的垃圾文件");  
    TimeDifference(StOld, StNow);  
  
}  
  
DWORD FindDirFile(LPSTR szPath)  
{  
    CHAR szFilePath[MAX_PATH];  
  
    WIN32_FIND_DATA FindFileData;  
    HANDLE hListFile;  
    CHAR szFullPath[MAX_PATH];  
  
    //子目录和文件夹路径的字符串，用通配符“*”  
    lstrcpy(szFilePath, szPath);  
    lstrcat(szFilePath, "\\*");  
  
    //第一个文件和目录，获得句柄  
    hListFile = FindFirstFile(szFilePath, &FindFileData);  
  
    if(hListFile == INVALID_HANDLE_VALUE)  
    {  
        //错误处理  
        switch(GetLastError())  
        {  
        case 3:  
            printf("\n路径错误\n");  
            break;  
        case 5:  
            printf("\n文件拒绝访问\n");  
            break;  
        case 21:  
            printf("\n设备未就绪\n");  
            break;  
        case 111:  
            printf("\n文件名太长\n");  
            break;  
        default:  
            printf("\n错误编号：%d\n", GetLastError());  
        }  
        return 1;  
    }  
    else  
    {         
        do  
        {  
            //“.”和“..”不遍历  
            if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 || lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)  
            {  
                continue;  
            }  
          
            wsprintf(szFullPath, "%s\\%s", szPath, FindFileData.cFileName);//路径  
  
            Total++;  
            dwFile++;  
              
  
            printf("\n%d\t%s", Total, szFullPath);  
  
            //如果是目录，则进入  
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
            {  
                printf("--------------------------------目录");  
                dwFolder++;  
                dwFile--;  
                FindDirFile(szFullPath);  
            }             
        }  
        while(FindNextFile(hListFile, &FindFileData));  
    //  while(0);//找到回收站  
    }  
    return 0;  
}  
  
//转化格式在\后面在加个\，eg：“\”→“\\”  
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
        printf("输入盘符或文件夹路径(例如 D: 或者 D:\\360safe )\n路径:");  
        scanf("%s", szCurrentPath);  
        Conversion(szCurrentPath);  
  
        GetLocalTime(&StOld);  
        FindDirFile(szCurrentPath);  
        GetLocalTime(&StNow);  
  
      
        printf("\n文件总数%d:\t", dwFile);  
        printf("文件夹总数%d:", dwFolder);  
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
      
    printf("QQ垃圾");  
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
  
//  printf("\n共删除 %d QQ文件\n\n", dwDeleteFile);  
    printf("\n还原完毕\n");  
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
  
    printf("清理痕迹\n");  
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