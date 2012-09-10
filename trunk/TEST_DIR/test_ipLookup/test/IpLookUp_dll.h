#pragma once

#include <vector>
using namespace std;

#include <Windows.h>

//IP反查导出函数
//宽字符
//获取当前库所在目录
extern "C" __declspec(dllexport) bool GetCurrentPathW(PWCHAR pwstrPath);
//设置第三方解析网址和其对应的正则表达式
extern "C" __declspec(dllexport) void SetUrlRegexW(PWCHAR pwstrUrl, PWCHAR pwstrRegex);
//开始反查
extern "C" __declspec(dllexport) bool StartIpLookupW(PWCHAR pwstrIP);
//获取反查域名
extern "C" __declspec(dllexport) bool GetLookupDomainW(vector<wstring>& wstrVecDomain);

//窄字符
//获取当前库所在目录
extern "C" __declspec(dllexport) bool GetCurrentPathA(PCHAR pstrPath);
//设置第三方解析网址和其对应的正则表达式
extern "C" __declspec(dllexport) void SetUrlRegexA(PCHAR pstrUrl, PCHAR pstrRegex);
//开始反查
extern "C" __declspec(dllexport) bool StartIpLookupA(PCHAR pstrIP);
//获取反查到的域名个数
extern "C" __declspec(dllexport) int GetLookupDomainTotalNum();
//获取域名 域名PCHAR pszOutDomain,域名序号int iNum 
//返回域名长度 -1输入参数pszOutDomain错误 -2 入参数iNum错误 -3转化出错
extern "C" __declspec(dllexport) int GetLookupDomainA(PCHAR pszOutDomain, int iNum);


