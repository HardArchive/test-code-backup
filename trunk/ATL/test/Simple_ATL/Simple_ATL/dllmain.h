// dllmain.h : 模块类的声明。

class CSimple_ATLModule : public CAtlDllModuleT< CSimple_ATLModule >
{
public :
	DECLARE_LIBID(LIBID_Simple_ATLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SIMPLE_ATL, "{1BF0E88E-CAF4-44BB-A730-26D42275DED7}")
};

extern class CSimple_ATLModule _AtlModule;
