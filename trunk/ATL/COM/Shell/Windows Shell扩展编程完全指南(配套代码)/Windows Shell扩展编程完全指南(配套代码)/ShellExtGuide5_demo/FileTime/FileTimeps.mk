
FileTimeps.dll: dlldata.obj FileTime_p.obj FileTime_i.obj
	link /dll /out:FileTimeps.dll /def:FileTimeps.def /entry:DllMain dlldata.obj FileTime_p.obj FileTime_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del FileTimeps.dll
	@del FileTimeps.lib
	@del FileTimeps.exp
	@del dlldata.obj
	@del FileTime_p.obj
	@del FileTime_i.obj
