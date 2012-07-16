#pragma once

class CDirWatch
{
public:
	CDirWatch(void);
	CDirWatch(char* pWatchDirPath, char* pDestDirPath, char* pDestFilePath);
	~CDirWatch(void);

public:
	int TraverseDirectory(char* pszDirPath, bool bIsRoot);   //遍历目录
	void StopTraverse();               //停止遍历
	void DeleteAll(bool bQuit);        //清空队列里面未删除的文件
	// 
private:	
	int DealFile(char* pFileName);           //处理文件
	bool CheckDirType(char* pDirPath);        //检查文件夹类型，判断是否是符合要求的文件夹
	bool CheckFileIsHide(char* pFileName);    //判断文件属性是否为隐藏
	bool CreatePath(char* pDirPath);          //创建多级目录，如不存在则创建 失败返回false，其他返回true
	bool GetParentDirName(char* pInFileName, char* pOutParentDirName);  //传入文件完整路径获取其父目录名
	//bool GetFileName(char* pInFileName, char* pOutFileName);  //传入完整文件名 传出不包括路径的文件名
	void DeleteDir(char* pDirPath);           //删除文件夹
	long ClacTime(SYSTEMTIME stuSystemTime);  //计算时间与当前时间之差，反回与现在时间之差精确到分钟
	// 用RemoveDirectory暂代



private:	
	char m_szWatchDirPath[MAX_PATH];//监控目录路径	
	char m_szDestDirPath[MAX_PATH];   //目录移动目标路径	
	char m_szDestFilePath[MAX_PATH]; //文件移动目录路径
	bool m_bIsStoped;                //停止遍历
};
