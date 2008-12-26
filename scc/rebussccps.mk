
rebussccps.dll: dlldata.obj rebusscc_p.obj rebusscc_i.obj
	link /dll /out:rebussccps.dll /def:rebussccps.def /entry:DllMain dlldata.obj rebusscc_p.obj rebusscc_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del rebussccps.dll
	@del rebussccps.lib
	@del rebussccps.exp
	@del dlldata.obj
	@del rebusscc_p.obj
	@del rebusscc_i.obj
