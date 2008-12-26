
pbcompilerps.dll: dlldata.obj pbcompiler_p.obj pbcompiler_i.obj
	link /dll /out:pbcompilerps.dll /def:pbcompilerps.def /entry:DllMain dlldata.obj pbcompiler_p.obj pbcompiler_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del pbcompilerps.dll
	@del pbcompilerps.lib
	@del pbcompilerps.exp
	@del dlldata.obj
	@del pbcompiler_p.obj
	@del pbcompiler_i.obj
