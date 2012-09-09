
ContextShellExtps.dll: dlldata.obj ContextShellExt_p.obj ContextShellExt_i.obj
	link /dll /out:ContextShellExtps.dll /def:ContextShellExtps.def /entry:DllMain dlldata.obj ContextShellExt_p.obj ContextShellExt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del ContextShellExtps.dll
	@del ContextShellExtps.lib
	@del ContextShellExtps.exp
	@del dlldata.obj
	@del ContextShellExt_p.obj
	@del ContextShellExt_i.obj
