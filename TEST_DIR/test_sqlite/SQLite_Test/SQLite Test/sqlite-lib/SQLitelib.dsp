# Microsoft Developer Studio Project File - Name="SQLitelib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SQLitelib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SQLitelib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SQLitelib.mak" CFG="SQLitelib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SQLitelib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SQLitelib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SQLitelib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"SQLite.lib"

!ELSEIF  "$(CFG)" == "SQLitelib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"SQLite_d.lib"

!ENDIF 

# Begin Target

# Name "SQLitelib - Win32 Release"
# Name "SQLitelib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\sqlite-src\alter.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\analyze.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\attach.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\auth.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\bitvec.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\btmutex.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\btree.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\build.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\callback.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\complete.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\date.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\delete.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\expr.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\fault.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\func.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\global.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\hash.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\insert.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\journal.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\legacy.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\loadext.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\main.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\malloc.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mem0.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mem1.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mem2.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mem3.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mem4.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mem5.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\memjournal.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mutex.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mutex_noop.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mutex_os2.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mutex_unix.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mutex_w32.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\opcodes.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\os.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\os_os2.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\os_unix.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\os_win.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\pager.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\parse.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\pcache.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\pcache1.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\pragma.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\prepare.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\printf.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\random.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\resolve.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\rtree.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\select.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\shell.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\status.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\table.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\tokenize.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\trigger.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\update.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\utf.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\util.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vacuum.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vdbe.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vdbeapi.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vdbeaux.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vdbeblob.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vdbefifo.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vdbemem.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vtab.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\walker.c"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\where.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\sqlite-src\btree.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\btreeInt.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\hash.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\hwtime.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\keywordhash.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\mutex.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\opcodes.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\os.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\os_common.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\pager.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\parse.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\pcache.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\rtree.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\sqlite3.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\sqlite3ext.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\sqliteicu.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\sqliteInt.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\sqliteLimit.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vdbe.h"
# End Source File
# Begin Source File

SOURCE=".\sqlite-src\vdbeInt.h"
# End Source File
# End Group
# End Target
# End Project
