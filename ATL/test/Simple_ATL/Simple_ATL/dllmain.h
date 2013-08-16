// dllmain.h : 模块类的声明。

class CSimple_ATLModule : public CAtlDllModuleT< CSimple_ATLModule >
{
public :
	DECLARE_LIBID(LIBID_Simple_ATLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIMPLE_ATL, "{504B6A63-3508-4BA3-BC54-6A5183311AAC}")
};

extern class CSimple_ATLModule _AtlModule;
