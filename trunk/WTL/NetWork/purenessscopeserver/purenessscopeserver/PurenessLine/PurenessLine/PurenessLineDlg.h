
// PurenessLineDlg.h : ͷ�ļ�
//

#pragma once

#include "ChartViewer.h"
#include <stdlib.h>
#include <time.h>
#include "afxwin.h"
#include <afxsock.h>

//����������ʱ��Ϣ
struct _ServerRunInfo
{
	int m_nCPU;        //��ǰCPUռ����
	int m_nMemorySize; //��ǰ�ڴ�ռ����

	_ServerRunInfo()
	{
		m_nCPU        = 0;
		m_nMemorySize = 0;
	}

	_ServerRunInfo& operator = (const _ServerRunInfo& ar)
	{
		this->m_nCPU        = ar.m_nCPU;
		this->m_nMemorySize = ar.m_nMemorySize;
		return *this;
	}
};

//����һ���������
inline void InitRandom()
{
	srand((int)time(NULL));
};

//��һ��ֵ���л�ȡһ�����ֵ
inline int RandomValue(int nMin, int nMax)
{
	return  nMin + (int) (nMax * (rand() / (RAND_MAX + 1.0)));
};

#define TIMER_ID       1        //��ʱ��ID
#define TIMER_INTERVAL 1000     //��ʱ�����
#define MAX_DATA_COUNT 25       //�����������

// CPurenessLineDlg �Ի���
class CPurenessLineDlg : public CDialog
{
// ����
public:
	CPurenessLineDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PURENESSLINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnViewPortChanged();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	CEdit m_txtServerIP;
	CEdit m_txtServerPort;
	afx_msg void OnBnClickedButton1();

private:
	void shiftData(double *data, int len, double newValue);
	void drawChart();
	void InitView();
	BOOL GetServerStateInfo(_ServerRunInfo& objServerRunInfo);

private:
	CChartViewer m_cvCPU;                     //CPUʹ����
	CChartViewer m_cvMemory;                  //�ڴ�ʹ����
	double       m_dbCPU[MAX_DATA_COUNT];
	double       m_dbMemory[MAX_DATA_COUNT];
	CSocket      m_sckServer;                 //����Զ��Server������
	bool         m_blState;                   //����״̬

};
