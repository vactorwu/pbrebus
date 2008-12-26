
mvinfops.dll: dlldata.obj mvinfo_p.obj mvinfo_i.obj
	link /dll /out:mvinfops.dll /def:mvinfops.def /entry:DllMain dlldata.obj mvinfo_p.obj mvinfo_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del mvinfops.dll
	@del mvinfops.lib
	@del mvinfops.exp
	@del dlldata.obj
	@del mvinfo_p.obj
	@del mvinfo_i.obj
