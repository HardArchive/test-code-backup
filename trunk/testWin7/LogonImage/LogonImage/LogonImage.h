#include "stdafx.h"

extern "C" __declspec(dllexport) bool LogonImage(const WCHAR* pwszImagePath); 
//�滻��ǰϵͳ�Ŀ������棬ֻ�轫·�����뼴�ɣ��˺������Լ��ж�ϵͳ�汾��ͼƬ�Ƿ����Ҫ��

extern "C" __declspec(dllexport) bool UnLogonImage();    //�Ƴ����������޸�,�ɹ�true��ʧ��false

extern "C" __declspec(dllexport) int GetOSVer();
/******************************************************************************* 
1�� �������ƣ� GetOSVer
2�� ���������� ��ȡ����ϵͳ�İ汾��
3�� ��������� ��
4�� �� �� ֵ�� ʧ�ܷ��� 0�� xp ���� 1�� windows 2003 ���� 2 ��windows 2003 ���� 3��vista and 2008 ���� 4��2008 R2 and Windows 7 ����  5��
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-8-30 14:15:56
8�� ��    ע�� 
********************************************************************************/ 
extern "C" __declspec(dllexport) bool CreateMultiDir(const WCHAR* pwInDir);
//����pwInDir�����༶Ŀ¼