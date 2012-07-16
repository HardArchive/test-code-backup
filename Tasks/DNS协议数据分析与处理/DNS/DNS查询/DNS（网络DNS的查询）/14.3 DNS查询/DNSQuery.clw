; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDNSQueryDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "dnsquery.h"
LastPage=0

ClassCount=3
Class1=CDNSQueryApp
Class2=CAboutDlg
Class3=CDNSQueryDlg

ResourceCount=2
Resource1=IDD_ABOUTBOX
Resource2=IDD_DNSQUERY_DIALOG

[CLS:CDNSQueryApp]
Type=0
BaseClass=CWinApp
HeaderFile=DNSQueryApp.h
ImplementationFile=DNSQueryApp.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=DNSQueryDlg.cpp
ImplementationFile=DNSQueryDlg.cpp
LastObject=IDOK

[CLS:CDNSQueryDlg]
Type=0
BaseClass=CDialog
HeaderFile=DNSQueryDlg.h
ImplementationFile=DNSQueryDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CDNSQueryDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DNSQUERY_DIALOG]
Type=1
Class=CDNSQueryDlg
ControlCount=10
Control1=IDC_STATIC,static,1342308352
Control2=IDC_SERVER_IP,SysIPAddress32,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_NAME_EDIT,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_SERVER_LIST,listbox,1352728833
Control7=IDC_STATIC,static,1342308352
Control8=IDC_RESULT_EDIT,edit,1350633604
Control9=ID_QUERY,button,1342242817
Control10=IDCANCEL,button,1342242816

