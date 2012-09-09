# pull in the master SDK makefile that defines all of the macros
#  and all of the build rules

_WIN32_IE = 0x0500

!include <win32.mak>

#----- OUTDIR is defined in WIN32.MAK This is the name of the destination directory-------
all: $(OUTDIR)\IcnOvrly.dll 

LINK32_OBJS= \
	$(OUTDIR)\IcnOvrly.obj \
	$(OUTDIR)\IcnOvrly.res 
        
#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

# itemize all of the required Object files


Includes =  IcnOvrly.h \
            resource.h

# Build rule for resource file
SOURCE=.\IcnOvrly.rc

$(OUTDIR)\IcnOvrly.res: $(SOURCE) $(OUTDIR) $(Includes)
    $(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\IcnOvrly.res $(SOURCE)

SOURCE=.\IcnOvrly.cpp

$(OUTDIR)\IcnOvrly.obj : $(SOURCE) $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvarsdll) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $(SOURCE)

# Build rule for DLL
$(OUTDIR)\IcnOvrly.dll: $(OUTDIR) $(LINK32_OBJS) 
    $(link) $(ldebug) $(dllllflags) \
    $(LINK32_OBJS) $(dlllibs) $(olelibs) shlwapi.lib \
    -out:$(OUTDIR)\IcnOvrly.dll \
    -def:IcnOvrly.def

# Rules for cleaning out those old files
clean:
        $(CLEANUP)
