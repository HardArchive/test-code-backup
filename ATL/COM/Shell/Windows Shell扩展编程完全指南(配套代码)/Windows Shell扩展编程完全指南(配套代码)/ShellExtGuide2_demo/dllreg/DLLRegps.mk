
DLLRegps.dll: dlldata.obj DLLReg_p.obj DLLReg_i.obj
	link /dll /out:DLLRegps.dll /def:DLLRegps.def /entry:DllMain dlldata.obj DLLReg_p.obj DLLReg_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del DLLRegps.dll
	@del DLLRegps.lib
	@del DLLRegps.exp
	@del dlldata.obj
	@del DLLReg_p.obj
	@del DLLReg_i.obj
