# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1993-1999.  Microsoft Corporation.  All rights reserved.
#
# Nmake macros for building Windows 32-Bit apps
#

!include <Win32.Mak>

all: client32.exe

# Update the resource if necessary

client32.res: client32.rc client32.h
    rc -r -fo client32.res client32.rc

# Update the object file if necessary

client32.obj: client32.c client32.h
    $(cc) $(cdebug) $(cflags) $(cvars) client32.c

# Update the executable file if necessary, and if so, add the resource back in.

client32.exe: client32.obj client32.res
    $(link) $(ldebug) $(guilflags) -out:client32.exe client32.obj client32.res $(guilibs)
