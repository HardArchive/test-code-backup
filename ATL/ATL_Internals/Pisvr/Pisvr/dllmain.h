// dllmain.h : 模块类的声明。

class CPisvrModule : public CAtlDllModuleT< CPisvrModule >
{
public :
	DECLARE_LIBID(LIBID_PisvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PISVR, "{B9AC3B86-9FAD-4A16-94DC-A75ECFB65675}")
};

extern class CPisvrModule _AtlModule;
