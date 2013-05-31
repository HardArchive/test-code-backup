
// PurenessLineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PurenessLine.h"
#include "PurenessLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPurenessLineDlg �Ի���




CPurenessLineDlg::CPurenessLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPurenessLineDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPurenessLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CPUVIEW, m_cvCPU);
	DDX_Control(pDX, IDC_MEMORYVIEW, m_cvMemory);
	DDX_Control(pDX, IDC_EDIT1, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT2, m_txtServerPort);
}

BEGIN_MESSAGE_MAP(CPurenessLineDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CPUVIEW, OnViewPortChanged)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CPurenessLineDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPurenessLineDlg ��Ϣ�������

BOOL CPurenessLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitView();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPurenessLineDlg::InitView()
{
	m_blState = FALSE;
	AfxSocketInit();

	//��ʼ���б�
	for(int i = 0; i < MAX_DATA_COUNT; i++)
	{
		m_dbCPU[i]    = 0.0;
		m_dbMemory[i] = 0.0;
	}

	//�����������
	InitRandom();

	//������ʱ��
	SetTimer(TIMER_ID, TIMER_INTERVAL, 0);

	//������Ӧ������ͼ��
	drawChart();
}

BOOL CPurenessLineDlg::GetServerStateInfo(_ServerRunInfo& objServerRunInfo)
{
	if(m_blState == FALSE)
	{
		return FALSE;
	}

	//ƴ�ӷ���Э��
	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "ShowCurrProcessInfo -a");
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//��������
	int nSend = m_sckServer.Send(szSendDate, nAllLen + 4);
	if(nSend != nAllLen + 4)
	{
		return FALSE;
	}

	//��������
	char szBuffRecv[200] = {'\0'};
	m_sckServer.Receive(szBuffRecv, 200);

	int nStrLen              = 0;
	int nPos                 = 4;
	int nClientCount         = 0;
	//�������߳�
	memcpy(&objServerRunInfo.m_nCPU, &szBuffRecv[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy(&objServerRunInfo.m_nMemorySize, &szBuffRecv[nPos], sizeof(int));
	nPos += sizeof(int);

	return TRUE;
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPurenessLineDlg::OnViewPortChanged()
{
	_ServerRunInfo objServerRunInfo;
	if(GetServerStateInfo(objServerRunInfo) == TRUE)
	{
		//��������
		double dbData = (double)RandomValue(1, 100);
		shiftData(m_dbCPU, MAX_DATA_COUNT, (double)objServerRunInfo.m_nCPU/100.0);

		dbData = (double)RandomValue(1, 100);
		shiftData(m_dbMemory, MAX_DATA_COUNT, (double)objServerRunInfo.m_nMemorySize/(1000*1000));

		//������Ӧ������ͼ��
		drawChart();
	}
	else
	{
		//��������
		double dbData = 0.0;
		shiftData(m_dbCPU, MAX_DATA_COUNT, (double)objServerRunInfo.m_nCPU/100.0);

		dbData = 0.0;
		shiftData(m_dbMemory, MAX_DATA_COUNT, (double)objServerRunInfo.m_nMemorySize/(1000*1000));

		//������Ӧ������ͼ��
		drawChart();
	}
}

void CPurenessLineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPurenessLineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPurenessLineDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

void CPurenessLineDlg::drawChart()
{
	const char* szLabels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23",
		"24"};

	XYChart* pXYCPU    = NULL;
	XYChart* pXYMemory = NULL;

	//����Ĭ�ϴ����С(CPU)
	pXYCPU = new XYChart(410, 220, 0xeeeeff, 0x000000, 1);
	pXYCPU->setRoundedFrame();

	//����X���±�
	pXYCPU->xAxis()->setLabels(StringArray(szLabels, (int)(sizeof(szLabels) / sizeof(szLabels[0]))));

	//����λ��
	pXYCPU->setPlotArea(30, 20, 360, 160, 0x000200, -1, -1, 0x337f59);

	//���һ���µ��߲�
	LineLayer* pLayer = pXYCPU->addLineLayer();

	if(NULL != pLayer)
	{
		pLayer->setLineWidth(2);
		pLayer->addDataSet(DoubleArray(m_dbCPU, (int)(sizeof(m_dbCPU) / sizeof(m_dbCPU[0]))), 0xff0000, "Server #1");
	}

	//���ƶ���Chart����󶨸�ָ���ؼ�
	m_cvCPU.setChart(pXYCPU);
	delete pXYCPU;

	//����Ĭ�ϴ����С(Memory)
	pXYMemory = new XYChart(410, 220, 0xeeeeff, 0x000000, 1);
	pXYMemory->setRoundedFrame();

	//����X���±�
	pXYMemory->xAxis()->setLabels(StringArray(szLabels, (int)(sizeof(szLabels) / sizeof(szLabels[0]))));

	//����λ��
	pXYMemory->setPlotArea(30, 20, 360, 160, 0x000200, -1, -1, 0x337f59);

	//���һ���µ��߲�
	pLayer = pXYMemory->addLineLayer();

	if(NULL != pLayer)
	{
		pLayer->setLineWidth(2);
		pLayer->addDataSet(DoubleArray(m_dbMemory, (int)(sizeof(m_dbMemory) / sizeof(m_dbMemory[0]))), 0xff0000, "Server #1");
	}

	//���ƶ���Chart����󶨸�ָ���ؼ�
	m_cvMemory.setChart(pXYMemory);
	delete pXYMemory;
}

void CPurenessLineDlg::shiftData(double *data, int len, double newValue)
{
	memmove(data, data + 1, sizeof(*data) * (len - 1));
	data[len - 1] = newValue;
}

void CPurenessLineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_ID);
	{
		m_cvCPU.updateViewPort(true, true);
	}

	CDialog::OnTimer(nIDEvent);
}

void CPurenessLineDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString strServerIP;
	CString strServerPort;

	int nPort = 0;

	m_txtServerIP.GetWindowText(strServerIP);
	m_txtServerPort.GetWindowText(strServerPort);

	if(strServerIP.GetLength() == 0 || strServerPort.GetLength() == 0)
	{
		MessageBox(_T("�����������ķ�����IP��Port��"), _T("������Ϣ"), MB_OK);
		return;
	}

	//�����˿ڵ�ַ
	nPort = _wtoi((LPCTSTR)strServerPort);

	m_sckServer.Close();

	BOOL blFlag = m_sckServer.Create();
	if(FALSE == blFlag)
	{
		MessageBox(_T("��ʼ��Socketʧ�ܡ�"), _T("������Ϣ"), MB_OK);
		return;
	}

	blFlag = m_sckServer.Connect((LPCTSTR)strServerIP, nPort);
	if(FALSE == blFlag)
	{
		wchar_t szError[100] = {'\0'};
		wsprintf(szError, _T("����[%s]���˿�[%d]ʧ�ܣ�error[%d]��"), (LPCTSTR)strServerIP, nPort, GetLastError());
		MessageBox((LPCTSTR)szError, _T("������Ϣ"), MB_OK);
		return;
	}

	m_blState = TRUE;
}
