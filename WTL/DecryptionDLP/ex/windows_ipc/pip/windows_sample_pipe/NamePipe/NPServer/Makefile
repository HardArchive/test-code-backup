# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1993-1999.  Microsoft Corporation.  All rights reserved.
#
# Nmake macros for building Windows 32-Bit apps
#

!include <win32.mak>

all: server32.exe

# Update the resource if necessary

server32.res: server32.rc server32.h
    rc -r -fo server32.res server32.rc

# Update the object file if necessary

server32.obj: server32.c server32.h
    $(cc) $(cdebug) $(cflags) $(cvars) server32.c

# Update the executable file if necessary, and if so, add the resource back in.

server32.exe: server32.obj server32.res
    $(link) $(ldebug) $(guilflags) -out:server32.exe server32.obj server32.res $(guilibs) advapi32.lib
