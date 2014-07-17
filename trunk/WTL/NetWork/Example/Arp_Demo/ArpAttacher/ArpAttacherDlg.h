
// ArpAttacherDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
//#include "pcap/pcap.h"
#include "pcap.h"
#include <winsock2.h>

#include "remote-ext.h"

#pragma pack(push)
#pragma pack(1)
// DLC Header
typedef struct tagDLCHeader            /*��̫������֡ͷ���ṹ*/
{
	unsigned char      DesMAC[6];      /* destination HW addrress */
	unsigned char      SrcMAC[6];      /* source HW addresss */
	unsigned short     Ethertype;      /* ethernet type */
} DLCHEADER, *PDLCHEADER;

// ARP Frame
typedef struct tagARPFrame 
{
	unsigned short     HW_Type;         /* hardware type */
	unsigned short     Prot_Type;       /* protocol type */
	unsigned char      HW_Addr_Len;     /* length of hardware address */
	unsigned char      Prot_Addr_Len;   /* length of protocol address */
	unsigned short     Opcode;          /* ARP/RARP */
	unsigned char      Send_HW_Addr[6]; /* sender hardware address */
	unsigned long      Send_Prot_Addr;  /* sender protocol address */
	unsigned char      Targ_HW_Addr[6]; /* target hardware address */
	unsigned long      Targ_Prot_Addr;  /* target protocol address */
	unsigned char      padding[18];     /*additional bytes,without real meaning*/
} ARPFRAME, *PARPFRAME;

// ARP Packet = DLC header + ARP Frame
typedef struct tagARPPacket   
{
	DLCHEADER     dlcHeader;
	ARPFRAME      arpFrame;
} ARPPACKET, *PARPPACKET;

#pragma pack(pop)

// CArpAttacherDlg �Ի���
class CArpAttacherDlg : public CDialog
{
// ����
public:
	CArpAttacherDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ARPATTACHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedOk();

	pcap_if_t *alldevs;
	pcap_t *dev_handle;

	ARPPACKET m_Packet1;
	ARPPACKET m_Packet2;
	BOOL m_bStop;
	
	int m_iFreq;
	CString m_strAttackIp;   //����ip
	CString m_strAttackMac;  //����ip

	CString m_strGateWayIp;   //����ip
	CString m_strGateWayMac;  //����mac

	CString m_strFalseIp;
	CString m_strFalseMac;
	CComboBox m_ifList;
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancel();
};

