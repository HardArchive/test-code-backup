
MP3TagViewerps.dll: dlldata.obj MP3TagViewer_p.obj MP3TagViewer_i.obj
	link /dll /out:MP3TagViewerps.dll /def:MP3TagViewerps.def /entry:DllMain dlldata.obj MP3TagViewer_p.obj MP3TagViewer_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del MP3TagViewerps.dll
	@del MP3TagViewerps.lib
	@del MP3TagViewerps.exp
	@del dlldata.obj
	@del MP3TagViewer_p.obj
	@del MP3TagViewer_i.obj
