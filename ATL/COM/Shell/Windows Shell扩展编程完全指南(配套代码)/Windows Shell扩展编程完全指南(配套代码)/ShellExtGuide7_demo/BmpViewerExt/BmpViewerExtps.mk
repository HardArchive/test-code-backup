
BmpViewerExtps.dll: dlldata.obj BmpViewerExt_p.obj BmpViewerExt_i.obj
	link /dll /out:BmpViewerExtps.dll /def:BmpViewerExtps.def /entry:DllMain dlldata.obj BmpViewerExt_p.obj BmpViewerExt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del BmpViewerExtps.dll
	@del BmpViewerExtps.lib
	@del BmpViewerExtps.exp
	@del dlldata.obj
	@del BmpViewerExt_p.obj
	@del BmpViewerExt_i.obj
