
rebusmailps.dll: dlldata.obj rebusmail_p.obj rebusmail_i.obj
	link /dll /out:rebusmailps.dll /def:rebusmailps.def /entry:DllMain dlldata.obj rebusmail_p.obj rebusmail_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del rebusmailps.dll
	@del rebusmailps.lib
	@del rebusmailps.exp
	@del dlldata.obj
	@del rebusmail_p.obj
	@del rebusmail_i.obj
