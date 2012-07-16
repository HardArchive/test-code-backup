#ifndef _RAINSOFT_TOOL_H_
#define _RAINSOFT_TOOL_H_

typedef struct tagFileInfo
{
	int iNum;             //�ڼ�·�绰
	char PhoneNum[16];    //�ֻ�����
	char Pwd[16];         //����
	unsigned short ErrTimes; //�������
	tagFileInfo()
	{
		::memset(this,0,sizeof(tagFileInfo));
	}
}FileInfo;

class CTool
{
public:
	static void GetRegConfig(LPCTSTR pKey,char *pValue);  //��ȡע���ֵ
	static void SetRegConfig(LPCTSTR pKey,char *pValue);  //����ע���ֵ
	static int GetNum(const char *pszFileName);
	static void Get400RegConfig(LPCTSTR pKey,char *pValue);
	static bool SaveResult(int iFileNum, const char *pFileContent); //���洦������������ϵͳ
};
#endif