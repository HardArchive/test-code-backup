
COMRegisterps.dll: dlldata.obj COMRegister_p.obj COMRegister_i.obj
	link /dll /out:COMRegisterps.dll /def:COMRegisterps.def /entry:DllMain dlldata.obj COMRegister_p.obj COMRegister_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del COMRegisterps.dll
	@del COMRegisterps.lib
	@del COMRegisterps.exp
	@del dlldata.obj
	@del COMRegister_p.obj
	@del COMRegister_i.obj
