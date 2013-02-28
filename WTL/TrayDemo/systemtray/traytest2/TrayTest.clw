; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyApp
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TrayTest.h"
LastPage=0

ClassCount=3
Class1=CMyApp
Class3=CMainFrame

ResourceCount=3
Resource1=IDR_MAINFRAME
Class2=CChildView
Resource2=IDR_TRAYICON
Resource3=IDD_ABOUTBOX

[CLS:CMyApp]
Type=0
HeaderFile=TrayTest.h
ImplementationFile=TrayTest.cpp
Filter=N
LastObject=CMyApp

[CLS:CChildView]
Type=0
HeaderFile=ChildView.h
ImplementationFile=ChildView.cpp
Filter=N

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T




[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=7
Control1=IDC_STATIC_ICON,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDB_STATIC_IMG,static,1350565902
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC_TEXT,static,1342308352
Control6=IDOK,button,1342373889
Control7=IDC_STATIC_MAIL,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_ABOUT
Command2=ID_APP_EXIT
Command3=ID_VIEW_STATUS_BAR
Command4=ID_VIEW_NOTIFICATIONS
Command5=ID_VIEW_CLEAR
Command6=ID_TOGGLE_ICON
CommandCount=6

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_EDIT_PASTE
Command3=ID_EDIT_UNDO
Command4=ID_EDIT_CUT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_CUT
Command10=ID_EDIT_UNDO
CommandCount=10

[MNU:IDR_TRAYICON]
Type=1
Class=?
Command1=ID_APP_OPEN
Command2=ID_APP_ABOUT
Command3=ID_APP_SUSPEND
CommandCount=3

