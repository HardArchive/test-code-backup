/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   10:03
	file base:	CWindowIterator
	file ext:	h
	author:		zhangwf
	
	purpose:	��װö�ٴ�����
*********************************************************************/
#ifndef _CWINDOW_ITERATOR_ZWF_H_
#define _CWINDOW_ITERATOR_ZWF_H_
//////////////////////////////////////////////////////////////////////////
#include <vector>
using namespace std;
//////////////////////////////////////////////////////////////////////////
class CWindowIterator
{
public:
	// ������������
	CWindowIterator();
	~CWindowIterator();

public:
	// �ӿں���
	void EnumWnds();                        // ö�ٴ���
	unsigned long GetWndCounts();           // ��ô�������
	HWND First();                           // ��õ�һ�����ھ��
	HWND Next();                            // �����һ�����ھ��
	HWND GetHWnd(unsigned long wndIndex);   // ����������ô��ھ��		

protected:
	vector<HWND> m_hWndArray;               // ���ö�ٵõ��Ĵ��ھ��
	unsigned long m_nCurrentWndIndex;       // ��ǰ���ھ��

	// ö�ٴ��ڻص�����
	static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lp);

	// virtual enumerator
	virtual BOOL AddWnd(HWND hwnd);

	// override to filter different kinds of windows
	virtual BOOL IsTargetWnd(HWND hwnd) {return TRUE;}

};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// �������������ֻࣨ��ȡ���̵������ڣ�
class CMainHWnd : public CWindowIterator
{
public:
	// ������������
	CMainHWnd();
	CMainHWnd(unsigned long pid);
	~CMainHWnd();

public:
	// �ӿں���
	void EnumWndsByPid(DWORD pid);

protected:
	DWORD m_pid;
	virtual BOOL IsTargetWnd(HWND hwnd);

};
//////////////////////////////////////////////////////////////////////////
#endif