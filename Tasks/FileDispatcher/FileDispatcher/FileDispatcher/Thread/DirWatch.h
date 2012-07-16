#pragma once

class CDirWatch
{
public:
	CDirWatch(void);
	CDirWatch(char* pWatchDirPath, char* pDestDirPath, char* pDestFilePath);
	~CDirWatch(void);

public:
	int TraverseDirectory(char* pszDirPath, bool bIsRoot);   //����Ŀ¼
	void StopTraverse();               //ֹͣ����
	void DeleteAll(bool bQuit);        //��ն�������δɾ�����ļ�
	// 
private:	
	int DealFile(char* pFileName);           //�����ļ�
	bool CheckDirType(char* pDirPath);        //����ļ������ͣ��ж��Ƿ��Ƿ���Ҫ����ļ���
	bool CheckFileIsHide(char* pFileName);    //�ж��ļ������Ƿ�Ϊ����
	bool CreatePath(char* pDirPath);          //�����༶Ŀ¼���粻�����򴴽� ʧ�ܷ���false����������true
	bool GetParentDirName(char* pInFileName, char* pOutParentDirName);  //�����ļ�����·����ȡ�丸Ŀ¼��
	//bool GetFileName(char* pInFileName, char* pOutFileName);  //���������ļ��� ����������·�����ļ���
	void DeleteDir(char* pDirPath);           //ɾ���ļ���
	long ClacTime(SYSTEMTIME stuSystemTime);  //����ʱ���뵱ǰʱ��֮�����������ʱ��֮�ȷ������
	// ��RemoveDirectory�ݴ�



private:	
	char m_szWatchDirPath[MAX_PATH];//���Ŀ¼·��	
	char m_szDestDirPath[MAX_PATH];   //Ŀ¼�ƶ�Ŀ��·��	
	char m_szDestFilePath[MAX_PATH]; //�ļ��ƶ�Ŀ¼·��
	bool m_bIsStoped;                //ֹͣ����
};
