// dllmain.h : 模块类的声明。

class CSimpleExtModule : public CAtlDllModuleT< CSimpleExtModule >
{
public :
	DECLARE_LIBID(LIBID_SimpleExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIMPLEEXT, "{1718B063-6BB8-4CAA-A714-9D75ED33339D}")
};

extern class CSimpleExtModule _AtlModule;
