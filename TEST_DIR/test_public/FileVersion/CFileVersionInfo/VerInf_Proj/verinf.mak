# Microsoft Developer Studio Generated NMAKE File, Based on verinf.dsp
!IF "$(CFG)" == ""
CFG=verinf - Win32 Debug
!MESSAGE No configuration specified. Defaulting to verinf - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "verinf - Win32 Release" && "$(CFG)" != "verinf - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "verinf.mak" CFG="verinf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "verinf - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "verinf - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "verinf - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\verinf.exe"


CLEAN :
	-@erase "$(INTDIR)\HyperLink.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\verinf.obj"
	-@erase "$(INTDIR)\verinf.pch"
	-@erase "$(INTDIR)\verinf.res"
	-@erase "$(INTDIR)\verinfDlg.obj"
	-@erase "$(OUTDIR)\verinf.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_WSL_LIB" /Fp"$(INTDIR)\verinf.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\verinf.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\verinf.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\verinf.pdb" /machine:I386 /out:"$(OUTDIR)\verinf.exe" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\verinf.obj" \
	"$(INTDIR)\verinfDlg.obj" \
	"$(INTDIR)\verinf.res" \
	"$(INTDIR)\HyperLink.obj"

"$(OUTDIR)\verinf.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "verinf - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\verinf.exe" "$(OUTDIR)\verinf.bsc"


CLEAN :
	-@erase "$(INTDIR)\HyperLink.obj"
	-@erase "$(INTDIR)\HyperLink.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\verinf.obj"
	-@erase "$(INTDIR)\verinf.pch"
	-@erase "$(INTDIR)\verinf.res"
	-@erase "$(INTDIR)\verinf.sbr"
	-@erase "$(INTDIR)\verinfDlg.obj"
	-@erase "$(INTDIR)\verinfDlg.sbr"
	-@erase "$(OUTDIR)\verinf.bsc"
	-@erase "$(OUTDIR)\verinf.exe"
	-@erase "$(OUTDIR)\verinf.ilk"
	-@erase "$(OUTDIR)\verinf.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_WSL_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\verinf.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\verinf.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\verinf.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\verinf.sbr" \
	"$(INTDIR)\verinfDlg.sbr" \
	"$(INTDIR)\HyperLink.sbr"

"$(OUTDIR)\verinf.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\verinf.pdb" /debug /machine:I386 /out:"$(OUTDIR)\verinf.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\verinf.obj" \
	"$(INTDIR)\verinfDlg.obj" \
	"$(INTDIR)\verinf.res" \
	"$(INTDIR)\HyperLink.obj"

"$(OUTDIR)\verinf.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("verinf.dep")
!INCLUDE "verinf.dep"
!ELSE 
!MESSAGE Warning: cannot find "verinf.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "verinf - Win32 Release" || "$(CFG)" == "verinf - Win32 Debug"
SOURCE=.\HyperLink.cpp

!IF  "$(CFG)" == "verinf - Win32 Release"


"$(INTDIR)\HyperLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\verinf.pch"


!ELSEIF  "$(CFG)" == "verinf - Win32 Debug"


"$(INTDIR)\HyperLink.obj"	"$(INTDIR)\HyperLink.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\verinf.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "verinf - Win32 Release"

CPP_SWITCHES=/nologo /MD /W4 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_WSL_LIB" /Fp"$(INTDIR)\verinf.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\verinf.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "verinf - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_WSL_LIB" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\verinf.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\verinf.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\verinf.cpp

!IF  "$(CFG)" == "verinf - Win32 Release"


"$(INTDIR)\verinf.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\verinf.pch"


!ELSEIF  "$(CFG)" == "verinf - Win32 Debug"


"$(INTDIR)\verinf.obj"	"$(INTDIR)\verinf.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\verinf.pch"


!ENDIF 

SOURCE=.\verinf.rc

"$(INTDIR)\verinf.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\verinfDlg.cpp

!IF  "$(CFG)" == "verinf - Win32 Release"


"$(INTDIR)\verinfDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\verinf.pch"


!ELSEIF  "$(CFG)" == "verinf - Win32 Debug"


"$(INTDIR)\verinfDlg.obj"	"$(INTDIR)\verinfDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\verinf.pch"


!ENDIF 


!ENDIF 

