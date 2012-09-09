
DirBkgndExtps.dll: dlldata.obj DirBkgndExt_p.obj DirBkgndExt_i.obj
	link /dll /out:DirBkgndExtps.dll /def:DirBkgndExtps.def /entry:DllMain dlldata.obj DirBkgndExt_p.obj DirBkgndExt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del DirBkgndExtps.dll
	@del DirBkgndExtps.lib
	@del DirBkgndExtps.exp
	@del dlldata.obj
	@del DirBkgndExt_p.obj
	@del DirBkgndExt_i.obj
