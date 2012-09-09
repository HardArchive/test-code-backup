
HardLinkps.dll: dlldata.obj HardLink_p.obj HardLink_i.obj
	link /dll /out:HardLinkps.dll /def:HardLinkps.def /entry:DllMain dlldata.obj HardLink_p.obj HardLink_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del HardLinkps.dll
	@del HardLinkps.lib
	@del HardLinkps.exp
	@del dlldata.obj
	@del HardLink_p.obj
	@del HardLink_i.obj
