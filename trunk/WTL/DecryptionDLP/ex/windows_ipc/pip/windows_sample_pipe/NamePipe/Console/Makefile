# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1993-1999.  Microsoft Corporation.  All rights reserved.
#

!include <Win32.Mak>

all: nmpipe.exe

nmpipe.obj: $*.c

.c.obj:
  $(cc) $(cflags) $(cdebug) $(cvarsdll) $*.c

.obj.exe:
  $(link) $(ldebug) $(conlflags) -out:$*.exe $*.obj $(conlibsdll) \
      user32.lib
