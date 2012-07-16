#ifndef _RAINSOFT_TOOL_H_
#define _RAINSOFT_TOOL_H_

typedef struct tagFileInfo
{
	int iNum;             //第几路电话
	char PhoneNum[16];    //手机号码
	char Pwd[16];         //密码
	unsigned short ErrTimes; //错误次数
	tagFileInfo()
	{
		::memset(this,0,sizeof(tagFileInfo));
	}
}FileInfo;

class CTool
{
public:
	static void GetRegConfig(LPCTSTR pKey,char *pValue);  //获取注册表值
	static void SetRegConfig(LPCTSTR pKey,char *pValue);  //设置注册表值
	static int GetNum(const char *pszFileName);
	static void Get400RegConfig(LPCTSTR pKey,char *pValue);
	static bool SaveResult(int iFileNum, const char *pFileContent); //保存处理结果，给语音系统
};
#endif