#pragma once

#include <vector>
using namespace std;

#include <Windows.h>

//IP���鵼������
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
//��ȡ���鵽����������
extern "C" __declspec(dllexport) int GetLookupDomainTotalNum();
//��ȡ���� ����PCHAR pszOutDomain,�������int iNum 
//������������ -1�������pszOutDomain���� -2 �����iNum���� -3ת������
extern "C" __declspec(dllexport) int GetLookupDomainA(PCHAR pszOutDomain, int iNum);


