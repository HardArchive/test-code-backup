#pragma once

#include <vector>
using namespace std;
#include "QNA_File.h"
#include "QNA_DownLoad.h"
using namespace QNA;


//IP反向解析域名

class CIpLookup
{
public:
	CIpLookup(void);
	~CIpLookup(void);

public:
	//传入从配置文件获取的第三解析服务器网址和与其相关的正则表达式
	//void Init();
	void SetUrlRegex(LPCTSTR pstrUrl, LPCTSTR pstrRegex); //设置第三方解析服务器网址和与其相关的正则表达
	//传入IP 进行反向解析
	bool StartIpLookup(LPCTSTR pstrIP);      //开始反查

	//获取解析结果 从向量表中获取  每次取一个
	//获取当前路径 
	bool GetCurrentPath(LPTSTR pstrPath);
private:
	vector<wstring> m_strVecDomain;   //用于存放域名的字符串向量
	//vector<string>::iterator m_stringIter; //声明指向字符串的迭代器变量

	TCHAR m_tszUrl[MAX_PATH];       //第三方解析服务器网址
	TCHAR m_tszRegex[MAX_PATH];     //第三方解析服务器网址相关的正则表达式
	//TCHAR m_tszIP[64];              //需反查的IP
	//TCHAR m_tszLoadBuf[512*1024];  //网页文件读取Buffer 1MB

private:
	//bool Download(void);
	void LoadFile(std::wstring& wstrOut, std::wistream& wistreamIn);
	//bool GetHtmlBuffer();  //读取已经下载页面到内存
	bool ParseBuffer(LPCTSTR pstrPath);    //通过正则解析Buffer 并将结果回到向量
};
