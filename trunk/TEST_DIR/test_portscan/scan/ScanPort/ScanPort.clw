; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CScanPortDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ScanPort.h"

ClassCount=3
Class1=CScanPortApp
Class2=CScanPortDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SCANPORT_DIALOG

[CLS:CScanPortApp]
Type=0
HeaderFile=ScanPort.h
ImplementationFile=ScanPort.cpp
Filter=N

[CLS:CScanPortDlg]
Type=0
HeaderFile=ScanPortDlg.h
ImplementationFile=ScanPortDlg.cpp
Filter=D
LastObject=IDC_MAX_THREAD
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ScanPortDlg.h
ImplementationFile=ScanPortDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SCANPORT_DIALOG]
Type=1
Class=CScanPortDlg
ControlCount=17
Control1=IDC_STATIC,static,1342308352
Control2=IDC_HOST_OR_IP,edit,1350631552
Control3=IDC_STATIC,static,1342308864
Control4=IDC_START_PORT,edit,1350639744
Control5=IDC_STATIC,static,1342308864
Control6=IDC_END_PORT,edit,1350639744
Control7=IDC_TREE,SysTreeView32,1350631431
Control8=IDC_SCAN,button,1342242816
Control9=IDC_IS_SPECIFIED_PORT,button,1342242819
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308864
Control12=IDC_MAX_THREAD,edit,1350639744
Control13=IDC_STATIC,static,1342308864
Control14=IDC_TIME_OUT,edit,1350639744
Control15=IDC_PROGRESS1,msctls_progress32,1350565889
Control16=IDC_TIME,static,1342308352
Control17=IDC_SPECIFIED_PORTS,edit,1486950404

