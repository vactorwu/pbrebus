
syslevelps.dll: dlldata.obj syslevel_p.obj syslevel_i.obj
	link /dll /out:syslevelps.dll /def:syslevelps.def /entry:DllMain dlldata.obj syslevel_p.obj syslevel_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del syslevelps.dll
	@del syslevelps.lib
	@del syslevelps.exp
	@del dlldata.obj
	@del syslevel_p.obj
	@del syslevel_i.obj
