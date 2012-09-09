Shell Icon Overlay Identifier Sample "IcnOvrly"

IcnOvrly is a simple example of a Windows 2000 shell icon overlay
identifier provider. This sample will add a small blue overlay to any
file object that has the .TXT extension. The Windows 2000 shell allows
third-party applications to add overlay icons to file and folder objects
without replacing the entire icon. Overlay icons are used to present
additional information for a file or folder object. A familiar example
of an overlay icon is the small arrow in the lower left-hand corner
that indicates that an object is a shortcut.

A shell icon overlay identifier handler is implemented by creating an
OLE COM InProc server that implements IShellIconOverlayIdentifier. The
IShellIconOverlayIdentifier methods are used by the shell to obtain
overlay icon and priority information and to determine if the item's
overlay should be displayed for a particular object.

The shell icon overlay identifier handler object is created when the
shell starts and remains in memory as long as the shell is running. Also,
the IShellIconOverlayIdentifier::IsMemberOf method is called every time
a file object is displayed. For these reasons, it is important that these
objects, and the IShellIconOverlayIdentifier::IsMemberOf method in
particular, is implemented in as an efficient manner as possible.

Once the sample has been built, you need to register the server. To do
this, just execute the following from the command line, substituting the
path where the DLL is located for "<path>":

regsvr32.exe /s /c <path>\IcnOvrly.dll 

To unregister the server, just execute the following from the command line, 
substituting the path where the DLL is located for "<path>":

regsvr32.exe /u <path>\IcnOvrly.dll 

Once the handler has been registered or unregistered, it is necessary to 
restart the shell to cause the change to be recognized. 

To properly register the extension on Windows NT 4.0 and Windows 2000,
the logged on user must have write access to the following registry
key:

   HKEY_LOCAL_MACHINE
      Software
         Microsoft
            Windows
               CurrentVersion
                  Shell Extensions
                     Approved
