
SendToCloneps.dll: dlldata.obj SendToClone_p.obj SendToClone_i.obj
	link /dll /out:SendToCloneps.dll /def:SendToCloneps.def /entry:DllMain dlldata.obj SendToClone_p.obj SendToClone_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del SendToCloneps.dll
	@del SendToCloneps.lib
	@del SendToCloneps.exp
	@del dlldata.obj
	@del SendToClone_p.obj
	@del SendToClone_i.obj
