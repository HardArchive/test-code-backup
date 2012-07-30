#pragma once

#include <vector>
using namespace std;
#include <Windows.h>

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
//获取反查域名
extern "C" __declspec(dllexport) bool GetLookupDomainA(vector<string>& strVecDomain);
