
Simple3ps.dll: dlldata.obj Simple3_p.obj Simple3_i.obj
	link /dll /out:Simple3ps.dll /def:Simple3ps.def /entry:DllMain dlldata.obj Simple3_p.obj Simple3_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Simple3ps.dll
	@del Simple3ps.lib
	@del Simple3ps.exp
	@del dlldata.obj
	@del Simple3_p.obj
	@del Simple3_i.obj
