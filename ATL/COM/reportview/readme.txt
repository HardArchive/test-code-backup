REPORTVIEW - XP only - Freeware

Systemwide hook to force all commondialogs to
display in Report-view. 

Works under XP. Untested under 2000/NT4, but will
probably work fine. Will NOT work under Win95/98/ME.

To install, copy reportview.dll to your harddisk somewhere (i.e. system32)
Then type:

  regsvr32 <path\>reportview.dll

at the command-prompt. i.e. type the full path to the DLL. 
This will allow the DLL to register itself as an AppInit_Dlls module,
so it will be started every time you start a new process - no
need to reboot.

To uninstall simply type:

  regsvr32 -u <path\>reportview.dll

Send feedback to www.catch22.net