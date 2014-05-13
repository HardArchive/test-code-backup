// dllmain.h : 模块类的声明。

class CDllRegModule : public CAtlDllModuleT< CDllRegModule >
{
public :
	DECLARE_LIBID(LIBID_DllRegLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DLLREG, "{9D18ADE9-01ED-4FD5-86AD-14A59AD22BF1}")
};

extern class CDllRegModule _AtlModule;
extern class CComModule _Module;