#pragma once

#include <vector>
using namespace std;
#include <Windows.h>

//���ַ�
//��ȡ��ǰ������Ŀ¼
extern "C" __declspec(dllexport) bool GetCurrentPathW(PWCHAR pwstrPath);
//���õ�����������ַ�����Ӧ��������ʽ
extern "C" __declspec(dllexport) void SetUrlRegexW(PWCHAR pwstrUrl, PWCHAR pwstrRegex);
//��ʼ����
extern "C" __declspec(dllexport) bool StartIpLookupW(PWCHAR pwstrIP);
//��ȡ��������
extern "C" __declspec(dllexport) bool GetLookupDomainW(vector<wstring>& wstrVecDomain);

//խ�ַ�
//��ȡ��ǰ������Ŀ¼
extern "C" __declspec(dllexport) bool GetCurrentPathA(PCHAR pstrPath);
//���õ�����������ַ�����Ӧ��������ʽ
extern "C" __declspec(dllexport) void SetUrlRegexA(PCHAR pstrUrl, PCHAR pstrRegex);
//��ʼ����
extern "C" __declspec(dllexport) bool StartIpLookupA(PCHAR pstrIP);
//��ȡ��������
extern "C" __declspec(dllexport) bool GetLookupDomainA(vector<string>& strVecDomain);
