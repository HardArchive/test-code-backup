
TxtFileIconsps.dll: dlldata.obj TxtFileIcons_p.obj TxtFileIcons_i.obj
	link /dll /out:TxtFileIconsps.dll /def:TxtFileIconsps.def /entry:DllMain dlldata.obj TxtFileIcons_p.obj TxtFileIcons_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del TxtFileIconsps.dll
	@del TxtFileIconsps.lib
	@del TxtFileIconsps.exp
	@del dlldata.obj
	@del TxtFileIcons_p.obj
	@del TxtFileIcons_i.obj
